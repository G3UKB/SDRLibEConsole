/*
audio.cpp

Audio selection window implementation for the FLTK Console

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
// Main Audio Component Window
//==============================================================================

//----------------------------------------------------
// Constructor/Destructor
Audio::Audio(int w, int h) : Fl_Window(w, h) {

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 5, w - 10, h - 10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w - 10, h - 10, 4, 5, 5);
	metrics m;
	// Add labels
	m = grid->get_cell_metrics(0, 0);
	Fl_Box* l_sink = new Fl_Box(m.x, m.y, m.w, m.h, "Sink");
	m = grid->get_cell_metrics(1, 0);
	Fl_Box* l_dev = new Fl_Box(m.x, m.y, m.w, m.h, "Dev");
	m = grid->get_cell_metrics(2, 0);
	Fl_Box* l_ch = new Fl_Box(m.x, m.y, m.w, m.h, "Ch");
	
	// Add choices
	m = grid->get_cell_metrics(0, 1, 1, 4);
	Fl_Choice* sink = new Fl_Choice(m.x, m.y, m.w, m.h);
	m = grid->get_cell_metrics(1, 1, 1, 4);
	Fl_Choice* device = new Fl_Choice(m.x, m.y, m.w, m.h);

	// Add channel selection
	m = grid->get_cell_metrics(2, 1, 1, 4);
	Fl_Group *ch_group = new Fl_Group(m.x, m.y, m.w, m.h);
	m = grid->get_cell_metrics(2, 1);
	Fl_Radio_Light_Button* left = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Left");
	m = grid->get_cell_metrics(2, 2);
	Fl_Radio_Light_Button* right = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Right");
	m = grid->get_cell_metrics(2, 3);
	Fl_Radio_Light_Button* both = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Both");
	m = grid->get_cell_metrics(2, 4);
	Fl_Radio_Light_Button* none = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "None");
	ch_group->end();

	// Add buttons
	m = grid->get_cell_metrics(3, 3);
	Fl_Button* apply = new Fl_Button(m.x, m.y, m.w, m.h, "Apply");
	m = grid->get_cell_metrics(3, 4);
	Fl_Button* cancel = new Fl_Button(m.x, m.y, m.w, m.h, "Cancel");

	// Close up and display
	top_group->end();
	end();
	border(false);
	show();
}