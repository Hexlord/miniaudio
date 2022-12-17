#include "ma_continuous_processor_node.h"

MA_API ma_continuous_processor_node_config ma_continuous_processor_node_config_init() {
  ma_continuous_processor_node_config config;

  MA_ZERO_OBJECT(&config);
  config.nodeConfig =
      ma_node_config_init();

  return config;
}

static void ma_continuous_processor_node_process_pcm_frames(ma_node *pNode,
                                             const float **ppFramesIn,
                                             ma_uint32 *pFrameCountIn,
                                             float **ppFramesOut,
                                             ma_uint32 *pFrameCountOut) {
  ma_continuous_processor_node *pContinuousProcessorNode = (ma_continuous_processor_node *)pNode;

  ma_uint32 framesIn = *pFrameCountIn;
  ma_uint32 framesOut = *pFrameCountOut;
  assert(framesIn == framesOut);
  
  pContinuousProcessorNode->callback(ppFramesIn, ppFramesOut, ma_node_get_time(pNode), framesIn, pContinuousProcessorNode->userData);
}

static ma_node_vtable g_ma_continuous_processor_node_vtable = {
    ma_continuous_processor_node_process_pcm_frames, NULL, 
    MA_NODE_BUS_COUNT_UNKNOWN,
    MA_NODE_BUS_COUNT_UNKNOWN,
    MA_NODE_FLAG_CONTINUOUS_PROCESSING};

MA_API ma_result ma_continuous_processor_node_init(
    ma_node_graph *pNodeGraph, const ma_continuous_processor_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_continuous_processor_node *pContinuousProcessorNode) {
  ma_result result;
  ma_node_config baseConfig;

  if (pContinuousProcessorNode == NULL) {
    return MA_INVALID_ARGS;
  }

  MA_ZERO_OBJECT(pContinuousProcessorNode);

  if (pConfig == NULL) {
    return MA_INVALID_ARGS;
  }

  baseConfig = pConfig->nodeConfig;
  baseConfig.vtable = &g_ma_continuous_processor_node_vtable;

  pContinuousProcessorNode->userData = pConfig->userData;
  pContinuousProcessorNode->callback = pConfig->callback;

  result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks,
                        &pContinuousProcessorNode->baseNode);
  if (result != MA_SUCCESS) {
    return result;
  }

  return MA_SUCCESS;
}

MA_API void
ma_continuous_processor_node_uninit(ma_continuous_processor_node *pContinuousProcessorNode,
                     const ma_allocation_callbacks *pAllocationCallbacks) {
  /* The base node is always uninitialized first. */
  ma_node_uninit(pContinuousProcessorNode, pAllocationCallbacks);
}
