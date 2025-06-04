https://github.com/hoivb612/onnx-genai

==================

==========================================================

REM FusionAI14

Prerequisites:
cmake 3.29 
Python3.12 + requests

==========================================================
=============== C ======== P ======= U ===================
==========================================================

d:\onnxruntime>.\build.bat --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync

D:\onnxruntime>dir d:\onnxruntime\*.dll /s
Volume in drive D is New Volume
Volume Serial Number is 3A95-2857
 
Directory of d:\onnxruntime\build\Windows\RelWithDebInfo\RelWithDebInfo
 
12/10/2024  02:19 PM            60,416 custom_op_get_const_input_test_library.dll
12/10/2024  02:19 PM            34,304 custom_op_invalid_library.dll
12/10/2024  02:21 PM            98,304 custom_op_library.dll
12/10/2024  02:19 PM            63,488 custom_op_local_function.dll
12/10/2024  02:26 PM        12,737,024 onnxruntime.dll
12/10/2024  02:19 PM            11,264 onnxruntime_providers_shared.dll
12/10/2024  02:21 PM           133,120 test_execution_provider.dll
               7 File(s)     13,137,920 bytes

D:\onnxgenai>build --config RelWithDebInfo
2024-12-10 17:36:39,432 util.run [INFO] - Running subprocess in 'D:\onnxgenai'
  'e:\cmake\bin\cmake.EXE' -G 'Visual Studio 17 2022' -DCMAKE_BUILD_TYPE=RelWithDebInfo -S 'D:\onnxgenai' -B 'D:\onnxgenai\build\Windows\RelWithDebInfo' -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DUSE_CUDA=OFF -DUSE_ROCM=OFF -DUSE_WEBGPU=OFF -DUSE_DML=OFF -DENABLE_JAVA=OFF -DBUILD_WHEEL=ON
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.26100.
-- The C compiler identification is MSVC 19.41.34123.0
-- The CXX compiler identification is MSVC 19.41.34123.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Using ONNX Runtime package Microsoft.ML.OnnxRuntime version 1.20.0-dev-20241007-1101-407c1ab2e2
-- ORT_HEADER_DIR: D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/ortlib-src/build/native/include
-- ORT_LIB_DIR: D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/ortlib-src/runtimes/win-x64/native
Loading Dependencies URLs ...
Loading Dependencies ...
CMake Deprecation Warning at build/Windows/RelWithDebInfo/_deps/pybind11_project-src/CMakeLists.txt:8 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- pybind11 v2.10.1
CMake Warning (dev) at build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/FindPythonLibsNew.cmake:98 (find_package):
  Policy CMP0148 is not set: The FindPythonInterp and FindPythonLibs modules
  are removed.  Run "cmake --help-policy CMP0148" for policy details.  Use
  the cmake_policy command to set the policy and suppress this warning.

Call Stack (most recent call first):
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/pybind11Tools.cmake:50 (find_package)
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/pybind11Common.cmake:180 (include)
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/CMakeLists.txt:206 (include)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Found PythonInterp: E:/xCloud_Reuse/python312/python.exe (found suitable version "3.12.8", minimum required is "3.6")
-- Found PythonLibs: E:/xCloud_Reuse/python312/libs/python312.lib
-- Performing Test HAS_MSVC_GL_LTCG
-- Performing Test HAS_MSVC_GL_LTCG - Success
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- Performing Test HAS_SDL
-- Performing Test HAS_SDL - Success
-- Performing Test HAS_QSPECTRE
-- Performing Test HAS_QSPECTRE - Failed
-- Performing Test HAS_GUARD_CF
-- Performing Test HAS_GUARD_CF - Success
-- _STATIC_MSVC_RUNTIME_LIBRARY: OFF
-- Fetch json
-- Tokenizer needed.
Test is using C++20
------------------Enabling tests------------------
Test is using C++20
------------------Enabling Python Wheel------------------
Test is using C++20
Setting up wheel files in : D:/onnxgenai/build/Windows/RelWithDebInfo/wheel
------------------Enabling model benchmark------------------
-- Configuring done (65.8s)
-- Generating done (0.3s)
-- Build files have been written to: D:/onnxgenai/build/Windows/RelWithDebInfo
2024-12-10 17:37:45,752 util.run [INFO] - Running subprocess in 'D:\onnxgenai'
  'e:\cmake\bin\cmake.EXE' --build 'D:\onnxgenai\build\Windows\RelWithDebInfo' --config RelWithDebInfo
