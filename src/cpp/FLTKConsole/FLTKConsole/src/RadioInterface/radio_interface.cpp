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
	all_state = new AllState;
	p = (Preferences*)RSt::inst().get_obj("PREFS");
}

//----------------------------------------------------
// Reset server
void RadioInterface::reset() {
	audio_set = false;
	server_running = false;
	radio_discovered = false;
	radio_running = false;
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
// Set the default audio path (used when no configured paths)
bool RadioInterface::ri_set_default_audio() {
	int direction;
	char* host_api = nullptr;
	char* dev = nullptr;

	if (!audio_set) {
		audio_set = true;
		// Set up a route for RX1 to default output
		DeviceEnumList* l = c_server_enum_audio_outputs();
		for (int i = 0; i < l->entries; i++) {
			printf("%d,%s,%s\n", l->devices[i].default_id, l->devices[i].name, l->devices[i].host_api);
			if (l->devices[i].default_id) {
				direction = l->devices[i].direction;
				host_api = l->devices[i].host_api;
				dev = l->devices[i].name;
				break;
			}
				}
		if (host_api != nullptr && dev != nullptr) {
			c_server_set_audio_route(direction, (char*)LOCAL, 1, host_api, dev, (char*)BOTH);
			return true;
		}
	}
	return false;
}

//----------------------------------------------------
// Start the inbuilt server
bool RadioInterface::ri_server_start() {

	if (!server_running) {
		// Start server
		if (c_server_start()) {
			server_running = true;
		} else {
			std::cout << "Interface: Failed to start server!" << std::endl;
			return false;
		}
	}
	return true;
}

//----------------------------------------------------
// Terminate the inbuilt server
bool RadioInterface::ri_server_terminate() {

}

//----------------------------------------------------
// Try and discover radio hardware
bool RadioInterface::ri_radio_discover() {
	if (!radio_discovered) {
		// Discover radio
		if (c_radio_discover()) {
			radio_discovered = true;
		} else {
			std::cout << "Interface: Radio hardware not found!" << std::endl;
			return false;
		}
	}
	return true;
}

//----------------------------------------------------
// Start the radio hardware
bool RadioInterface::ri_radio_start(int wbs) {
	if (!radio_running) {
		// Start radio
		if (c_radio_start(wbs)) {
			radio_running = true;
		} else {
			std::cout << "Interface: Failed to start radio!" << std::endl;
			return false;
		}
	}
	return true;
}

//----------------------------------------------------
// Stop the radio hardware
bool RadioInterface::ri_radio_stop() {
	if (radio_running) {
		radio_running = false;
		// Stop radio
		if (!c_radio_stop()) {
			printf("Interface: Failed to stop radio!");
			return false;
		}
	}
	return true;
}

//----------------------------------------------------
// Set RX mode and adjust filters
void RadioInterface::ri_server_set_rx_mode(int channel, int mode) {
	if (server_running) {
		set_current_mode(channel, mode);
		set_mode_filter(channel, mode, get_current_filt_low(channel), get_current_filt_high(channel));
	}
}

//----------------------------------------------------
// Set a new filter, adjust for the current mode
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
		set_current_filt_low(channel, low);
		set_current_filt_high(channel, high);
		set_mode_filter(channel, get_current_mode(channel), low, high);
	}
}

//----------------------------------------------------
// Set receiver 1/2/3 frequency
void RadioInterface::ri_server_cc_out_set_rx_1_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_1_freq(freq_in_hz);
	set_current_freq(0, freq_in_hz);
}

void RadioInterface::ri_server_cc_out_set_rx_2_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_2_freq(freq_in_hz);
	set_current_freq(1, freq_in_hz);
}

void RadioInterface::ri_server_cc_out_set_rx_3_freq(unsigned int freq_in_hz) {
	c_server_cc_out_set_rx_3_freq(freq_in_hz);
	set_current_freq(2, freq_in_hz);
}

//==============================================================================
// Get methods for current radio(s) state
bool RadioInterface::is_radio_discovered() {
	return radio_discovered;
}

bool RadioInterface::is_server_running() {
	return server_running;
}

bool RadioInterface::is_radio_running() {
	return radio_running;
}

int RadioInterface::get_current_frequency(int channel) {
	return get_current_freq(channel);
}

int RadioInterface::get_current_rx_mode(int channel) {
	return get_current_mode(channel);
}

