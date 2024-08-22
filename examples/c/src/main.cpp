#include <iostream>
#include <span>
#include <string>
#include <fstream>
#include <time.h>
#include <vector>
#include "ort_genai.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static int64_t timer_freq = 0, timer_start = 0;
void timer_init(void) {
    if (!timer_freq) {
        LARGE_INTEGER t;
        QueryPerformanceFrequency(&t);
        timer_freq = t.QuadPart;

        // The multiplication by 1000 or 1000000 below can cause an overflow if timer_freq
        // and the uptime is high enough.
        // We subtract the program start time to reduce the likelihood of that happening.
        QueryPerformanceCounter(&t);
        timer_start = t.QuadPart;
    }
}

int64_t timer_us(void) {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return ((t.QuadPart-timer_start) * 1000000) / timer_freq;
}

std::string custom_template_prompt;
std::vector<std::string> custom_prompts;

void CheckResult(OgaResult* result) {
  if (result) {
    std::string string=OgaResultGetError(result);
    OgaDestroyResult(result);
    throw std::runtime_error(string);
  }
}

bool processCustomPromptsFromFile(const char *cpf) {
    std::string custom_p_file(cpf);
    std::ifstream cpfile(custom_p_file);
    if (!cpfile.is_open()) {
        printf("[%s]: failed to open [%s]\n", __func__, custom_p_file.c_str());
        return false;
    }

    std::string line;

    bool templatePromptMode = false;
    bool userPromptMode = false;
    custom_prompts.clear();
    custom_template_prompt = "";

    // process CUSTOM_SYSTEM_PROMPT
    while (std::getline(cpfile, line)) {
        if (line == "CUSTOM_TEMPLATE_PROMPT") {
            templatePromptMode = true;
            continue;
        } else if (line == "CUSTOM_PROMPT") {
            userPromptMode = true;
            continue;
        } else if (line == "END_SECTION") {
            templatePromptMode = false;
            userPromptMode = false;
            continue;
        }

        if (templatePromptMode) {
            custom_template_prompt += line + '\n';
        } else if (userPromptMode) {
            custom_prompts.push_back(line);
        }
    }

    cpfile.close();

    return true;
}

void PrintOutput(const char *output_string) {
  size_t pos = 0;
  std::string reply(output_string);
  std::string delimiter("-response-\nYou: \n");
  std::string result;
  
  pos = reply.find(delimiter);
  if (pos != std::string::npos) {
    result = reply.substr(pos);
  }

  result.erase(0, delimiter.length());
  
  pos = result.find("}");
  if (pos != std::string::npos) {
    result.erase(pos + 1);
  }

  std::cout << "Output: " << std::endl
            << result << std::endl;
}

// C++ API Example

void CXX_API(const char* model_path) {
  int64_t t_start = timer_us();
  auto model = OgaModel::Create(model_path);
  auto tokenizer = OgaTokenizer::Create(*model);

  size_t current_custom_prompt_index = 0;
  std::vector<std::string>::iterator custom_prompts_it = custom_prompts.begin();

  while (custom_prompts_it != custom_prompts.end()) {
    // Create custom user prompt
    std::string& custom_prompt = *custom_prompts_it;
    printf("\n"
           "> Running with custom prompt => [%zd/%zd][%s]\n", 
           ++current_custom_prompt_index, 
           custom_prompts.size(),
           custom_prompt.c_str());

    std::string full_custom_prompt = custom_template_prompt;
    size_t pos = full_custom_prompt.find("{message}");
    if (pos != std::string::npos) {
        full_custom_prompt.replace(pos, std::string("{message}").length(), custom_prompt);
    }

    int64_t t0 = timer_us();
    auto sequences = OgaSequences::Create();
    tokenizer->Encode(full_custom_prompt.c_str(), *sequences);
    int64_t t1 = timer_us();
    printf("  - Encode time     %9.2fms\n", (double)(t1 - t0) / 1000.);

    auto params = OgaGeneratorParams::Create(*model);
    params->SetSearchOption("max_length", 1024);
    params->SetInputSequences(*sequences);
    auto output_sequences = model->Generate(*params);

    const auto output_token_generated = output_sequences->SequenceCount(0);

    int64_t t2 = timer_us();
    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t2 - t1) / 1000.0, 
      (double)(t2 - t1)/ (1000.0 * 1000.0 * output_token_generated), 
      output_token_generated);

    const auto* output_sequence_data = output_sequences->SequenceData(0);
    auto out_string = tokenizer->Decode(output_sequence_data, output_token_generated);

    int64_t t3 = timer_us();
    printf("  - Decode time     %9.2fms\n", (double)(t3 - t2) / 1000.0);

    std::cout << "Output: " << std::endl
              << out_string << std::endl;

    custom_prompts_it++;

    OgaDestroyString(out_string);
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n\n", (double)t_elapsed / (1000.0 * 1000.0));
}

