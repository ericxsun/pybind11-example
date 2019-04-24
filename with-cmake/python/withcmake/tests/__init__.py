#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright @2019 R&D, NTC Inc. (ntc.ai)
#
# Author: qinluo <eric.x.sun@gmail.com>
#

from withcmake import ComputeType
from withcmake import func

assert func(1, 1, ComputeType.SUM) == 2
assert func(1, 1, ComputeType.SUBTRACT) == 0
