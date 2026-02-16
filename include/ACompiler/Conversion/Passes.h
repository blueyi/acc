//===- Passes.h - ACompiler Conversion Pass Registration --------*- C++ -*-===//
//
// Part of the ACompiler Project
//
//===----------------------------------------------------------------------===//
//
// This header file defines prototypes that expose pass constructors for
// all ACompiler conversion passes.
//
//===----------------------------------------------------------------------===//

#ifndef ACOMPILER_CONVERSION_PASSES_H
#define ACOMPILER_CONVERSION_PASSES_H

#include "ACompiler/Conversion/ACHighToACMid/ACHighToACMid.h"
#include "ACompiler/Conversion/ACMidToLinalg/ACMidToLinalg.h"
#include "ACompiler/Conversion/ACMidToLLVM/ACMidToLLVM.h"

namespace acompiler {

/// Register all conversion passes
void registerConversionPasses();

} // namespace acompiler

#endif // ACOMPILER_CONVERSION_PASSES_H
