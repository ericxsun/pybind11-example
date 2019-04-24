/**
 * Copyright (c) 2019-present, NTC, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <cuBERT.h>


namespace py = pybind11;

PYBIND11_MODULE(cuBERT_pybind, m) {
  py::enum_<cuBERT_ComputeType>(m, "cuBERT_ComputeType")
    .value("cuBERT_COMPUTE_FLOAT", cuBERT_ComputeType::cuBERT_COMPUTE_FLOAT)
    .value("cuBERT_COMPUTE_HALF", cuBERT_ComputeType::cuBERT_COMPUTE_HALF)
    .export_values();

  py::enum_<cuBERT_OutputType>(m, "cuBERT_OutputType")
    .value("cuBERT_LOGITS", cuBERT_OutputType::cuBERT_LOGITS)
    .value("cuBERT_POOLED_OUTPUT", cuBERT_OutputType::cuBERT_POOLED_OUTPUT)
    .value("cuBERT_SEQUENCE_OUTPUT", cuBERT_OutputType::cuBERT_SEQUENCE_OUTPUT)
    .value("cuBERT_EMBEDDING_OUTPUT", cuBERT_OutputType::cuBERT_EMBEDDING_OUTPUT)
    .export_values();

  m.def(
    "cuBERT_initialize",
    []() { cuBERT_initialize(); }
  );

  m.def(
    "cuBERT_finalize",
    []() { cuBERT_finalize(); }
  );

  m.def(
    "cuBERT_open",
    [](
      const char *model_file,
      int max_batch_size,
      int seq_length,
      int num_hidden_layers,
      int num_attention_heads,
      cuBERT_ComputeType compute_type
    ) {
      return cuBERT_open(model_file, max_batch_size, seq_length, num_hidden_layers, num_attention_heads);
    },
    py::arg("model_file"),
    py::arg("max_batch_size"),
    py::arg("seq_length"),
    py::arg("num_hidden_layers"),
    py::arg("num_attention_heads"),
    py::arg("compute_type") = cuBERT_ComputeType::cuBERT_COMPUTE_FLOAT
  );

  m.def(
    "cuBERT_compute",
    [](
      void* model,
      int batch_size,
      int* input_ids,
      char* input_mask,
      char* segment_ids,
      cuBERT_OutputType output_type,
      cuBERT_ComputeType compute_type
    ) {
      float* output = new float[batch_size];

      cuBERT_compute(model, batch_size, input_ids, input_mask, segment_ids, (void*)output, output_type, compute_type);

      // Create a Python object that will free the allocated memory when destroyed:
      py::capsule free_when_done(output, [](void *f) {
        float* output = reinterpret_cast<float *>(f);
        delete[] output;
      });

      return py::array_t<float>(batch_size, output, free_when_done);
    },
    py::arg("model"),
    py::arg("batch_size"),
    py::arg("input_ids"),
    py::arg("input_mask"),
    py::arg("segment_ids"),
    py::arg("output_type") = cuBERT_OutputType::cuBERT_LOGITS,
    py::arg("compute_type") = cuBERT_ComputeType::cuBERT_COMPUTE_FLOAT
  );

  m.def(
    "cuBERT_close",
    [](
      void* model,
      cuBERT_ComputeType compute_type
    ) {
      cuBERT_close(model, compute_type);
    },
    py::arg("model"),
    py::arg("compute_type") = cuBERT_ComputeType::cuBERT_COMPUTE_FLOAT
  );

  /** high level API including tokenization **/
  m.def(
    "cuBERT_open_tokenizer",
    [](const char* vocab_file, int do_lower_case = 1) {
      return cuBERT_open_tokenizer(vocab_file, do_lower_case);
    },
    py::arg("vocab_file"),
    py::arg("do_lower_case") = 1
  );

  m.def(
    "cuBERT_close_tokenizer",
    [](void* tokenizer) { cuBERT_close_tokenizer(tokenizer); },
    py::arg("tokenizer")
  );

  m.def(
    "cuBERT_tokenize_compute",
    [](
      void* model,
      void* tokenizer,
      int batch_size,
      std::vector<char *> text_a,
      std::vector<char *> text_b,
      cuBERT_OutputType output_type,
      cuBERT_ComputeType compute_type
    ) {
      char** arr_text_a = new char* [text_a.size()];
      char** arr_text_b = new char* [text_b.size()];

      for (size_t i=0; i<text_a.size(); ++i) { arr_text_a[i] = text_a[i]; }
      for (size_t i=0; i<text_b.size(); ++i) { arr_text_b[i] = text_b[i]; }

      float* output = new float[batch_size];

      cuBERT_tokenize_compute(
        model, tokenizer, batch_size,
        (const char**)arr_text_a, (const char**)arr_text_b, (void*)output, output_type, compute_type
      );

      // Create a Python object that will free the allocated memory when destroyed:
      py::capsule free_when_done(output, [](void *f) {
        float* output = reinterpret_cast<float *>(f);
        delete[] output;
      });

      return py::array_t<float>(batch_size, output, free_when_done);
    },
    py::arg("model"),
    py::arg("tokenizer"),
    py::arg("batch_size"),
    py::arg("text_a"),
    py::arg("text_b"),
    py::arg("output_type") = cuBERT_OutputType::cuBERT_LOGITS,
    py::arg("compute_type") = cuBERT_ComputeType::cuBERT_COMPUTE_FLOAT
  );
}
