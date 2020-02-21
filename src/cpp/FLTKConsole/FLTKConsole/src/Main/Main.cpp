#include <iostream>
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Window.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Group.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Box.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Pack.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Button.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Light_Button.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Round_Clock.H"
#include "../Includes/extern.h"
#include "../Includes/radio_interface.h"


int main(int argc, char **argv) {

	// Create radio interface
	RadioInterface* r_i = new RadioInterface();

	// Make wisdom file if not already made
	c_server_make_wisdom((char *)"E:/Projects/SDRLibEConsole/trunk/src/cpp/wisdom/");

	// Initialise server
	if (!c_server_init()) {
		printf("Failed to initialise server!");
		return -1;
	}
	c_server_set_num_rx(1);
	if (!r_i->ri_set_default_audio()) {
		// Something is kinda wrong here, can't continue
		std::cout << std::endl << "Failed to configure audio, unable to continue!" << std::endl;
		return -1;
	}
	if (!r_i->ri_radio_discover()) {
		// Radio hardware not running or comms problem
		std::cout << "Failed to discover radio, please check hardware." << std::endl;
		// Start the main UI only
		//MainComponent* c = mainWindow->get_component();
		//mainWindow->get_component()->start_ui();
	}
	else {
		if (!r_i->ri_server_start()) {
			// Something also kinda wrong here, cant continue
			std::cout << std::endl << "Failed to start server processes, unable to continue!" << std::endl;
			return -1;
		}
		else {
			// Temp start radio
			r_i->ri_radio_start(0);
			// Do GUI stuff
			Fl_Window *window = new Fl_Window(340, 300);
			window->resizable(window);
			Fl_Group *group = new Fl_Group(25, 25, 200, 100);
			group->box(FL_EMBOSSED_FRAME);
			Fl_Pack *pack = new Fl_Pack(28, 28, 194, 94);
			//pack->box(FL_EMBOSSED_FRAME);
			//Fl_Group *group = new Fl_Group(25, 25, 200, 100);
			//group->box(FL_UP_FRAME);
			//group->begin();
			Fl_Light_Button *b1 = new Fl_Light_Button(10, 10, 60, 20, "Start");
			Fl_Light_Button *b2 = new Fl_Light_Button(10, 10, 60, 20, "Stop");
			//group->end();
			window->end();
			window->show(argc, argv);
		}
	}

	// Run until quit
	return Fl::run();
}




