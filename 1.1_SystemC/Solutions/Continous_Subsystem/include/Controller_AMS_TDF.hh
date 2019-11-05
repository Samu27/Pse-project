#ifndef CONTROLLER_AMS_TDF_HH
#define CONTROLLER_AMS_TDF_HH

#include "systemc-ams.h"
#include "systemc.h"
#include "Command_enum.hh"

SCA_TDF_MODULE (Controller_AMS_TDF) {
  public:
    // Declarations
    sca_tdf::sca_in<double > watertank;
    sca_tdf::sca_out<double > threshold;
    sca_tdf::sca_out<int > command;

    // Constructor
    Controller_AMS_TDF(sc_core::sc_module_name);

    void set_attributes();
    void processing();

  private:
    long int counter;
    int samples, samplesToWait; 
    double thresh;
};

#endif
