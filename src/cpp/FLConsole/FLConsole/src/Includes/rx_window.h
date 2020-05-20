/*
rx_window.h

Auxiliary receivers window header for the FLTK Console

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

#include "includes.h"

//==============================================================================
// Defines

// Forward refs


//==============================================================================
// The one and only Main Window

class RxWindow : public Fl_Double_Window
{
public:
	//==============================================================================
	RxWindow(int radio, int x, int y, int w, int h);
	~RxWindow() {};
	int handle(int event);
	void  resize(int x, int y, int w, int h);
	void handle_idle_timeout();
	void close();

	// Widget callbacks
	int audio_handle_event(int e);
	int mode_handle_event(int e);
	int filt_handle_event(int e);

	//==============================================================================

private:
	//==============================================================================
	// State variables

	// Params
	int width;
	int height;

	// State
	int radio_id;

	//Layout
	GridLayout *grid;
	metrics m;

	// Widget main group
	Fl_Group *top_group;

	// Labels - must be fixed storage
	char audio_str_up[12] = "Audio>>";
	char audio_str_dwn[12] = "<<Audio";
	char mode_str_up[12] = "Mode>>";
	char mode_str_dwn[12] = "<<Mode";
	char filt_str_up[12] = "Filter>>";
	char filt_str_dwn[12] = "<<Filter";

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components
	C_ToggleButton* AudioBtn;
	C_ToggleButton* ModeBtn;
	C_ToggleButton* FilterBtn;
	AudioOutput* audio_out;
	Modes *modes;
	Filters *filters;

	// Structures
	struct struct_w_loc {
		int x;
		int y;
		int w;
		int h;
	};
	struct_w_loc w_loc;

	//==============================================================================
	// Method prototypes
	void do_layout();
	void set_location();
};

