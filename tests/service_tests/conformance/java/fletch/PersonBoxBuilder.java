// Copyright (c) 2015, the Fletch project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

// Generated file. Do not edit.

package fletch;

import java.util.List;
public class PersonBoxBuilder extends Builder {
  public static int kSize = 8;
  public PersonBoxBuilder(BuilderSegment segment, int offset) {
    super(segment, offset);
  }

  public PersonBoxBuilder() {
    super();
  }

  public PersonBuilder initPerson() {
    PersonBuilder result = new PersonBuilder();
    newStruct(result, 0, 24);
    return result;
  }
}
