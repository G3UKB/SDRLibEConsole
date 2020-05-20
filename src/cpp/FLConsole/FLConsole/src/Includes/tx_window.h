/*
tx_window.h

Transmit window header for the FLTK Console

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
class RFSlider;

//==============================================================================
// The one and only Main Window

class TxWindow : public Fl_Double_Window
{
public:
	//==============================================================================
	TxWindow(int radio, int x, int y, int w, int h);
	~TxWindow() {};
	int handle(int event);
	void  resize(int x, int y, int w, int h);
	void handle_idle_timeout();
	void close();
	void mox_on();
	void mox_off();

	// Widget callbacks
	int duplex_handle_event(int state, int id);
	int mox_handle_event(int state, int id);
	int rf_gain_handle_event(int state, int id);
	int audio_handle_event(int state, int id);
	int mode_handle_event(int state, int id);
	int filt_handle_event(int state, int id);

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
	char duplex_str_up[8] = "Simplex";
	char duplex_str_dwn[8] = "Duplex";
	char mox_str_up[3] = "RX";
	char mox_str_dwn[3] = "TX";
	char rf_gain_str[5] = "Gain";
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
	C_ToggleButton* DuplexBtn;
	C_ToggleButton* MOXBtn;
	RFSlider* RFGain;
	C_ToggleButton* AudioBtn;
	C_ToggleButton* ModeBtn;
	C_ToggleButton* FilterBtn;
	AudioInput* audio_in;
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

//==============================================================================
// The RF gain slider
class RFSlider : public Fl_Value_Slider
{
public:
	//==============================================================================
	RFSlider(std::string cb_key, char* label, int x, int y, int w, int h, Fl_Color back_col);
	~RFSlider() {};
	void handle_event();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	TxWindow* myparent;

	// Key to callback cache
	std::string key;
	// The callback function retrieved from the cache
	std::function< int(int, int) > cb;

	//==============================================================================
	// Method prototypes
};