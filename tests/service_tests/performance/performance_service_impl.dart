// Copyright (c) 2015, the Fletch project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

import 'dart/performance_service.dart';

class PerformanceServiceImpl implements PerformanceService {
  int echo(int n) => n;
  int ping() => 42;
}

main() {
  var impl = new PerformanceServiceImpl();
  PerformanceService.initialize(impl);
  while (PerformanceService.hasNextEvent()) {
    PerformanceService.handleNextEvent();
  }
}