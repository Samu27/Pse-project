#include "Xtea_TLM_AT4.hh"

// Constructor
Xtea_TLM_AT4::Xtea_TLM_AT4(sc_module_name name_) : sc_module(name_) 
    , target_socket("target_socket"), pending_transaction(NULL) {
  target_socket(*this);
  SC_THREAD(IOPROCESS);
}


tlm::tlm_sync_enum Xtea_TLM_AT4::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
  if (pending_transaction != NULL) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  // First phase must be a begin_request
  if (phase != tlm::BEGIN_REQ) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  cout << "\t[XTEA:] nb_transport_fw invocation" << endl;
  cout << "\t[XTEA:] IOPROCESS activation" << endl;

  pending_transaction = &trans;
  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  phase = tlm::END_REQ;
  io_event.notify();
  cout << "\t[XTEA:] nb_transport_fw ended" << endl;

  return tlm::TLM_UPDATED;  
}


void Xtea_TLM_AT4::IOPROCESS() {
  sc_time timing_annotation;

  while (true)  {
    // Waiting end_request
    wait(io_event);

    cout << "\t[XTEA:] IOPROCESS activated" <<endl;
    wait(100, SC_NS);    

    // If trans.is_write() then encryption mode
    if (pending_transaction->is_write()) {
      xtea_function();

      pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);
      ioDataStruct.output0 = output_temp0;
      ioDataStruct.output1 = output_temp1;
      *((iostruct*) pending_transaction->get_data_ptr()) = ioDataStruct;
  
    // If trans.is_read() then decryption mode
    } else if (pending_transaction->is_read()) {
      xtea_function();

      pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);
      ioDataStruct.output0 = output_temp0;
      ioDataStruct.output1 = output_temp1;
      *((iostruct*) pending_transaction->get_data_ptr()) = ioDataStruct;
    }
    
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    
    cout << "[TESTB:] nb_transport_bw invocation, mode: write" << endl;
    
    target_socket->nb_transport_bw(*pending_transaction, phase, timing_annotation);
    pending_transaction = NULL;
  }

}


void Xtea_TLM_AT4::xtea_function() {
  sc_uint<64> sum;
  sc_uint<32> i, delta, w0, w1, temp;
  sum = 0;
  output_temp0 = 0;
  output_temp1 = 0;

  // Encryption mode
  if (ioDataStruct.mode == 0) {
    w0 = ioDataStruct.word0;
    w1 = ioDataStruct.word1;
    
    delta = 0x9e3779b9;
    
    for (i = 0; i < 32; i++) {
      if ((sum & 3) == 0)
        temp = ioDataStruct.key0;
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key1;
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      w0 += (((w1 << 4) ^ (w1 >> 5)) + w1) ^ (sum + temp);

      sum += delta;

      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key0;
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key1;
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      w1 += (((w0 << 4) ^ (w0 >> 5)) + w0) ^ (sum + temp);
    }

  // Decryption mode
  } else if (ioDataStruct.mode == 1) {
    delta = 0x9e3779b9;
    sum = delta * 32;
    w0 = ioDataStruct.output0;
    w1 = ioDataStruct.output1;
    
    for (i = 0; i < 32; i++) {
      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key0;
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key1;
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      w1 -= (((w0 << 4) ^ (w0 >> 5)) + w0) ^ (sum + temp);

      sum -= delta;

      if ((sum & 3) == 0)
        temp = ioDataStruct.key0;
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key1;
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key2;
      else
        temp = ioDataStruct.key3;

      w0 -= (((w1 << 4) ^ (w1 >> 5)) + w1) ^ (sum + temp);
    }
  }
  output_temp0 = w0;
  output_temp1 = w1;
}


// Not used, must be implemented to be compliant with the standard   

void Xtea_TLM_AT4::b_transport(tlm::tlm_generic_payload& trans, sc_time& t) {
}

bool Xtea_TLM_AT4::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
  return false;
}

unsigned int Xtea_TLM_AT4::transport_dbg(tlm::tlm_generic_payload& trans) {
  return 0;
}
