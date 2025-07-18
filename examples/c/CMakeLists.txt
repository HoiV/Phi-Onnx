cmake_minimum_required(VERSION 3.18.1)

project(ortgenaiapp)
set(CMAKE_CXX_STANDARD 20)

option(USE_CUDA "Build with CUDA support" OFF)
option(USE_CXX "Invoke the C++ example" ON)
option(PHI3 "Build the Phi example" OFF)
option(PHI3V "Build the Phi3v example" OFF)
option(WHISPER "Build the Whisper example" OFF)
option(CPF "Build the cpf example" OFF)

if(USE_CXX)
  add_compile_definitions(USE_CXX)
endif()

if(USE_CUDA)
  include(CheckLanguage)
  enable_language(CUDA)
endif()

set(ORT_GENAI_LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

if(WIN32)
  set(ONNXRUNTIME_GENAI_LIB "onnxruntime-genai.dll")
  set(ONNXRUNTIME_GENAI_DEPENDENCY "*.dll")
elseif(APPLE)
  set(ONNXRUNTIME_GENAI_LIB "libonnxruntime-genai.dylib")
  set(ONNXRUNTIME_GENAI_DEPENDENCY "*.dylib")
else()
  set(ONNXRUNTIME_GENAI_LIB "libonnxruntime-genai.so")
  set(ONNXRUNTIME_GENAI_DEPENDENCY "*.so")
endif()

file(GLOB ort_genai_libs "${CMAKE_SOURCE_DIR}/lib/${ONNXRUNTIME_GENAI_DEPENDENCY}")

function(prepare_executable executable)
  target_link_directories(${executable} PRIVATE ${ORT_GENAI_LIB_DIR})
  target_link_libraries(${executable} PRIVATE ${ONNXRUNTIME_GENAI_LIB})
  target_include_directories(${executable} PRIVATE ${CMAKE_SOURCE_DIR}/include)

  if(USE_CUDA)
    set_target_properties(${executable} PROPERTIES LINKER_LANGUAGE CUDA)
    target_link_libraries(${executable} PRIVATE cublas curand cudart)
  endif()

  target_link_libraries(${executable} PUBLIC onnxruntime-genai)

  foreach(DLL_FILE ${ort_genai_libs})
    add_custom_command(
      TARGET ${executable} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILE} $<TARGET_FILE_DIR:${executable}>
    )
  endforeach()
endfunction()

set(TARGET phi3)
  add_executable(phi3 ${CMAKE_SOURCE_DIR}/src/phi3.cpp)
  prepare_executable(phi3)

set(TARGET phi3v)
  add_executable(phi3v ${CMAKE_SOURCE_DIR}/src/phi3v.cpp)
  prepare_executable(phi3v)

#set(TARGET whisper)
#  add_executable(whisper ${CMAKE_SOURCE_DIR}/src/whisper.cpp)
#  prepare_executable(whisper)

set(TARGET cpf)
  add_executable(cpf ${CMAKE_SOURCE_DIR}/src/cpf.cpp)
  prepare_executable(cpf)

set(TARGET cpf_v2)
  add_executable(cpf_v2 ${CMAKE_SOURCE_DIR}/src/cpf_v2.cpp)
  prepare_executable(cpf_v2)

  set(TARGET cpf_v2_1)
  add_executable(cpf_v2_1 ${CMAKE_SOURCE_DIR}/src/cpf_v2.1.cpp)
  prepare_executable(cpf_v2_1)
