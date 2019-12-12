/*
radio_interface.cpp

Radio interface for the Juce Console

Copyright (C) 2019 by G3UKB Bob Cowdery

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

#include "radio_interface.h"
#include "../Common/extern.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public

//Macro for Singleton
juce_ImplementSingleton(RadioInterface)

//==============================================================================
// Call down methods

bool RadioInterface::ri_set_default_audio() {
	int direction;
	char* host_api;
	char* dev;

	if (!audio_set) {
		audio_set = true;
		// Set up a default route for RX1 to Speaker output
		DeviceEnumList* l = c_server_enum_audio_outputs();
		for (int i = 0; i < l->entries; i++) {
			if (String(l->devices[i].name).startsWith("Speakers")) {
				if (String(l->devices[i].host_api) == "MME") {
					direction = l->devices[i].direction;
					host_api = l->devices[i].host_api;
					dev = l->devices[i].name;
					break;
				}
			}
		}
		c_server_set_audio_route(direction, LOCAL, 1, host_api, dev, BOTH);
	}
	return true;
}

bool RadioInterface::ri_server_start() {

	if (!server_running) {
		server_running = true;
		// Start server
		if (!c_server_start()) {
			printf("Failed to start server!");
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_server_terminate() {

}

bool RadioInterface::ri_radio_discover() {
	if (!radio_discovered) {
		radio_discovered = true;
		// Discover radio
		if (!c_radio_discover()) {
			printf("Radio hardware not found!");
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_radio_start(int wbs) {
	if (!radio_running) {
		radio_running = true;
		// Start radio
		if (!c_radio_start(wbs)) {
			printf("Failed to start radio!");
			return false;
		}
	}
	return true;
}

bool RadioInterface::ri_radio_stop() {
	if (radio_running) {
		radio_running = false;
		// Stop radio
		if (!c_radio_stop()) {
			printf("Failed to stop radio!");
			return false;
		}
	}
	return true;
}

void RadioInterface::ri_server_set_rx_mode(int channel, int mode) {
	if (server_running) {
		current_rx_mode = mode;
		set_mode_filter(channel, mode, current_rx_filter_low, current_rx_filter_high);
	}
}

void RadioInterface::ri_server_set_rx_filter_freq(int channel, int filter) {

	int low;
	int high;

	if (server_running) {
		switch (filter) {
		case 0: low = 100; high = 6100; break;
		case 1: low = 100; high = 4100; break;
		case 2: low = 300; high = 3000; break;
		case 3: low = 300; high = 2700; break;
		case 4: low = 300; high = 2400; break;
		case 5: low = 300; high = 1300; break;
		case 6: low = 500; high = 1000; break;
		case 7: low = 600; high = 850; break;
		case 8: low = 700; high = 800; break;
		default: low = 300; high = 2400;
		}
		current_rx_filter_low = low;
		current_rx_filter_high = high;
		set_mode_filter(channel, current_rx_mode, low, high);
	}
}

void RadioInterface::ri_server_cc_out_set_rx_1_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_1_freq(freq_in_hz);
	current_freq = freq_in_hz;
}

void RadioInterface::ri_server_cc_out_set_rx_2_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_2_freq(freq_in_hz);
	current_freq = freq_in_hz;
}

void RadioInterface::ri_server_cc_out_set_rx_3_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_3_freq(freq_in_hz);
	current_freq = freq_in_hz;
}

//==============================================================================
// Get methods
bool RadioInterface::is_radio_running() {
	return radio_running;
}

int RadioInterface::get_current_frequency() {
	return current_freq;
}

int RadioInterface::get_current_rx_mode() {
	return current_rx_mode;
}

filter_desc RadioInterface::get_current_rx_filter_desc() {
	filter_desc filter_desc;
	if (radio_running) {
		set_mode_filter(current_rx_mode, current_rx_filter_low, current_rx_filter_high);
	}
	filter_desc.f_lower = filt_freq_lower;
	filter_desc.f_upper = filt_freq_upper;
	return filter_desc;
}

//==============================================================================
// Private
// ToDo: Split this to just set for the display struct.
void RadioInterface::set_mode_filter(int channel, int mode, int filter_low, int filter_high) {
	int low;
	int high;
	if ((MODES)mode == MODES::CH_LSB || (MODES)mode == MODES::CH_CWL || (MODES)mode == MODES::CH_DIGL) {
		low = -filter_high;
		high = -filter_low;
		filt_freq_lower = current_freq - filter_high;
		filt_freq_upper = current_freq - filter_low;
	}
	else if ((MODES)mode == MODES::CH_USB || (MODES)mode == MODES::CH_CWU || (MODES)mode == MODES::CH_DIGU) {
		low = filter_low;
		high = filter_high;
		filt_freq_lower = current_freq + filter_low;
		filt_freq_upper = current_freq + filter_high;
	}
	else {
		low = -filter_high;
		high = filter_high;
		filt_freq_lower = current_freq - filter_high;
		filt_freq_upper = current_freq + filter_high;
	}

	// Set new filter and/or mode
	c_server_set_rx_mode(channel, mode);
	c_server_set_rx_filter_freq(channel, low, high);
}
