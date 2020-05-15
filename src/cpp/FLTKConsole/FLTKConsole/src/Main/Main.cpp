/*
main.cpp

Main module for the FLTK Console

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

#include "../Includes/includes.h"

RadioInterface* r_i;
Preferences* prefs;
std::thread* cat;

//==============================================================================
// FLTKConsole entry point
int main(int argc, char **argv) {

	// Test hamlib
	RIG *my_rig;


	// Create preferences
	prefs = new Preferences();
	RSt::inst().put_obj("PREFS", (void*)prefs);

	// Create radio interface
	r_i = new RadioInterface();
	RSt::inst().put_obj("RADIO-IF", (void*)r_i);

	// Make wisdom file if not already made
	r_i->ri_make_wisdom();

	// Perform a cold start
	// State is set in running state for system interrogation
	r_i->cold_start();

	// Create the main window
	Fl_Window* main_window = new MainWindow(prefs->get_window_x(), prefs->get_window_y(), prefs->get_window_w(), prefs->get_window_h());
	RSt::inst().put_obj("MAIN_W", (void*)main_window);

	// Start the CAT thread
	cat = new std::thread(CATStart, prefs, "COM5");
	CATThrd* t = (CATThrd*)RSt::inst().get_obj("CAT");
	
	// Run UI event loop until quit
	Fl::run();

	// Wait for threads to exit
	t->terminate();
	cat->join();
	Sleep(1000);
	return 0;
}



