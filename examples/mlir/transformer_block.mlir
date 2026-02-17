// Transformer块示例 (简化版BERT)
// examples/mlir/transformer_block.mlir

// RUN: acompiler-opt %s | FileCheck %s

module {
  // Self-Attention机制
  // MultiHeadAttention: Q, K, V projection -> scaled dot-product -> output projection
  
  // CHECK-LABEL: @multi_head_attention
  func.func @multi_head_attention(
      %input: tensor<1x128x768xf32>)  // [batch, seq_len, hidden_size]
      -> tensor<1x128x768xf32> {
    
    %hidden_size = arith.constant 768 : index
    %num_heads = arith.constant 12 : index
    %head_dim = arith.constant 64 : index  // 768 / 12
    
    // Q, K, V 投影矩阵
    %wq = arith.constant dense<1.0> : tensor<768x768xf32>
    %wk = arith.constant dense<1.0> : tensor<768x768xf32>
    %wv = arith.constant dense<1.0> : tensor<768x768xf32>
    
    // Linear projections: Q = X @ Wq
    // CHECK: ai.batch_matmul
    %query = ai.batch_matmul %input, %wq {
      transpose_rhs = false
    } : (tensor<1x128x768xf32>, tensor<768x768xf32>) 
     -> tensor<1x128x768xf32>
    
    %key = ai.batch_matmul %input, %wk {
      transpose_rhs = false
    } : (tensor<1x128x768xf32>, tensor<768x768xf32>)
     -> tensor<1x128x768xf32>
    
    %value = ai.batch_matmul %input, %wv {
      transpose_rhs = false
    } : (tensor<1x128x768xf32>, tensor<768x768xf32>)
     -> tensor<1x128x768xf32>
    
    // 重塑为多头: [batch, seq, hidden] -> [batch, num_heads, seq, head_dim]
    // CHECK: ai.reshape
    %q_multihead = ai.reshape %query {
      shape = [1, 128, 12, 64]
    } : tensor<1x128x768xf32> -> tensor<1x128x12x64xf32>
    
    // 转置: [batch, seq, heads, head_dim] -> [batch, heads, seq, head_dim]
    // CHECK: ai.transpose
    %q_transposed = ai.transpose %q_multihead {
      perm = [0, 2, 1, 3]
    } : tensor<1x128x12x64xf32> -> tensor<1x12x128x64xf32>
    
    %k_multihead = ai.reshape %key {
      shape = [1, 128, 12, 64]
    } : tensor<1x128x768xf32> -> tensor<1x128x12x64xf32>
    
    %k_transposed = ai.transpose %k_multihead {
      perm = [0, 2, 1, 3]
    } : tensor<1x128x12x64xf32> -> tensor<1x12x128x64xf32>
    
    %v_multihead = ai.reshape %value {
      shape = [1, 128, 12, 64]
    } : tensor<1x128x768xf32> -> tensor<1x128x12x64xf32>
    
    %v_transposed = ai.transpose %v_multihead {
      perm = [0, 2, 1, 3]
    } : tensor<1x128x12x64xf32> -> tensor<1x12x128x64xf32>
    
    // Scaled dot-product attention: scores = Q @ K^T / sqrt(d_k)
    // [batch, heads, seq_q, head_dim] @ [batch, heads, head_dim, seq_k]
    // = [batch, heads, seq_q, seq_k]
    %scores = ai.batch_matmul %q_transposed, %k_transposed {
      transpose_rhs = true
    } : (tensor<1x12x128x64xf32>, tensor<1x12x128x64xf32>)
     -> tensor<1x12x128x128xf32>
    
    // 缩放因子: 1/sqrt(64) = 0.125
    %scale = arith.constant dense<0.125> : tensor<f32>
    // CHECK: ai.mul
    %scores_scaled = ai.mul %scores, %scale : tensor<1x12x128x128xf32>
    
    // Softmax (简化表示)
    // TODO: 添加softmax算子
    // %attention_weights = ai.softmax %scores_scaled {axis = -1}
    
    // 注意力加权: attention_output = attention_weights @ V
    // [batch, heads, seq_q, seq_k] @ [batch, heads, seq_k, head_dim]
    // = [batch, heads, seq_q, head_dim]
    %attention_output = ai.batch_matmul %scores_scaled, %v_transposed {
      transpose_rhs = false
    } : (tensor<1x12x128x128xf32>, tensor<1x12x128x64xf32>)
     -> tensor<1x12x128x64xf32>
    
    // 转置回: [batch, heads, seq, head_dim] -> [batch, seq, heads, head_dim]
    %output_transposed = ai.transpose %attention_output {
      perm = [0, 2, 1, 3]
    } : tensor<1x12x128x64xf32> -> tensor<1x128x12x64xf32>
    
    // 重塑: [batch, seq, heads, head_dim] -> [batch, seq, hidden]
    %output_reshaped = ai.reshape %output_transposed {
      shape = [1, 128, 768]
    } : tensor<1x128x12x64xf32> -> tensor<1x128x768xf32>
    
    // 输出投影
    %wo = arith.constant dense<1.0> : tensor<768x768xf32>
    %final_output = ai.batch_matmul %output_reshaped, %wo {
      transpose_rhs = false
    } : (tensor<1x128x768xf32>, tensor<768x768xf32>)
     -> tensor<1x128x768xf32>
    
    return %final_output : tensor<1x128x768xf32>
  }
  
  // Feed-Forward Network
  // FFN: Linear(hidden -> intermediate) -> GELU -> Linear(intermediate -> hidden)
  
  // CHECK-LABEL: @feed_forward
  func.func @feed_forward(%input: tensor<1x128x768xf32>)
                          -> tensor<1x128x768xf32> {
    // 第一个线性层: 768 -> 3072
    %w1 = arith.constant dense<1.0> : tensor<768x3072xf32>
    %b1 = arith.constant dense<0.0> : tensor<3072xf32>
    
    // CHECK: ai.batch_matmul
    %linear1 = ai.batch_matmul %input, %w1 {
      transpose_rhs = false
    } : (tensor<1x128x768xf32>, tensor<768x3072xf32>)
     -> tensor<1x128x3072xf32>
    
    // 加bias (简化)
    // %linear1_bias = ai.add %linear1, %b1
    
    // GELU激活
    // CHECK: ai.gelu
    %gelu = ai.gelu %linear1 : tensor<1x128x3072xf32>
    
    // 第二个线性层: 3072 -> 768
    %w2 = arith.constant dense<1.0> : tensor<3072x768xf32>
    %b2 = arith.constant dense<0.0> : tensor<768xf32>
    
    %linear2 = ai.batch_matmul %gelu, %w2 {
      transpose_rhs = false
    } : (tensor<1x128x3072xf32>, tensor<3072x768xf32>)
     -> tensor<1x128x768xf32>
    
    return %linear2 : tensor<1x128x768xf32>
  }
  
  // 完整的Transformer块
  // TransformerBlock: MultiHeadAttention -> Add & Norm -> FFN -> Add & Norm
  
  // CHECK-LABEL: @transformer_block
  func.func @transformer_block(%input: tensor<1x128x768xf32>)
                                -> tensor<1x128x768xf32> {
    // Multi-head attention
    %attention = func.call @multi_head_attention(%input) 
        : (tensor<1x128x768xf32>) -> tensor<1x128x768xf32>
    
    // Residual connection
    // CHECK: ai.add
    %add1 = ai.add %input, %attention : tensor<1x128x768xf32>
    
    // Layer normalization
    %scale1 = arith.constant dense<1.0> : tensor<768xf32>
    %bias1 = arith.constant dense<0.0> : tensor<768xf32>
    
    // CHECK: ai.layer_norm
    %norm1 = ai.layer_norm %add1, %scale1, %bias1 {
      epsilon = 1.0e-12 : f32,
      normalized_shape = [768]
    } : (tensor<1x128x768xf32>, tensor<768xf32>, tensor<768xf32>)
     -> tensor<1x128x768xf32>
    
    // Feed-forward network
    %ffn = func.call @feed_forward(%norm1)
        : (tensor<1x128x768xf32>) -> tensor<1x128x768xf32>
    
    // Residual connection
    %add2 = ai.add %norm1, %ffn : tensor<1x128x768xf32>
    
    // Layer normalization
    %scale2 = arith.constant dense<1.0> : tensor<768xf32>
    %bias2 = arith.constant dense<0.0> : tensor<768xf32>
    
    %norm2 = ai.layer_norm %add2, %scale2, %bias2 {
      epsilon = 1.0e-12 : f32,
      normalized_shape = [768]
    } : (tensor<1x128x768xf32>, tensor<768xf32>, tensor<768xf32>)
     -> tensor<1x128x768xf32>
    
    return %norm2 : tensor<1x128x768xf32>
  }
}
