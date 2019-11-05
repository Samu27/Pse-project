#ifndef XTEA_RTL_TESTBENCH_HH
#define XTEA_RTL_TESTBENCH_HH

#include <systemc.h>

SC_MODULE(Xtea_testbench) {
  public:
    // Clock and reset signal
    sc_in_clk clk;    
    sc_out<bool> rst;

    // Input signal
    sc_in<sc_uint<32> > output0;
    sc_in<sc_uint<32> > output1;
    sc_in<bool> output_ready;
    
    // Output signal
    sc_out<sc_uint<32> > word0; 
    sc_out<sc_uint<32> > word1;
    sc_out<sc_uint<32> > key_0;
    sc_out<sc_uint<32> > key_1;
    sc_out<sc_uint<32> > key_2;
    sc_out<sc_uint<32> > key_3;
    sc_out<bool> input_ready; 
    sc_out<sc_uint<1> > mode;

    SC_HAS_PROCESS(Xtea_testbench);
    Xtea_testbench(sc_module_name name);

  private:
    void run();
};

#endif
