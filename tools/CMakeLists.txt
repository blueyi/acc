function(acompiler_add_tool target source_file)
  add_executable(${target} ${source_file})
  target_link_libraries(${target} PRIVATE ACompiler::Core)

  if(TARGET ACompiler::Runtime)
    target_link_libraries(${target} PRIVATE ACompiler::Runtime)
  endif()
endfunction()

acompiler_add_tool(acompiler-c acompiler-c/main.cpp)
acompiler_add_tool(acompiler-opt acompiler-opt/main.cpp)
acompiler_add_tool(acompiler-translate acompiler-translate/main.cpp)
acompiler_add_tool(acompiler-run acompiler-run/main.cpp)
