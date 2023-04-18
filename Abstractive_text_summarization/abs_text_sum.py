from transformers import PegasusForConditionalGeneration, PegasusTokenizer

def abs_text_sum(source_text):
    model_name = 'google/pegasus-xsum'
    device = 'cpu'
    tokenizer = PegasusTokenizer.from_pretrained(model_name)
    model = PegasusForConditionalGeneration.from_pretrained(model_name).to(device)
    tokenized_text = tokenizer.prepare_seq2seq_batch(source_text, truncation=True, padding='longest',return_tensors='pt')
    translated = model.generate(**tokenized_text)
    result = tokenizer.batch_decode(translated, skip_special_tokens=True)
    print(result)


text = input("Please type in the text:\n")
abs_text_sum(text)