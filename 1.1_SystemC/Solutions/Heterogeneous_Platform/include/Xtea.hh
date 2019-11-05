#ifndef XTEA_RTL_HH
#define XTEA_RTL_HH

#include <systemc.h>

SC_MODULE(Xtea) {
    public:
        // Signal declaration
        sc_in<sc_logic > clk;
        sc_in<bool> rst;
        sc_in<bool> input_ready;
        // Words for encryption and decryption
        sc_in<sc_uint<32> > word0;
        sc_in<sc_uint<32> > word1;
        // Keys
        sc_in<sc_uint<32> > key1;
        sc_in<sc_uint<32> > key2;
        sc_in<sc_uint<32> > key3;
        sc_in<sc_uint<32> > key4;
        // Mode encryption/decryption
        sc_in<sc_uint<1> > mode;
        
        // Output signal
        sc_out<bool> output_ready;
        sc_out<sc_uint<32> > output0;
        sc_out<sc_uint<32> > output1;

        SC_CTOR(Xtea) {
            SC_METHOD(fsm);
            sensitive << STATUS << input_ready;

            SC_METHOD(datapath);
            sensitive << clk.pos();
            sensitive << rst.pos();
        };

        // Variable declaration
        sc_signal<sc_uint<5> > counter;     // Loop counter
        sc_signal<sc_uint<32> > word0_temp;
        sc_signal<sc_uint<32> > word1_temp;
        sc_signal<sc_uint<32> > delta;      // Delta value
        sc_signal<sc_uint<32> > k1;
        sc_signal<sc_uint<32> > k2;
        sc_signal<sc_uint<32> > k3;
        sc_signal<sc_uint<32> > k4;
        sc_signal<sc_uint<64> > sum;        // Sum variable

        // FSM states
        typedef enum {
            START,
            INITIAL,
            ASSIGN,
            ENCRYPT_1,
            ENCRYPT_2,
            DECRYPT_1,
            DECRYPT_2,
            TERM
        } STATES;

        sc_signal<int> STATUS, NEXT_STATUS;
        
        void fsm();
        void datapath();
        void encryption1();
        void encryption2();
        void decryption1();
        void decryption2();
};
#endif