void CXX_API_Generator(const char* model_path) {
  int64_t t_start = timer_us();
  auto model = OgaModel::Create(model_path);
  auto tokenizer = OgaTokenizer::Create(*model);
  auto tokenizer_stream = OgaTokenizerStream::Create(*tokenizer);

  size_t current_custom_prompt_index = 0;
  std::vector<std::string>::iterator custom_prompts_it = custom_prompts.begin();

  while (custom_prompts_it != custom_prompts.end()) {
    // Create custom user prompt
    std::string& custom_prompt = *custom_prompts_it;
    printf("\n"
           "> Running with custom prompt => [%zd/%zd][%s]\n", 
           ++current_custom_prompt_index, 
           custom_prompts.size(),
           custom_prompt.c_str());

    std::string full_custom_prompt = custom_template_prompt;
    size_t pos = full_custom_prompt.find("{message}");
    if (pos != std::string::npos) {
        full_custom_prompt.replace(pos, std::string("{message}").length(), custom_prompt);
    }

    int64_t t0 = timer_us();
    auto sequences = OgaSequences::Create();
    tokenizer->Encode(full_custom_prompt.c_str(), *sequences);
    int64_t t1 = timer_us();
    printf("  - Encode time     %9.2fms\n", (double)(t1 - t0) / 1000.);

    auto params = OgaGeneratorParams::Create(*model);
    params->SetSearchOption("max_length", 1024);
    params->SetInputSequences(*sequences);

    auto generator = OgaGenerator::Create(*model, *params);
    std::string out_string;
    size_t tokens_generated = 0;

    int64_t t2 = timer_us();

    while (!generator->IsDone()) {
      generator->ComputeLogits();
      generator->GenerateNextToken();

      const auto num_tokens = generator->GetSequenceCount(0);
      const auto new_token = generator->GetSequenceData(0)[num_tokens - 1];
      tokens_generated++;

      auto new_token_string = tokenizer_stream->Decode(new_token);
      // std::cout << new_token_string << std::flush;
      out_string += new_token_string;
      if (strchr(new_token_string, '}') != NULL) {
        // we can stop now
        break;
      }
    }

    int64_t t3 = timer_us();
    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t3 - t2) / 1000.0, 
      tokens_generated / ((double)(t3 - t2)/ (1000.0 * 1000.0)), 
      tokens_generated);

    printf("%s\n\n", out_string.c_str());

    custom_prompts_it++;
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n\n", (double)t_elapsed / (1000.0 * 1000.0));
}

// C API Example

