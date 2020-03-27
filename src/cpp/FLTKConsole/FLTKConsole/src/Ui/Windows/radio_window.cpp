/*
radio_window.cpp

Radio 2/3 window implementation for the FLTK Console

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
RadioWindow::RadioWindow(int radio, int x, int y, int w, int h) : WindowBase(radio, x, y, w, h) {

	r = radio;
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Display main window
	show();
}

//===================================================
// Event handlers
// Resize event
void  RadioWindow::resize(int x, int y, int w, int h) {
	// Tell window to resize all widgets
	Fl_Double_Window::resize(x, y, w, h);
	// Save position and size
	if (r == 2) {
		p->set_radio2_x(x);
		p->set_radio2_y(y);
		p->set_radio2_w(w);
		p->set_radio2_h(h);
	}
	else if (r == 3) {
		p->set_radio3_x(x);
		p->set_radio3_y(y);
		p->set_radio3_w(w);
		p->set_radio3_h(h);
	}
}


//===================================================
// Callbacks
//----------------------------------------------------
