/*
modes.cpp

Modes window implementation for the FLTK Console

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
// Main Modes Component Window
//==============================================================================

//==============================================================================
// Constructor/Destructor
Modes::Modes(RadioInterface* radio_interface, int w, int h) : Fl_Window(w, h) {

	r_i = radio_interface;
	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 5, w-10, h-10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w-10, h-10, 3, 4);

	// Add the mode buttons to the group
	metrics m;
	m = grid->get_cell_metrics(0, 0);
	mode_0_btn = new ModeButton(r_i, lsb, 0 , m);
	m = grid->get_cell_metrics(0, 1);
	ModeButton* mode_1_btn = new ModeButton(r_i, "USB", 1, m);
	m = grid->get_cell_metrics(0, 2);
	ModeButton* mode_2_btn = new ModeButton(r_i, "DSB", 2, m);
	m = grid->get_cell_metrics(0, 3);
	ModeButton* mode_3_btn = new ModeButton(r_i, "CW-L", 3, m);
	m = grid->get_cell_metrics(1, 0);
	ModeButton* mode_4_btn = new ModeButton(r_i, "CW-U", 4, m);
	m = grid->get_cell_metrics(1, 1);
	ModeButton* mode_5_btn = new ModeButton(r_i, "FM", 5, m);
	m = grid->get_cell_metrics(1, 2);
	ModeButton* mode_6_btn = new ModeButton(r_i, "AM", 6, m);
	m = grid->get_cell_metrics(1, 3);
	ModeButton* mode_7_btn = new ModeButton(r_i, "DIG-U", 7, m);
	m = grid->get_cell_metrics(2, 0);
	ModeButton* mode_8_btn = new ModeButton(r_i, "SPEC", 8, m);
	m = grid->get_cell_metrics(2, 1);
	ModeButton* mode_9_btn = new ModeButton(r_i, "DIG-L", 9, m);
	m = grid->get_cell_metrics(2, 2);
	ModeButton* mode_10_btn = new ModeButton(r_i, "SAM", 10, m);
	m = grid->get_cell_metrics(2, 3);
	ModeButton* mode_11_btn = new ModeButton(r_i, "DRM", 11, m);

	// Close up and display
	top_group->end();
	end();
	show();
}

//==============================================================================
// Mode buttons
ModeButton::ModeButton(RadioInterface* radio_interface, std::string button_label, int mode_id, metrics m) : Fl_Button(m.x, m.y, m.w, m.h, button_label.c_str()) {
	r_i = radio_interface;
	id = mode_id;
	color((Fl_Color)33);
}

//----------------------------------------------------
// Handle click event
int ModeButton::handle(int event) {
	switch (event) {
	case FL_LEFT_MOUSE: {
		printf("Clicked %d\n", id);
		r_i->ri_server_set_rx_mode(0, id);
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}