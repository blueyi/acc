add_executable(acompilerc
  acompilerc.cpp
)

target_link_libraries(acompilerc
  PRIVATE
    LLVMSupport
)

