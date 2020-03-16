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
	Filters(int w, int h);
	~Filters() {};

	//==============================================================================
	void handle_filter_button_state(int id);

private:
	//==============================================================================
	// State variables
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
	FilterButton* filter_0_btn;
	FilterButton* filter_1_btn;
	FilterButton* filter_2_btn;
	FilterButton* filter_3_btn;
	FilterButton* filter_4_btn;
	FilterButton* filter_5_btn;
	FilterButton* filter_6_btn;
	FilterButton* filter_7_btn;
	FilterButton* filter_8_btn;

	// Structures to drive generation and updating
	struct filter_button {
		FilterButton* filter;
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

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// A mode button
class FilterButton : public ToggleButtonBase
{
public:
	//==============================================================================
	FilterButton(Filters *filters, char* label, int filter_id, metrics m, Fl_Color back_col, Fl_Color label_col);
	~FilterButton() {};

	//==============================================================================
	int handle(int event);

private:
	//==============================================================================
	// State variables
	Filters *t_l;
	RadioInterface* r_i;
	int id;

	//==============================================================================
	// Method prototypes

};