void C_API(const char* model_path) {
  int64_t t_start = timer_us();
  OgaModel* model;
  OgaCreateModel(model_path, &model);

  OgaTokenizer* tokenizer;
  CheckResult(OgaCreateTokenizer(model, &tokenizer));

  size_t current_custom_prompt_index = 0;
  std::vector<std::string>::iterator custom_prompts_it = custom_prompts.begin();

  while (custom_prompts_it != custom_prompts.end()) {
    // Create custom user prompt
    std::string& custom_prompt = *custom_prompts_it;
    printf("\n"
           "> Running with custom prompt => [%zd/%zd][%s]\n", 
           ++current_custom_prompt_index, 
           custom_prompts.size(),
           custom_prompt.c_str());

    std::string full_custom_prompt = custom_template_prompt;
    size_t pos = full_custom_prompt.find("{message}");
    if (pos != std::string::npos) {
        full_custom_prompt.replace(pos, std::string("{message}").length(), custom_prompt);
    }

    int64_t t0 = timer_us();
    OgaSequences* sequences;
    CheckResult(OgaCreateSequences(&sequences));
    CheckResult(OgaTokenizerEncode(tokenizer, full_custom_prompt.c_str(), sequences));
    int64_t t1 = timer_us();
    printf("  - Encode time     %9.2fms\n", (double)(t1 - t0) / 1000.);

    OgaGeneratorParams* params;
    CheckResult(OgaCreateGeneratorParams(model, &params));
    CheckResult(OgaGeneratorParamsSetSearchNumber(params, "max_length", 1024));
    CheckResult(OgaGeneratorParamsSetInputSequences(params, sequences));

    OgaSequences* output_sequences = NULL;
    const int32_t* sequence = NULL;
    size_t token_generated = 0;

    CheckResult(OgaGenerate(model, params, &output_sequences));
    token_generated = OgaSequencesGetSequenceCount(output_sequences, 0);
    sequence = OgaSequencesGetSequenceData(output_sequences, 0);

    int64_t t2 = timer_us();

    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t2 - t1) / 1000.0, 
      (double)(t2 - t1)/ (1000.0 * 1000.0 * token_generated), 
      token_generated);

    const char* out_string;
    CheckResult(OgaTokenizerDecode(tokenizer, sequence, token_generated, &out_string));
    int64_t t3 = timer_us();
    printf("  - Decode time     %9.2fms\n", (double)(t3 - t2) / 1000.0);

    PrintOutput(out_string);

    custom_prompts_it++;

    OgaDestroyString(out_string);
    OgaDestroyGeneratorParams(params);
    OgaDestroySequences(sequences);
    OgaDestroySequences(output_sequences);
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n\n", (double)t_elapsed / (1000.0 * 1000.0));

  OgaDestroyTokenizer(tokenizer);
  OgaDestroyModel(model);
}

void C_API_Generator(const char* model_path) {
  int64_t t_start = timer_us();
  OgaModel* model;
  OgaCreateModel(model_path, &model);

  OgaTokenizer* tokenizer;
  CheckResult(OgaCreateTokenizer(model, &tokenizer));

  OgaTokenizerStream* tokenizer_stream;
  CheckResult(OgaCreateTokenizerStream(tokenizer, &tokenizer_stream));

size_t current_custom_prompt_index = 0;
  std::vector<std::string>::iterator custom_prompts_it = custom_prompts.begin();

  while (custom_prompts_it != custom_prompts.end()) {
    // Create custom user prompt
    std::string& custom_prompt = *custom_prompts_it;
    printf("\n"
           "> Running with custom prompt => [%zd/%zd][%s]\n", 
           ++current_custom_prompt_index, 
           custom_prompts.size(),
           custom_prompt.c_str());

    std::string full_custom_prompt = custom_template_prompt;
    size_t pos = full_custom_prompt.find("{message}");
    if (pos != std::string::npos) {
        full_custom_prompt.replace(pos, std::string("{message}").length(), custom_prompt);
    }

    int64_t t0 = timer_us();

    OgaSequences* sequences;
    CheckResult(OgaCreateSequences(&sequences));
    CheckResult(OgaTokenizerEncode(tokenizer, full_custom_prompt.c_str(), sequences));
 
    int64_t t1 = timer_us();
    printf("  - Encode time     %9.2fms\n", (double)(t1 - t0) / 1000.);

    OgaGeneratorParams* params;
    CheckResult(OgaCreateGeneratorParams(model, &params));
    CheckResult(OgaGeneratorParamsSetSearchNumber(params, "max_length", 1024));
    CheckResult(OgaGeneratorParamsSetInputSequences(params, sequences));

    OgaGenerator *generator = NULL;
    const int32_t* sequence = NULL;
    size_t tokens_generated = 0;

    std::string out_string;
    CheckResult(OgaCreateGenerator(model, params, &generator));

    int64_t t2 = timer_us();

    while (!OgaGenerator_IsDone(generator)) {
      CheckResult(OgaGenerator_ComputeLogits(generator));
      CheckResult(OgaGenerator_GenerateNextToken(generator));

      const int32_t num_tokens = OgaGenerator_GetSequenceCount(generator, 0);
      int32_t new_token = OgaGenerator_GetSequenceData(generator, 0)[num_tokens - 1];
      tokens_generated++;

      const char* new_token_string;
      CheckResult(OgaTokenizerStreamDecode(tokenizer_stream, new_token, &new_token_string));
      // std::cout << new_token_string << std::flush;
      out_string += new_token_string;
      if (strchr(new_token_string, '}') != NULL) {
        // we can stop now
        break;
      }
    }

    int64_t t3 = timer_us();
    printf("%s\n\n", out_string.c_str());

    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t3 - t2) / 1000.0, 
      tokens_generated / ((double)(t3 - t2)/ (1000.0 * 1000.0)), 
      tokens_generated);

    custom_prompts_it++;

    OgaDestroyGeneratorParams(params);
    OgaDestroySequences(sequences);
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n\n", (double)t_elapsed / (1000.0 * 1000.0));

  OgaDestroyTokenizerStream(tokenizer_stream);
  OgaDestroyTokenizer(tokenizer);
  OgaDestroyModel(model);
}

