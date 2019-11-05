#ifndef VALVE_HH
#define VALVE_HH

#include <systemc.h>
#include <systemc-ams.h>
#include "Command_enum.hh"

SCA_TDF_MODULE (Valve) {
  public:
    // Port declarations
    sca_tdf::sca_in<sc_uint<32> > command;
    sca_tdf::sca_out<double > current_aperture;
    sca_tdf::sca_in<double > threshold;

    // Constructor
    Valve(sc_core::sc_module_name);

    void set_attributes();
    void processing();
    
  private:
    // Keep track of the current aperture
    double current_valve;
};

#endif
