#ifndef CONTROLLER_TLM_HH
#define CONTROLLER_TLM_HH

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>

#include "struct.hh"
#include "Command_enum.hh"


class Controller : public sc_module,public virtual tlm::tlm_bw_transport_if<>,
    tlm::tlm_fw_transport_if<> {
  public:
    tlm::tlm_initiator_socket<> initiator_socket;
    tlm::tlm_target_socket<> target_socket;

    //Quantum for TLM LT
    tlm_utils::tlm_quantumkeeper m_qk;

    double thresh;
    double water_level;

    virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range);
    virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t);
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &t);

    // Not used, must be implemented to be compliant with the standard   
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data);
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload &trans);

    void run();
    void xtea_encrypt(int command);

    //Constructor
    Controller( sc_module_name name, sc_uint<32> key0, sc_uint<32> key1,
        sc_uint<32> key2, sc_uint<32> key3, sc_uint<32> delta);

    SC_HAS_PROCESS(Controller);

  private:
    sc_uint<32> word0;
    sc_uint<32> word1;
    sc_uint<32> key0;
    sc_uint<32> key1;
    sc_uint<32> key2;
    sc_uint<32> key3;
    sc_uint<32> delta;
    sc_time timing;

};  

#endif
