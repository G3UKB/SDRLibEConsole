/*
radio_window.h

Radio 2/3 window header for the FLTK Console

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
	The one and only main window
*/
class RadioWindow : public WindowBase
{
public:
	//==============================================================================
	RadioWindow(int radio, int x, int y, int w, int h);
	~RadioWindow() {};
	void handle_idle_timeout();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// labels must be fixed storage
	// Radio inst
	int r;

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components

	//==============================================================================
	// Method prototypes

};
