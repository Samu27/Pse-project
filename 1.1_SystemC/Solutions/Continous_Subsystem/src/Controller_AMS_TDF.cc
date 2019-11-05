#include "Controller_AMS_TDF.hh"

// Constructor
Controller_AMS_TDF::Controller_AMS_TDF( sc_core::sc_module_name):
    command("command"), threshold("threshold"), watertank("watertank") {
  thresh = 0.7;
  counter = 0;
  samples = 0;
}


void Controller_AMS_TDF::processing() {
  // Command based on water level
  if (watertank.read() >= 5 && watertank.read() <= 8.8) {
    command = IDLE;
    counter = 0;
  } else if (watertank.read() > 8.8 && counter == samplesToWait) {
    command = CLOSE;
    thresh *= 0.7;
    counter = 0;
  } else if (watertank.read() < 5 && counter == samplesToWait) {
    command = OPEN;
    thresh *= 1.1;
    counter = 0;
  }
  
  threshold.write(thresh);
  counter++;
}


void Controller_AMS_TDF::set_attributes() {
  watertank.set_timestep(5,SC_MS); // Waiting time
  watertank.set_delay(1);          // Delay
  samplesToWait = 1000;            // Number of samples
}
