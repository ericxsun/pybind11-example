#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright @2019 R&D, NTC Inc. (ntc.ai)
#
# Author: qinluo <eric.x.sun@gmail.com>
#

from cuBERT import cuBERT_open
from cuBERT import cuBERT_ComputeType
from cuBERT import cuBERT_open_tokenizer
from cuBERT import cuBERT_tokenize_compute


def compute_tokenize():
    max_batch_size = 128
    batch_size = 2
    seq_length = 32
    text_a = ['知乎', '知乎']
    text_b = ['在家刷知乎', '知乎发现更大的世界']

    model_filename = 'bert_frozen_seq32.pb'
    vocab_filename = 'vocab.txt'
    num_hidden_layers = 12
    num_attention_heads = 12

    model = cuBERT_open(model_filename, max_batch_size, seq_length, num_hidden_layers, num_attention_heads)
    tokenizer = cuBERT_open_tokenizer(vocab_filename)

    output = cuBERT_tokenize_compute(model, tokenizer, batch_size, text_a, text_b)

    print(output)

compute_tokenize()
