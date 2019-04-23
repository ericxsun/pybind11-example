/**
 * Copyright (c) 2019-present, NTC, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef WITH_CMAKE_SRC_TEST_H_
#define WITH_CMAKE_SRC_TEST_H_

enum ComputeType {
  SUM = 0,
  SUBTRACT = 1
};

int func(int i, int j, ComputeType computeType);

#endif  // WITH_CMAKE_SRC_TEST_H_
