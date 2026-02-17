//===- Passes.h - ACC Conversion Pass Registration --------*- C++ -*-===//
//
// Part of the ACC Project
//
//===----------------------------------------------------------------------===//
//
// This header file defines prototypes that expose pass constructors for
// all ACC conversion passes.
//
//===----------------------------------------------------------------------===//

#ifndef ACC_CONVERSION_PASSES_H
#define ACC_CONVERSION_PASSES_H

#include "ACC/Conversion/ACHighToACMid/ACHighToACMid.h"
#include "ACC/Conversion/ACMidToLinalg/ACMidToLinalg.h"
#include "ACC/Conversion/ACMidToLLVM/ACMidToLLVM.h"

namespace acc {

/// Register all conversion passes
void registerConversionPasses();

} // namespace acc

#endif // ACC_CONVERSION_PASSES_H
