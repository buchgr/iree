// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IREE_MODULES_STRINGS_DIALECT_STRINGS_DIALECT_H_
#define IREE_MODULES_STRINGS_DIALECT_STRINGS_DIALECT_H_

#include "iree/compiler/Dialect/IREE/IR/IREETypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"

namespace mlir {
namespace iree_compiler {
namespace IREE {
namespace Strings {

class StringsDialect : public Dialect {
 public:
  explicit StringsDialect(MLIRContext *context);
  static StringRef getDialectNamespace() { return "strings"; }

  Type parseType(DialectAsmParser &parser) const override;
  void printType(Type type, DialectAsmPrinter &p) const override;
};

class StringType : public Type::TypeBase<StringType, RefObjectType> {
 public:
  using Base::Base;
  static StringType get(MLIRContext *context) {
    return Base::get(context, TypeKind::String);
  }
  static bool kindof(unsigned kind) { return kind == TypeKind::String; }
};

#define GET_OP_CLASSES
#include "iree/modules/strings/dialect/strings_ops.h.inc"

}  // namespace Strings
}  // namespace IREE
}  // namespace iree_compiler
}  // namespace mlir

#endif  // IREE_MODULES_STRINGS_DIALECT_STRINGS_DIALECT_H_