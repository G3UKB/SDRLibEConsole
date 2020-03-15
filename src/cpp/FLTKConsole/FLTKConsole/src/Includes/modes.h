/*
modes.h

Modes window header for the FLTK Console

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
class ModeButton;
class Modes : public Fl_Window
{
public:
	//==============================================================================
	Modes(Preferences* prefs, RadioInterface* radio_interface, int w, int h);
	~Modes() {};

	//==============================================================================
	void handle_mode_button_state(int id);

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
	char lsb[10] = "LSB";
	char usb[10] = "USB";
	char dsb[10] = "DSB";
	char cwl[10] = "CW-L";
	char cwu[10] = "CW-U";
	char fm[10] = "FM";
	char am[10] = "AM";
	char digu[10] = "DIG-U";
	char spec[10] = "SPEC";
	char digl[10] = "DIG-L";
	char sam[10] = "SAM";
	char drm[10] = "DRM";
	// Button references
	ModeButton* mode_0_btn;
	ModeButton* mode_1_btn;
	ModeButton* mode_2_btn;
	ModeButton* mode_3_btn;
	ModeButton* mode_4_btn;
	ModeButton* mode_5_btn;
	ModeButton* mode_6_btn;
	ModeButton* mode_7_btn;
	ModeButton* mode_8_btn;
	ModeButton* mode_9_btn;
	ModeButton* mode_10_btn;
	ModeButton* mode_11_btn;

	// Structures to drive generation and updating
	struct mode_button {
		ModeButton* mode;
		char *label;
		int id;
	};
	struct mode_buttons {
		int n;					// Number of buttons
		mode_button items[12];	// Button descriptors
	}m_b = {
		12,
		{
			{ mode_0_btn, lsb, 0 },
			{ mode_1_btn, usb, 1 },
			{ mode_2_btn, dsb, 2 },
			{ mode_3_btn, cwl, 3 },
			{ mode_4_btn, cwu, 4 },
			{ mode_5_btn, fm, 5 },
			{ mode_6_btn, am, 6 },
			{ mode_7_btn, digu, 7 },
			{ mode_8_btn, spec, 8 },
			{ mode_9_btn, digl, 9 },
			{ mode_10_btn, sam, 10 },
			{ mode_11_btn, drm, 11 }
		}
	};

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// A mode button
class ModeButton : public ToggleButtonBase
{
public:
	//==============================================================================
	ModeButton(Modes *modes, RadioInterface* radio_interface, char* label, int mode_id, metrics m, Fl_Color back_col, Fl_Color label_col);
	~ModeButton() {};

	//==============================================================================
	int handle(int event);

private:
	//==============================================================================
	// State variables
	Modes *t_l;
	RadioInterface* r_i;
	int id;

	//==============================================================================
	// Method prototypes

};
