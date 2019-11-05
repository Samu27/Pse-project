#include<systemc.h>
#include<systemc-ams.h>

#include "Valve.hh"
#include "Watertank.hh"
#include "Watertank_system.hh"

int sc_main( int ac, char *av[] ) {
  // File creation
  sca_trace_file *file = sca_create_vcd_trace_file("water");
  sc_trace_file *fileSC = sc_create_vcd_trace_file("wave");
  fileSC->set_time_unit(1,SC_NS);

  Watertank_system wt_system("watertank_system");

  sca_trace(file, wt_system.threshold_ams, "threshold");
  sca_trace(file, wt_system.command_ams, "command");
  sca_trace(file, wt_system.aperture, "aperture");
  sca_trace(file, wt_system.water_level_ams, "water_level");
      
  sc_trace(fileSC, wt_system.clk, "clock");
  sc_trace(fileSC, wt_system.rst, "reset");
  sc_trace(fileSC, wt_system.din, "din");
  sc_trace(fileSC, wt_system.dout, "dout");

  sc_trace(fileSC, wt_system.mode, "mode");
  sc_trace(fileSC, wt_system.word0_rt, "word0");
  sc_trace(fileSC, wt_system.word1_rt, "word1");
  sc_trace(fileSC, wt_system.xtea.word0_temp, "word0_processed");
  sc_trace(fileSC, wt_system.xtea.word1_temp, "word1_processed");
  sc_trace(fileSC, wt_system.word0_decrypt, "word0_decr");
  sc_trace(fileSC, wt_system.word1_decrypt, "word1_decr");
  sc_trace(fileSC, wt_system.xtea.STATUS, "status");
  sc_trace(fileSC, wt_system.xtea.counter, "counter");

  sc_start(1200,SC_SEC);

  sca_close_vcd_trace_file(file);

  return 0;
}
