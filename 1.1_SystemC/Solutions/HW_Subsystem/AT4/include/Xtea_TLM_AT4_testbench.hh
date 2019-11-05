#ifndef XTEA_TLM_AT4_TESTBENCH_HH
#define XTEA_TLM_AT4_TESTBENCH_HH

#include <systemc.h>
#include <tlm.h>
#include "struct.hh"

class Xtea_TLM_AT4_testbench : public sc_module, public virtual tlm::tlm_bw_transport_if<> {
  public:
    tlm::tlm_initiator_socket<> initiator_socket;

    // Constructor
    Xtea_TLM_AT4_testbench(sc_module_name name);

  private:
    SC_HAS_PROCESS(Xtea_TLM_AT4_testbench);

    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t);

    // Not used, must be implemented to be compliant with the standard   
    virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range);

    void run();

    sc_event available_response;
    bool response_pending;
};

#endif
