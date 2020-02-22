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

#include <iostream>
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Window.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Group.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Box.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Pack.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Button.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Light_Button.H"
#include "../Includes/extern.h"
#include "../Includes/radio_interface.h"
#include "../Includes/vfo.h"

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
	c_server_make_wisdom((char *)"E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom/");

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

	// Do GUI stuff
	Fl_Window *window = new Fl_Window(200, 400);
	window->resizable(window);
	Fl_Group *group = new Fl_Group(5, 5,190, 390);
	group->box(FL_EMBOSSED_FRAME);
	Fl_Pack *pack = new Fl_Pack(10, 10, 180, 380);
	Fl_Light_Button *b1 = new Fl_Light_Button(0, 0, 60, 20, "Start");
	b1->callback(startradio, 0);
	Fl_Light_Button *b2 = new Fl_Light_Button(0, 0, 60, 20, "Stop");
	b2->callback(stopradio, 0);
	VFOComponent *c = new VFOComponent("1", 0, 0,0, 170, 330);
	window->end();
	window->show(argc, argv);

	// Run until quit
	return Fl::run();
}



