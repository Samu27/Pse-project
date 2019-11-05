#ifndef STRUCT_HH
#define STRUCT_HH

#include <systemc.h>

struct watertank {
    double water_level;
};

struct valve_struct {
  sc_uint<32> word0;
  sc_uint<32> word1;
  sc_uint<32> key0;
  sc_uint<32> key1;
  sc_uint<32> key2;
  sc_uint<32> key3;
  double threshold;
};

#endif
