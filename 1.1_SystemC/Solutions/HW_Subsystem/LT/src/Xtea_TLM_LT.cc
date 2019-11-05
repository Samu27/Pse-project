#include "Xtea_TLM_LT.hh"

// Constructor
Xtea_TLM_LT::Xtea_TLM_LT(sc_module_name name_) : sc_module(name_) 
    , target_socket("target_socket"), pending_transaction(NULL) {
  target_socket(*this);
}

void Xtea_TLM_LT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t) {
  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  timing_annotation = SC_ZERO_TIME;

  // If trans.is_write() then encryption mode
  if (trans.is_write()) {
    xtea_function();

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    ioDataStruct.output0 = output_temp0;
    ioDataStruct.output1 = output_temp1;
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  
  // If trans.is_read() then decryption mode
  } else if (trans.is_read()) {
    xtea_function();

    ioDataStruct.output0 = output_temp0;
    ioDataStruct.output1 = output_temp1;
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  }

  // Timing annotation, time increment
  t += timing_annotation;

}


void Xtea_TLM_LT::xtea_function() {
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

  timing_annotation += sc_time(100, SC_NS);
}


// Not used, must be implemented to be compliant with the standard   

bool Xtea_TLM_LT::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data) {
  return false;
}

tlm::tlm_sync_enum Xtea_TLM_LT::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t) {
  return tlm::TLM_COMPLETED;
}

unsigned int Xtea_TLM_LT::transport_dbg(tlm::tlm_generic_payload& trans) {
  return 0;
}


void Xtea_TLM_LT::end_of_elaboration() {
}

void Xtea_TLM_LT::reset() {
}
