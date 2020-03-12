/*
main_window.cpp

Window implementation for the FLTK Console

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

#include "../../Includes/includes.h"

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only main window
*/

//----------------------------------------------------
// Constructor
MainWindow::MainWindow(RadioInterface* radio_interface, int w, int h) : Fl_Double_Window(w, h) {

	r_i = radio_interface;

	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Add a group box
	top_group = new Fl_Group(5, 10, 320, 105);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Add start and stop buttons to the group
	StartBtn = new ControlButton(this, r_i, start_str, 0, 15, 20, 100, 20, (Fl_Color)33, (Fl_Color)67);
	StopBtn = new ControlButton(this, r_i, stop_str, 1, 120, 20, 100, 20, (Fl_Color)33, (Fl_Color)80);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	VFOComponent *c = new VFOComponent(r_i, radio_id, 0, 15, 50, 300, 55);

	// Close up and display
	top_group->end();
	end();
	show();

	// Show temp modes window
	m = new Modes(r_i, 270, 110);
}

//----------------------------------------------------
// Handle button state
void MainWindow::handle_button_state(int id) {
	if (id == 0) {
		// Start button pressed
		StartBtn->set();
		StopBtn->clear();
	}
	else if (id == 1) {
		// Stop button pressed
		StartBtn->clear();
		StopBtn->set();
	}
}

//==============================================================================
// Toggle button base
ControlButton::ControlButton(MainWindow* parent_widget, RadioInterface* radio_interface, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(radio_interface, button_label, x, y, w, h, back_col, label_col) {
	myparent = parent_widget;
	r_i = radio_interface;
	id = button_id;
}

//----------------------------------------------------
// Handle click event
// Each button is given a separate id. The id is used to determin the button purpose and therefore
// the action to take and also the class to callback to handle the button state.
int ControlButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (id == 0) {
			// Start events
			r_i->ri_radio_start(0);
			myparent->handle_button_state(id);
		}
		else if (id == 1) {
			// Stop events
			r_i->ri_radio_stop();
			myparent->handle_button_state(id);
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}