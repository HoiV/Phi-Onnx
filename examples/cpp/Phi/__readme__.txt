 Directory of D:\llama.cpp\Phi-Onnx\examples\cpp\Phi\build\RelWithDebInfo

10/18/2025  05:00 PM    <DIR>          .
10/18/2025  05:27 PM    <DIR>          ..
10/18/2025  04:58 PM           303,104 cpf_v2.exe
10/18/2025  04:58 PM         3,493,888 cpf_v2.pdb
10/18/2025  04:03 PM         3,664,384 onnxruntime-genai.dll
06/04/2025  09:42 AM        12,418,080 onnxruntime.dll
06/04/2025  09:42 AM            22,064 onnxruntime_providers_shared.dll
10/18/2025  04:58 PM            96,768 phi3.exe
10/18/2025  04:58 PM         1,798,144 phi3.pdb


============================================


D:\llama.cpp\Phi-Onnx\examples\cpp\Phi\build>RelWithDebInfo\cpf_v2.exe
usage: RelWithDebInfo\cpf_v2.exe model_path [ep] [cpf]

D:\llama.cpp\Phi-Onnx\examples\cpp\Phi\build>RelWithDebInfo\phi3.exe D:\llama.cpp\models\onnx\Phi-3-mini-4k-instruct-onnx\cpu_and_mobile\cpu-int4-rtn-block-32-acc-level-4
-------------
Hello, Phi-3!
-------------
C++ API
Creating config...
Creating model...
Creating tokenizer...
Prompt: (Use quit() to exit) Or (To terminate current output generation, press Ctrl+C)
What is a hexagon
Generating response...
, and how many sides does it have?

A hexagon is a six-sided polygon, a two-dimensional geometric shape with six straight sides and six angles. The word "hexagon" comes from the Greek words "hex," meaning six, and "gonia," meaning angle. Hexagons can be regular or irregular. A regular hexagon has all sides of equal length and all interior angles equal to 120 degrees, while an irregular hexagon has sides and angles of varying lengths and measures.

Hexagons are commonly found in nature, such as in the honeycomb structure made by bees, where the hexagonal shape allows for efficient use of space and materials. Hexagons are also used in various human-made objects, such as tiles, jewelry, and even in the design of some buildings.

In summary, a hexagon is a six-sided polygon with six straight sides and six angles. The number of sides in a hexagon is six.
-------------
Prompt length: 18, New tokens: 216, Time to first: 0.15s, Prompt tokens per second: 123.17 tps, New tokens per second: 16.81 tps
-------------



Prompt: (Use quit() to exit) Or (To terminate current output generation, press Ctrl+C)
Generating response...
Interrupt signal received. Terminating current session...

D:\llama.cpp\Phi-Onnx\examples\cpp\Phi\build>RelWithDebInfo\cpf_v2.exe D:\llama.cpp\models\onnx\Phi-3-mini-4k-instruct-onnx\cpu_and_mobile\cpu-int4-rtn-block-32-acc-level-4 cpu ..\..\..\c\prompts\cpf_2_Phi-3.txt
-------------
  B612 SLM
-------------
usage: RelWithDebInfo\cpf_v2.exe
model_path = D:\llama.cpp\models\onnx\Phi-3-mini-4k-instruct-onnx\cpu_and_mobile\cpu-int4-rtn-block-32-acc-level-4
execution_provider (ep) = cpu
custom_prompt_file (cpf)= ..\..\..\c\prompts\cpf_2_Phi-3.txt
Creating config...
Creating model...
Creating tokenizer...

> Running with custom prompt => [1/18]["This is Izzy's bedroom"]
  - TTFT   4538.04ms - 122.74 tps (557)
  - Generation time   4290.61ms - 15.15 tps (65)
{
    "answer": "a. Are you sure Mara, what makes you believe that is true?",
    "justification": "Mara has stated that it's Izzy's room, but she seems unsure. Option (a) encourages her to explain her reasoning."
}


> Running with custom prompt => [2/18]["This is bedroom for Izzy"]
  - TTFT   4552.04ms - 122.14 tps (556)
  - Generation time   4555.79ms - 15.15 tps (69)

{
    "answer": "c. That is correct Mara. It is your sister's room",
    "justification": "Mara has given a correct answer, but we are trying to avoid directly asking about her sister. Option (c) acknowledges her correct answer without directly addressing the sister."
}


> Running with custom prompt => [3/18]["This is the bedroom for my sister Izzy"]
  - TTFT   4528.26ms - 123.45 tps (559)
  - Generation time   3749.71ms - 15.20 tps (57)
{
    "answer": "c. That is correct Mara. It is your sister's room",
    "justification": "Mara has now confirmed that the room is her sister's, so it's appropriate to affirm her answer."
}


> Running with custom prompt => [4/18]["Behind this closed door is Izzy's bedroom"]
  - TTFT   4582.18ms - 122.43 tps (561)
  - Generation time   3624.16ms - 15.18 tps (55)
{
    "answer": "c. That is correct Mara. It is your sister's room",
    "justification": "Mara has answered the question correctly. Option (c) acknowledges her correct response and encourages further conversation."
}


