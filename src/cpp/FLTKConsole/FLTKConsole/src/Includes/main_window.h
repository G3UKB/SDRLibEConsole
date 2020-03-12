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

//==============================================================================
/*
	The one and only main window
*/
class ControlButton;
class MainWindow : public Fl_Double_Window
{
public:
	//==============================================================================
	MainWindow(RadioInterface* radio_interface, int w, int h);
	~MainWindow() {};
	void handle_button_state(int id);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// labels must be fixed storage
	char start_str[10] = "Start";
	char stop_str[10] = "Stop";
	char radio_id[10] = "radio-1";
	char mode_str[10] = "Mode";

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Widgets
	Fl_Group *top_group;

	// Components
	ControlButton* StartBtn;
	ControlButton* StopBtn;
	Fl_Button* ModeBtn;
	Modes *modes;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The start/stop button
class ControlButton : public ToggleButtonBase
{
public:
	//==============================================================================
	ControlButton(MainWindow* parent_widget, RadioInterface* radio_interface, char* label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col);
	~ControlButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int id;
	MainWindow* myparent;

	//==============================================================================
	// Method prototypes

};