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
std::vector<std::string>::iterator custom_prompts_it;

#if 0
// C++ API Example

void CXX_API(const char* model_path) {
  auto model = OgaModel::Create(model_path);
  auto tokenizer = OgaTokenizer::Create(*model);

  const char* prompt = "def is_prime(num):";
  std::cout << "Prompt: " << std::endl << prompt << std::endl;

  auto sequences = OgaSequences::Create();
  tokenizer->Encode(prompt, *sequences);

  auto params = OgaGeneratorParams::Create(*model);
  params->SetSearchOption("max_length", 200);
  params->SetInputSequences(*sequences);

  auto output_sequences = model->Generate(*params);
  auto out_string = tokenizer->Decode(output_sequences->Get(0));

  std::cout << "Output: " << std::endl << out_string << std::endl;
}
#endif

// C API Example

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

void C_API(const char* model_path) {
  int64_t t_start = timer_us();;
  OgaModel* model;
  OgaCreateModel(model_path, &model);

  OgaTokenizer* tokenizer;
  CheckResult(OgaCreateTokenizer(model, &tokenizer));

  size_t current_custom_prompt_index = 0;
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

    OgaSequences* output_sequences;
    CheckResult(OgaGenerate(model, params, &output_sequences));
    int64_t t2 = timer_us();

    size_t sequence_length = OgaSequencesGetSequenceCount(output_sequences, 0);
    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t2 - t1) / 1000.0, 
      (double)(t2 - t1)/ (1000.0 * 1000.0 * sequence_length), 
      sequence_length);

    const int32_t* sequence = OgaSequencesGetSequenceData(output_sequences, 0);
    const char* out_string;
    CheckResult(OgaTokenizerDecode(tokenizer, sequence, sequence_length, &out_string));
    int64_t t3 = timer_us();
    printf("  - Decode time     %9.2fms\n", (double)(t3 - t2) / 1000.0);

    PrintOutput(out_string);

    custom_prompts_it++;

    OgaDestroyString(out_string);
    OgaDestroySequences(output_sequences);
    OgaDestroyGeneratorParams(params);
    OgaDestroySequences(sequences);
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n", (double)t_elapsed / (1000.0 * 1000.0));

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

//  std::cout << "C++ API" << std::endl;
//  CXX_API(argv[1]);

  if (argc > 2) {
    processCustomPromptsFromFile(argv[2]);
    custom_prompts_it = custom_prompts.begin();
  }

  std::cout << "C API" << std::endl;
  C_API(argv[1]);

  return 0;
}