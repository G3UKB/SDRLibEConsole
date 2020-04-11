/*
tx_window.cpp

Single TX window implementation for the FLTK Console

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

// Idle time callback
// We call back to the window to do housekeeping
void tx_window_idle_cb(void* data) {
	TxWindow* w = (TxWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, tx_window_idle_cb, data);
}

/*
	The one and only main window
*/
//----------------------------------------------------
// Constructor
TxWindow::TxWindow(int radio, int x, int y, int w, int h) : WindowBase(radio, x, y, w, h, 3, 4, 0) {
	r = radio;
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Display main window
	show();

	// Set an idle timeout
	Fl::add_timeout(0.2, tx_window_idle_cb, (void*)this);
}

//----------------------------------------------------
// Close window
void TxWindow::close() {
	Fl::remove_timeout(tx_window_idle_cb);
	Fl::delete_widget(this);
}

//===================================================
// Callbacks
//----------------------------------------------------

// Handle idle timeout
void TxWindow::handle_idle_timeout() {
	// Nothing todo here
	// Just call base class
	WindowBase::handle_idle_timeout();
}