#include "ma_processor_node.h"

MA_API ma_processor_node_config ma_processor_node_config_init() {
  ma_processor_node_config config;

  MA_ZERO_OBJECT(&config);
  config.nodeConfig =
      ma_node_config_init();

  return config;
}

static void ma_processor_node_process_pcm_frames(ma_node *pNode,
                                             const float **ppFramesIn,
                                             ma_uint32 *pFrameCountIn,
                                             float **ppFramesOut,
                                             ma_uint32 *pFrameCountOut) {
  ma_processor_node *pProcessorNode = (ma_processor_node *)pNode;

  ma_uint32 framesIn = *pFrameCountIn;
  ma_uint32 framesOut = *pFrameCountOut;
  assert(framesIn == framesOut);
  
  pProcessorNode->callback(ppFramesIn, ppFramesOut, ma_node_get_time(pNode), framesIn, pProcessorNode->userData);
}

static ma_node_vtable g_ma_processor_node_vtable = {
    ma_processor_node_process_pcm_frames, NULL, 
    MA_NODE_BUS_COUNT_UNKNOWN,
    MA_NODE_BUS_COUNT_UNKNOWN,
    0};

MA_API ma_result ma_processor_node_init(
    ma_node_graph *pNodeGraph, const ma_processor_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_processor_node *pProcessorNode) {
  ma_result result;
  ma_node_config baseConfig;

  if (pProcessorNode == NULL) {
    return MA_INVALID_ARGS;
  }

  MA_ZERO_OBJECT(pProcessorNode);

  if (pConfig == NULL) {
    return MA_INVALID_ARGS;
  }

  baseConfig = pConfig->nodeConfig;
  baseConfig.vtable = &g_ma_processor_node_vtable;

  pProcessorNode->userData = pConfig->userData;
  pProcessorNode->callback = pConfig->callback;

  result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks,
                        &pProcessorNode->baseNode);
  if (result != MA_SUCCESS) {
    return result;
  }

  return MA_SUCCESS;
}

MA_API void
ma_processor_node_uninit(ma_processor_node *pProcessorNode,
                     const ma_allocation_callbacks *pAllocationCallbacks) {
  /* The base node is always uninitialized first. */
  ma_node_uninit(pProcessorNode, pAllocationCallbacks);
}
