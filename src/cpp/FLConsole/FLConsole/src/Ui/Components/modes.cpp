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
Modes::Modes(int radio, int w, int h) : Fl_Window(w, h) {

	// radio instance
	radio_id = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	color((Fl_Color)24);
	align(Fl_Align(65));

	// Get the mode
	int mode = p->get_mode(radio_id);

	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 5, w-10, h-10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w-10, h-10, 3, 4);

	// Add the mode buttons to the group
	int i, j, k;
	metrics m;
	// Create a single callback for all mode buttons for this radio
	sprintf_s(key, "MODE_%d_CB", radio_id);
	std::function< int(int, int) > f2 = std::bind(&Modes::mode_handle_event, this, std::placeholders::_1, std::placeholders::_2);
	RSt::inst().put_cb(key, f2);

	for (i=0, j=0, k=0 ; i<m_b.n; i++) {
		m = grid->get_cell_metrics(j, k);
		m_b.items[i].mode = new C_ToggleButton(key, m_b.items[i].label, m_b.items[i].label, m_b.items[i].id, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
		if (i == mode) {
			m_b.items[i].mode->set();
			// Crashes!!
			//r_i->ri_server_set_mode(r-1, mode);
		}
		if (k++ == 3) {
			k = 0;
			j++;
		}
	}
	
	// Close up and display
	top_group->end();
	end();
	border(false);
	show();
}

//----------------------------------------------------
// Close window
void Modes::close() {
	Fl::delete_widget(this);
}

//----------------------------------------------------
// Handle button state
int Modes::mode_handle_event(int state, int id) {
	int i;
	for (i = 0 ; i < m_b.n; i++) {
		if (m_b.items[i].id == id) {
			// Toggle pressed
			m_b.items[i].mode->button_up();
			// Remember mode
			p->set_mode(radio_id, id);
		}
		else {
			// Toggle released
			m_b.items[i].mode->button_down();
		}
	}
	// Tell radio to change mode
	r_i->ri_server_set_mode(radio_id - 1, id);

	return true;
}
