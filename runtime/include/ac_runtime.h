//===- ac_runtime.h - ACC Runtime Public API --------------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// Public C API for the ACC runtime library. This API is called by
// generated code at inference time, and can also be used directly by
// applications for model loading and execution.
//
// Design: Handle-based API for resource management (runtime, tensor, model).
//
//===----------------------------------------------------------------------===//

#ifndef AC_RUNTIME_H
#define AC_RUNTIME_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===========================================================================
// Opaque Handle Types
// ===========================================================================

/// Runtime instance handle (one per backend).
typedef struct ACCRuntime *ac_runtime_t;

/// Tensor handle (managed by runtime).
typedef struct ACCTensor *ac_tensor_t;

/// Compiled model handle.
typedef struct ACCModel *ac_model_t;

// ===========================================================================
// Data Type Enumeration
// ===========================================================================

typedef enum {
  AC_FLOAT32 = 0,
  AC_FLOAT16 = 1,
  AC_BFLOAT16 = 2,
  AC_INT32 = 3,
  AC_INT16 = 4,
  AC_INT8 = 5,
  AC_INT4 = 6,
  AC_UINT8 = 7,
} ac_dtype_t;

/// Returns the byte size of one element of the given dtype.
size_t ac_dtype_size(ac_dtype_t dtype);

// ===========================================================================
// Runtime Management
// ===========================================================================

/// Create a runtime instance for the specified backend.
/// @param backend  Backend identifier: "cpu", "cuda", "ascend"
/// @return Runtime handle, or NULL on failure.
ac_runtime_t ac_create_runtime(const char *backend);

/// Destroy a runtime instance and free all associated resources.
void ac_destroy_runtime(ac_runtime_t runtime);

/// Get the backend name for a runtime instance.
const char *ac_runtime_backend(ac_runtime_t runtime);

// ===========================================================================
// Tensor Operations
// ===========================================================================

/// Allocate a tensor on the given runtime's backend.
/// Memory is zero-initialized and aligned to 64 bytes (AVX-512 friendly).
/// @param runtime  Runtime handle
/// @param shape    Array of dimension sizes
/// @param ndim     Number of dimensions
/// @param dtype    Element data type
/// @return Tensor handle, or NULL on failure.
ac_tensor_t ac_alloc_tensor(ac_runtime_t runtime,
                            const int64_t *shape, int ndim,
                            ac_dtype_t dtype);

/// Free a tensor and release its memory.
void ac_free_tensor(ac_tensor_t tensor);

/// Copy data from host memory into a tensor.
/// @param tensor  Destination tensor
/// @param data    Source host pointer
/// @param size    Number of bytes to copy
void ac_tensor_from_host(ac_tensor_t tensor, const void *data, size_t size);

/// Copy data from a tensor to host memory.
/// @param tensor  Source tensor
/// @param data    Destination host pointer
/// @param size    Number of bytes to copy
void ac_tensor_to_host(ac_tensor_t tensor, void *data, size_t size);

/// Get the raw data pointer of a tensor (for direct access on CPU).
void *ac_tensor_data(ac_tensor_t tensor);

/// Get the shape of a tensor.
const int64_t *ac_tensor_shape(ac_tensor_t tensor);

/// Get the number of dimensions of a tensor.
int ac_tensor_ndim(ac_tensor_t tensor);

/// Get the data type of a tensor.
ac_dtype_t ac_tensor_dtype(ac_tensor_t tensor);

/// Get the total number of elements in a tensor.
int64_t ac_tensor_numel(ac_tensor_t tensor);

// ===========================================================================
// Model Loading and Execution
// ===========================================================================

/// Load a compiled model from a shared library or object file.
/// @param runtime     Runtime handle
/// @param model_path  Path to the compiled model (.so / .o)
/// @return Model handle, or NULL on failure.
ac_model_t ac_load_model(ac_runtime_t runtime, const char *model_path);

/// Execute model inference.
/// @param model       Model handle
/// @param inputs      Array of input tensor handles
/// @param num_inputs  Number of input tensors
/// @param outputs     Array of output tensor handles (pre-allocated)
/// @param num_outputs Number of output tensors
void ac_execute(ac_model_t model,
                ac_tensor_t *inputs, int num_inputs,
                ac_tensor_t *outputs, int num_outputs);

/// Unload a model and free associated resources.
void ac_unload_model(ac_model_t model);

// ===========================================================================
// Low-Level Utilities (called by compiler-generated code)
// ===========================================================================

/// Allocate aligned memory.
void *ac_alloc(size_t size, size_t alignment);

/// Free aligned memory.
void ac_free(void *ptr);

/// Print a tensor for debugging.
void ac_print_tensor_f32(float *data, int64_t *shape, int64_t rank);

/// Get current time in milliseconds (for profiling).
double ac_get_time_ms(void);

#ifdef __cplusplus
}
#endif

#endif // AC_RUNTIME_H
