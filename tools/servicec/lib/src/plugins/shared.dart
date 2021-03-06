// Copyright (c) 2015, the Fletch project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

library servicec.plugins.shared;

import 'package:strings/strings.dart' as strings;

import '../parser.dart';
export '../parser.dart';

abstract class CodeGenerationVisitor extends Visitor {
  final String path;
  final StringBuffer buffer = new StringBuffer();
  CodeGenerationVisitor(this.path);

  void write(String s) => buffer.write(s);
  void writeln([String s = '']) => buffer.writeln(s);

  String camelize(String name) {
    return strings.camelize(strings.underscore(name));
  }

  visit(Node node) => node.accept(this);

  visitStruct(Struct node) {
    // TODO(kasper): Stop ignoring this.
  }

  visitUnion(Union node) {
    // TODO(kasper): Stop ignoring this.
  }

  visitNodes(List<Node> nodes, String separatedBy(bool first)) {
    bool first = true;
    for (int i = 0; i < nodes.length; i++) {
      write(separatedBy(first));
      if (first) first = false;
      visit(nodes[i]);
    }
  }
}
