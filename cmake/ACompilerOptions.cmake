# ACompilerOptions.cmake
# Project-specific CMake options and default configurations

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Option: Use LLVM's tablegen (mlir-tblgen) for code generation
option(ACOMPILER_USE_TABLEGEN "Use TableGen for ODS generation" ON)

# Option: Build with assertions (Debug-like)
option(ACOMPILER_ENABLE_ASSERTIONS "Enable assertions" OFF)

if(ACOMPILER_ENABLE_ASSERTIONS)
  add_compile_definitions(ACOMPILER_ENABLE_ASSERTIONS=1)
endif()
