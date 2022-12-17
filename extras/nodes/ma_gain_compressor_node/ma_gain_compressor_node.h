/* Include ma_gain_compressor_node.h after miniaudio.h */
#ifndef ma_gain_compressor_node_h
#define ma_gain_compressor_node_h

#ifdef __cplusplus
extern "C" {
#endif

/*
The trim node has one input and one output.
*/
typedef struct {
  ma_node_config nodeConfig;
  ma_uint32 channels;
  // -0.1 means weak compression.
  // -2.0 means strong compression.
  // Anything inbetween would also make sense.
  double coef;
} ma_gain_compressor_node_config;

MA_API ma_gain_compressor_node_config ma_gain_compressor_node_config_init(ma_uint32 channels);

typedef struct {
  ma_node_base baseNode;
  double coef;
} ma_gain_compressor_node;

MA_API ma_result ma_gain_compressor_node_init(
    ma_node_graph *pNodeGraph, const ma_gain_compressor_node_config *pConfig,
    const ma_allocation_callbacks *pAllocationCallbacks,
    ma_gain_compressor_node *pDebugNode);
MA_API void
ma_gain_compressor_node_uninit(ma_gain_compressor_node *pTrimNode,
                     const ma_allocation_callbacks *pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
#endif /* ma_gain_compressor_node_h */