static void print_usage(int /*argc*/, char** argv) {
  std::cerr << "usage: " << argv[0] << " model_path [custom_prompt_file] " << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage(argc, argv);
    return -1;
  }

  std::cout << "-------------" << std::endl;
  std::cout << "Hello, Phi-3!" << std::endl;
  std::cout << "-------------" << std::endl;

  timer_init();

  if (argc > 2) {
    processCustomPromptsFromFile(argv[2]);
  }

  std::cout << "-------" << std::endl;
  std::cout << "C++ API" << std::endl;
  std::cout << "-------" << std::endl;
  //CXX_API(argv[1]); // This construct generates no output
  CXX_API_Generator(argv[1]);

  std::cout << "-----" << std::endl;
  std::cout << "C API" << std::endl;
  std::cout << "-----" << std::endl;
  // C_API(argv[1]); // This construct generates no output
  C_API_Generator(argv[1]);

  return 0;
}

/*
# Compiled library
import onnxruntime_genai as engine

# Option 1: Phi-3 mini 4K instruct model path
model_path = "Phi-3-mini-4k-instruct-onnx/cpu_and_mobile/cpu-int4-rtn-block-32-acc-level-4"

# Option 2: Uncomment the following line to use the Mistral 7B instruct v0.2 model path
# model_path = "mistral-7b-instruct-v0.2-ONNX/onnx/cpu_and_mobile/mistral-7b-instruct-v0.2-cpu-int4-rtn-block-32-acc-level-4"

# Option 3: Uncomment the following line to use the Phi-3 medium 128K instruct model path
# model_path = "Phi-3-medium-128k-instruct-onnx-cpu/cpu-int4-rtn-block-32-acc-level-4"

print(f"Loading {model_path} model\n")

model = engine.Model(f'{model_path}')
tokenizer = engine.Tokenizer(model)
tokenizer_stream = tokenizer.create_stream()

# Set the maximum number of tokens to generate
# The full example includes additional parameters for more complex configurations
search_options = {'max_length': 2048}

# Template for formatting prompt messages in the script
chat_tpl = '<|user|>\n{input}<|end|>\n<|assistant|>'

print("Let's chat!\n")

# Chat loop. Exit with Ctrl+C
while True:
    text = input("> User: ")
    if not text:
        print("Please, answer something")
        continue

    # Populate the chat template with user input
    prompt = f'{chat_tpl.format(input=text)}'

    input_tokens = tokenizer.encode(prompt)

    gen_params = engine.GeneratorParams(model)
    gen_params.set_search_options(**search_options)
    gen_params.input_ids = input_tokens
    generator = engine.Generator(model, gen_params)

    print("\n> Assistant: ", end='', flush=True)

    try:
        # Loop to generate and display answer tokens
        while not generator.is_done():
            generator.compute_logits()
            generator.generate_next_token()
            next_token = generator.get_next_tokens()[0]
            print(tokenizer_stream.decode(next_token), end='', flush=True)
        print('\n')
    except KeyboardInterrupt:
        print("\nCtrl+C pressed, break\n")

# Delete the generator
del generator
*/