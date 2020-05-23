/*
hamlib_client.cpp

CAT control client using hamlib library

Copyright (C) 2020 by G3UKB Bob Cowdery

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

The authors can be reached by email at:

	bob@bobcowdery.plus.com

*/

#include "../Includes/includes.h"

//----------------------------------------------------
// Constructor
HamlibClient::HamlibClient(char* port, rig_model_t model) {
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Parameters
	strncpy_s(ser_port, port, 20);
	rig_model = model;

	// Set initial serial parameters
	current_port.type.rig = RIG_PORT_SERIAL;
	current_port.parm.serial.rate = 9600;
	current_port.parm.serial.data_bits = 8;
	current_port.parm.serial.stop_bits = 2;
	current_port.parm.serial.parity = RIG_PARITY_NONE;
	current_port.parm.serial.handshake = RIG_HANDSHAKE_NONE;
	strncpy_s(current_port.pathname, ser_port, FILPATHLEN - 1);
	// Turn off back end debug
	rig_set_debug_level(RIG_DEBUG_NONE);
 
}

//----------------------------------------------------
// Initialise
bool HamlibClient::init() {
	// Initialise the model
	rig_load_all_backends();
	current_rig = rig_init(rig_model);
	if (!current_rig) {
		printf("Hamlib: rig_init- error = %s\n", rigerror(retcode));
		return false;
	}
	return true;
}

//----------------------------------------------------
// Open comms to rig
bool HamlibClient::open() {

	strncpy_s(current_rig->state.rigport.pathname, ser_port, FILPATHLEN - 1);
	retcode = rig_open(current_rig);
	if (retcode != RIG_OK) {
		printf("Hamlib: rig_open- error = %s\n", rigerror(retcode));
		return false;
	}
	printf("Hamlib: port %s opened ok\n", ser_port);
	return true;
}

//----------------------------------------------------
// Open comms to rig
void HamlibClient::close() {
	rig_close(current_rig);
}

//----------------------------------------------------
// Set rig mode
bool HamlibClient::set_mode(int mode) {
	if (RSt::inst().get_analog_radio_running()) {
		switch (mode) {
		case 0:
			retcode = rig_set_mode(current_rig, RIG_VFO_CURR, RIG_MODE_LSB, 2700);
			break;
		case 1:
			retcode = rig_set_mode(current_rig, RIG_VFO_CURR, RIG_MODE_USB, 2700);
			break;
		case 5:
			retcode = rig_set_mode(current_rig, RIG_VFO_CURR, RIG_MODE_FM, 6000);
			break;
		case 6:
			retcode = rig_set_mode(current_rig, RIG_VFO_CURR, RIG_MODE_AM, 6000);
			break;
		default:
			retcode = rig_set_mode(current_rig, RIG_VFO_CURR, RIG_MODE_LSB, 2700);
			break;
		}
	}
	if (retcode != RIG_OK) {
		printf("Hamlib: rig_set_mode- error = %s \n", rigerror(retcode));
		return false;
	}
	return true;
}

//----------------------------------------------------
// Set rig frequency
bool HamlibClient::set_freq(int freq) {
	if (RSt::inst().get_analog_radio_running()) {
		retcode = rig_set_freq(current_rig, RIG_VFO_CURR, freq);
		if (retcode != RIG_OK) {
			printf("Hamlib: rig_set_freq- error = %s \n", rigerror(retcode));
			return false;
		}
	}
	return true;
}