MSBuild version 17.11.9+a69bbaaf5 for .NET Framework

...

D:\onnxgenai>build --config RelWithDebInfo --skip_wheel
2024-12-10 17:38:27,926 util.run [INFO] - Running subprocess in 'D:\onnxgenai'
  'e:\cmake\bin\cmake.EXE' -G 'Visual Studio 17 2022' -DCMAKE_BUILD_TYPE=RelWithDebInfo -S 'D:\onnxgenai' -B 'D:\onnxgenai\build\Windows\RelWithDebInfo' -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DUSE_CUDA=OFF -DUSE_ROCM=OFF -DUSE_WEBGPU=OFF -DUSE_DML=OFF -DENABLE_JAVA=OFF -DBUILD_WHEEL=OFF
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.26100.
-- Using ONNX Runtime package Microsoft.ML.OnnxRuntime version 1.20.0-dev-20241007-1101-407c1ab2e2
-- ORT_HEADER_DIR: D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/ortlib-src/build/native/include
-- ORT_LIB_DIR: D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/ortlib-src/runtimes/win-x64/native
Loading Dependencies URLs ...
Loading Dependencies ...
CMake Deprecation Warning at build/Windows/RelWithDebInfo/_deps/pybind11_project-src/CMakeLists.txt:8 (cmake_minimum_required):
  Compatibility with CMake < 3.5 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- pybind11 v2.10.1
CMake Warning (dev) at build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/FindPythonLibsNew.cmake:98 (find_package):
  Policy CMP0148 is not set: The FindPythonInterp and FindPythonLibs modules
  are removed.  Run "cmake --help-policy CMP0148" for policy details.  Use
  the cmake_policy command to set the policy and suppress this warning.

Call Stack (most recent call first):
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/pybind11Tools.cmake:50 (find_package)
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/tools/pybind11Common.cmake:180 (include)
  build/Windows/RelWithDebInfo/_deps/pybind11_project-src/CMakeLists.txt:206 (include)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- _STATIC_MSVC_RUNTIME_LIBRARY: OFF
-- Fetch json
-- Tokenizer needed.
Test is using C++20
------------------Enabling tests------------------
Test is using C++20
------------------Enabling Python Wheel------------------
Test is using C++20
------------------Enabling model benchmark------------------
-- Configuring done (13.8s)
-- Generating done (2.7s)
-- Build files have been written to: D:/onnxgenai/build/Windows/RelWithDebInfo
2024-12-10 17:38:44,688 util.run [INFO] - Running subprocess in 'D:\onnxgenai'
  'e:\cmake\bin\cmake.EXE' --build 'D:\onnxgenai\build\Windows\RelWithDebInfo' --config RelWithDebInfo
