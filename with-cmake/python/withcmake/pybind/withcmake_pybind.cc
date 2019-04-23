/**
 * Copyright (c) 2019-present, NTC, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <pybind11/pybind11.h>

#include <test.h>

namespace py = pybind11;


PYBIND11_MODULE(withcmake_pybind, m) {
  py::enum_<ComputeType>(m, "ComputeType")
    .value("SUM", ComputeType::SUM)
    .value("SUBTRACT", ComputeType::SUBTRACT)
    .export_values();

  m.def(
    "func",
    [](
      int i, int j,
      ComputeType compute_type
    ) {
      return func(i, j, compute_type);
    });
}
