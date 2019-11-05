#include "Watertank_system.hh"

// Constructor
Watertank_system::Watertank_system(sc_core::sc_module_name) : controller("controller", 0x6a1d78c8, 0x8c86d67f, 0x2a65bfbe, 0xb4bd6e46, 0x9e3779b9),
    xtea("Xtea"), valve("valve"), watertank("watertank"),
    rtl_to_ams("rtl_to_ams"), ams_to_rtl("ams_to_rtl"),
    rtl_to_tlm("rtl_to_tlm"), tlm_to_rtl("tlm_to_rtl") {

  clock_out(clk);

  // From controller to transactor_tlm_rtl
  controller.initiator_socket(tlm_to_rtl.target_socket);
  tlm_to_rtl.word0(word0_rt);
  tlm_to_rtl.word1(word1_rt);
  tlm_to_rtl.key1(key0);
  tlm_to_rtl.key2(key1);
  tlm_to_rtl.key3(key2);
  tlm_to_rtl.key4(key3);
  tlm_to_rtl.mode(mode);
  tlm_to_rtl.threshold(threshold_rt);
  tlm_to_rtl.rst(rst);
  tlm_to_rtl.input_ready(din);
  tlm_to_rtl.clk(clk);

  // From transactor_tlm_rtl to xtea
  xtea.word0(word0_rt);
  xtea.word1(word1_rt);
  xtea.key1(key0);
  xtea.key3(key2);
  xtea.key2(key1);
  xtea.key4(key3);
  xtea.mode(mode);
  xtea.rst(rst);
  xtea.input_ready(din);
  xtea.clk(clk);
  xtea.output0(word0_decrypt);
  xtea.output1(word1_decrypt);
  xtea.output_ready(dout);

  // From transactor_tlm_rtl to transactor_rtl_ams
  rtl_to_ams.word0_rtl(word0_decrypt);
  rtl_to_ams.word1_rtl(word1_decrypt);
  rtl_to_ams.output_ready(dout);
  rtl_to_ams.threshold_rtl(threshold_rt);
  rtl_to_ams.command_ams(command_ams);
  rtl_to_ams.threshold_ams(threshold_ams);

  // From transactor_rtl_ams to valve
  valve.threshold(threshold_ams);
  valve.command(command_ams);
  valve.current_aperture(aperture);
  
  // From valve to watertank
  watertank.aperture(aperture);
  watertank.water_level(water_level_ams);

  // From watertank to transactor_ams_rtl
  ams_to_rtl.water_level_ams(water_level_ams);
  ams_to_rtl.water_level_rtl(water_level_rt);

  // From transactor_ams_rtl to transactor_rtl_tlm
  rtl_to_tlm.water_level_rt(water_level_rt);
  rtl_to_tlm.initiator_socket(controller.target_socket);

  SC_THREAD(clock_gen);
}


// Clock
void Watertank_system::clock_gen() {
  while (true) {
    clock_out.write(SC_LOGIC_1);
    wait(PERIOD / 2, SC_US);
    clock_out.write(SC_LOGIC_0);
    wait(PERIOD / 2, SC_US);
  }
}