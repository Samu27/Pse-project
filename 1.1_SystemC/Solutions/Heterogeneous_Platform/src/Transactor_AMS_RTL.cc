#include "Transactor_AMS_RTL.hh"

// Constructor
Transactor_AMS_RTL::Transactor_AMS_RTL(sc_core::sc_module_name) :
		water_level_ams("water_ams"), water_level_rtl("water_rtl") {
}

// From rtl to ams
void Transactor_AMS_RTL::processing() {
  water_level_rtl.write(water_level_ams.read());
}

void Transactor_AMS_RTL::set_attribute() {
}
