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
// Main Window

// Idle time callback
// We call back to the window to do housekeeping
void main_window_idle_cb(void* data) {
	MainWindow* w = (MainWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, main_window_idle_cb, data);
}

// Radio select callback
void radio_cb(Fl_Widget* w, void* user_data) {
	// Call down to our Audio instance
	((MainWindow*)user_data)->handle_radio(w);
}

// Type select callback
void type_cb(Fl_Widget* w, void* user_data) {
	// Call down to our Audio instance
	((MainWindow*)user_data)->handle_type(w);
}

//----------------------------------------------------
// Constructor
MainWindow::MainWindow(int x, int y, int w, int h) : Fl_Double_Window(w, h) {

	// Stash paramaters
	width = w;
	height = h;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Set window attributes
	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));
	char label[20];
	sprintf_s(label, "Control - RX-1");
	copy_label(label);

	// Set window position
	position(x, y);

	// Get num radios
	num_radios = p->get_num_radios();
	radio_id = 1;
	radio2_id = 2;
	radio3_id = 3;
	tx_id = 4;

	// Populate
	do_layout();

	// Create the audio panel hidden
	audio_out = new AudioOutput(radio_id, 350, 130);
	audio_out->hide();

	// Create the modes panel hidden
	modes = new Modes(radio_id, 230, 80);
	modes->hide();

	// Create the filters panel hidden
	filters = new Filters(radio_id, 230, 80);
	filters->hide();

	// Deactivate buttons
	// Get re-activated as required in idle processing
	CtrlBtn->deactivate();
	DiscoverBtn->deactivate();
	SelectRadio->deactivate();
	ModeBtn->deactivate();
	FilterBtn->deactivate();

	// Display main window
	show();
	
	// Show radio windows as appropriate
	if (num_radios > 1) {
		Radio2_Win = new RxWindow(radio2_id, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
	}
	if (num_radios > 2) {
		Radio3_Win = new RxWindow(radio3_id, p->get_radio3_x(), p->get_radio3_y(), p->get_radio3_w(), p->get_radio3_h());
	}
	
	// Create and hide TX window
	TX_Win = new TxWindow(tx_id, p->get_tx_x(), p->get_tx_y(), p->get_tx_w(), p->get_tx_h());
	TX_Win->hide();
	
	// Start the idle timeout
	Fl::add_timeout(0.2, main_window_idle_cb, (void*)this);
}

