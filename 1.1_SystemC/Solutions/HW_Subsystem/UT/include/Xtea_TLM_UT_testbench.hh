#ifndef XTEA_TLM_UT_TESTBENCH_HH
#define XTEA_TLM_UT_TESTBENCH_HH

#include <systemc.h>
#include <tlm.h>
#include "struct.hh"


class Xtea_TLM_UT_testbench : public sc_module , public virtual tlm::tlm_bw_transport_if<> {
  public:
    tlm::tlm_initiator_socket<> initiator_socket;

    // Constructor
    Xtea_TLM_UT_testbench(sc_module_name name);

  private:
    SC_HAS_PROCESS(Xtea_TLM_UT_testbench);

    // Not used, must be implemented to be compliant with the standard   
    virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range);
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t);

    void run();
};

#endif