MSBuild version 17.11.9+a69bbaaf5 for .NET Framework

  Checking File Globs
  1>Checking Build System
  noexcep_operators.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\lib\RelWithDebInfo\noexcep_operators.lib
  ocos_operators.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\lib\RelWithDebInfo\ocos_operators.lib
  ops_registry.cc
  c_api_utils.cc
  c_api_tokenizer.cc
  tokenizer_impl.cc
  c_api_feature_extraction.cc
  speech_extractor.cc
  c_api_processor.cc
  image_processor.cc
  Generating Code...
  image_resample.c
  ortcustomops.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\lib\RelWithDebInfo\ortcustomops.lib
  Building Custom Rule D:/onnxgenai/CMakeLists.txt
  beam_search_scorer.cpp
  config.cpp
  interface.cpp
  generators.cpp
  json.cpp
  logging.cpp
  adapters.cpp
  audio_processor.cpp
  captured_graph_pool.cpp
  debugging.cpp
  decoder_only.cpp
  decoder_only_pipeline.cpp
  embeddings.cpp
  env_utils.cpp
  extra_inputs.cpp
  gpt.cpp
  image_features.cpp
  input_ids.cpp
  kv_cache.cpp
  logits.cpp
  Generating Code...
  Compiling...
  model.cpp
  multi_modal_vision_model.cpp
  position_inputs.cpp
  prompt_image_processor.cpp
  static_buffer.cpp
  utils.cpp
  whisper.cpp
  ort_genai_c.cpp
  runtime_settings.cpp
  search.cpp
  sequences.cpp
  softmax_cpu.cpp
  top_k_cpu.cpp
  Generating Code...
  onnxruntime-genai-static.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\RelWithDebInfo\onnxruntime-genai-static.lib
  Building Custom Rule D:/onnxgenai/src/python/CMakeLists.txt
  python.cpp
     Creating library D:/onnxgenai/build/Windows/RelWithDebInfo/src/python/RelWithDebInfo/onnxruntime_genai.lib and object D:/onnxgena
  i/build/Windows/RelWithDebInfo/src/python/RelWithDebInfo/onnxruntime_genai.exp
  python.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\src\python\RelWithDebInfo\onnxruntime_genai.cp312-win_amd64.pyd
  Building Custom Rule D:/onnxgenai/benchmark/c/CMakeLists.txt
  main.cpp
  options.cpp
  resource_utils.cpp
  Generating Code...
     Creating library D:/onnxgenai/build/Windows/RelWithDebInfo/benchmark/c/RelWithDebInfo/model_benchmark.lib and object D:/onnxgenai
  /build/Windows/RelWithDebInfo/benchmark/c/RelWithDebInfo/model_benchmark.exp
  model_benchmark.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\benchmark\c\RelWithDebInfo\model_benchmark.exe
  Building Custom Rule D:/onnxgenai/CMakeLists.txt
  beam_search_scorer.cpp
  config.cpp
  interface.cpp
  generators.cpp
  json.cpp
  logging.cpp
  adapters.cpp
  audio_processor.cpp
  captured_graph_pool.cpp
  debugging.cpp
  decoder_only.cpp
  decoder_only_pipeline.cpp
  embeddings.cpp
  env_utils.cpp
  extra_inputs.cpp
  gpt.cpp
  image_features.cpp
  input_ids.cpp
  kv_cache.cpp
  logits.cpp
  Generating Code...
  Compiling...
  model.cpp
  multi_modal_vision_model.cpp
  position_inputs.cpp
  prompt_image_processor.cpp
  static_buffer.cpp
  utils.cpp
  whisper.cpp
  ort_genai_c.cpp
  runtime_settings.cpp
  search.cpp
  sequences.cpp
  softmax_cpu.cpp
  top_k_cpu.cpp
  Generating Code...
     Creating library D:/onnxgenai/build/Windows/RelWithDebInfo/RelWithDebInfo/onnxruntime-genai.lib and object D:/onnxgenai/build/Win
  dows/RelWithDebInfo/RelWithDebInfo/onnxruntime-genai.exp
  onnxruntime-genai.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\RelWithDebInfo\onnxruntime-genai.dll
  Building Custom Rule D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/googletest-src/googletest/CMakeLists.txt
  gtest-all.cc
  gtest.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\lib\RelWithDebInfo\gtest.lib
  Building Custom Rule D:/onnxgenai/build/Windows/RelWithDebInfo/_deps/googletest-src/googletest/CMakeLists.txt
  gtest_main.cc
  gtest_main.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\lib\RelWithDebInfo\gtest_main.lib
  Building Custom Rule D:/onnxgenai/test/CMakeLists.txt
  main.cpp
  c_api_tests.cpp
  model_tests.cpp
  sampling_tests.cpp
  sampling_benchmark.cpp
  Generating Code...
     Creating library D:/onnxgenai/build/Windows/RelWithDebInfo/test/RelWithDebInfo/unit_tests.lib and object D:/onnxgenai/build/Windo
  ws/RelWithDebInfo/test/RelWithDebInfo/unit_tests.exp
  unit_tests.vcxproj -> D:\onnxgenai\build\Windows\RelWithDebInfo\RelWithDebInfo\unit_tests.exe
  Building Custom Rule D:/onnxgenai/CMakeLists.txt
