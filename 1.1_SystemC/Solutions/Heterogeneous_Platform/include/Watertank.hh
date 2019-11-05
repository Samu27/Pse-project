#ifndef WATERTANK_HH
#define WATERTANK_HH

#include <systemc-ams.h>

class Watertank : public sc_core::sc_module {
  public:
    // Valve input
    sca_tdf::sca_in<double> aperture;
    // Watertank output
    sca_tdf::sca_out<double> water_level;

    // Constructor
    Watertank(sc_module_name);

  private:
    // Converters
    sca_lsf::sca_tdf::sca_source converter;
    sca_lsf::sca_tdf::sca_sink converterLSFtoTDF;
    // Multiplication
    sca_lsf::sca_gain gainA, gainK2;
    // Substraction
    sca_lsf::sca_sub sub;
    // Integration
    sca_lsf::sca_integ integ;
    // Signals
    sca_lsf::sca_signal sig1, sig2, sig3, sig4, sig5;
};

#endif
