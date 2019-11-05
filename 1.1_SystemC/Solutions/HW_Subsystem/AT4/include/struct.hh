#ifndef XTEA_TLM_AT4_STRUCT_HH
#define XTEA_TLM_AT4_STRUCT_HH

struct iostruct {
  sc_uint<32> word0;    // Input words
  sc_uint<32> word1;
  sc_uint<32> key0;     // Keys
  sc_uint<32> key1;
  sc_uint<32> key2;
  sc_uint<32> key3;
  sc_uint<32> output0;  // Output result
  sc_uint<32> output1;
  bool mode;            // Encryption/decryption mode
};

#endif