cmake_minimum_required(VERSION 3.18.1)

project(Ryzen_ortgenai)
set(CMAKE_CXX_STANDARD 20)

if(USE_CXX)
  add_compile_definitions(USE_CXX)
endif()

set(ORT_GENAI_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
  set(ONNXRUNTIME_GENAI_LIB "onnxruntime-genai.dll")
  set(ONNXRUNTIME_GENAI_DEPENDENCY "*.dll")
else()
  set(ONNXRUNTIME_GENAI_LIB "libonnxruntime-genai.so")
  set(ONNXRUNTIME_GENAI_DEPENDENCY "*.so")
endif()

file(GLOB ort_genai_libs "${CMAKE_SOURCE_DIR}/lib/${ONNXRUNTIME_GENAI_DEPENDENCY}")

function(prepare_executable executable)
  target_link_directories(${executable} PRIVATE ${ORT_GENAI_LIB_DIR})
  target_link_libraries(${executable} PRIVATE ${ONNXRUNTIME_GENAI_LIB})
  target_include_directories(${executable} PRIVATE ${CMAKE_SOURCE_DIR}/include)

  target_link_libraries(${executable} PUBLIC onnxruntime-genai)

  foreach(DLL_FILE ${ort_genai_libs})
    add_custom_command(
      TARGET ${executable} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILE} $<TARGET_FILE_DIR:${executable}>
    )
  endforeach()
endfunction()

set(TARGET run_llm)
  add_executable(run_llm ${CMAKE_SOURCE_DIR}/run_llm.cpp ${CMAKE_SOURCE_DIR}/util/getopt.c)
  prepare_executable(run_llm)