2024-12-10 17:43:08,996 util.dependency_resolver [INFO] - Downloading Microsoft.ML.OnnxRuntime version 1.21.0-dev-20241210-1201-b14b4ec703
2024-12-10 17:43:21,335 util.dependency_resolver [INFO] - Downloading Microsoft.ML.OnnxRuntime version 1.21.0-dev-20241210-1201-b14b4ec703
2024-12-10 17:43:32,585 util.run [INFO] - Running subprocess in 'D:\onnxgenai\build\Windows\RelWithDebInfo\test'
  'e:\cmake\bin\ctest.EXE' --build-config RelWithDebInfo --verbose --timeout 10800
UpdateCTestConfiguration  from :D:/onnxgenai/build/Windows/RelWithDebInfo/test/DartConfiguration.tcl
UpdateCTestConfiguration  from :D:/onnxgenai/build/Windows/RelWithDebInfo/test/DartConfiguration.tcl
Test project D:/onnxgenai/build/Windows/RelWithDebInfo/test
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
No tests were found!!!

D:\onnxgenai>dir /s build\*.dll
 Volume in drive D is New Volume
 Volume Serial Number is 3A95-2857

 Directory of D:\onnxgenai\build\Windows\RelWithDebInfo

12/10/2024  05:43 PM        11,659,296 onnxruntime.dll
12/10/2024  05:43 PM            22,064 onnxruntime_providers_shared.dll
               2 File(s)     11,681,360 bytes

 Directory of D:\onnxgenai\build\Windows\RelWithDebInfo\RelWithDebInfo

12/10/2024  05:42 PM         2,020,864 onnxruntime-genai.dll
               1 File(s)      2,020,864 bytes


C:\llama.cpp\phi-Onnx\examples\c\build>cmake .. -DPHI3=ON -DUSE_CXX=ON
-- Building for: Visual Studio 16 2019
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.26100.
-- The C compiler identification is MSVC 19.29.30157.0
-- The CXX compiler identification is MSVC 19.29.30157.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: C:/llama.cpp/phi-Onnx/examples/c/build

C:\llama.cpp\phi-Onnx\examples\c\build>cmake --build . --config RelWithDebInfo --target phi3
Microsoft (R) Build Engine version 16.11.2+f32259642 for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Building Custom Rule C:/llama.cpp/phi-Onnx/examples/c/CMakeLists.txt
  phi3.cpp
  phi3.vcxproj -> C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo\phi3.exe

C:\llama.cpp\phi-Onnx\examples\c\build>cd RelWithDebInfo

C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo>dir
 Volume in drive C has no label.
 Volume Serial Number is C0D3-2B9F

 Directory of C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo

12/10/2024  06:26 PM    <DIR>          .
12/10/2024  06:26 PM    <DIR>          ..
12/10/2024  06:26 PM         2,020,864 onnxruntime-genai.dll
12/10/2024  06:26 PM        12,737,024 onnxruntime.dll
12/10/2024  06:26 PM            11,264 onnxruntime_providers_shared.dll
12/10/2024  06:26 PM            77,824 phi3.exe
12/10/2024  06:26 PM         1,486,848 phi3.pdb
               5 File(s)     16,333,824 bytes
               2 Dir(s)  284,549,308,416 bytes free

C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo>phi3
usage: phi3 model_path

