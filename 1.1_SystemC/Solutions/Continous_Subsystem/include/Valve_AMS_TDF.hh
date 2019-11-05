#ifndef VALVE_AMS_TDF_HH
#define VALUE_AMS_TDF_HH

#include "systemc-ams.h"
#include "systemc.h"
#include "Command_enum.hh"

SCA_TDF_MODULE (Valve_AMS_TDF) {
  public:
    // Port declarations
    sca_tdf::sca_in<int > command;
    sca_tdf::sca_out<double > current_aperture;
    sca_tdf::sca_in<double > threshold;

    // Constructor
    Valve_AMS_TDF(sc_core::sc_module_name);

    void set_attributes();
    void processing();
    
  private:
    // Keep track of the current aperture
    double current_valve;
};

#endif
