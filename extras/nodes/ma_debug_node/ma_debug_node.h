/* Include ma_debug_node.h after miniaudio.h */
#ifndef ma_debug_node_h
#define ma_debug_node_h

#ifdef __cplusplus
extern "C" {
#endif

/*
The trim node has one input and one output.
*/
typedef struct {
  ma_node_config nodeConfig;
  ma_uint32 channels;
  void *userData;
  void (*callback)(const float *InFrames, ma_uint64 LocalStartFrame, ma_uint32 FrameCount, void *UserData);
} ma_debug_node_config;

MA_API ma_debug_node_config ma_debug_node_config_init(ma_uint32 channels);

typedef struct {
  ma_node_base baseNode;
  void *userData;
  void (*callback)(const float *InFrames, ma_uint64 LocalStartFrame, ma_uint32 FrameCount, void *UserData);
} ma_debug_node;

MA_API ma_result ma_debug_node_init(
    ma_node_graph *pNodeGraph, const ma_debug_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_debug_node *pDebugNode);
MA_API void
ma_debug_node_uninit(ma_debug_node *pTrimNode,
                     const ma_allocation_callbacks *pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
#endif /* ma_debug_node_h */
