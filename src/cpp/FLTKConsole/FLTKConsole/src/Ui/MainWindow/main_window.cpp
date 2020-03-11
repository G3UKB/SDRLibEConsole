/*
main_window.cpp

Window implementation for the FLTK Console

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
// Defines

//==============================================================================
/*
	The one and only main window
*/

//==============================================================================
MainWindow(RadioInterface* radio_interface, int w, int h) : Fl_Double_Window(w, h) {

	// Create a main window
	//Fl_Window* main_window = new Fl_Double_Window(331, 124);
	resizable(main_window);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 10, 320, 105);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Add start and stop buttons to the group
	Fl_Button* start_btn = new Fl_Button(15, 20, 100, 20, "Start");
	start_btn->color((Fl_Color)33);
	start_btn->labelcolor((Fl_Color)67);
	start_btn->callback(startradio, 0);
	Fl_Button* stop_button = new Fl_Button(120, 20, 100, 20, "Stop");
	stop_button->color((Fl_Color)33);
	stop_button->labelcolor((Fl_Color)80);
	stop_button->callback(stopradio, 0);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	VFOComponent *c = new VFOComponent(r_i, "radio-1", 0, 15, 50, 300, 55);

	// Close up and display
	top_group->end();
	end();
	show(argc, argv);

	// Show temp modes window
	Modes *m = new Modes(r_i, 300, 110);
}

//==============================================================================
