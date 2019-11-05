#include "Transactor_TLM_RTL.hh"

// Constructor
Transactor_TLM_RTL::Transactor_TLM_RTL(sc_module_name name) : 
    sc_module(name), target_socket("target_socket") {
  target_socket(*this);
  SC_THREAD(transduce);
  sensitive << clk.pos();
}

void Transactor_TLM_RTL::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {
    valve_commands = *((valve_struct *)trans.get_data_ptr());
    traduce.notify();
    wait(end_traduce);
}

void Transactor_TLM_RTL::transduce() {
  while(1) {
    wait(traduce);

    rst.write(1);
    wait();
    
    rst.write(0);
    wait();
    
    mode.write(1);
    
    key1.write(valve_commands.key0);
    key2.write(valve_commands.key1);
    key3.write(valve_commands.key2);
    key4.write(valve_commands.key3);
    word0.write(valve_commands.word0);
    word1.write(valve_commands.word1);
    input_ready.write(1);
    wait();
    
    input_ready.write(0);
    end_traduce.notify();
    threshold.write(valve_commands.threshold);
    wait();
  }
}

bool Transactor_TLM_RTL::get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) {
    return false;
}

tlm::tlm_sync_enum Transactor_TLM_RTL::nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) {
    return tlm::TLM_COMPLETED;
}

unsigned int Transactor_TLM_RTL::transport_dbg(tlm::tlm_generic_payload &trans) {
    return 0;
}
