import numpy as np
import onnxruntime
import onnx
from transformers import AutoTokenizer
def generate_text(model_path, prompt, tokenizer, max_gen_tokens, total_sequence, window, context):
    model = onnx.load(model_path)
    #we create the inputs for the first iteration
    input_tensor = tokenizer(prompt, return_tensors="pt")
    prompt_size = len(input_tensor['input_ids'][0])
    actual_input = input_tensor['input_ids']
    if prompt_size < window:
        actual_input = np.concatenate((tokenizer.bos_token_id*np.ones([1, window - prompt_size], dtype = 'int64'),
                                       actual_input), axis=1)
    if prompt_size + max_gen_tokens > total_sequence:
        print("ERROR: Longer total sequence is needed!")
        return
    first_attention = np.concatenate((np.zeros([1, total_sequence - window], dtype = 'int64'),
                                      np.ones((1, window), dtype = 'int64')), axis=1)
    max_gen_tokens += prompt_size #we need to generate on top of parsing the prompt
    inputs_names =[node.name for node in model.graph.input]
    output_names =[node.name for node in model.graph.output]
    n_heads = 32 #gqa-heads of the kvc
    inputs_dict = {}
    inputs_dict['input_ids'] = actual_input[:, :window].reshape(1, window).numpy()
    inputs_dict['attention_mask'] = first_attention
    index_pos = sum(first_attention[0])
    inputs_dict['position_ids'] = np.concatenate((np.zeros([1, total_sequence - index_pos], dtype = 'int64'), np.arange(index_pos, dtype = 'int64').reshape(1, index_pos)), axis=1)
    inputs_dict['tree_attention'] = np.triu(-65504*np.ones(total_sequence), k= 1).astype('float16').reshape(1, 1, total_sequence, total_sequence)
    for name in inputs_names:
        if name == 'input_ids' or name == 'attention_mask' or name == 'position_ids' or name == 'tree_attention': continue
        inputs_dict[name] = np.zeros([1, n_heads, context-window, 96], dtype="float16")
    index = 0
    new_token = np.array([10])
    next_index = window
    old_j = 0
    total_input = actual_input.numpy()
    rt_session = onnxruntime.InferenceSession(model_path)
    ## We run the inferences
    while next_index < max_gen_tokens:
        if new_token.any() == tokenizer.eos_token_id:
            break
        #inference
        output = rt_session.run(output_names, inputs_dict)
        outs_dictionary = {name: content for (name, content) in zip (output_names, output)}
        #we prepare the inputs for the next inference
        for name in inputs_names:
            if name == 'input_ids':
                old_j = next_index
                if next_index < prompt_size:
                    if prompt_size - next_index >= window: next_index += window
                    else: next_index = prompt_size 
                    j = next_index - window
                else:
                    next_index +=1
                    j = next_index - window
                    new_token = outs_dictionary['logits'].argmax(-1).reshape(1, window)
                    total_input = np.concatenate((total_input, new_token[: , -1:]), axis = 1)
                inputs_dict['input_ids']= total_input[:, j:next_index].reshape(1, window)
            elif name == 'attention_mask':
                inputs_dict['attention_mask'] = np.concatenate((np.zeros((1, total_sequence-next_index), dtype = 'int64'), np.ones((1, next_index), dtype = 'int64')), axis=1)
            elif name == 'position_ids':
                inputs_dict['position_ids'] = np.concatenate((np.zeros([1, total_sequence - next_index], dtype = 'int64'), np.arange(next_index, dtype = 'int64').reshape(1, next_index)), axis=1)
            elif name == 'tree_attention': continue
            else:
                old_name = name.replace("past_key_values", "present")
                inputs_dict[name] = outs_dictionary[old_name][:, :, next_index-old_j:context-window+(next_index - old_j), :]
    answer = tokenizer.decode(total_input[0], skip_special_tokens=True, clean_up_tokenization_spaces=False)
    return answer

We now run the inferences:

tokenizer = AutoTokenizer.from_pretrained("Esperanto/phi-3.5-mini-instruct-kvc-fp16-onnx")
model_path = "phi-3.5-mini-instruct-kvc-fp16-onnx/model.onnx"
max_gen_tokens = 20    #number of tokens we want tog eneral
total_sequence = 128   #total sequence_length
context = 1024         #the context to extend the kvc
window = 16            #number of tokens we want to parse at the time
messages = [
    {"role": "system", "content": "You are a pirate chatbot who always responds in pirate speak!"},
    {"role": "user", "content": "Who are you?"},
]
prompt = tokenizer.apply_chat_template(messages, tokenize=False, add_generation_prompt=True)
generated = generate_text(model_path, prompt, tokenizer, max_gen_tokens, total_sequence, window, context)
print(generated)
