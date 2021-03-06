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
	Modes(int radio, int w, int h);
	~Modes() {};
	void close();

	//==============================================================================
	int mode_handle_event(int state, int id);

private:
	//==============================================================================
	// State variables
	// radio instance
	int radio_id;
	// Our prefs
	Preferences* p;
	// Ref to set radio parameters
	RadioInterface* r_i;
	// Fixed labels (labels are not copied and must remain in scope
	// Note: dont use std:string as std:string.cstr() creates a new
	// cstr in a fixed overwritten area and all labels will be the last label.
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
	C_ToggleButton* mode_0_btn;
	C_ToggleButton* mode_1_btn;
	C_ToggleButton* mode_2_btn;
	C_ToggleButton* mode_3_btn;
	C_ToggleButton* mode_4_btn;
	C_ToggleButton* mode_5_btn;
	C_ToggleButton* mode_6_btn;
	C_ToggleButton* mode_7_btn;
	C_ToggleButton* mode_8_btn;
	C_ToggleButton* mode_9_btn;
	C_ToggleButton* mode_10_btn;
	C_ToggleButton* mode_11_btn;

	// Structures to drive generation and updating
	struct mode_button {
		C_ToggleButton* mode;
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

	// Key to callback cache
	char key[20];

	//==============================================================================
	// Method prototypes
};
