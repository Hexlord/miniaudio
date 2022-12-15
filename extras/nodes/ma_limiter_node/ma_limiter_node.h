/* Include ma_limiter_node.h after miniaudio.h */
#ifndef ma_limiter_node_h
#define ma_limiter_node_h

#ifdef __cplusplus
extern "C" {
#endif

/*
The trim node has one input and one output.
*/
typedef struct {
  ma_node_config nodeConfig;
  ma_uint32 channels;
  // Gain = 1 - pow(min(1, signal / threshold), 4)
  double threshold;
} ma_limiter_node_config;

MA_API ma_limiter_node_config ma_limiter_node_config_init(ma_uint32 channels);

typedef struct {
  ma_node_base baseNode;
  double thresholdInv;
} ma_limiter_node;

MA_API ma_result ma_limiter_node_init(
    ma_node_graph *pNodeGraph, const ma_limiter_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_limiter_node *pDebugNode);
MA_API void
ma_limiter_node_uninit(ma_limiter_node *pTrimNode,
                     const ma_allocation_callbacks *pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
#endif /* ma_limiter_node_h */
