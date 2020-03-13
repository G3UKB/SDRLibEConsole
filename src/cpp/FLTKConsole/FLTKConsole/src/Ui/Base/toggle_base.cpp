/*
toggle_base.cpp

Toggle button base component implementation for the FLTK Console

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

#include "../../Includes/includes.h"

//==============================================================================
// Toggle button base
ToggleButtonBase::ToggleButtonBase(RadioInterface* radio_interface, char* button_label, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Toggle_Button(x, y, w, h, button_label) {
	r_i = radio_interface;
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

