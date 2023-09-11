#include "ma_limiter_node.h"
#include "SimpleComp.cpp"
#include "SimpleEnvelope.cpp"
#include "SimpleGate.cpp"
#include "SimpleLimit.cpp"

using namespace chunkware_simple;

MA_API ma_limiter_node_config ma_limiter_node_config_init(ma_uint32 channels) {
  ma_limiter_node_config config;

  MA_ZERO_OBJECT(&config);
  config.nodeConfig =
      ma_node_config_init(); /* Input and output channels will be set in
                                ma_limiter_node_init(). */
  config.channels = channels;
  config.attackMS = 10.0;
  config.releaseMS = 100.0;
  config.thresholdDB = 0.0;
  config.ratioDB = 1.0;

  return config;
}

static void ma_limiter_node_process_pcm_frames(ma_node *pNode,
                                             const float **ppFramesIn,
                                             ma_uint32 *pFrameCountIn,
                                             float **ppFramesOut,
                                             ma_uint32 *pFrameCountOut) {
  ma_limiter_node *pLimiterNode = (ma_limiter_node *)pNode;
  SimpleComp *Comp = (SimpleComp *)pLimiterNode->user;

  ma_uint32 framesIn = *pFrameCountIn;
  ma_uint32 framesOut = *pFrameCountOut;
  assert(framesIn == framesOut);
  ma_uint32 channelCount = ma_node_get_input_channels(pNode, 0);
  assert(channelCount == 2);
  ma_uint32 frameIndex;
  for(frameIndex = 0; frameIndex < framesIn; ++frameIndex) {
      double left = (double)ppFramesIn[0][frameIndex * channelCount + 0];
      double right = (double)ppFramesIn[0][frameIndex * channelCount + 1];
      Comp->process(left, right);
      ppFramesOut[0][frameIndex * channelCount + 0] = (float)left;
      ppFramesOut[0][frameIndex * channelCount + 1] = (float)right;
  }
}

static ma_node_vtable g_ma_limiter_node_vtable = {
    ma_limiter_node_process_pcm_frames, NULL, 1, /* 1 input channel. */
    1,                                         /* 1 output channel. */
    0};

MA_API ma_result ma_limiter_node_init(
    ma_node_graph *pNodeGraph, const ma_limiter_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_limiter_node *pLimiterNode) {
  ma_result result;
  ma_node_config baseConfig;

  if (pLimiterNode == NULL) {
    return MA_INVALID_ARGS;
  }

  MA_ZERO_OBJECT(pLimiterNode);

  if (pConfig == NULL) {
    return MA_INVALID_ARGS;
  }

  baseConfig = pConfig->nodeConfig;
  baseConfig.vtable = &g_ma_limiter_node_vtable;
  baseConfig.pInputChannels = &pConfig->channels;
  baseConfig.pOutputChannels = &pConfig->channels;

  SimpleComp *Comp = new SimpleComp;
  Comp->initRuntime();
  Comp->setAttack(pConfig->attackMS);
  Comp->setRelease(pConfig->releaseMS);
  Comp->setThresh(pConfig->thresholdDB);
  Comp->setRatio(pConfig->ratioDB);
  
  pLimiterNode->user = (void *)Comp;

  result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks,
                        &pLimiterNode->baseNode);
  if (result != MA_SUCCESS) {
    return result;
  }

  return MA_SUCCESS;
}

MA_API void
ma_limiter_node_uninit(ma_limiter_node *pLimiterNode,
                     const ma_allocation_callbacks *pAllocationCallbacks) {
  /* The base node is always uninitialized first. */
  ma_node_uninit(pLimiterNode, pAllocationCallbacks);
  delete (SimpleComp *)pLimiterNode->user;
}
