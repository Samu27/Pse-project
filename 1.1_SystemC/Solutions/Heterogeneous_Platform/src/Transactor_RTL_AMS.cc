#include "Transactor_RTL_AMS.hh"

// Constructor
Transactor_RTL_AMS::Transactor_RTL_AMS( sc_core::sc_module_name ) :
  command_ams( "comand_ams"), word0_rtl( "word0_rt"), word1_rtl("word1_rt"),
  threshold_ams("threshold_ams"), threshold_rtl("threshold_rtl"), output_ready("output_ready") {
}

// From rtl to ams
void Transactor_RTL_AMS::processing() {
  command_ams.write(word0_rtl.read() * 2 + word1_rtl.read());
  threshold_ams.write(threshold_rtl.read());
}

void Transactor_RTL_AMS::set_attribute(){
}
