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

//----------------------------------------------------
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
	int i, j, k;
	metrics m;
	for (i=0, j=0, k=0 ; i<m_b.n; i++) {
		m = grid->get_cell_metrics(j, k);
		m_b.items[i].mode = new ModeButton(this, r_i, m_b.items[i].label, m_b.items[i].id, m, (Fl_Color)33, (Fl_Color)67);
		if (k++ == 3) {
			k = 0;
			j++;
		}
	}
	
	// Close up and display
	top_group->end();
	end();
	show();
}

//----------------------------------------------------
// Handle button state
void Modes::handle_button_state(int id) {
	int i;

	for (i = 0 ; i < m_b.n; i++) {
		if (m_b.items[i].id == id) {
			// Toggle pressed
			m_b.items[i].mode->set();
		}
		else {
			// Toggle released
			m_b.items[i].mode->clear();
		}
	}
}

//==============================================================================
// Mode buttons
ModeButton::ModeButton(Modes *top_level, RadioInterface* radio_interface, char* button_label, int mode_id, metrics m, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(radio_interface, button_label, m.x, m.y, m.w, m.h, back_col, label_col) {
	t_l = top_level;
	r_i = radio_interface;
	id = mode_id;
}

//----------------------------------------------------
// Handle click event
int ModeButton::handle(int event) {
	switch (event) {
	case FL_LEFT_MOUSE: {
		// Tell radio to change mode
		r_i->ri_server_set_rx_mode(0, id);
		// Tell parent to reflect state in buttons
		t_l->handle_button_state(id);
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}
