// Copyright (c) 2015, the Fletch project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

library fletchc.fletch_context;

import 'package:compiler/src/tree/tree.dart' show
    Node;

import 'package:compiler/src/resolution/resolution.dart' show
    TreeElements;

import 'package:compiler/src/constants/expressions.dart' show
    ConstantExpression;

import 'package:compiler/src/dart2jslib.dart' show
    DartConstantCompiler;

import 'fletch_compiler.dart' show
    FletchCompiler;

export 'fletch_compiler.dart' show
    FletchCompiler;

import 'fletch_backend.dart' show
    FletchBackend;

export 'fletch_backend.dart' show
    FletchBackend;

import 'fletch_resolution_callbacks.dart' show
    FletchResolutionCallbacks;

export 'fletch_resolution_callbacks.dart' show
    FletchResolutionCallbacks;

export 'bytecode_builder.dart' show
    BytecodeBuilder;

class FletchContext {
  final FletchCompiler compiler;

  FletchResolutionCallbacks resolutionCallbacks;

  FletchContext(this.compiler);

  FletchBackend get backend => compiler.backend;

  /// If [isConst] is true, a compile-time error is reported.
  ConstantExpression compileConstant(
      Node node,
      TreeElements elements,
      {bool isConst}) {
    assert(isConst != null);
    DartConstantCompiler constantCompiler =
        backend.constantCompilerTask.constantCompiler;
    return constantCompiler.compileNodeWithDefinitions(
        node, elements, isConst: isConst);
  }
}