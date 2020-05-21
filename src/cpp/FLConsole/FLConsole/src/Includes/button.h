/*
button.h

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

#pragma once

#include "includes.h"

//==============================================================================
// The generic button class
class C_Button : public Fl_Button
{
public:
	//==============================================================================
	C_Button(std::string cb_key, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color button_back_col, Fl_Color button_label_col);
	~C_Button() {};
	int handle(int event);
	void button_up();
	void button_down();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Ref to radio interface
	RadioInterface* r_i;

	// Our button id if required
	int id;

	// Button attributes
	char* label;
	Fl_Color back_col;
	Fl_Color label_col;

	// Key to callback cache
	std::string key;
	// The callback function retrieved from the cache
	std::function< int(int, int) > cb;

	//==============================================================================
	// Method prototypes
};