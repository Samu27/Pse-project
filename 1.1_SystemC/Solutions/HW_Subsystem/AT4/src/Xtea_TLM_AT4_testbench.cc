#include "Xtea_TLM_AT4_testbench.hh"

// Constructor
Xtea_TLM_AT4_testbench::Xtea_TLM_AT4_testbench(sc_module_name name) : sc_module(name) {
  initiator_socket(*this);
  SC_THREAD(run);
}


void Xtea_TLM_AT4_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {
}


void Xtea_TLM_AT4_testbench::run() {  
  sc_time local_time = SC_ZERO_TIME;
  iostruct xtea_packet;
  tlm::tlm_phase phase = tlm::BEGIN_REQ;

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
  xtea_packet.mode = 0;

  // Print origial text
  cout << "\nOriginal text: " << hex << xtea_packet.word0 << "-" << hex << xtea_packet.word1 << endl << endl;                        

  cout << "[TESTB:] nb_transport_fw invoked, mode: write" << endl;
  tlm::tlm_sync_enum result = initiator_socket->nb_transport_fw(payload, phase, local_time); //invoco la primitiva

  // Check
  if (result == tlm::TLM_COMPLETED) {
    cout << "[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop." << endl;
    sc_stop();
  }

  // Check
  if (phase != tlm::END_REQ) {
    cout << "[ERROR:] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "[TESTB:] Waiting nb_transport_bw" << endl;
  response_pending = true;
  wait(available_response);

  // Print encrypted text if status OK
  if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "\nEncrypted text: " << hex << xtea_packet.output0 << "-" << hex << xtea_packet.output1 << endl << endl;
  }

  response_pending = false;
  
  phase = tlm::BEGIN_REQ;

  payload.set_address(0);
  payload.set_data_ptr((unsigned char*) &xtea_packet);
  payload.set_read();
  

  // Decryption mode
  xtea_packet.mode = 1;

  cout << "[TESTB:] nb_transport_fw invoked, mode: read" << endl;
  result = initiator_socket->nb_transport_fw(payload, phase, local_time);

  // Check
  if (result == tlm::TLM_COMPLETED) {
    cout << "[ERROR:] Transaction error on nb_transport_fw. Forcing simulation stop." << endl;
    sc_stop();
  }

  // Check
  if (phase != tlm::END_REQ) {
    cout << "[ERROR:] Unexpected protocol phase on nb_transport_fw. Forcing simulation stop." << endl;
    sc_stop();
  }

  cout << "[TESTB:] Waiting nb_transport_bw" << endl;
  response_pending = true;
  wait(available_response);

  response_pending = false;    

  // Print decrypted text if status OK
  if(payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "\nDecrypted text: " << hex << xtea_packet.output0 << "-" << hex << xtea_packet.output1 << "\n";
  }

  sc_stop();
}


tlm::tlm_sync_enum Xtea_TLM_AT4_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t) {
  if (!response_pending) {
    cout << "[ERROR:] Unexpected state for nb_transport_be" << endl;
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }
  
  if (phase != tlm::BEGIN_RESP) {
    cout << "[ERROR:] Unexpected phase for nb_transport_be" << endl;
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  available_response.notify();
  
  phase = tlm::END_RESP;
  
  return tlm::TLM_COMPLETED;
}
