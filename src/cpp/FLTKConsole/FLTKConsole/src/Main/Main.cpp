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

//==============================================================================
// FLTKConsole entry point
int main(int argc, char **argv) {

	// Create preferences
	prefs = new Preferences();

	// Create radio interface
	r_i = new RadioInterface();

	// Make wisdom file if not already made
#ifdef linux
	c_server_make_wisdom((char *)"./wisdom/");
#else
	c_server_make_wisdom((char *)"E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom/");
#endif
	// Initialise and run server
	if (c_server_init()) {
		c_server_set_num_rx(1);
		if (r_i->ri_set_default_audio()) {
			if (r_i->ri_radio_discover()) {
				if (!r_i->ri_server_start())
					std::cout << std::endl << "Failed to start server!" << std::endl;
			}
			else
				std::cout << std::endl << "Failed to discover radio!" << std::endl;
		}
		else
			std::cout << std::endl << "Failed to configure audio!" << std::endl;
	}
	else
		std::cout << std::endl << "Failed to initialise server!" << std::endl;

	// Create the main window
	Fl_Window* main_window = new MainWindow(prefs, r_i);

	// Run event loop until quit
	return Fl::run();

}



