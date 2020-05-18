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
C_ToggleButton::C_ToggleButton(std::string cb_key, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Toggle_Button(x, y, w, h, button_label) {
	// Retrieve our callback function
	key = cb_key;
	cb = RSt::inst().get_cb(key);
	// Radio interface ref
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	// Set button attributes
	id = button_id;
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int C_ToggleButton::handle(int event) {
	//std::function< void(void) > cb = RSt::inst().get_cb(key);
	switch (event) {
	case FL_PUSH: {
		if (value()) {
			// Hide 
			cb();
			// Button release
			clear();
		}
		else {
			// Show
			cb();
			// Button depressed
			set();
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}