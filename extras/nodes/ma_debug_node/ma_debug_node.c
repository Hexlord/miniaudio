#include "ma_debug_node.h"

MA_API ma_debug_node_config ma_debug_node_config_init(ma_uint32 channels) {
  ma_debug_node_config config;

  MA_ZERO_OBJECT(&config);
  config.nodeConfig =
      ma_node_config_init(); /* Input and output channels will be set in
                                ma_debug_node_init(). */
  config.channels = channels;

  return config;
}

static void ma_debug_node_process_pcm_frames(ma_node *pNode,
                                             const float **ppFramesIn,
                                             ma_uint32 *pFrameCountIn,
                                             float **ppFramesOut,
                                             ma_uint32 *pFrameCountOut) {
  ma_debug_node *pDebugNode = (ma_debug_node *)pNode;

  ma_uint32 framesIn = *pFrameCountIn;
  ma_uint32 framesOut = *pFrameCountOut;
  assert(framesIn == framesOut);
  ma_uint32 channelCount = ma_node_get_input_channels(pNode, 0);
  void (*callback)(float signal, ma_uint32 channel) = pDebugNode->callback;
  if (callback) {
    ma_uint32 frame;
    for (frame = 0; frame < framesIn; ++frame) {
      ma_uint32 channel = 0;
      for (channel = 0; channel < channelCount; ++channel) {
        float sample = ppFramesIn[0][frame * channelCount + channel];
        callback(sample, channel);
      }
    }
  }

  ma_copy_pcm_frames(ppFramesOut[0], ppFramesIn[0], framesOut, ma_format_f32,
                     channelCount);
}

static ma_node_vtable g_ma_debug_node_vtable = {
    ma_debug_node_process_pcm_frames, NULL, 1, /* 1 input channel. */
    1,                                         /* 1 output channel. */
                                               //    MA_NODE_FLAG_PASSTHROUGH |
    MA_NODE_FLAG_CONTINUOUS_PROCESSING};

MA_API ma_result ma_debug_node_init(
    ma_node_graph *pNodeGraph, const ma_debug_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_debug_node *pDebugNode) {
  ma_result result;
  ma_node_config baseConfig;

  if (pDebugNode == NULL) {
    return MA_INVALID_ARGS;
  }

  MA_ZERO_OBJECT(pDebugNode);

  if (pConfig == NULL) {
    return MA_INVALID_ARGS;
  }

  baseConfig = pConfig->nodeConfig;
  baseConfig.vtable = &g_ma_debug_node_vtable;
  baseConfig.pInputChannels = &pConfig->channels;
  baseConfig.pOutputChannels = &pConfig->channels;

  pDebugNode->callback = pConfig->callback;

  result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks,
                        &pDebugNode->baseNode);
  if (result != MA_SUCCESS) {
    return result;
  }

  return MA_SUCCESS;
}

MA_API void
ma_debug_node_uninit(ma_debug_node *pDebugNode,
                     const ma_allocation_callbacks *pAllocationCallbacks) {
  /* The base node is always uninitialized first. */
  ma_node_uninit(pDebugNode, pAllocationCallbacks);
}
