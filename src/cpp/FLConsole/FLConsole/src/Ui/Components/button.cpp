/*
button.cpp

Button component for the FLTK Console

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
// Generic button
C_Button::C_Button(std::string cb_key, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color button_back_col, Fl_Color button_label_col) : Fl_Button(x, y, w, h, button_label) {
	
	// Stash button attributes
	id = button_id;
	label = button_label;
	back_col = button_back_col;
	label_col = button_label_col;

	// Retrieve our callback function from the callback cache
	key = cb_key;
	cb = RSt::inst().get_cb(key);

	// Set Radio interface ref
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");

	// Set relevent button attributes
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)button_label_col);
}

//----------------------------------------------------
// Handle click event
int C_Button::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		cb(true, id);
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
void C_Button::button_up() {
	set();
}
void C_Button::button_down() {
	clear();
}