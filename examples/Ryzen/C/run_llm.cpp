// Copyright © 2024 Advanced Micro Devices, Inc. All rights reserved.

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>

#include "ort_genai.h"
#include "util/prompt_reader.h"

#if _WIN32
extern "C" {
#include "util/getopt.h"
}
#include <codecvt>
#include <locale>
using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;
#else
#include <getopt.h>
#include <cstring>
#include <sstream>
#endif

std::string custom_template_prompt;
std::vector<std::string> custom_prompts;

bool processCustomPromptsFromFile(std::string custom_p_file) {
  if (custom_p_file.empty()) return false;
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

void CXX_API_CustomPrompt(const char* model_path, int max_new_tokens, std::istream& istr, std::string custom_prompt_file) {

  processCustomPromptsFromFile(custom_prompt_file);

  double duration = 0.0;

  std::cout << "Creating model..." << std::endl;
  try {
    auto start = std::chrono::steady_clock::now();
    auto model = OgaModel::Create(model_path);
    auto end = std::chrono::steady_clock::now();

    duration = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "\nSession Create time (ms): " << duration << std::endl << std::endl;

    std::cout << "Creating tokenizer..." << std::endl;
    auto tokenizer = OgaTokenizer::Create(*model);
    auto tokenizer_stream = OgaTokenizerStream::Create(*tokenizer);

    // Enable for profiling
    const char* prof_var = "EN_PROFILE";
    bool en_profile = 0;

    // Retrieve the environment variable as a string
    const char* env_profile_dump = getenv(prof_var);

    if (env_profile_dump != nullptr)
      en_profile = std::stoi(env_profile_dump);

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

      auto sequences = OgaSequences::Create();
      // printf("Full prompt = \n[%s]\n=====\n", full_custom_prompt.c_str());
      tokenizer->Encode(full_custom_prompt.c_str(), *sequences);

      const OgaSequences* p = sequences.get();
      auto token_sequences = (*reinterpret_cast<const std::vector<std::vector<int>>*>(p));
      auto tokens = token_sequences[0];

      auto prompt_sequences = OgaSequences::Create();
      OgaSequences* new_sequences = prompt_sequences.get();
      auto& prompt_tokenSequences = (*reinterpret_cast<std::vector<std::vector<int>>*>(new_sequences));
      std::vector<int32_t> prompt_tokens;

      if (sequences->SequenceCount(0) > 2048){
        std::cout << "\nPrompt Number of Tokens too large: " << sequences->SequenceCount(0) << std::endl;
        std::cout << "Reducing the number of tokens for input prompt from " << sequences->SequenceCount(0) << " to 2048" << std::endl;
        prompt_tokens.assign(tokens.begin(), tokens.begin() + 2048);
      }
      else{
        prompt_tokens = (tokens);
      }
      
      prompt_tokenSequences.emplace_back(prompt_tokens);

      if (prompt_sequences->SequenceCount(0) > 2048) {
        std::cout << "\nPrompt Number of Tokens: " << prompt_sequences->SequenceCount(0) << std::endl;
        std::cout << "Error: Only <= 2048 Prompt length is supported currently!\n" << std::endl;
        continue;
      }

      // Generating response
      auto params = OgaGeneratorParams::Create(*model);

      params->SetSearchOption("max_length", 4096);
      // Deprecated: params->SetInputSequences(*sequences);

      double duration = 0, duration_prefil = 0, token_time = 0;
      unsigned int ntokens = 0;
      unsigned int default_token = 128;
      unsigned int max_tkns = (max_new_tokens < default_token)? default_token : max_new_tokens;

      // Create Generator
      auto generator = OgaGenerator::Create(*model, *params);
      generator->AppendTokenSequences(*prompt_sequences);

      bool first = true;
      while (!generator->IsDone()) {
        auto start = std::chrono::steady_clock::now();
        // Deprecated: generator->ComputeLogits();
        generator->GenerateNextToken();
        if (first) {
          auto end = std::chrono::steady_clock::now();
          duration = std::chrono::duration<double, std::milli>(end - start).count();
          duration_prefil = duration;
          first = false;
        } else {
          auto end = std::chrono::steady_clock::now();
          duration = std::chrono::duration<double, std::milli>(end - start).count();
          token_time += duration;
          ntokens++;
        }

        const auto num_tokens = generator->GetSequenceCount(0);
        const auto new_token = generator->GetSequenceData(0)[num_tokens - 1];
        std::cout << tokenizer_stream->Decode(new_token) << std::flush;

        if (ntokens == max_tkns)
          break;
      }
      std::cout << std::endl << std::endl;

      std::cout << "\nPrompt length: " << sequences->SequenceCount(0) << std::endl;
      std::cout << "New tokens: " << ntokens << std::endl << std::endl;

      std::cout << "Prompt TTFT (ms): " << duration_prefil << std::endl;
      std::cout << "Tokens / sec: " << ntokens / (token_time / 1000.0) << std::endl;

      for (int i = 0; i < 3; ++i)
        std::cout << std::endl;

      // bump iterator
      custom_prompts_it++;

      if (en_profile)
        break;
    }
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

// C++ API Example
void CXX_API(const char* model_path, int max_new_tokens, std::istream& istr, bool use_chat_template, std::string inp_prompt_len) {

  double duration = 0.0;

  std::cout << "Creating model..." << std::endl;
  try {
    auto start = std::chrono::steady_clock::now();
    auto model = OgaModel::Create(model_path);
    auto end = std::chrono::steady_clock::now();

    duration = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "\nSession Create time (ms): " << duration << std::endl << std::endl;

    std::cout << "Creating tokenizer..." << std::endl;
    auto tokenizer = OgaTokenizer::Create(*model);
    auto tokenizer_stream = OgaTokenizerStream::Create(*tokenizer);

    // Enable for profiling
    const char* prof_var = "EN_PROFILE";
    bool en_profile = 0;

    // Retrieve the environment variable as a string
    const char* env_profile_dump = getenv(prof_var);

    if (env_profile_dump != nullptr)
      en_profile = std::stoi(env_profile_dump);

    std::vector<int>prompt_lengths ;
    std::stringstream ss(inp_prompt_len);
 
    // Parse the string
    for (int i; ss >> i;) {
        prompt_lengths.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    int prompt_ind=0;
    int input_prompt_size=prompt_lengths.size();

    while (true) {
      std::string text;

      // Ask for prompt when prompt is not from file
      if (&istr == &std::cin)
        std::cout << "Enter Prompt: " << std::endl;

      if (getline_in_a_loop(istr, text) == false)
        break;

      std::string prompt;
      if (use_chat_template)
        prompt = "<|user|>\n" + text + "<|end|>\n<|assistant|>";
      else
        prompt = text;


      auto sequences = OgaSequences::Create();
      tokenizer->Encode(prompt.c_str(), *sequences);

      const OgaSequences* p = sequences.get();
      auto token_sequences = (*reinterpret_cast<const std::vector<std::vector<int>>*>(p));
      auto tokens = token_sequences[0];

      auto prompt_sequences = OgaSequences::Create();
      OgaSequences* new_sequences = prompt_sequences.get();
      auto& prompt_tokenSequences = (*reinterpret_cast<std::vector<std::vector<int>>*>(new_sequences));
      std::vector<int32_t> prompt_tokens;

      if (prompt_ind < input_prompt_size && sequences->SequenceCount(0) > prompt_lengths[prompt_ind]){
        std::cout << "Reducing the number of tokens for input prompt from " << sequences->SequenceCount(0) << " to "<<
        prompt_lengths[prompt_ind]<< std::endl;
        prompt_tokens.assign(tokens.begin(),tokens.begin()+prompt_lengths[prompt_ind]);
      }
      else{
        prompt_tokens = (tokens);
      }
      
      prompt_tokenSequences.emplace_back(prompt_tokens);
      prompt_ind++;

      if (prompt_sequences->SequenceCount(0) > 2048) {
        std::cout << "\nPrompt Number of Tokens: " << prompt_sequences->SequenceCount(0) << std::endl;
        std::cout << "Error: Only <= 2048 Prompt length is supported currently!\n" << std::endl;
        continue;
      }

      std::cout << "Generating response..." << std::endl;
      auto params = OgaGeneratorParams::Create(*model);

      params->SetSearchOption("max_length", 4096);
      // Deprecated: params->SetInputSequences(*prompt_sequences);

      double duration = 0, duration_prefil = 0, token_time = 0;
      unsigned int ntokens = 0;
      unsigned int defaul_token = 20;
      unsigned int max_tkns = (max_new_tokens < defaul_token)? defaul_token : max_new_tokens;

      auto generator = OgaGenerator::Create(*model, *params);
      generator->AppendTokenSequences(*prompt_sequences);
      std::cout << "Generator created ..." << std::endl;
      bool first = true;
      while (!generator->IsDone()) {
        auto start = std::chrono::steady_clock::now();
        // Deprecated: generator->ComputeLogits();
        generator->GenerateNextToken();
        if (first) {
          auto end = std::chrono::steady_clock::now();
          duration = std::chrono::duration<double, std::milli>(end - start).count();
          duration_prefil = duration;
          first = false;
        } else {
          auto end = std::chrono::steady_clock::now();
          duration = std::chrono::duration<double, std::milli>(end - start).count();
          token_time += duration;
          ntokens++;
        }

        const auto num_tokens = generator->GetSequenceCount(0);
        const auto new_token = generator->GetSequenceData(0)[num_tokens - 1];
        std::cout << tokenizer_stream->Decode(new_token) << std::flush;

        if (ntokens == max_tkns)
          break;
      }
      std::cout << std::endl << std::endl;

      std::cout << "\nPrompt length: " << prompt_sequences->SequenceCount(0) << std::endl;
      std::cout << "New tokens: " << ntokens << std::endl << std::endl;

      std::cout << "Prompt TTFT (ms): " << duration_prefil << std::endl;
      std::cout << "Tokens / sec: " << ntokens / (token_time / 1000.0) << std::endl;

      for (int i = 0; i < 3; ++i)
        std::cout << std::endl;

      if (en_profile)
        break;
    }
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

static void usage(const char *name) {
  std::cout
      << "usage: " << name << "\n"
         " [-m: model path]\n"
         " [-f: prompt file]\n"
         " [-n: max new tokens]\n"
         " [-c: use chat template]\n"
         " [-t: input prompt token length ]\n"
         " [-p: custom prompt file]\n"
         " [-h: help]\n"
      << std::endl;
}

int main(int argc, char** argv) {
  std::cout << "Model Generate using OGA ..." << std::endl;
  int opt = 0;
  bool use_chat_template = false;
  std::string model_path;
  std::string prompt_file;
  std::string custom_prompt_file;
  int max_new_tokens = 64;
  int num_iterations = 1;
  std::string inp_prompt_len;
  while ((opt = getopt(argc, argv, "chm:f:n:t:p:")) != -1) {
    switch (opt) {
    case 'c':
      use_chat_template = true;
      break;
    case 'm':
      model_path = std::string(optarg);
      break;
    case 'n':
      max_new_tokens = std::stoi(optarg);
      break;
    case 'f':
      prompt_file = std::string(optarg);
      break;
    case 't':
      inp_prompt_len = std::string(optarg);
      break;
    case 'p':
      custom_prompt_file = std::string(optarg);
      break;
    case 'h':
      usage(argv[0]);
      exit(1);
    default:
      usage(argv[0]);
      exit(1);
    }
  }

  if (optind > argc) {
    usage(argv[0]);
    exit(1);
  }

  std::cout << "- Model Path: " << model_path << std::endl;
  std::cout << "- Prompt File: " << prompt_file << std::endl;
  std::cout << "- Max new tokens.: " << max_new_tokens << std::endl;
  std::cout << "- Use chat template: " << use_chat_template << std::endl;
  std::cout << "- Input prompt token length : " << inp_prompt_len << std::endl;
  std::cout << "- Custom prompt file: " << custom_prompt_file << std::endl;
  std::cout << "---------------------------------------" << std::endl;

  // Responsible for cleaning up the library during shutdown
  OgaHandle handle;

  if (!custom_prompt_file.empty()) {
    CXX_API_CustomPrompt(model_path.c_str(), max_new_tokens, std::cin, custom_prompt_file);
  } else if (prompt_file.empty()) {
    CXX_API(model_path.c_str(), max_new_tokens, std::cin, use_chat_template, inp_prompt_len);
  } else {
    std::ifstream ifs(prompt_file.c_str());
    if (!ifs.is_open()) {
      std::cerr << "Couldn't open file : " << prompt_file << std::endl;
      return -1;
    }
    CXX_API(model_path.c_str(), max_new_tokens, ifs, use_chat_template, inp_prompt_len);
  }

  return 0;
}

