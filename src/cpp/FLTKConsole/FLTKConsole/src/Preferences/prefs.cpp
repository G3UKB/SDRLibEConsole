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
#define DEFAULT_X 100
#define DEFAULT_Y 100
#define DEFAULT_W 331
#define DEFAULT_H 124

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
	app.set("window_w", window_w);
	app.set("window_h", window_h);
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
int Preferences::get_window_w() {
	return window_w;
}
int Preferences::get_window_h() {
	return window_h;
}
void Preferences::set_window_w(int w) {
	window_w = w;
}
void Preferences::set_window_h(int h) {
	window_h = h;
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
	app.get("window_x", window_x, DEFAULT_X);
	app.get("window_y", window_y, DEFAULT_Y);
	app.get("window_w", window_w, DEFAULT_W);
	app.get("window_h", window_h, DEFAULT_H);
}
