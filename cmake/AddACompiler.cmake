# CMake helper functions for ACompiler

# Add ACompiler library
function(add_acompiler_library name)
  cmake_parse_arguments(ARG
    "SHARED"
    ""
    "DEPENDS;LINK_LIBS"
    ${ARGN}
    )

  set(srcs ${ARG_UNPARSED_ARGUMENTS})
  
  if(ARG_SHARED)
    add_library(${name} SHARED ${srcs})
  else()
    add_library(${name} STATIC ${srcs})
  endif()

  if(ARG_DEPENDS)
    add_dependencies(${name} ${ARG_DEPENDS})
  endif()

  if(ARG_LINK_LIBS)
    target_link_libraries(${name} ${ARG_LINK_LIBS})
  endif()

  set_target_properties(${name} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )
endfunction()

# Add ACompiler executable
function(add_acompiler_executable name)
  cmake_parse_arguments(ARG
    ""
    ""
    "DEPENDS;LINK_LIBS"
    ${ARGN}
    )

  set(srcs ${ARG_UNPARSED_ARGUMENTS})
  
  add_executable(${name} ${srcs})

  if(ARG_DEPENDS)
    add_dependencies(${name} ${ARG_DEPENDS})
  endif()

  if(ARG_LINK_LIBS)
    target_link_libraries(${name} ${ARG_LINK_LIBS})
  endif()

  set_target_properties(${name} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )
endfunction()

# Add ACompiler Dialect using TableGen
function(add_acompiler_dialect dialect_name)
  set(LLVM_TARGET_DEFINITIONS ${dialect_name}.td)
  
  mlir_tablegen(${dialect_name}.h.inc -gen-op-decls)
  mlir_tablegen(${dialect_name}.cpp.inc -gen-op-defs)
  mlir_tablegen(${dialect_name}Dialect.h.inc -gen-dialect-decls)
  mlir_tablegen(${dialect_name}Dialect.cpp.inc -gen-dialect-defs)
  
  add_public_tablegen_target(MLIR${dialect_name}IncGen)
  add_dependencies(mlir-headers MLIR${dialect_name}IncGen)
endfunction()

# Add ACompiler Pass using TableGen
function(add_acompiler_pass_gen)
  set(LLVM_TARGET_DEFINITIONS Passes.td)
  mlir_tablegen(Passes.h.inc -gen-pass-decls)
  add_public_tablegen_target(ACompilerPassIncGen)
endfunction()