//----------------------------------------------------
// Populate window
void MainWindow::do_layout() {

	// Layout params
	int rows = 5;
	int cols = 4;
	int grid_margin = 5;
	int group_margin = 10;
	int x = 5, y = 5;

	// Add a group box
	top_group = new Fl_Group(x, y, width - group_margin, height - group_margin);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	grid = new GridLayout(x, y, width - group_margin, height - group_margin, rows, cols, grid_margin);

	// Add Exit button
	std::function< int(int, int) > f = std::bind(&MainWindow::exit_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("EXIT_CB", f);
	m = grid->get_cell_metrics(0, 3);
	ExitBtn = new C_Button(std::string("EXIT_CB"), exit_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
	top_group->add(ExitBtn);

	// Add discover button
	std::function< int(int, int) > f1 = std::bind(&MainWindow::discover_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("DISCOVER_CB", f1);
	m = grid->get_cell_metrics(0, 1);
	DiscoverBtn = new C_Button(std::string("DISCOVER_CB"), discover_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
	top_group->add(DiscoverBtn);

	// Add radio choice
	m = grid->get_cell_metrics(0, 2);
	SelectRadio = new Fl_Choice(m.x + 20, m.y, m.w - 20, m.h, "RX:");
	SelectRadio->add("1");
	SelectRadio->add("2");
	SelectRadio->add("3");
	SelectRadio->color((Fl_Color)33);
	SelectRadio->labelcolor((Fl_Color)80);
	SelectRadio->callback((Fl_Callback*)radio_cb, (void*)this);
	top_group->add(SelectRadio);
	SelectRadio->value(num_radios - 1);

	// Add radio type
	m = grid->get_cell_metrics(1, 0, 1, 2);
	RadioType = new Fl_Choice(m.x + 40, m.y, m.w - 40, m.h, "TYPE:");
	RadioType->add("HPSDR");
	RadioType->add("FCDPro+");
	RadioType->add("FT817ND");
	RadioType->add("IC7100");
	RadioType->color((Fl_Color)33);
	RadioType->labelcolor((Fl_Color)80);
	RadioType->callback((Fl_Callback*)type_cb, (void*)this);
	top_group->add(RadioType);
	RadioType->value(0);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	m = grid->get_cell_metrics(2, 0, 2, 3);
	VFOComponent *vfo = new VFOComponent(radio_id, 0, m.x, m.y, m.w, m.h);
	top_group->add(vfo);

	// Add start/stop button
	std::function< int(int, int) > f2 = std::bind(&MainWindow::ctrl_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("CTRL_CB", f2);
	m = grid->get_cell_metrics(0, 0);
	CtrlBtn = new C_ToggleButton(std::string("CTRL_CB"), ctrl_str_up, ctrl_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(CtrlBtn);

	// Add TX button
	std::function< int(int, int) > f3 = std::bind(&MainWindow::tx_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("TX_CB", f3);
	m = grid->get_cell_metrics(1, 3);
	TXBtn = new C_ToggleButton(std::string("TX_CB"), tx_str_up, tx_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(TXBtn);

	// Add CAT button
	std::function< int(int, int) > f4 = std::bind(&MainWindow::cat_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("CAT_CB", f4);
	m = grid->get_cell_metrics(1, 2);
	CATBtn = new C_ToggleButton(std::string("CAT_CB"), cat_str_up, cat_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(CATBtn);

	// Add audio button
	std::function< int(int, int) > f5 = std::bind(&MainWindow::audio_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("AUDIO_R1_CB", f5);
	m = grid->get_cell_metrics(2, 3);
	AudioBtn = new C_ToggleButton(std::string("AUDIO_R1_CB"), audio_str_up, audio_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(AudioBtn);

	// We place the radio buttons into another grid
	// Get metrics from grid
	m = grid->get_cell_metrics(3, 3, 2, 1);
	// Create grid_1 with the new metrics
	GridLayout *grid_1 = new GridLayout(m.x, m.y, m.w, m.h, 2, 1, 2);

	// Add mode trigger in grid_1
	std::function< int(int, int) > f6 = std::bind(&MainWindow::mode_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("MODE_R1_CB", f6);
	m = grid_1->get_cell_metrics(0, 0);
	ModeBtn = new C_ToggleButton(std::string("MODE_R1_CB"), mode_str_up, mode_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Add filter trigger in grid_1
	std::function< int(int, int) > f7 = std::bind(&MainWindow::filt_handle_event, this, std::placeholders::_1, std::placeholders::_1);
	RSt::inst().put_cb("FILT_R1_CB", f7);
	m = grid_1->get_cell_metrics(1, 0);
	FilterBtn = new C_ToggleButton(std::string("FILT_R1_CB"), filt_str_up, filt_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Close up and display
	top_group->end();
	// Finish up
	end();
}

//===================================================
// Event handlers

//----------------------------------------------------
// Window event handlers
//----------------------------------------------------
// Resize event
void  MainWindow::resize(int x, int y, int w, int h) {
	// Tell window to resize all widgets
	Fl_Double_Window::resize(x, y, w, h);
	// Save position and size
	p->set_window_x(x);
	p->set_window_y(y);
	p->set_window_w(w);
	p->set_window_h(h);
}

//----------------------------------------------------
// General event handler
int MainWindow::handle(int event) {
	// Use this to print readable form of event
	//printf("Event was %s (%d)\n", fl_eventnames[event], event);
	switch (event) {
	// Window close raises an FL_HIDE event?
	case FL_HIDE: {
		// Save prefs
		p->save();
		// Close tool windows
		modes->close();
		filters->close();
		// Close radio windows if active
		if (Radio2_Win != NULL) Radio2_Win->close();
		if (Radio3_Win != NULL) Radio3_Win->close();
		if (TX_Win != NULL) TX_Win->close();
		break;
		}
	}
	// Pass all events down
	return Fl_Window::handle(event);
}

//----------------------------------------------------
// Handle radio type
void MainWindow::handle_type(Fl_Widget* w) {
	Fl_Choice* c = (Fl_Choice*)w;
	int value = c->value();
	switch (value) {
		case (int)RadioType::HP_SDR : {
			// HPSDR
			if (hamlib != NULL) {
				hamlib->close();
				delete (hamlib);
			}
			break;
		}
		case (int)RadioType::FCDPro: {
			// FCDPro+
			break;
		}
		case (int)RadioType::FT817: {
			// FT817ND
			if (Radio2_Win != NULL) Radio2_Win->close();
			if (Radio3_Win != NULL) Radio3_Win->close();
			if (TX_Win != NULL) TX_Win->close();
			if (hamlib != NULL) {
				hamlib->close();
				delete (hamlib);
			}
			hamlib = new HamlibClient((char*)"COM2", (rig_model_t)HamlibID::FT817);
			RSt::inst().put_obj("HAMLIB", (void*)hamlib);
			hamlib->init();
			hamlib->open();
			break;
		}
		case (int)RadioType::IC7100: {
			// IC7100
			if (Radio2_Win != NULL) Radio2_Win->close();
			if (Radio3_Win != NULL) Radio3_Win->close();
			if (TX_Win != NULL) TX_Win->close();
			if (hamlib != NULL) {
				hamlib->close();
				delete (hamlib);
			}
			hamlib = new HamlibClient((char*)"COM2", (rig_model_t)HamlibID::IC7100);
			RSt::inst().put_obj("HAMLIB", (void*)hamlib);
			hamlib->init();
			hamlib->open();
			break;
		}
		default:
			printf("Unknown radio type %d\n", value);
	}
	RSt::inst().set_type(value);
}

//----------------------------------------------------
// Handle number of radio instances
void MainWindow::handle_radio(Fl_Widget* w) {
	Fl_Choice* c = (Fl_Choice*)w;
	int value = c->value();
	bool success = false;
	if (value + 1 != p->get_num_radios()) {
		// Number of radios has changed
		// Update number of radios
		p->set_num_radios(value + 1);
		// Bounce the server
		success = r_i->restart();
		// Reset the radio, state and start/stop button if radio is running
		if (RSt::inst().get_radio_running()) {
			r_i->ri_radio_stop();
			RSt::inst().set_radio_running(false);
			CtrlBtn->clear();
			CtrlBtn->label(ctrl_str_up);
			CtrlBtn->labelcolor((Fl_Color)80);
		}
		// Rejig the UI
		if (value == 0) {
			// One radio
			// Close radios 2/3 if active
			if (Radio2_Win != NULL) {
				Radio2_Win->close();
				Radio2_Win = NULL;
			}
			if (Radio3_Win != NULL) {
				Radio3_Win->close();
				Radio3_Win = NULL;
			}
		}
		else if (value == 1) {
			;
			// Two radios
			if (Radio2_Win == NULL) Radio2_Win = new RxWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
			// Close radio 3 if active
			if (Radio3_Win != NULL) {
				Radio3_Win->close();
				Radio3_Win = NULL;
			}
		}
		else if (value == 2) {
			// Three radios
			if (Radio2_Win == NULL) Radio2_Win = new RxWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
			if (Radio3_Win == NULL) Radio3_Win = new RxWindow(3, p->get_radio3_x(), p->get_radio3_y(), p->get_radio3_w(), p->get_radio3_h());
		}
		// Reset all radio state
		if (success) r_i->reset_radio_state();
	}
}

//----------------------------------------------------
// Callbacks from widget event handlers

//----------------------------------------------------
// Exit handler
int MainWindow::exit_handle_event(int state, int id) {
	hide();
	return true;
}

//----------------------------------------------------
// Discover handler
int MainWindow::discover_handle_event(int state, int id) {
	// Discover event
	if (r_i->ri_radio_discover()) {
		RSt::inst().set_discovered(true);
		if (r_i->ri_server_start())
			RSt::inst().set_server_running(true);
		else
			std::cout << std::endl << "Failed to start server!" << std::endl;
	}
	else
		std::cout << std::endl << "Failed to discover radio!" << std::endl;

	return true;
}

//----------------------------------------------------
// Ctrl handler
int MainWindow::ctrl_handle_event(int state, int id) {

	int radio_type = RSt::inst().get_type();
	if (radio_type == (int)RadioType::FT817 || radio_type == (int)RadioType::IC7100) {
		// Analog
		bool running = RSt::inst().get_analog_radio_running();
		if (running) {
			RSt::inst().set_analog_radio_running(false);
		}
		else {
			RSt::inst().set_analog_radio_running(true);
		}
	}
	else {
		// SDR
		bool running = RSt::inst().get_radio_running();
		if (running) {
			// Stop event
			r_i->ri_radio_stop();
			RSt::inst().set_radio_running(false);
		}
		else {
			// Start event
			r_i->ri_radio_start(0);
			RSt::inst().set_radio_running(true);
		}
	}
	return true;
}

//----------------------------------------------------
// CAT handler
int MainWindow::cat_handle_event(int state, int id) {
	// Retrieve CAT object
	CATThrd* t = (CATThrd*)RSt::inst().get_obj("CAT");
	if (state) {
		// Enable the CAT thread
		t->enable(true);
	}
	else {
		// Disable the CAT thread
		t->enable(false);
	}
	return true;
}

//----------------------------------------------------
// TX handler
int MainWindow::tx_handle_event(int state, int id) {
	if (state)
		TX_Win->show();
	else
		TX_Win->hide();
	return true;
}

//----------------------------------------------------
// Audio handler
int MainWindow::audio_handle_event(int state, int id) {
	set_location();
	if (state) {
		audio_out->position(w_loc.x + w_loc.w + 5, w_loc.y);
		audio_out->show();
	}
	else {
		audio_out->hide();
	}
	return true;
}

//----------------------------------------------------
// Mode handler
int MainWindow::mode_handle_event(int state, int id) {
	set_location();
	if (state) {
		modes->position(w_loc.x + w_loc.w + 5, w_loc.y);
		modes->show();
	}
	else {
		modes->hide();
	}
	return true;
}

//----------------------------------------------------
// Filter handler
int MainWindow::filt_handle_event(int state, int id) {
	set_location();
	if (state) {
		filters->position(w_loc.x + w_loc.w + 5, w_loc.y);
		filters->show();
	}
	else {
		filters->hide();
	}
	return true;
}

//===================================================
// Idle timeout callback
void MainWindow::handle_idle_timeout() {
	// Handle enable/disable of controls here
	int radio_type = RSt::inst().get_type();
	char label[20];
	if ((radio_type == (int)RadioType::HP_SDR) || (radio_type == (int)RadioType::FCDPro)) {
		// SDR type radio
		static int last_discovered = -1;
		bool discovered = RSt::inst().get_discovered();
		if (discovered != last_discovered) {
			if (discovered) {
				CtrlBtn->activate();
				SelectRadio->activate();
				DiscoverBtn->deactivate();
				ModeBtn->activate();
				FilterBtn->activate();
			}
			else {
				CtrlBtn->deactivate();
				SelectRadio->deactivate();
				DiscoverBtn->activate();
				ModeBtn->deactivate();
				FilterBtn->deactivate();
			}
		}
		last_discovered = discovered;
		sprintf_s(label, "Control - RX-1");
		copy_label(label);
	}
	else {
		// Analog type radio
		CtrlBtn->activate();
		SelectRadio->deactivate();
		DiscoverBtn->deactivate();
		CATBtn->deactivate();
		AudioBtn->deactivate();
		ModeBtn->activate();
		FilterBtn->deactivate();
		if (radio_type == (int)RadioType::FT817)
			sprintf_s(label, "Control - FT817");
		else if (radio_type == (int)RadioType::IC7100)
			sprintf_s(label, "Control - IC7100");
		copy_label(label);
	}
}

//==============================================================================
// Helpers
void MainWindow::set_location() {
	w_loc.x = p->get_window_x();
	w_loc.y = p->get_window_y();
	w_loc.w = p->get_window_w();
	w_loc.h = p->get_window_h();
}
