#include "Xtea_testbench.hh"
#include <string>
#include <sstream>
#include <fstream>

Xtea_testbench::Xtea_testbench(sc_module_name name) : sc_module(name) {
  // Thread definition with sensitivity list (clock and reset)
  SC_THREAD(run);
  sensitive << clk.pos() << rst.pos();
}


void Xtea_testbench::run() {
  sc_uint<32> key[4] = {0x6a1d78c8, 0x8c86d67f, 0x2a65bfbe, 0xb4bd6e46};
  sc_uint<32> text[2] = {0x12345678, 0x9abcdeff};
  
  // Mode = false/0, encryption mode
  mode.write(0);

  // Print original text
  cout << "\nOriginal text: " << hex << text[0] << "-" << hex << text[1] << "\n";

  // Keys
  key_0.write(key[0]);
  key_1.write(key[1]);
  key_2.write(key[2]);
  key_3.write(key[3]);
  word0.write(text[0]);
  word1.write(text[1]);
  
  input_ready.write(true);
  wait(100);

  // Encrypted words
  text[0] = output0.read();
  text[1] = output1.read();

  input_ready.write(false);
  wait();

  // Print encpryted text
  cout << "\nEncrypted text: " << hex << text[0] << "-" << hex << text[1] << "\n";

  // Mode = true/1, decryption mode
  mode.write(true);
  wait(); 

  // Keys
  key_0.write(key[0]);
  key_1.write(key[1]);
  key_2.write(key[2]);
  key_3.write(key[3]);
  word0.write(text[0]);
  word1.write(text[1]);
  
  input_ready.write(true);
  wait(150);

  // Decrypted words
  text[0] = output0.read();
  text[1] = output1.read();

  input_ready.write(false);

  // Print decrypted text
  cout << "\nDecrypted text: " << hex << text[0] << "-" << hex << text[1] << "\n";

  sc_stop();
}