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

void startradio(Fl_Widget *, void *) {
	r_i->ri_radio_start(0);
}

void stopradio(Fl_Widget *, void *) {
	r_i->ri_radio_stop();
}

int main(int argc, char **argv) {

	// Create radio interface
	r_i = new RadioInterface();

	// Make wisdom file if not already made
#ifdef linux
	c_server_make_wisdom((char *)"E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom/");
#else
	c_server_make_wisdom((char *)"./wisdom/");
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
	// Create a main window
	Fl_Window* main_window = new Fl_Double_Window(331, 124);
	main_window->resizable(main_window);
	main_window->color((Fl_Color)24);
	main_window->align(Fl_Align(65));

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
	main_window->end();
	main_window->show(argc, argv);

	// Run event loop until quit
	return Fl::run();
}



