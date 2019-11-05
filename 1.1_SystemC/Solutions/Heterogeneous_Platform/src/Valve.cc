#include "Valve.hh"

Valve::Valve( sc_core::sc_module_name): command("command"), 
    threshold("threshold"), current_aperture("current_aperture") {
  current_valve = 0;
}


void Valve::processing() {
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


void Valve::set_attributes() {
  threshold.set_timestep(5,SC_MS);
  threshold.set_delay(1);
  command.set_timestep(5,SC_MS);
  command.set_delay(1);
}
