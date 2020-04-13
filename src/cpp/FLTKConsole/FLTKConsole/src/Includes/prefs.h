/*
prefs.h

Preferences header for the FLTK Console

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
	The one and only main window
*/

typedef struct struct_audio_desc {
	bool valid;
	char* sink_part;
	char* dev_part;
	char* api_part;
	char* ch_part;
};

class Preferences
{
public:
	//==============================================================================
	Preferences();
	~Preferences() {};

	//==============================================================================
	// Method prototypes
	// Save prefs
	void save();

	//------------------------------------
	// Public API
	// Main window position and size
	int get_window_x();
	int get_window_y();
	void set_window_x(int x);
	void set_window_y(int y);
	int get_window_w();
	int get_window_h();
	void set_window_w(int w);
	void set_window_h(int h);
	// Radio 2 window position and size
	int get_radio2_x();
	int get_radio2_y();
	void set_radio2_x(int x);
	void set_radio2_y(int y);
	int get_radio2_w();
	int get_radio2_h();
	void set_radio2_w(int w);
	void set_radio2_h(int h);
	// Radio 3 window position and size
	int get_radio3_x();
	int get_radio3_y();
	void set_radio3_x(int x);
	void set_radio3_y(int y);
	int get_radio3_w();
	int get_radio3_h();
	void set_radio3_w(int w);
	void set_radio3_h(int h);
	// TX window position and size
	int get_tx_x();
	int get_tx_y();
	void set_tx_x(int x);
	void set_tx_y(int y);
	int get_tx_w();
	int get_tx_h();
	void set_tx_w(int w);
	void set_tx_h(int h);
	// Radio mode, filter etc
	int get_num_radios();
	void set_num_radios(int num);
	int get_freq(int radio);
	void set_freq(int radio, int freq);
	int get_mode(int radio);
	void set_mode(int radio, int filter);
	int get_filter(int radio);
	void set_filter(int radio, int filter);
	// Audio paths
	void get_audio_path(int radio, char* audio_path);
	void set_audio_path(int radio, char* path);
	struct struct_audio_desc get_audio_desc(int radio);

private:
	//==============================================================================
	// State variables
	// For Fl_Preferences constructor
	char project[6] = "G3UKB";
	char application[12] = "FLTKConsole";

	//------------------------------------
	// Prefs items
	// Main window position and size
	int window_x;
	int window_y;
	int window_w;
	int window_h;
	int radio2_x;
	int radio2_y;
	int radio2_w;
	int radio2_h;
	int radio3_x;
	int radio3_y;
	int radio3_w;
	int radio3_h;
	int tx_x;
	int tx_y;
	int tx_w;
	int tx_h;
	// Radio mode, filter etc
	int num_radios;
	int radio_freq[3] = { 7100000,7100000,7100000 };
	int tx_freq;
	int radio_mode[3] = { 0,0,0 };
	int tx_mode;
	int radio_filter[3] = { 2,2,2 };
	int tx_filter;
	char radio_audio_path_1[100];
	char radio_audio_path_2[100];
	char radio_audio_path_3[100];
	char tx_audio_path[100];
	// Audio tokens
	struct_audio_desc audio_desc;

	//==============================================================================
	// Method prototypes
	void restore();
	struct struct_audio_desc parse_audio_desc(int radio);
};
