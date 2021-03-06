/*
radio_interface.cpp

Radio interface for the FLTK Console

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

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public
//----------------------------------------------------
// Create a repository
RadioInterface::RadioInterface() {
	p = (Preferences*)RSt::inst().get_obj("PREFS");
}

//----------------------------------------------------
// Make a wisdom file fot FFT if not present
void RadioInterface::ri_make_wisdom() {
#ifdef linux
	c_server_make_wisdom((char *)"./wisdom/");
#else
	c_server_make_wisdom((char *)"E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom/");
#endif
}

//----------------------------------------------------
// Cold start the radio server
void RadioInterface::cold_start() {
	// Initialise and run server
	if (c_server_init()) {
		c_server_set_num_rx(p->get_num_radios());
		if (ri_set_default_audio()) {
			if (ri_radio_discover()) {
				RSt::inst().set_discovered(true);
				if (ri_server_start())
					RSt::inst().set_server_running(true);
				else
					std::cout << std::endl << "Failed to start server!" << std::endl;
			}
			else
				std::cout << std::endl << "Failed to discover radio!" << std::endl;
		}
		else
			std::cout << std::endl << "Failed to configure audio!" << std::endl;
	}
	else
		std::cout << std::endl << "Failed to initialise server!" << std::endl;
}

//----------------------------------------------------
// Restart the radio server
bool RadioInterface::restart() {
	// Stop server if running
	if (RSt::inst().get_server_running()) {
		ri_server_terminate();
	}
	// Initialise and run server
	bool success = false;
	RSt::inst().set_discovered(false);
	RSt::inst().set_server_running(false);
	if (c_server_init()) {
		c_server_set_num_rx(p->get_num_radios());
		if (ri_radio_discover()) {
			RSt::inst().set_discovered(true);
			if (ri_server_start()) {
				RSt::inst().set_server_running(true);
				success = true;
			}
			else
				std::cout << std::endl << "Failed to start server!" << std::endl;
		}
		else
			std::cout << std::endl << "Failed to discover radio!" << std::endl;
	}
	else
		std::cout << std::endl << "Failed to initialise server!" << std::endl;

	return success;
}

//----------------------------------------------------
// Update the radio server
void RadioInterface::reset_radio_state() {
	// Re-establish all radio state
	if (RSt::inst().get_server_running()) {
		set_audio_paths();
		set_frequencies();
		set_modes();
		set_filters();
	}
}

//----------------------------------------------------
// Set the default audio path (used when no configured paths)
bool RadioInterface::ri_set_default_audio() {
	int direction;
	char* host_api = nullptr;
	char* dev = nullptr;

	// Set up a route for RX1 to default output
	DeviceEnumList* l = c_server_enum_audio_outputs();
	for (int i = 0; i < l->entries; i++) {
		//printf("%d,%s,%s\n", l->devices[i].default_id, l->devices[i].name, l->devices[i].host_api);
		if (l->devices[i].default_id) {
			direction = l->devices[i].direction;
			host_api = l->devices[i].host_api;
			dev = l->devices[i].name;
			break;
		}
	}
	if (host_api != nullptr && dev != nullptr) {
		c_server_set_audio_route(direction, (char*)LOCAL_AF, 1, host_api, dev, (char*)BOTH);
		return true;
	}
}

//----------------------------------------------------
// Start the inbuilt server
bool RadioInterface::ri_server_start() {

	if (RSt::inst().get_server_running()) {
		return true;
	}
	else {
		// Start server
		if (c_server_start())
			return true;
		else
			return false;
	}
}

//----------------------------------------------------
// Terminate the inbuilt server
bool RadioInterface::ri_server_terminate() {
	if (RSt::inst().get_server_running()) {
		if (c_server_terminate())
			return true;
		else
			return false;
	}
}

//----------------------------------------------------
// Try and discover radio hardware
bool RadioInterface::ri_radio_discover() {
	if (RSt::inst().get_discovered()) {
		return true;
	} else {
		// Discover radio
		if (c_radio_discover())
			return true;
		else
			return false;
	}
}

//----------------------------------------------------
// Start the radio hardware
bool RadioInterface::ri_radio_start(int wbs) {
	if (RSt::inst().get_radio_running()) {
		return true;
	}
	else {
		// Discover radio
		if (c_radio_start(wbs))
			return true;
		else
			return false;
	}
}

//----------------------------------------------------
// Stop the radio hardware
bool RadioInterface::ri_radio_stop() {
	if (RSt::inst().get_radio_running()) {
		// Stop radio
		if (!c_radio_stop())
			return false;
	}
	return true;
}

//----------------------------------------------------
// Set mode and adjust filters
void RadioInterface::ri_server_set_mode(int channel, int mode) {
	if (RSt::inst().get_server_running()) {
		set_mode_filter(channel, mode, p->get_filter(channel), true);
	}
}

//----------------------------------------------------
// Set a new filter, adjust for the current mode
void RadioInterface::ri_server_set_filter_freq(int channel, int filter) {

	if (RSt::inst().get_server_running()) {
		set_mode_filter(channel, p->get_mode(channel), filter, true);
	}
}

//----------------------------------------------------
// Set receiver 1/2/3/TX frequency
void RadioInterface::ri_server_cc_out_set_freq(int radio, unsigned int freq_in_hz) {
	if (radio == 1) {
		c_server_cc_out_set_rx_tx_freq(freq_in_hz);
	} 
	else if (radio == 2) {
		c_server_cc_out_set_rx_2_freq(freq_in_hz);
	}
	else if (radio == 3) {
		c_server_cc_out_set_rx_3_freq(freq_in_hz);
	}
	// If duplex we set the TX freq independent of RX1
	else if (radio == 4) {
		if (RSt::inst().get_duplex()) {
			c_server_cc_out_set_rx_tx_freq(freq_in_hz);
		}
	}
}

//----------------------------------------------------
// Set duplex/simplex
void RadioInterface::ri_server_cc_out_set_duplex(bool state) {
	if (RSt::inst().get_server_running()) {
		c_server_cc_out_duplex(state);
	}
}

//----------------------------------------------------
// Set TX/RX
void RadioInterface::ri_server_cc_out_set_mox(bool state) {
	if (RSt::inst().get_server_running()) {
		c_server_mox(state);
	}
}

//----------------------------------------------------
// Reset all audio paths on audio init for each rx or after a change in number of rx's
void RadioInterface::set_audio_paths() {
	// We don't know what receivers were left alone and which were started so
	// all paths must be updated.
	// Clear all routes
	if (!c_server_clear_audio_routes()) {
		std::cout << "Failed to clear audio routes!" << std::endl;
		return;
	}
	// Retrieve and set audio routes
	struct_audio_desc desc;
	for (int radio = 1; radio <= p->get_num_radios(); radio++) {
		// ToDo change for TX
		desc = p->get_audio_desc(radio);
		if (desc.valid) {
			// Set audio path
			if (radio == 4) {
				// TX
				// ToDo add to server
				c_server_set_audio_route((int)AudioType::INPUT, desc.loc_part, radio, desc.api_part, desc.dev_part, desc.ch_part);
				//printf("%s,%d,%s,%s,%s\n", desc.sink_part, radio, desc.api_part, desc.dev_part, desc.ch_part);
			}
			else {
				// RX
				c_server_set_audio_route((int)AudioType::OUTPUT, desc.loc_part, radio, desc.api_part, desc.dev_part, desc.ch_part);
				//printf("%s,%d,%s,%s,%s\n", desc.sink_part, radio, desc.api_part, desc.dev_part, desc.ch_part);
			}
		}
	}
	// Restart audio
	c_server_restart_audio_routes();
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Return the high, low filter frequency for a given filter id
void RadioInterface::get_filter_limits(int filter, int* low, int* high) {

	switch (filter) {
	case 0: *low = 100; *high = 6100; break;
	case 1: *low = 100; *high = 4100; break;
	case 2: *low = 300; *high = 3000; break;
	case 3: *low = 300; *high = 2700; break;
	case 4: *low = 300; *high = 2400; break;
	case 5: *low = 300; *high = 1300; break;
	case 6: *low = 500; *high = 1000; break;
	case 7: *low = 600; *high = 850; break;
	case 8: *low = 700; *high = 800; break;
	default: *low = 300; *high = 2400;
	}
}

//----------------------------------------------------
// Mode and filter are interrelated so must be set in unison
void RadioInterface::set_mode_filter(int channel, int mode, int filter_id, bool set_radio) {
	int filter_low, filter_high, low, high;
	// Get filter limits
	get_filter_limits(filter_id, &filter_low, &filter_high);
	// Adjust according to current mode
	if ((MODES)mode == MODES::CH_LSB || (MODES)mode == MODES::CH_CWL || (MODES)mode == MODES::CH_DIGL) {
		low = -filter_high;
		high = -filter_low;
	}
	else if ((MODES)mode == MODES::CH_USB || (MODES)mode == MODES::CH_CWU || (MODES)mode == MODES::CH_DIGU) {
		low = filter_low;
		high = filter_high;
	}
	else {
		low = -filter_high;
		high = filter_high;
	}

	// Set new filter and/or mode
	if (set_radio) {
		if (channel == 4) {
			c_server_set_tx_mode(channel, mode);
			c_server_set_tx_filter_freq(channel, low, high);
		}
		else {
			c_server_set_rx_mode(channel, mode);
			c_server_set_rx_filter_freq(channel, low, high);
		}
	}
}

//----------------------------------------------------
// Reset all frequencies after a change in number of receivers
void RadioInterface::set_frequencies() {
	// We don't know what receivers were left alone and which were started so
	// all paths must be updated.

	// Retrieve and set frequency
	int frequency;
	for (int radio = 1; radio <= p->get_num_radios(); radio++) {
		frequency = p->get_freq(radio);
		ri_server_cc_out_set_freq(radio-1, frequency);
	}
}

//----------------------------------------------------
// Reset all modes after a change in number of receivers
void RadioInterface::set_modes() {
	// We don't know what receivers were left alone and which were started so
	// all paths must be updated.
	
	// Retrieve and set modes
	int mode;
	for (int radio = 1; radio <= p->get_num_radios(); radio++) {
		mode = p->get_mode(radio);
		ri_server_set_mode(radio-1, mode);
	}
}

//----------------------------------------------------
// Reset all filters after a change in number of receivers
void RadioInterface::set_filters() {
	// We don't know what receivers were left alone and which were started so
	// all paths must be updated.

	// Retrieve and set filters
	int filter;
	for (int radio = 1; radio <= p->get_num_radios(); radio++) {
		filter = p->get_filter(radio);
		ri_server_set_filter_freq(radio-1, filter);
	}
}