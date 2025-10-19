#include <iostream>
#include <span>
#include <string>
#include <cstring>
#include <fstream>
#include <time.h>
#include <vector>
#include <thread>
#include <csignal>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <functional>

#include "ort_genai.h"
#include "common.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static TerminateSession catch_terminate;
 
void signalHandlerWrapper(int signum) {
  printf("--> %s: %d\n", __func__, signum);
  catch_terminate.signalHandler(signum);
}
 
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
  if (!cpf || !*cpf) return false;
  std::string custom_p_file(cpf);
  std::ifstream cpfile(custom_p_file);
  if (!cpfile) {
    printf("[%s]: failed to open [%s]\n", __func__, custom_p_file.c_str());
    return false;
  }

  enum class Mode { None, Template, User } mode = Mode::None;
  custom_prompts.clear();
  custom_template_prompt.clear();

  std::string line;
  std::string template_accum;
  template_accum.reserve(4096);

  auto strip_cr = [](std::string &s) {
    if (!s.empty() && s.back() == '\r') s.pop_back();
  };
  auto strip_utf8_bom = [](std::string &s) {
    if (s.size() >= 3 && (unsigned char)s[0] == 0xEF && (unsigned char)s[1] == 0xBB && (unsigned char)s[2] == 0xBF)
      s.erase(0, 3);
  };
  auto trimmed_key = [](const std::string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
  };

  while (std::getline(cpfile, line)) {
    strip_cr(line);
    strip_utf8_bom(line);

    std::string key = trimmed_key(line);
    if (key == "CUSTOM_TEMPLATE_PROMPT") { mode = Mode::Template; continue; }
    if (key == "CUSTOM_PROMPT") { mode = Mode::User; continue; }
    if (key == "END_SECTION") { mode = Mode::None; continue; }

    if (mode == Mode::Template) {
      template_accum.append(line);
      template_accum.push_back('\n');
    } else if (mode == Mode::User) {
      custom_prompts.emplace_back(std::move(line));
    }
  }

  custom_template_prompt = std::move(template_accum);
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

void Onnx_Generator(const char* model_path, const char* execution_provider) {
  // signal(SIGINT, signalHandlerWrapper);

  std::cout << "Creating config..." << std::endl;
  auto config = OgaConfig::Create(model_path);

  config->ClearProviders();
  std::string provider(execution_provider);
  if (provider.compare("cpu") != 0) {
    config->AppendProvider(execution_provider);
    if (provider.compare("cuda") == 0) {
      config->SetProviderOption(execution_provider, "enable_cuda_graph", "0");
    }
  }

  OgaHandle ogaHandle; // for proper shutdown :-(
  int64_t t_start = timer_us();

  std::cout << "Creating model..." << std::endl;
  auto model = OgaModel::Create(*config);

  std::cout << "Creating tokenizer..." << std::endl;
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
    // const std::string prompt = "<|user|>\n" + text + "<|end|>\n<|assistant|>";
    bool is_first_token = true;
    int64_t t0 = timer_us();
  
    auto sequences = OgaSequences::Create();
    tokenizer->Encode(full_custom_prompt.c_str(), *sequences);
    auto params = OgaGeneratorParams::Create(*model);
    params->SetSearchOption("max_length", 1024);

    auto generator = OgaGenerator::Create(*model, *params);
    // std::thread th(std::bind(&TerminateSession::Generator_SetTerminate_Call, &catch_terminate, generator.get()));

    generator->AppendTokenSequences(*sequences);

    std::string out_string;
    size_t tokens_generated = 0;
    int64_t t_gen_tokens = 0;

    try {

      auto num_tokens = generator->GetSequenceCount(0);
      while (!generator->IsDone()) {
        generator->GenerateNextToken();

        if (is_first_token) {
          int64_t t1 = timer_us();
          printf("  - TTFT %9.2fms - %5.2f tps (%zd)\n", 
            (double)(t1 - t0) / 1000.,
            (num_tokens * 1000.0 * 1000.0) / (t1 - t0),
            num_tokens);

          // timing.RecordFirstTokenTimestamp();
          is_first_token = false;
          t_gen_tokens = timer_us();
        }

#if 0
        // Show usage of GetOutput
        std::unique_ptr<OgaTensor> output_logits = generator->GetOutput("logits");

        // Assuming output_logits.Type() is float as it's logits
        // Assuming shape is 1 dimensional with shape[0] being the size
        auto logits = reinterpret_cast<float*>(output_logits->Data());

        // Print out the logits using the following snippet, if needed
        //auto shape = output_logits->Shape();
        //for (size_t i=0; i < shape[0]; i++)
        //   std::cout << logits[i] << " ";
        //std::cout << std::endl;
#endif

        num_tokens = generator->GetSequenceCount(0);
        const auto new_token = generator->GetSequenceData(0)[num_tokens - 1];
        tokens_generated += 1;

        auto new_token_string = tokenizer_stream->Decode(new_token);

        // std::cout << new_token_string << std::flush;
        out_string += new_token_string;
        if (strchr(new_token_string, '}') != NULL) {
          // found the marker so stop now
          break;
        }
      } // while

    } // try
    catch (const std::exception& e) {
      std::cout << "Session Terminated: " << e.what() << std::endl;
    }

    int64_t t_done = timer_us();
    printf("  - Generation time %9.2fms - %5.2f tps (%zd)\n",
      (double)(t_done - t_gen_tokens) / 1000.0, 
      tokens_generated / ((double)(t_done - t_gen_tokens)/ (1000.0 * 1000.0)), 
      tokens_generated);

    printf("%s\n\n", out_string.c_str());

    custom_prompts_it++;

    //if (th.joinable()) {
    //  th.join();  // Join the thread if it's still running
    //}
  }

  int64_t t_elapsed = timer_us() - t_start;
  printf("\n\nTotal elapsed time:     %9.2fs\n\n", (double)t_elapsed / (1000.0 * 1000.0));
}

static void print_usage(int /*argc*/, char** argv) {
  std::cerr << "usage: " << argv[0] << " model_path [ep] [cpf] " << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 4) {
    print_usage(argc, argv);
    return -1;
  }

  std::cout << "-------------" << std::endl;
  std::cout << "  B612 SLM   " << std::endl;
  std::cout << "-------------" << std::endl;

  std::cerr << "usage: " << argv[0] << std::endl;
  std::cerr << "model_path = " << argv[1] << std::endl;
  std::cerr << "execution_provider (ep) = " << argv[2] << std::endl;
  std::cerr << "custom_prompt_file (cpf)= " << argv[3] << std::endl;

  timer_init();

  if (argc > 2) {
    processCustomPromptsFromFile(argv[3]);
  }

  Onnx_Generator(argv[1], argv[2]);

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