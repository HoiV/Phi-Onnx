#pragma once

#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <string.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

#include "input_decoder.h"
#include "ort_genai.h"

#ifdef _WIN32
#ifdef BUILDING_SLM_ENGINE
#define SLM_ENGINE_EXPORT __declspec(dllexport)
#else
#define SLM_ENGINE_EXPORT /* __declspec(dllimport) */
#endif
#else
// To make symbols visible on macOS/iOS
#ifdef __APPLE__
#define SLM_ENGINE_EXPORT __attribute__((visibility("default")))
#else
#define SLM_ENGINE_EXPORT
#endif
#endif

namespace microsoft {
namespace slm_engine {

/// @brief SLM Engine class to interact with the GenAI Model
///
/// The SLM Engine class is responsible for loading the GenAI Model and
/// interacting with it to generate responses to user prompts. The class
/// provides a complete() function that takes a user prompt and returns the
/// generated response.
///
/// The class also provides a CreateEngine() function to create a new instance
/// of the SLM Engine and initialize it.
///
/// The class also provides utility functions to convert between the model type
/// enum and string representations.
///
/// The class also provides a struct to hold the runtime performance metrics
/// of the SLM Engine.
///
/// Example Usage:
/// @code
/// // Create a new instance of the SLM Engine
/// auto slm_engine = SLMEngine::CreateEngine("path/to/model", "phi", true);
/// if (!slm_engine) {
///     std::cout << "Error creating the SLM Engine" << std::endl;
///     return -1;
/// }
///
/// // Generate a response to a user prompt
/// std::string prompt =
///     "{\"role\": \"user\", \"content\": \"Hello, how are you?\"}";
/// std::string response = slm_engine->complete(prompt.c_str());
/// std::cout << "Response: " << response["response"]["answer"] << std::endl;
/// @endcode
///

class SLM_ENGINE_EXPORT SLMEngine {
 public:
  /// @brief Enum to define the supported model types
  enum class SupportedModelType { PHI,
                                  Llama,
                                  CUSTOM,
                                  UNKNOWN };

  // Utility to convert string to SupportedModelType
  static SupportedModelType StringToModelType(const std::string& model_type) {
    if (strncasecmp(model_type.c_str(), "phi", 3) == 0) {
      return SupportedModelType::PHI;
    } else if (strncasecmp(model_type.c_str(), "llama", 5) == 0) {
      return SupportedModelType::Llama;
    } else if (strncasecmp(model_type.c_str(), "custom", 6) == 0) {
      return SupportedModelType::CUSTOM;
    }
    return SupportedModelType::UNKNOWN;
  }

  // Utility to convert SupportedModelType to string
  static std::string ModelTypeToString(SupportedModelType model_type) {
    switch (model_type) {
      case SupportedModelType::PHI:
        return "phi";
      case SupportedModelType::Llama:
        return "llama";
      case SupportedModelType::CUSTOM:
        return "custom";
      case SupportedModelType::UNKNOWN:
      default:
        return "unknown";
    }
  }

  /// @brief Creates a new instance of the SLM Engine and initializes it
  /// @param model_path Path to ONNX GenAI Model Directory
  /// @param verbose When set, the LLM Generated output is displayed on stdout
  /// @return New object or null if unsuccessful

  static std::unique_ptr<SLMEngine> CreateEngine(
      const char* model_path, bool verbose);

  /// @brief Generates a response to the user prompt using the GenAI Model
  /// @param prompt User prompt to generate response for. The format for this
  /// string is exactly the same as the OpenAI Text Completion API
  /// @return Generated response JSON object as a string.
  ///
  /// The complete() function takes a user prompt and generates a response
  /// using the GenAI Model. The function returns the generated response as a
  /// string.
  ///
  /// The user prompt should be in the following format:
  /// {
  ///     "messages": [
  ///         {
  ///             "role": "system",
  ///             "content": "System message"
  ///         },
  ///         {
  ///             "role": "user",
  ///             "content": "User message"
  ///         }
  ///     ],
  ///     "temperature": 0.0,
  ///     "stop": ["Stop token 1", "Stop token 2"],
  ///     "max_tokens": 250
  /// }
  ///
  /// Format of the response string when the call succeeds
  /// {
  ///     "status": "success",
  ///     "response": {
  ///         "answer": "Generated response",
  ///         "kpi": {
  ///             "prompt_toks": 10,
  ///             "response_toks": 20,
  ///             "ttft": 1000,
  ///             "tok_rate": 10,
  ///             "total_time": 10000,
  ///             "memory_usage": 100
  ///         }
  ///     }
  /// }
  ///
  /// Format of the response string when the call fails
  /// {
  ///     "status": "error",
  ///     "message": "Error message"
  /// }
  ///
  /// @note To support multi-turn conversations, the history should be
  /// maintained by the caller and submitted just like how the OpenAI API
  /// works
  std::string complete(const char* prompt);

