/*
running_state.h

Cache and state header for the FLTK Console

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

#pragma once

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only running state.
	This is a singleton pattern so easily accessible and contains all instance cache
	data and running state.
*/

// Track the audio allocations so we can ensure there are no conflicts
typedef struct audio_defs {
	char dev[30];
	char api[20];
	char ch[10];
};
typedef struct audio_alloc {
	// Def per radio
	audio_defs defs[3];
};
// State for SDR type radio
typedef struct t_sdr_state{
	bool discovered = false;
	bool server_running = false;
	bool radio_running = false;
	int num_radios = 1;
	bool active_radios[3] = { true, false, false };
	audio_alloc audio;
	bool duplex = false;
	bool mox = false;
};

// State for analog type radio
typedef struct t_analog_state {
	bool radio_running = false;
	bool duplex = false;
	bool mox = false;
};

// Full application state
typedef struct t_app_state {
	int radio_type = 0;
	t_sdr_state sdr_state;
	t_analog_state analog_state;
};

class RSt
{
public:
	static RSt& inst()
	{
		static RSt instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}
private:
	RSt() {}                    // Constructor? (the {} brackets) are needed here.
	// Map for cache of objects
	std::map < std::string, void* > obj_map;
	// Map for cache of callback methods
	std::map < std::string, std::function< int(int, int) > > cb_map;
	// Repository for state
	t_app_state app_state;

public:
	// Delete methods we don't need
	RSt(RSt const&) = delete;
	void operator=(RSt const&) = delete;
	// API
	// Object cache
	void put_obj(std::string name, void* obj);
	void* get_obj(std::string name);
	// Callback cache
	void put_cb(std::string name, std::function< int(int, int) > f);
	std::function< int(int, int) > get_cb(std::string name);

	// State
	void set_type(int radio_type);
	int get_type();
	// SDR type
	void set_discovered(bool found);
	bool get_discovered();
	void set_server_running(bool running);
	bool get_server_running();
	void set_radio_running(bool running);
	bool get_radio_running();
	void set_duplex(bool state);
	bool get_duplex();
	void set_mox(bool state);
	bool get_mox();
	// Analog type
	void set_analog_radio_running(bool running);
	bool get_analog_radio_running();
	void set_analog_duplex(bool state);
	bool get_analog_duplex();
	void set_analog_mox(bool state);
	bool get_analog_mox();

};