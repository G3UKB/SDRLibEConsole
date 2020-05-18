/*
toggle.h

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

#pragma once

#include "includes.h"

//==============================================================================
// The audio button class
class C_ToggleButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	C_ToggleButton(std::string cb_key, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~C_ToggleButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Ref to radio interface
	RadioInterface* r_i;

	// Our button id if required
	int id;

	// Button attributes
	char* up_label;
	char* down_label;
	Fl_Color back_col;
	Fl_Color up_col;
	Fl_Color down_col;

	// Key to callback cache
	std::string key;
	// The callback function retrieved from the cache
	std::function< int(int) > cb;

	//==============================================================================
	// Method prototypes
};