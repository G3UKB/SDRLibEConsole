/*
audio.h

Audio selection window header for the FLTK Console

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
enum class AudioType {
	INPUT,
	OUTPUT
};

class Audio : public Fl_Window
{
public:
	//==============================================================================
	Audio(int w, int h);
	~Audio() {};

	//==============================================================================
	// Method prototypes
	void handle_apply();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Our prefs
	Preferences* p;
	// Ref to set radio parameters
	RadioInterface* r_i;

	Fl_Choice* sink;
	Fl_Choice* device;
	Fl_Radio_Light_Button* left;
	Fl_Radio_Light_Button* right;
	Fl_Radio_Light_Button* both;
	Fl_Radio_Light_Button* none;
	Fl_Button* apply;
	Fl_Button* cancel;

	//==============================================================================
	// Method prototypes
	void set_path(int rx, char* sink, char* api, char* dev, char* ch);
	void save_route(int rx, char* sink, char* api, char* dev, char* ch);
	void set_widget_state(char* vsink, char* vapi, char* vdev, char* vch);
};