C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo>phi3 c:\llama.cpp\models\Phi3\onnx\cpu_and_mobile\cpu-int4-rtn-block-32
-------------
Hello, Phi-3!
-------------
C++ API
Creating model...
Creating tokenizer...
Prompt: (Use quit() to exit) Or (To terminate current output generation, press Ctrl+C)
Where is Paris?
Generating response...
 Paris is the capital city of France, located in the north-central part of the country. It is situated on the Seine River, at the heart of the ├Äle-de-France region. Paris is not only the most populous city in France but also a major European city and a global center for art, fashion, gastronomy, and culture. Its 19th arrondissement is home to the iconic Eiffel Tower, while the 16th arrondissement is where the city's prestigious museums, including the Louvre and Mus├⌐e d'Orsay, are located.
-------------
Prompt length: 11, New tokens: 127, Time to first: 0.43s, Prompt tokens per second: 25.36 tps, New tokens per second: 9.86 tps
-------------



Prompt: (Use quit() to exit) Or (To terminate current output generation, press Ctrl+C)

==========================================================
================ D ==== M === L ==========================
==========================================================

C:\llama.cpp\Onnx-GenAI>set path=e:\xCloud_Reuse\Python38.diffusers.v2;%path%

C:\llama.cpp\Onnx-GenAI>build --use_dml --skip_wheel

 Directory of C:\llama.cpp\Onnx-GenAI\build\Windows\RelWithDebInfo\RelWithDebInfo

10/02/2024  01:58 PM    <DIR>          .
10/02/2024  01:59 PM    <DIR>          ..
10/02/2024  01:52 PM        33,208,696 onnxruntime-genai-static.lib
10/02/2024  01:52 PM         9,367,552 onnxruntime-genai-static.pdb
10/02/2024  01:58 PM         2,114,560 onnxruntime-genai.dll
10/02/2024  01:58 PM            10,439 onnxruntime-genai.exp
10/02/2024  01:58 PM            18,480 onnxruntime-genai.lib
10/02/2024  01:58 PM        25,186,304 onnxruntime-genai.pdb
               6 File(s)     69,906,031 bytes
               2 Dir(s)  495,532,843,008 bytes free

 Directory of C:\llama.cpp\Onnx-GenAI\build\Windows\RelWithDebInfo

10/02/2024  01:59 PM         3,328,032 D3D12Core.dll
10/02/2024  01:59 PM        18,527,264 DirectML.dll
10/02/2024  01:59 PM        14,613,552 onnxruntime.dll
               3 File(s)     36,468,848 bytes
               0 Dir(s)  495,529,390,080 bytes free

==========================================================

C:\llama.cpp\phi-Onnx\examples\c\build>cmake ..
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.22631.
-- The C compiler identification is MSVC 19.41.34120.0
-- The CXX compiler identification is MSVC 19.41.34120.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.41.34120/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (14.6s)
-- Generating done (0.0s)
-- Build files have been written to: C:/llama.cpp/phi-Onnx/examples/c/build

C:\llama.cpp\phi-Onnx\examples\c\build>cmake --build . --config RelWithDebInfo
MSBuild version 17.11.9+a69bbaaf5 for .NET Framework

  1>Checking Build System
  Building Custom Rule C:/llama.cpp/phi-Onnx/examples/c/CMakeLists.txt
cl : command line  warning D9002: ignoring unknown option '-std=c++2a' [C:\llama.cpp\phi-Onnx\examples\c\build\phi3.vcxproj]
  main.cpp
  phi3.vcxproj -> C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo\phi3.exe
  Building Custom Rule C:/llama.cpp/phi-Onnx/examples/c/CMakeLists.txt

C:\llama.cpp\phi-Onnx\examples\c\build>

 Directory of C:\llama.cpp\phi-Onnx\examples\c\build\RelWithDebInfo

10/02/2024  02:20 PM    <DIR>          .
10/02/2024  11:18 AM    <DIR>          ..
10/02/2024  01:59 PM         3,328,032 D3D12Core.dll
10/02/2024  01:59 PM        18,527,264 DirectML.dll
10/02/2024  01:58 PM         2,114,560 onnxruntime-genai.dll
10/02/2024  01:59 PM        14,613,552 onnxruntime.dll
10/02/2024  11:18 AM           100,864 phi3.exe
10/02/2024  11:18 AM         1,445,888 phi3.pdb
               6 File(s)     40,130,160 bytes

