#include "Valve_AMS_TDF.hh"

Valve_AMS_TDF::Valve_AMS_TDF( sc_core::sc_module_name):
    command("command"), threshold("threshold"), current_aperture("current_aperture") {
  current_valve = 0;
}


void Valve_AMS_TDF::processing() {
  switch (command.read()) {
    case IDLE:
      // Unchanged aperture
      current_aperture.write(current_valve);
      break;

    case OPEN:
      // If valve opening can be greater
      if (current_valve < threshold.read()) {
        current_valve += 0.25 * get_timestep().to_seconds(); // += delta
        current_aperture.write(current_valve);
      } else {
        current_valve = threshold.read();
        current_aperture.write(threshold.read());
      }
      break;

    case CLOSE:
      // If valve opening can be lower
      if (current_valve > 0) { 
        current_valve -= 0.25 * get_timestep().to_seconds();
        current_aperture.write(current_valve);
      } else {
        current_valve = threshold.read();
        current_aperture.write(0);
      }
      break;
  }
}


void Valve_AMS_TDF::set_attributes(){
}
