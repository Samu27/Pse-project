#include "Transactor_RTL_TLM.hh"

// Constructor
Transactor_RTL_TLM::Transactor_RTL_TLM(sc_module_name name) : sc_module(name) {
  initiator_socket(*this);
  m_qk.set_global_quantum(sc_time(100, SC_NS));
  m_qk.reset();

  SC_THREAD(run);
}

void Transactor_RTL_TLM::run() {
  while(1) {
    // Wait 4sec for a stabilization
    wait(4,SC_SEC);
    transduce();
  }
}

void Transactor_RTL_TLM::transduce() {
    watertank packet;
    packet.water_level = water_level_rt.read();

    sc_time local_time = m_qk.get_local_time();

    tlm::tlm_generic_payload payload;
    payload.set_data_ptr((unsigned char *)&packet);
    payload.set_write();

    initiator_socket->b_transport(payload, local_time);
}


void Transactor_RTL_TLM::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {
}

tlm::tlm_sync_enum Transactor_RTL_TLM::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
    return tlm::TLM_ACCEPTED;
}
