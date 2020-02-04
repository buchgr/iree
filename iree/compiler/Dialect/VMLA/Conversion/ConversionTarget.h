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

#ifndef IREE_COMPILER_DIALECT_VMLA_CONVERSION_CONVERSIONTARGET_H_
#define IREE_COMPILER_DIALECT_VMLA_CONVERSION_CONVERSIONTARGET_H_

#include "iree/compiler/Dialect/VMLA/IR/VMLADialect.h"
#include "iree/compiler/Dialect/VMLA/IR/VMLATypes.h"
#include "mlir/Dialect/StandardOps/Ops.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/Module.h"
#include "mlir/IR/StandardTypes.h"
#include "mlir/Transforms/DialectConversion.h"

namespace mlir {
namespace iree_compiler {

// A conversion target for the VMLA dialect that ensures that tensor types are
// fully removed. Conversions targeting the VMLA dialect should always use this.
class VMLAConversionTarget : public ConversionTarget {
 public:
  VMLAConversionTarget(MLIRContext *context, TypeConverter &typeConverter);

  // Attempts to rewrite an op that may use tensor values into an op using VMLA
  // buffers. See VMLAOpConversion for more information.
  static LogicalResult applyDefaultBufferRewrite(
      Operation *srcOp, ArrayRef<Value> operands, StringRef dstOpName,
      TypeConverter &typeConverter, ConversionPatternRewriter &rewriter);

 private:
  bool isDynamicallyLegal(Operation *op) const override;

  MLIRContext &context;
  TypeConverter &typeConverter;
};

// VMLA tensor-to-buffer conversion utility.
// This can be used by dialects to model custom op conversion from a dialect
// that uses the MLIR tensor type to the IREE VMLA buffer type. At this point
// during conversion the source values will be TensorType and the target values
// will be IREE::VMLA::BufferTypes. Any static information available about the
// tensor (such as static dimensions, element type, layout, etc) are extracted
// here and lowered as expanded values.
template <typename SRC, typename DST>
class VMLAOpConversion : public OpConversionPattern<SRC> {
 public:
  VMLAOpConversion(MLIRContext *context, TypeConverter &typeConverter)
      : OpConversionPattern<SRC>(context), typeConverter(typeConverter) {}

  PatternMatchResult matchAndRewrite(
      SRC srcOp, ArrayRef<Value> operands,
      ConversionPatternRewriter &rewriter) const override {
    if (succeeded(VMLAConversionTarget::applyDefaultBufferRewrite(
            srcOp, operands, DST::getOperationName(), typeConverter,
            rewriter))) {
      return OpConversionPattern<SRC>::matchSuccess();
    }
    return OpConversionPattern<SRC>::matchFailure();
  }

 protected:
  TypeConverter &typeConverter;
};

}  // namespace iree_compiler
}  // namespace mlir

#endif  // IREE_COMPILER_DIALECT_VMLA_CONVERSION_CONVERSIONTARGET_H_