  /// @brief Struct to hold the runtime performance metrics of the SLM Engine
  /// @param PromptTokenCount Number of tokens in the prompt
  /// @param TimeToFirstToken Time taken to generate the first token
  /// @param GeneratedTokenCount Number of tokens generated
  /// @param TokenRate Number of tokens generated per second
  /// @param TotalTime Total time taken to generate the response
  /// @param CurrentMemoryUsed Current memory used by the SLM Engine
  struct RuntimePerf {
    uint32_t PromptTokenCount;
    uint32_t TimeToFirstToken;
    uint32_t GeneratedTokenCount;
    uint32_t TokenRate;
    uint32_t TotalTime;
    uint32_t CurrentMemoryUsed;
  };

  /// @brief Struct to hold the generation options for the GenAI Model
  /// @param MaxGeneratedTokens Maximum number of tokens to generate
  /// @param TopK Top K sampling
  /// @param TopP Top P sampling
  /// @param Temperature Temperature for sampling
  struct GenerationOptions {
    uint32_t MaxGeneratedTokens;
    uint32_t TopK;
    float TopP;
    float Temperature;
    explicit GenerationOptions() {
      MaxGeneratedTokens = 2048;
      Temperature = 0.00000000000001f;
      TopK = 50;
      TopP = 0.1f;
    }
  };

  /// @brief Asks the GenAI Model for a response
  /// @param formatted_prompt Formatted prompt to generate response for
  /// @param generation_options Generation options for the GenAI Model
  /// @param response_str Generated response
  /// @param kpi Runtime performance metrics of the SLM Engine
  void generate(
      const std::string& formatted_prompt,
      const GenerationOptions& generation_options,
      std::string& response_str,
      RuntimePerf& kpi);

  SLMEngine(const SLMEngine&) = delete;
  SLMEngine& operator=(const SLMEngine&) = delete;
  static void GetVersion(std::string& slm_version, std::string& ortga_version, std::string& ort_version);

  /// @brief Destructor for the SLM Engine
  ~SLMEngine();

  /// @brief  Get the current memory usage of the SLM Engine
  /// @return Current memory usage in MB
  static uint32_t GetMemoryUsage();

  /// @brief Get the model family from the model path
  /// @param model_path Path to the model file
  /// @return Model family as a string
  static std::string GetModelFamily(const std::string& model_path);

 private:
  SLMEngine(bool verbose) : m_verbose(verbose) {}

  /// @brief
  /// @param model_path
  /// @return
  bool load_model(const char* model_path, SupportedModelType model_type);

  // Define the Model related prompts
  struct PromptFormat {
    std::string prefix;
    std::string suffix;
  };

  struct PromptFormatDictionary {
    std::string llm_type;
    std::map<InputDecoder::InputParams::Role, PromptFormat> prompt_format;
  };

  bool parse_prompt_format_dict(SupportedModelType model_type,
                                const std::string& json_dict,
                                PromptFormatDictionary& prompt_format_dict);

  /// @brief Given the user input parameters formats by adding the necessary
  /// control strings for the current LLM Model (Phi3)
  /// @param input_params Input parameters to use
  /// @return Complete prompt to be fed to the LLM
  std::string format_input(const InputDecoder::InputParams& input_params);

  std::unique_ptr<OgaModel> m_onnx_model;
  std::unique_ptr<OgaTokenizer> m_tokenizer;
  std::unique_ptr<OgaTokenizerStream> m_tokenizer_stream;
  std::unique_ptr<InputDecoder> m_input_decoder;
  PromptFormatDictionary m_prompt_format;

  bool m_verbose;
  std::ofstream m_llm_input_dbg_stream;
  std::ofstream m_llm_output_dbg_stream;

  // Need a scoped mutex to ensure only one complete() call at a time
  std::mutex m_mutex;
};
}  // namespace slm_engine
}  // namespace microsoft
