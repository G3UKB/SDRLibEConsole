/*
main_window.h

Window header for the FLTK Console

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
class DiscoverButton;
class RadioChoice;
class ExitButton;
class CATThrd;

//==============================================================================
// The one and only Main Window

class MainWindow : public Fl_Double_Window
{
public:
	//==============================================================================
	MainWindow(int x, int y, int w, int h);
	~MainWindow() {};
	int handle(int event);
	void  resize(int x, int y, int w, int h);
	void handle_idle_timeout();
	void handle_radio(Fl_Widget* w);

	// Widget callbacks
	int exit_handle_event(int state, int id);
	int discover_handle_event(int state, int id);
	int cat_handle_event(int state, int id);
	int tx_handle_event(int state, int id);
	int ctrl_handle_event(int state, int id);
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
	int num_radios;
	int radio_id;
	int radio2_id;
	int radio3_id;
	int tx_id;

	//Layout
	GridLayout *grid;
	metrics m;

	// Widget main group
	Fl_Group *top_group;

	// Labels - must be fixed storage
	char ctrl_str_up[10] = "Start";
	char ctrl_str_dwn[10] = "Stop";
	char discover_str[12] = "Discover>>";
	char cat_str_up[12] = "CAT OFF";
	char cat_str_dwn[12] = "CAT ON";
	char tx_str_up[12] = "TX>>";
	char tx_str_dwn[12] = "<<TX";
	char audio_str_up[12] = "Audio>>";
	char audio_str_dwn[12] = "<<Audio";
	char mode_str_up[12] = "Mode>>";
	char mode_str_dwn[12] = "<<Mode";
	char filt_str_up[12] = "Filter>>";
	char filt_str_dwn[12] = "<<Filter";
	char exit_str[10] = "EXIT";

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components
	C_Button* DiscoverBtn;
	C_Button* ExitBtn;
	C_ToggleButton* CtrlBtn;
	C_ToggleButton* CATBtn;
	C_ToggleButton* TXBtn;
	C_ToggleButton* AudioBtn;
	C_ToggleButton* ModeBtn;
	C_ToggleButton* FilterBtn;
	Fl_Choice* SelectRadio;
	AudioOutput* audio_out;
	Modes *modes;
	Filters *filters;

	// RX windows
	RxWindow* Radio2_Win = NULL;
	RxWindow* Radio3_Win = NULL;

	// TX Window
	TxWindow* TX_Win = NULL;

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