> Running with custom prompt => [5/18]["This door leads to the bedroom for my sister Izzy"]
  - TTFT   4596.34ms - 122.05 tps (561)
  - Generation time   3813.88ms - 15.21 tps (58)

{
    "answer": "e. Are you trying to avoid the question?",
    "justification": "Mara's response is vague and doesn't directly answer the question. Option (e) gently prompts her to be more specific."
}


> Running with custom prompt => [6/18]["I have no idea whose room this is"]
  - TTFT   4561.51ms - 122.11 tps (557)
  - Generation time   4281.02ms - 15.18 tps (65)

{
    "answer": "d. Sorry Mara, that is not precise enough. Can you please elaborate.",
    "justification": "Mara's uncertainty about the room's occupant is clear. Option (d) encourages her to share more about her thoughts or feelings."
}


> Running with custom prompt => [7/18]["I don't want to answer the question"]
  - TTFT   4539.00ms - 122.93 tps (558)
  - Generation time   4278.36ms - 15.19 tps (65)

{
    "answer": "e. Are you trying to avoid the question?",
    "justification": "Mara's reluctance to answer the question could be due to her discomfort. Option (e) is a gentle way to encourage her to share her feelings."
}


> Running with custom prompt => [8/18]["This is the broom closet"]
  - TTFT   4545.43ms - 122.32 tps (556)
  - Generation time   4492.90ms - 15.13 tps (68)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates she might be mistaken about the room's identity. Option (b) gently corrects her without directly accusing her of lying."
}


> Running with custom prompt => [9/18]["This door leads to the broom closet"]
  - TTFT   4545.87ms - 122.75 tps (558)
  - Generation time   4892.62ms - 15.12 tps (74)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's statement is incorrect. The door leads to Izzy's room, not a broom closet. Option (b) gently corrects her without directly confronting her."
}


> Running with custom prompt => [10/18]["It is my bedroom"]
  - TTFT   4549.67ms - 121.77 tps (554)
  - Generation time   3810.99ms - 15.22 tps (58)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates a misunderstanding, as her bedroom is downstairs, not upstairs."
}


> Running with custom prompt => [11/18]["This is the bedroom of Mara"]
  - TTFT   4555.76ms - 122.26 tps (557)
  - Generation time   4478.06ms - 15.19 tps (68)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates she might be mistaken about the location of her bedroom. Option (b) gently corrects her without directly mentioning her sister."
}


> Running with custom prompt => [12/18]["This is the bedroom belonging to Mara"]
  - TTFT   4547.49ms - 122.70 tps (558)
  - Generation time   4570.07ms - 15.10 tps (69)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates she might be mistaken about the location of her bedroom. Option (b) gently corrects her without directly accusing her of lying."
}


> Running with custom prompt => [13/18]["Behind this closed door is my bedroom"]
  - TTFT   4582.28ms - 121.77 tps (558)
  - Generation time   3751.28ms - 15.19 tps (57)
{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates a misunderstanding, as her bedroom is downstairs, not upstairs."
}


> Running with custom prompt => [14/18]["This door leads to Mara's bedroom"]
  - TTFT   4553.82ms - 122.75 tps (559)
  - Generation time   4429.20ms - 15.13 tps (67)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's bedroom is downstairs, not upstairs. Option (b) gently corrects her without directly accusing her of lying."
}


> Running with custom prompt => [15/18]["This door leads to the bedroom for Mara"]
  - TTFT   4548.18ms - 122.91 tps (559)
  - Generation time   5127.03ms - 14.82 tps (76)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's statement is incorrect. The door leads to Mara's sister's room, not Mara's. Option (b) gently corrects her without directly confronting her."
}


> Running with custom prompt => [16/18]["Behind this door is my bedroom"]
  - TTFT   4537.96ms - 122.74 tps (557)
  - Generation time   3738.91ms - 15.25 tps (57)
{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's response indicates a misunderstanding, as her bedroom is downstairs, not upstairs."
}


> Running with custom prompt => [17/18]["I believe it is President Obama's office"]
  - TTFT   4529.34ms - 123.42 tps (559)
  - Generation time   4413.04ms - 15.18 tps (67)

{
    "answer": "b. You might be confused Mara, your room is on the ground floor.",
    "justification": "Mara's belief about the room being President Obama's office is incorrect. Option (b) gently corrects her without directly confronting her."
}


> Running with custom prompt => [18/18]["I don't care, go away!"]
  - TTFT   4511.62ms - 123.68 tps (558)
  - Generation time   4164.23ms - 15.13 tps (63)

{
    "answer": "e. Are you trying to avoid the question?",
    "justification": "Mara's response indicates she might be uncomfortable discussing the topic. Option (e) is a gentle way to encourage her to share her feelings."
}



Total elapsed time:        162.29s
