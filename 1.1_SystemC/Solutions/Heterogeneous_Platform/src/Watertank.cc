#include "Watertank.hh"

// Constructor
Watertank::Watertank( sc_core::sc_module_name ):
    aperture("aperture"), water_level("water_level"),
    gainA("gainA",0.6), gainK2("gainK2",0.03),
    sub("subtraction"), integ("integration"),
    converter("converter"), converterLSFtoTDF("converterLSFtoTDF"),
    sig1("sig1"),sig2("sig2"),sig3("sig3"), sig4("sig4"),sig5("sig5") {
     
  // Input
  converter.inp(aperture);
  converter.y(sig1);

  // Multiply by a
  gainA.x(sig1);
  gainA.y(sig2);

  // Subtraction
  sub.x1(sig2);
  sub.x2(sig5);
  sub.y(sig3);

  // Integration
  integ.x(sig3);
  integ.y(sig4);

  // Multiply by K2
  gainK2.x(sig4);
  gainK2.y(sig5);

  // Convertion from LSF to TDF
  converterLSFtoTDF.x(sig4);
  converterLSFtoTDF.outp(water_level);
}
