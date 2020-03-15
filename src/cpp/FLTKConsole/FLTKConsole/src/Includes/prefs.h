/*
prefs.h

Preferences header for the FLTK Console

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

#pragma once

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only main window
*/

class Preferences
{
public:
	//==============================================================================
	Preferences();
	~Preferences() {};

	//==============================================================================
	// Method prototypes
	// Save prefs
	void save();

	//------------------------------------
	// Public API
	// Windoe position and size
	int get_window_x();
	int get_window_y();
	void set_window_x(int x);
	void set_window_y(int y);
	int get_window_w();
	int get_window_h();
	void set_window_w(int w);
	void set_window_h(int h);

private:
	//==============================================================================
	// State variables
	// For Fl_Preferences constructor
	char project[6] = "G3UKB";
	char application[12] = "FLTKConsole";

	//------------------------------------
	// Prefs items
	// Main window position and size
	int window_x;
	int window_y;
	int window_w;
	int window_h;

	//==============================================================================
	// Method prototypes
	void restore();
	
};
