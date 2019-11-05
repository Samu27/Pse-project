#ifndef WATERTANK_SYSTEM_AMS_HH
#define WATERTANK_SYSTEM_AMS_HH

#include <systemc.h>
#include <systemc-ams.h>

#include "Controller_TLM.hh"
#include "Xtea.hh"
#include "Valve.hh"
#include "Watertank.hh"
#include "Transactor_AMS_RTL.hh"
#include "Transactor_RTL_AMS.hh"
#include "Transactor_RTL_TLM.hh"
#include "Transactor_TLM_RTL.hh"

#define PERIOD 10

class Watertank_system : public sc_core::sc_module {
  public:
    // Declarations
    sc_signal<double> threshold_rt;
    sc_signal<sc_uint<32> > word0_rt;
    sc_signal<sc_uint<32> > word1_rt;
    sc_signal<sc_uint<32> > word0_decrypt;
    sc_signal<sc_uint<32> > word1_decrypt;

    sca_tdf::sca_signal<double> threshold_ams;
    sca_tdf::sca_signal<sc_uint<32> > command_ams;
    sca_tdf::sca_signal<double> aperture;
    sca_tdf::sca_signal<double> water_level_ams;
    sc_signal<double> water_level_rt;


    sc_signal<sc_uint<32> > key0;
    sc_signal<sc_uint<32> > key1;
    sc_signal<sc_uint<32> > key2;
    sc_signal<sc_uint<32> > key3;
    sc_signal<sc_uint<1> > mode;
    sc_signal<sc_logic> clk; 
    sc_signal<bool> rst;
    sc_signal<bool> din;
    sc_signal<bool> dout;
    sc_out<sc_logic> clock_out;   


    // Modules
    Controller controller;
    Xtea xtea;
    Valve valve;
    Watertank watertank;

    Transactor_AMS_RTL ams_to_rtl;
    Transactor_RTL_AMS rtl_to_ams;
    Transactor_RTL_TLM rtl_to_tlm;
    Transactor_TLM_RTL tlm_to_rtl;


    Watertank_system(sc_core::sc_module_name);
    SC_HAS_PROCESS(Watertank_system);

    void clock_gen();

};

#endif