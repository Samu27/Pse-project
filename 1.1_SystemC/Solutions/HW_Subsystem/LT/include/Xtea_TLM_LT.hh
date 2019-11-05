#ifndef XTEA_TLM_LT_HH
#define XTEA_TLM_LT_HH

#include <systemc.h>
#include <tlm.h>
#include "struct.hh"

class Xtea_TLM_LT : public sc_module, public virtual tlm::tlm_fw_transport_if<> {
  public:
    tlm::tlm_target_socket<> target_socket;

    // Variables
    iostruct  ioDataStruct;
    tlm::tlm_generic_payload* pending_transaction;
    sc_uint<32> output_temp0;
    sc_uint<32> output_temp1;

    // Timing annotation for LT and temporal decoupling
    sc_time timing_annotation;

    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t);

    // Not used, must be implemented to be compliant with the standard
    virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& t);
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload& trans);

    void end_of_elaboration(); 
    void reset(); 

    void xtea_function();

    // Constructor
    SC_HAS_PROCESS(Xtea_TLM_LT);
    Xtea_TLM_LT(sc_module_name name_);

};

#endif
