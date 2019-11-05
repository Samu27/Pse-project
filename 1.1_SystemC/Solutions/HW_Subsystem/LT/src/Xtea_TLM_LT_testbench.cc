#include "Xtea_TLM_LT_testbench.hh"

// Constructor
Xtea_TLM_LT_testbench::Xtea_TLM_LT_testbench(sc_module_name name) : sc_module(name) {
  initiator_socket(*this);
  SC_THREAD(run);

  // Parameter for quantum
  m_qk.set_global_quantum(sc_time(200, SC_NS));
  m_qk.reset();
}


void Xtea_TLM_LT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {
}


tlm::tlm_sync_enum Xtea_TLM_LT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
  return tlm::TLM_COMPLETED;
}


void Xtea_TLM_LT_testbench::run() {  
  sc_time local_time = m_qk.get_local_time();
  iostruct xtea_packet;

  tlm::tlm_generic_payload payload;
  payload.set_data_ptr((unsigned char*) &xtea_packet);
  payload.set_address(0);
  payload.set_write();

  // Values insertion
  xtea_packet.word0 = 0x12345678;  
  xtea_packet.word1 = 0x9abcdeff;
  xtea_packet.key0 = 0x6a1d78c8; 
  xtea_packet.key1 = 0x8c86d67f; 
  xtea_packet.key2 = 0x2a65bfbe; 
  xtea_packet.key3 = 0xb4bd6e46;
  // Encryption mode
  xtea_packet.mode = 0;

  // Local time update
  local_time = m_qk.get_local_time();

  // Print origial text
  cout << "\nOriginal text: " << hex << xtea_packet.word0 << "-" << hex << xtea_packet.word1 << "\n";                        
  
  initiator_socket->b_transport(payload, local_time);

  // Print encrypted text if status OK
  if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "\nEncrypted text: " << hex << xtea_packet.output0 << "-" << hex << xtea_packet.output1 << "\n";
  }

  // Decryption mode
  xtea_packet.mode = 1;

  payload.set_read();
  initiator_socket->b_transport(payload, local_time);
  
  // Print decrypted text if status OK
  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE)  {
    cout << "\nDecrypted text: " << hex << xtea_packet.output0 << "-" << hex << xtea_packet.output1 << "\n";
  }

  // Synchronization
  cout << "\nTime: " << sc_time_stamp() << " + " << local_time << endl;
  m_qk.set(local_time);
  
  if (m_qk.need_sync()) {
    cout << "\nSynchronizing... ";
    m_qk.sync();
    cout << "Done" << endl;
  }
  
  sc_stop();
  
}
