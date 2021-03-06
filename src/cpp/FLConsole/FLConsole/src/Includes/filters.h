/*
filters.h

Filters window header for the FLTK Console

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
// Defines

//==============================================================================
/*
	A modes window component
*/
class FilterButton;
class Filters : public Fl_Window
{
public:
	//==============================================================================
	Filters(int radio, int w, int h);
	~Filters() {};
	void close();

	//==============================================================================
	int filter_handle_event(int state, int id);

private:
	//==============================================================================
	// State variables
	// Radio inst
	int radio_id;
	// Our prefs
	Preferences* p;
	// Ref to set radio parameters
	RadioInterface* r_i;
	// Fixed labels (labels are not copied and must remain in scope
	// Note: dont use std:string as std:string.cstr() creates a new
	// cstr in a fixed overwritten area and all labels will be the last label..
	char f6k0[10] = "6.0KHz";
	char f4k0[10] = "4.0KHz";
	char f2k7[10] = "2.7KHz";
	char f2k4[10] = "2.4KHz";
	char f2k1[10] = "2.1KHz";
	char f1k0[10] = "1.0KHz";
	char f500[10] = "500Hz";
	char f250[10] = "250Hz";
	char f100[10] = "100Hz";
	
	// Button references
	C_ToggleButton* filter_0_btn;
	C_ToggleButton* filter_1_btn;
	C_ToggleButton* filter_2_btn;
	C_ToggleButton* filter_3_btn;
	C_ToggleButton* filter_4_btn;
	C_ToggleButton* filter_5_btn;
	C_ToggleButton* filter_6_btn;
	C_ToggleButton* filter_7_btn;
	C_ToggleButton* filter_8_btn;

	// Structures to drive generation and updating
	struct filter_button {
		C_ToggleButton* filter;
		char *label;
		int id;
	};
	struct filter_buttons {
		int n;					// Number of buttons
		filter_button items[9];	// Button descriptors
	}m_b = {
		9,
		{
			{ filter_0_btn, f6k0, 0 },
			{ filter_1_btn, f4k0, 1 },
			{ filter_2_btn, f2k7, 2 },
			{ filter_3_btn, f2k4, 3 },
			{ filter_4_btn, f2k1, 4 },
			{ filter_5_btn, f1k0, 5 },
			{ filter_6_btn, f500, 6 },
			{ filter_7_btn, f250, 7 },
			{ filter_8_btn, f100, 8 }
		}
	};

	// Key to callback cache
	char key[20];

	//==============================================================================
	// Method prototypes

};

