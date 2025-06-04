https://github.com/hoivb612/onnx-genai

==================

C:\llama.cpp\Onnx-GenAI>set path=e:\xCloud_Reuse\Python38.diffusers.v2;%path%

C:\llama.cpp\Onnx-GenAI>build [--use_dml] --skip_wheel

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

Build Onnxruntime

.\build.bat --config RelWithDebInfo --build_shared_lib --parallel --compile_no_warning_as_error --skip_submodule_sync

OR 

Grab it from https://github.com/microsoft/onnxruntime/releases (refresh both include and lib)

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
cl : command line  warning D9002: ignoring unknown option '-std=c++2a' [C:\llama.cpp\phi-Onnx\examples\c\build\phi3.vc
xproj]
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

