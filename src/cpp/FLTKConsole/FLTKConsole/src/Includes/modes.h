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

/*
	A modes window component
*/
class ModeButton;
class Modes : public Fl_Window
{
public:
	//==============================================================================
	Modes(RadioInterface* radio_interface, int w, int h);
	~Modes() {};

	//==============================================================================
	//int handle(int event) override;

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
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
	ModeButton* mode_0_btn;

	//==============================================================================
	// Method prototypes

};

class ModeButton : public Fl_Button
{
public:
	//==============================================================================
	ModeButton(RadioInterface* radio_interface, char* label, int mode_id, metrics m);
	~ModeButton() {};

	//==============================================================================
	int handle(int event) override;
	void draw() override;

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int row;
	int col;
	int id;

	//==============================================================================
	// Method prototypes

};
