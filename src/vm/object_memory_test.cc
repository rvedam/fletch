// Copyright (c) 2014, the Fletch project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

#include "src/shared/assert.h"
#include "src/vm/heap.h"
#include "src/vm/object_memory.h"
#include "src/shared/test_case.h"

namespace fletch {

static Chunk* AllocateChunkAndTestIt(Space* space) {
  // Allocate chunk.
  Chunk* chunk = ObjectMemory::AllocateChunk(space, 4 * KB);
  EXPECT(chunk->base() != 0);

  // Write to the chunk and check the content.
  char* chars = reinterpret_cast<char*>(chunk->base());
  for (unsigned i = 0; i < chunk->size(); i++) {
    chars[i] = i % 128;
    EXPECT_EQ(chars[i], static_cast<char>(i % 128));
  }
  return chunk;
}

TEST_CASE(ObjectMemory) {
  Space space;

  // Allocate.
  Chunk* first = AllocateChunkAndTestIt(&space);
  Chunk* second = AllocateChunkAndTestIt(&space);

  // Compare.
  EXPECT(first != second);

  // Free.
  ObjectMemory::FreeChunk(first);
  ObjectMemory::FreeChunk(second);
}

}  // namespace fletch
