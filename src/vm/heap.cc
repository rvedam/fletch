// Copyright (c) 2014, the Fletch project authors. Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

#include "src/vm/heap.h"

#include <stdio.h>

#include "src/shared/assert.h"
#include "src/shared/flags.h"
#include "src/vm/object.h"

namespace fletch {

Object* Heap::Allocate(int size) {
  uword result = space_->Allocate(size);
  if (result == 0) return Failure::retry_after_gc();
  return HeapObject::FromAddress(result);
}

void Heap::TryDealloc(Object* object, int size) {
  uword location = reinterpret_cast<uword>(object) + size - HeapObject::kTag;
  space_->TryDealloc(location, size);
}

Object* Heap::CreateHeapObject(Class* the_class, Object* init_value,
                               bool immutable) {
  int size = the_class->instance_format().fixed_size();
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  HeapObject* result = HeapObject::cast(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  ASSERT(size == the_class->instance_format().fixed_size());
  result->Initialize(size, init_value);
  return result;
}

Object* Heap::CreateArray(Class* the_class, int length, Object* init_value,
                          bool immutable) {
  ASSERT(the_class->instance_format().type() == InstanceFormat::ARRAY_TYPE);
  int size = Array::AllocationSize(length);
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Array* result = reinterpret_cast<Array*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->Initialize(length, size, init_value);
  return Array::cast(result);
}

Object* Heap::CreateByteArray(Class* the_class, int length, bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::BYTE_ARRAY_TYPE);
  int size = ByteArray::AllocationSize(length);
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  ByteArray* result = reinterpret_cast<ByteArray*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->Initialize(length);
  return ByteArray::cast(result);
}

Object* Heap::CreateLargeInteger(Class* the_class, int64 value,
                                 bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::LARGE_INTEGER_TYPE);
  Object* raw_result = Allocate(LargeInteger::AllocationSize());
  if (raw_result->IsFailure()) return raw_result;
  LargeInteger* result = reinterpret_cast<LargeInteger*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->set_value(value);
  return LargeInteger::cast(result);
}

void Heap::TryDeallocInteger(LargeInteger* object) {
  TryDealloc(object, LargeInteger::AllocationSize());
}

Object* Heap::CreateDouble(Class* the_class, double value, bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::DOUBLE_TYPE);
  Object* raw_result = Allocate(Double::AllocationSize());
  if (raw_result->IsFailure()) return raw_result;
  Double* result = reinterpret_cast<Double*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->set_value(value);
  return Double::cast(result);
}

Object* Heap::CreateBoxed(Class* the_class, Object* value, bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::BOXED_TYPE);
  int size = the_class->instance_format().fixed_size();
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Boxed* result = reinterpret_cast<Boxed*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->set_value(value);
  return Boxed::cast(result);
}

Object* Heap::CreateInitializer(Class* the_class, Function* function,
                                bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::INITIALIZER_TYPE);
  int size = the_class->instance_format().fixed_size();
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Initializer* result = reinterpret_cast<Initializer*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->set_function(function);
  return Initializer::cast(result);
}

Object* Heap::CreateStringInternal(Class* the_class, int length, bool clear,
                                   bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::STRING_TYPE);
  int size = String::AllocationSize(length);
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  String* result = reinterpret_cast<String*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->Initialize(size, length, clear);
  return String::cast(result);
}

Object* Heap::CreateString(Class* the_class, int length, bool immutable) {
  return CreateStringInternal(the_class, length, true, immutable);
}

Object* Heap::CreateStringUninitialized(Class* the_class, int length,
                                        bool immutable) {
  return CreateStringInternal(the_class, length, false, immutable);
}

Object* Heap::CreateStack(Class* the_class, int length, bool immutable) {
  ASSERT(the_class->instance_format().type() == InstanceFormat::STACK_TYPE);
  int size = Stack::AllocationSize(length);
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Stack* result = reinterpret_cast<Stack*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->Initialize(length);
  return Stack::cast(result);
}

Object* Heap::AllocateRawClass(int size) {
  return Allocate(size);
}

Object* Heap::CreateMetaClass(bool immutable) {
  InstanceFormat format = InstanceFormat::class_format();
  int size = Class::AllocationSize();
  // Allocate the raw class objects.
  Class* meta_class = reinterpret_cast<Class*>(AllocateRawClass(size));
  if (meta_class->IsFailure()) return meta_class;
  // Bind the class loop.
  meta_class->set_class(meta_class);
  meta_class->set_immutable(immutable);
  // Initialize the classes.
  meta_class->Initialize(format, size, NULL);
  return meta_class;
}

Object* Heap::CreateClass(InstanceFormat format,
                          Class* meta_class,
                          HeapObject* null,
                          bool immutable) {
  ASSERT(meta_class->instance_format().type() ==
         InstanceFormat::CLASS_TYPE);

  int size = meta_class->instance_format().fixed_size();
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Class* result = reinterpret_cast<Class*>(raw_result);
  result->set_class(meta_class);
  result->set_immutable(immutable);
  result->Initialize(format, size, null);
  return Class::cast(result);  // Perform a cast to validate type.
}

Object* Heap::CreateFunction(Class* the_class,
                             int arity,
                             List<uint8> bytecodes,
                             int number_of_literals,
                             bool immutable) {
  ASSERT(the_class->instance_format().type() ==
         InstanceFormat::FUNCTION_TYPE);
  int literals_size = number_of_literals * kPointerSize;
  int bytecode_size = Function::BytecodeAllocationSize(bytecodes.length());
  int size = Function::AllocationSize(bytecode_size + literals_size);
  Object* raw_result = Allocate(size);
  if (raw_result->IsFailure()) return raw_result;
  Function* result = reinterpret_cast<Function*>(raw_result);
  result->set_class(the_class);
  result->set_immutable(immutable);
  result->set_arity(arity);
  result->set_literals_size(number_of_literals);
  result->Initialize(bytecodes);
  return Function::cast(result);
}

void Heap::ReplaceSpace(Space* space) {
  delete space_;
  space_ = space;
  AdjustAllocationBudget();
}

Space* Heap::TakeSpace() {
  Space* result = space_;
  space_ = NULL;
  return result;
}

}  // namespace fletch
