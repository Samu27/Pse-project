#include <systemc-ams.h>
#include "systemc.h"

#include "Controller_AMS_TDF.hh"
#include "Valve_AMS_TDF.hh"
#include "Watertank_AMS_LSF.hh"


int sc_main(int argc, char **argv) {
  // Signal declaration
  sca_tdf::sca_signal<double > water_level;
  sca_tdf::sca_signal<double > threshold;
  sca_tdf::sca_signal<int > command;
  sca_tdf::sca_signal<double > current_aperture;

  Valve_AMS_TDF valve("valve");
  Watertank_AMS_LSF watertank("watertank");
  Controller_AMS_TDF controller("controller");

  // Bind signals
  valve.command(command);
  valve.threshold(threshold);
  valve.current_aperture(current_aperture);

  watertank.water_level(water_level);
  watertank.aperture(current_aperture);

  controller.command(command);
  controller.watertank(water_level);
  controller.threshold(threshold);

  sca_trace_file *file = sca_create_vcd_trace_file("water");

  sca_trace(file,command,"command");
  sca_trace(file,water_level,"water_level");
  sca_trace(file,threshold,"threshold");
  sca_trace(file,current_aperture,"current_aperture");

  sc_start(1800,SC_SEC);

  sca_close_vcd_trace_file(file);

  return 0;
}
