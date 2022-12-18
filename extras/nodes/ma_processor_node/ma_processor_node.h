/* Include ma_processor_node.h after miniaudio.h */
#ifndef ma_processor_node_h
#define ma_processor_node_h

#ifdef __cplusplus
extern "C" {
#endif

/*
The trim node has one input and one output.
*/
typedef struct {
  ma_node_config nodeConfig;
  void *userData;
  void (*callback)(const float **InBuses, float **OutBuses, ma_uint64 EndpointFrame, ma_uint32 FrameCount, void *UserData);
} ma_processor_node_config;

MA_API ma_processor_node_config ma_processor_node_config_init();

typedef struct {
  ma_node_base baseNode;
  void *userData;
  void (*callback)(const float **InBuses, float **OutBuses, ma_uint64 EndpointFrame, ma_uint32 FrameCount, void *UserData);
} ma_processor_node;

MA_API ma_result ma_processor_node_init(
    ma_node_graph *pNodeGraph, const ma_processor_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_processor_node *pDebugNode);
MA_API void
ma_processor_node_uninit(ma_processor_node *pTrimNode,
                     const ma_allocation_callbacks *pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
#endif /* ma_processor_node_h */
