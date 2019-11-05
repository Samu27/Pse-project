#ifndef TRANSACTOR_TLM_RTL_HH
#define TRANSACTOR_TLM_RTL_HH

#include <systemc.h>
#include <tlm.h>
#include "struct.hh"

class Transactor_TLM_RTL : public sc_module, public virtual tlm::tlm_fw_transport_if<> {
  public:
    tlm::tlm_target_socket<> target_socket;
    sc_event traduce, end_traduce;
    valve_struct valve_commands;

    // Constructor
    Transactor_TLM_RTL(sc_module_name name);

    void transduce();
    SC_HAS_PROCESS(Transactor_TLM_RTL);

    virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& t );

    // Not used, must be implemented to be compliant with the standard   
    virtual bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi & dmi_data );
    virtual tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,tlm::tlm_phase& phase,sc_time& t);
    virtual unsigned int transport_dbg(tlm::tlm_generic_payload &trans);

    sc_in<sc_logic> clk;
    sc_out<double> threshold;
    sc_out<sc_uint<32> > word0;
    sc_out<sc_uint<32> > word1;
    sc_out<sc_uint<32> > key1;
    sc_out<sc_uint<32> > key2;
    sc_out<sc_uint<32> > key3;
    sc_out<sc_uint<32> > key4;
    sc_out<sc_uint<1> > mode;
    sc_out<bool> input_ready;
    sc_out<bool> rst;

};

#endif 