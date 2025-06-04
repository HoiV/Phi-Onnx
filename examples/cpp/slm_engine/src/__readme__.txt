C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build>cmake ..

C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build>cmake --build . --config RelWithDebInfo --target slm-runner
CMake is re-running because C:/llama.cpp/phi-Onnx/examples/cpp/slm_engine/src/build/CMakeFiles/generate.stamp is out-of-date.
  the file 'C:/llama.cpp/phi-Onnx/examples/cpp/slm_engine/src/CMakeLists.txt'
  is newer than 'C:/llama.cpp/phi-Onnx/examples/cpp/slm_engine/src/build/CMakeFiles/generate.stamp.depend'
  result='-1'
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.26100.
-- Configuring done (0.2s)
-- Generating done (0.2s)
-- Build files have been written to: C:/llama.cpp/phi-Onnx/examples/cpp/slm_engine/src/build
MSBuild version 17.13.26+f3741e9cf for .NET Framework

  slm_runner.cpp
  slm_engine.cpp
  input_decoder.cpp
  Generating Code...
  slm-runner.vcxproj -> C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build\RelWithDebInfo\slm-runner.exe

C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build>

===================================

C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build>RelWithDebInfo\slm-runner -t ..\..\test\10-inputs-to-slm.jsonl -o .\output.jsonl -m c:\llama.cpp\models\Phi-3\Phi-3-mini-4k-instruct-onnx\cpu_and_mobile\cpu-int4-rtn-block-32-acc-level-4
The requested API version [23] is not available, only API versions [1, 22] are supported in this build. Current ORT Version is: 1.22.0
SLM Runner Version: 1.0
ORT GenAI Version: 0.9.0-dev
ORT Version: 1.22.0
Model: c:\llama.cpp\models\Phi-3\Phi-3-mini-4k-instruct-onnx\cpu_and_mobile\cpu-int4-rtn-block-32-acc-level-4
Test File: ..\..\test\10-inputs-to-slm.jsonl
Question: "What makes a cloud form?"
Answer: " Clouds form when warm, moist air rises and cools, causing the water vapor in the air to condense into tiny water droplets or ice crystals. This process usually occurs in the atmosphere when air currents, such as those created by wind or convection, cause the air to rise. As the air ascends, it expands and cools, reaching its dew point and leading to condensation. The condensed water droplets or ice crystals cluster together around microscopic particles called condensation nuclei, eventually forming visible clouds. Factors like temperature, humidity, and atmospheric pressure also play crucial roles in cloud formation."
Prompt Tokens: 25 TTFT: 0.321 sec Generated: 148 Token Rate: 18 Time: 8.561 sec Memory: 3535 MB
Question: "Can drink and food taste different just by changing its color?"
Answer: "While color can influence our perception of taste to some extent, it doesn't directly change the taste of food or drink. However, our brain can associate certain colors with specific flavors, which can affect our expectations and perception of taste. For example, we might expect a bright red drink to taste like cherry or a green smoothie to taste like grass. But the actual taste will still depend on the ingredients and flavors present in the food or drink."
Prompt Tokens: 31 TTFT: 0.377 sec Generated: 99 Token Rate: 18 Time: 5.813 sec Memory: 3541 MB
Question: "What makes popcorn pop?"
Answer: "Popcorn pops due to the presence of water molecules inside the kernel's hard outer shell, called the pericarp. When heated, the water turns into steam, and the pressure from the steam causes the pericarp to rupture, allowing the starchy endosperm to expand and turn inside out, forming the fluffy popcorn we enjoy. The popcorn kernels have a unique structure with a starchy endosperm surrounded by a hard and moisture-resistant hull. The heat from the stove or microwave causes the water inside the kernel to heat up and turn into steam, and the steam builds up pressure until the hull can no longer contain it, resulting in the characteristic \"pop\" sound."
Prompt Tokens: 26 TTFT: 0.36 sec Generated: 161 Token Rate: 18 Time: 9.365 sec Memory: 3542 MB
Question: "Does temperature affect seed sprouting?"
Answer: "Yes, temperature does affect seed sprouting. Seeds have specific temperature ranges in which they can germinate, and these ranges vary among different plant species. Generally, most seeds require warm temperatures (between 70┬░F and 85┬░F or 21┬░C and 29┬░C) for optimal germination. Extreme temperatures, either too hot or too cold, can inhibit or delay sprouting. Some seeds may require stratification (exposure to cold temperatures) or scarification (exposure to warm temperatures) to break dormancy and initiate germination."
Prompt Tokens: 27 TTFT: 0.337 sec Generated: 135 Token Rate: 18 Time: 7.815 sec Memory: 3543 MB
Question: "Does the color of light affect photosynthesis?"
Answer: "Yes, the color of light does affect photosynthesis. Photosynthesis primarily uses red and blue light, as these wavelengths are most efficiently absorbed by chlorophyll, the pigment responsible for capturing light energy. Green light is least effective, as it is mostly reflected, which is why plants appear green. However, some wavelengths, such as far-red and blue-green light, can also be used by plants for photosynthesis, albeit less efficiently."
Prompt Tokens: 29 TTFT: 0.362 sec Generated: 106 Token Rate: 18 Time: 6.214 sec Memory: 3543 MB
Question: "Does exercise affect memory?"
Answer: "Yes, exercise can positively affect memory. Regular physical activity has been linked to improved cognitive function, including memory. Exercise increases blood flow to the brain, which can help nourish brain cells and promote the growth of new neurons. Aerobic exercise, in particular, has been shown to enhance memory and cognitive abilities, especially in older adults. Additionally, exercise can reduce stress and anxiety, which can further contribute to better memory function."
Prompt Tokens: 24 TTFT: 0.312 sec Generated: 99 Token Rate: 18 Time: 5.723 sec Memory: 3544 MB
Question: "Can fruits and vegetables generate electricity?"
Answer: "Yes, fruits and vegetables can generate electricity through a process called bio-electrogenesis. This involves using microorganisms, such as bacteria, that can break down organic matter in fruits and vegetables to produce electricity. However, this is typically done on a small scale and is not yet a widely-used method for generating electricity. Most commonly, bio-electrogenesis is researched for potential applications in wastewater treatment and bioremediation."
Prompt Tokens: 29 TTFT: 0.364 sec Generated: 103 Token Rate: 18 Time: 6.02 sec Memory: 3544 MB
Question: "Can you protect a raw egg from a drop using common materials?"
Answer: "Yes, you can protect a raw egg from a drop using common materials. One popular method is to create a \"sac\" using a plastic bag filled with crumpled newspaper or bubble wrap. Place the egg gently inside the bag, and then suspend the bag from a string or rubber band. When dropped, the bag will crumple and absorb the impact, reducing the force on the egg. However, please note that this method is not foolproof and the success rate may vary."
Prompt Tokens: 32 TTFT: 0.392 sec Generated: 107 Token Rate: 18 Time: 6.298 sec Memory: 3545 MB
Question: "What happens when a raw egg is put into vinegar?"
Answer: "When a raw egg is placed in vinegar, the acetic acid in the vinegar reacts with the calcium carbonate in the eggshell. This reaction gradually dissolves the eggshell, leaving behind the semi-permeable membrane that surrounds the egg white and yolk. As a result, the eggshell becomes translucent and eventually disappears, and the egg becomes rubbery and soft. This process can take several days, depending on the size of the egg and the concentration of the vinegar."
Prompt Tokens: 32 TTFT: 0.385 sec Generated: 112 Token Rate: 18 Time: 6.558 sec Memory: 3546 MB
Question: "What drink causes more stains on teeth?"
Answer: "Carbonated drinks, especially those high in sugar and acid, such as soda, can cause more stains on teeth due to their erosive effects on enamel and the potential for sugar to stick to teeth. However, it's essential to maintain good oral hygiene and limit the consumption of such drinks to minimize staining."
Prompt Tokens: 28 TTFT: 0.357 sec Generated: 76 Token Rate: 18 Time: 4.485 sec Memory: 3546 MB

C:\llama.cpp\phi-Onnx\examples\cpp\slm_engine\src\build>
