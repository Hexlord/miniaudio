#include "ma_gain_compressor_node.h"

MA_API ma_gain_compressor_node_config ma_gain_compressor_node_config_init(ma_uint32 channels) {
  ma_gain_compressor_node_config config;

  MA_ZERO_OBJECT(&config);
  config.nodeConfig =
      ma_node_config_init(); /* Input and output channels will be set in
                                ma_gain_compressor_node_init(). */
  config.channels = channels;
  config.coef = -1.0;

  return config;
}

static void ma_gain_compressor_node_process_pcm_frames(ma_node *pNode,
                                             const float **ppFramesIn,
                                             ma_uint32 *pFrameCountIn,
                                             float **ppFramesOut,
                                             ma_uint32 *pFrameCountOut) {
  ma_gain_compressor_node *pGainCompressorNode = (ma_gain_compressor_node *)pNode;

  ma_uint32 framesIn = *pFrameCountIn;
  ma_uint32 framesOut = *pFrameCountOut;
  assert(framesIn == framesOut);
  ma_uint32 channelCount = ma_node_get_input_channels(pNode, 0);
  ma_uint32 frameIndex;
  double coef = pGainCompressorNode->coef;
  double multiplier = 1.0 / coef; 
  for(frameIndex = 0; frameIndex < framesIn; ++frameIndex) {
      ma_uint32 channelIndex;
      for (channelIndex = 0; channelIndex < channelCount; channelIndex += 1) {
          double sample = (double)ppFramesIn[0][frameIndex * channelCount + channelIndex];
          double x = abs(sample);
          double value = exp(coef * x);
          double sign = sample < 0.0 ? -1.0 : 1.0;
          double signal = sign * (multiplier * value - multiplier * 1.0);
          ppFramesOut[0][frameIndex * channelCount + channelIndex] = (float)signal;
      }
  }
}

static ma_node_vtable g_ma_gain_compressor_node_vtable = {
    ma_gain_compressor_node_process_pcm_frames, NULL, 1, /* 1 input channel. */
    1,                                         /* 1 output channel. */
    0};

MA_API ma_result ma_gain_compressor_node_init(
    ma_node_graph *pNodeGraph, const ma_gain_compressor_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_gain_compressor_node *pGainCompressorNode) {
  ma_result result;
  ma_node_config baseConfig;

  if (pGainCompressorNode == NULL) {
    return MA_INVALID_ARGS;
  }

  MA_ZERO_OBJECT(pGainCompressorNode);

  if (pConfig == NULL) {
    return MA_INVALID_ARGS;
  }

  baseConfig = pConfig->nodeConfig;
  baseConfig.vtable = &g_ma_gain_compressor_node_vtable;
  baseConfig.pInputChannels = &pConfig->channels;
  baseConfig.pOutputChannels = &pConfig->channels;

  pGainCompressorNode->coef = pConfig->coef;

  result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks,
                        &pGainCompressorNode->baseNode);
  if (result != MA_SUCCESS) {
    return result;
  }

  return MA_SUCCESS;
}

MA_API void
ma_gain_compressor_node_uninit(ma_gain_compressor_node *pGainCompressorNode,
                     const ma_allocation_callbacks *pAllocationCallbacks) {
  /* The base node is always uninitialized first. */
  ma_node_uninit(pGainCompressorNode, pAllocationCallbacks);
}
