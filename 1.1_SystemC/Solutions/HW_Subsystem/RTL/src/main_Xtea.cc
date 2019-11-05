#include "Xtea.hh"
#include "Xtea_testbench.hh"

int sc_main(int argc, char **argv) {
  // Signals needed to link xtea and testbench
  sc_clock clk; 
  sc_signal<bool> rst, input_ready;
  sc_signal<sc_uint<1> > mode;

  sc_signal<sc_uint<32> > word0;
  sc_signal<sc_uint<32> > word1;
  sc_signal<sc_uint<32> > key_0;
  sc_signal<sc_uint<32> > key_1;
  sc_signal<sc_uint<32> > key_2;
  sc_signal<sc_uint<32> > key_3;
  sc_signal<sc_uint<32> > output0;
  sc_signal<sc_uint<32> > output1;
  sc_signal<bool> output_ready;


  Xtea xtea("xtea");
  Xtea_testbench testbench("xtea_testbench");

  // File creation
  sc_trace_file *fp = sc_create_vcd_trace_file("wave");
  
  sc_trace(fp, clk, "clk");
  sc_trace(fp, rst, "rst");
  sc_trace(fp, input_ready, "input_ready");
  sc_trace(fp, mode, "mode");
  sc_trace(fp, word0, "text_input_0");
  sc_trace(fp, word1, "text_input_1");
  sc_trace(fp, key_0, "key_input_0");
  sc_trace(fp, key_1, "key_input_1");
  sc_trace(fp, key_2, "key_input_2");
  sc_trace(fp, key_3, "key_input_3");
  sc_trace(fp, output0, "data_output_0");
  sc_trace(fp, output1, "data_output_1");

  // Xtea
  xtea.clk(clk);
  xtea.rst(rst);
  xtea.word0(word0);
  xtea.word1(word1);
  xtea.key1(key_0);
  xtea.key2(key_1);
  xtea.key3(key_2);
  xtea.key4(key_3);
  xtea.output0(output0);
  xtea.output1(output1);
  xtea.mode(mode);
  xtea.input_ready(input_ready);
  xtea.output_ready(output_ready);

  // Testbench
  testbench.clk(clk);
  testbench.rst(rst);
  testbench.word0(word0);
  testbench.word1(word1);
  testbench.key_0(key_0);
  testbench.key_1(key_1);
  testbench.key_2(key_2);
  testbench.key_3(key_3);
  testbench.output0(output0);
  testbench.output1(output1);
  testbench.mode(mode);
  testbench.input_ready(input_ready);
  testbench.output_ready(output_ready);

  sc_start();

  sc_close_vcd_trace_file(fp);

  return 0;
}