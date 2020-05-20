/*
toggle.cpp

Toggle button component for the FLTK Console

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
// PUBLIC

//==============================================================================
// Audio button
C_ToggleButton::C_ToggleButton(std::string cb_key, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color button_back_col, Fl_Color button_up_col, Fl_Color button_down_col) : Fl_Toggle_Button(x, y, w, h, button_up_label) {
	
	// Stash button attributes
	up_label = button_up_label;
	down_label = button_down_label;
	back_col = button_back_col;
	up_col = button_up_col;
	down_col = button_down_col;

	// Retrieve our callback function from the callback cache
	key = cb_key;
	cb = RSt::inst().get_cb(key);

	// Set Radio interface ref
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");

	// Set relevent button attributes
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)button_up_col);
	id = button_id;
}

//----------------------------------------------------
// Handle click event
int C_ToggleButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (value()) {
			// Callback for button down
			cb(false);
			clear();
			label(up_label);
			labelcolor(up_col);
		}
		else {
			// Callback for button up
			cb(true);
			set();
			label(down_label);
			labelcolor(down_col);
		}
		// Handled
		return 1;
	}
	default:
		// Not handled
		return Fl_Widget::handle(event);
	}
}

//----------------------------------------------------
// External activation
void C_ToggleButton::button_up() {
	set();
	label(down_label);
	labelcolor(down_col);
}
void C_ToggleButton::button_down() {
	clear();
	label(up_label);
	labelcolor(up_col);
}