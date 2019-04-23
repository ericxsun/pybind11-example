/**
 * Copyright (c) 2019-present, NTC, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include <iostream>

#include "test.h"


int func(int i, int j, ComputeType computeType) {
  if (computeType == ::SUM) {
    return i + j;
  } else if (computeType == ::SUBTRACT) {
    return i - j;
  } else {
    throw "unknown computeType";
  }
}

int main() {
  std::cout << "1+1=" << func(1, 1, ::SUM) << std::endl;
  std::cout << "1-1=" << func(1, 1, ::SUBTRACT) << std::endl;

  return 0;
}