filter_desc RadioInterface::get_current_rx_filter_desc(int channel) {
	filter_desc filter_desc;
	if (radio_running) {
		set_mode_filter(channel , get_current_mode(channel), get_current_filt_low(channel), get_current_filt_high(channel), false);
	}
	filter_desc.f_lower = get_current_filt_freq_low(channel);
	filter_desc.f_upper = get_current_filt_freq_high(channel);
	return filter_desc;
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Mode and filter are interrelated so must be set in unison
void RadioInterface::set_mode_filter(int channel, int mode, int filter_low, int filter_high, bool set_radio) {
	int low;
	int high;
	if ((MODES)mode == MODES::CH_LSB || (MODES)mode == MODES::CH_CWL || (MODES)mode == MODES::CH_DIGL) {
		low = -filter_high;
		high = -filter_low;
		set_current_filt_freq_low (channel, get_current_freq(channel) - filter_high);
		set_current_filt_freq_high (channel, get_current_freq(channel) - filter_low);
	}
	else if ((MODES)mode == MODES::CH_USB || (MODES)mode == MODES::CH_CWU || (MODES)mode == MODES::CH_DIGU) {
		low = filter_low;
		high = filter_high;
		set_current_filt_freq_low (channel, get_current_freq(channel) + filter_low);
		set_current_filt_freq_high (channel, get_current_freq(channel) + filter_high);
	}
	else {
		low = -filter_high;
		high = filter_high;
		set_current_filt_freq_low (channel, get_current_freq(channel) - filter_high);
		set_current_filt_freq_high (channel, get_current_freq(channel) + filter_high);
	}

	// Set new filter and/or mode
	if (set_radio) {
		c_server_set_rx_mode(channel, mode);
		c_server_set_rx_filter_freq(channel, low, high);
	}
}

//----------------------------------------------------
// Supporting Get/Set methods
void RadioInterface::set_current_freq(int channel, int freq) {
	switch (channel) {
	case 0: all_state->rx_1.freq = freq; break;
	case 1: all_state->rx_2.freq = freq; break;
	case 2: all_state->rx_3.freq = freq; break;
	}
}
void RadioInterface::set_current_mode(int channel, int mode) {
	switch (channel) {
	case 0: all_state->rx_1.mode = mode; break;
	case 1: all_state->rx_2.mode = mode; break;
	case 2: all_state->rx_3.mode = mode; break;
	}
}
void RadioInterface::set_current_filt_low(int channel, int filt_low) {
	switch (channel) {
	case 0: all_state->rx_1.filt_lower = filt_low; break;
	case 1: all_state->rx_2.filt_lower = filt_low; break;
	case 2: all_state->rx_3.filt_lower = filt_low; break;
	}
}
void RadioInterface::set_current_filt_high(int channel, int filt_high) {
	switch (channel) {
	case 0: all_state->rx_1.filt_upper = filt_high; break;
	case 1: all_state->rx_2.filt_upper = filt_high; break;
	case 2: all_state->rx_3.filt_upper = filt_high; break;
	}
}
void RadioInterface::set_current_filt_freq_low(int channel, int filt_freq_low) {
	switch (channel) {
	case 0: all_state->rx_1.filt_freq_lower = filt_freq_low; break;
	case 1: all_state->rx_2.filt_freq_lower = filt_freq_low; break;
	case 2: all_state->rx_3.filt_freq_lower = filt_freq_low; break;
	}
}
void RadioInterface::set_current_filt_freq_high(int channel, int filt_freq_high) {
	switch (channel) {
	case 0: all_state->rx_1.filt_freq_upper = filt_freq_high; break;
	case 1: all_state->rx_2.filt_freq_upper = filt_freq_high; break;
	case 2: all_state->rx_3.filt_freq_upper = filt_freq_high; break;
	}
}
int RadioInterface::get_current_freq(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.freq;
	case 1: return all_state->rx_2.freq;
	case 2: return all_state->rx_3.freq;
	}
}
int RadioInterface::get_current_mode(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.mode;
	case 1: return all_state->rx_2.mode;
	case 2: return all_state->rx_3.mode;
	}
}
int RadioInterface::get_current_filt_low(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_lower;
	case 1: return all_state->rx_2.filt_lower;
	case 2: return all_state->rx_3.filt_lower;
	}
}
int RadioInterface::get_current_filt_high(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_upper;
	case 1: return all_state->rx_2.filt_upper;
	case 2: return all_state->rx_3.filt_upper;
	}
}
int RadioInterface::get_current_filt_freq_low(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_freq_lower;
	case 1: return all_state->rx_2.filt_freq_lower;
	case 2: return all_state->rx_3.filt_freq_lower;
	}
}
int RadioInterface::get_current_filt_freq_high(int channel) {
	switch (channel) {
	case 0: return all_state->rx_1.filt_freq_upper;
	case 1: return all_state->rx_2.filt_freq_upper;
	case 2: return all_state->rx_3.filt_freq_upper;
	}
}