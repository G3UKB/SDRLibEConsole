/*
prefs.cpp

Preferences for the FLTK Console

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

/*
	Provides a high level interface to save/restore all application prefs. The underlying
	mechanism uses Fl-Preferences with a group for each logically related dataset.

*/

#include "../Includes/includes.h"

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only preferences class
*/

//==============================================================================
// PUBLIC
//----------------------------------------------------
// Constructor
Preferences::Preferences() {
	restore();
}

//----------------------------------------------------
// Save
void Preferences::save() {
	// Save the application data
	Fl_Preferences root(Fl_Preferences::USER, project, application);
	Fl_Preferences app(root, "APP");
	app.set("window_x", window_x);
	app.set("window_y", window_y);
}

//==============================================================================
// API
int Preferences::get_window_x() {
	return window_x;
}
int Preferences::get_window_y() {
	return window_y;
}
void Preferences::set_window_x(int x) {
	window_x = x;
}
void Preferences::set_window_y(int y) {
	window_y = y;
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Restore
void Preferences::restore() {
	// Restore the database
	Fl_Preferences root(Fl_Preferences::USER, project, application);
	// Read the application data
	Fl_Preferences app(root, "APP");
	app.get("window_x", window_x, 100);
	app.get("window_y", window_y, 100);
	printf("Data: %d,%d\n", window_x, window_y);
}
