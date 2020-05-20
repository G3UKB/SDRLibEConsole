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
		Radio2_Win = new RadioWindow(radio2_id, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
	}
	if (num_radios > 2) {
		Radio3_Win = new RadioWindow(radio3_id, p->get_radio3_x(), p->get_radio3_y(), p->get_radio3_w(), p->get_radio3_h());
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

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	m = grid->get_cell_metrics(2, 0, 2, 3);
	VFOComponent *c = new VFOComponent(radio_id, 0, m.x, m.y, m.w, m.h);

	// Add discover button
	m = grid->get_cell_metrics(0, 1);
	DiscoverBtn = new DiscoverButton(this, discover_str, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
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

	// Add start/stop buttons to the group
	std::function< int(int) > f = std::bind(&MainWindow::ctrl_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("CTRL_CB", f);
	m = grid->get_cell_metrics(0, 0);
	CtrlBtn = new C_ToggleButton(std::string("CTRL_CB"), ctrl_str_up, ctrl_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(CtrlBtn);

	// Add TX button to the group
	std::function< int(int) > f1 = std::bind(&MainWindow::tx_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("TX_CB", f1);
	m = grid->get_cell_metrics(1, 3);
	TXBtn = new C_ToggleButton(std::string("TX_CB"), tx_str_up, tx_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(TXBtn);

	// Add CAT button to the group
	std::function< int(int) > f2 = std::bind(&MainWindow::cat_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("CAT_CB", f2);
	m = grid->get_cell_metrics(1, 2);
	CATBtn = new C_ToggleButton(std::string("CAT_CB"), cat_str_up, cat_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(CATBtn);

	// Add audio button
	std::function< int(int) > f3 = std::bind(&MainWindow::audio_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("AUDIO_CB", f3);
	m = grid->get_cell_metrics(2, 3);
	AudioBtn = new C_ToggleButton(std::string("AUDIO_CB"), audio_str_up, audio_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(AudioBtn);

	// Add Exit button to the group
	m = grid->get_cell_metrics(0, 3);
	ExitBtn = new ExitButton(this, exit_str, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
	top_group->add(ExitBtn);

	// We place the radio buttons into another grid
	// Get metrics from grid
	m = grid->get_cell_metrics(3, 3, 2, 1);
	// Create grid_1 with the new metrics
	GridLayout *grid_1 = new GridLayout(m.x, m.y, m.w, m.h, 2, 1, 2);

	// Add mode trigger in grid_1
	std::function< int(int) > f4 = std::bind(&MainWindow::mode_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("MODE_CB", f4);
	m = grid_1->get_cell_metrics(0, 0);
	ModeBtn = new C_ToggleButton(std::string("AUDIO_CB"), mode_str_up, mode_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Add filter trigger in grid_1
	std::function< int(int) > f5 = std::bind(&MainWindow::filt_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("FILT_CB", f5);
	m = grid_1->get_cell_metrics(1, 0);
	FilterBtn = new C_ToggleButton(std::string("FILT_CB"), filt_str_up, filt_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Close up and display
	top_group->end();
	// Finish up
	end();
}

//===================================================
// Event handlers

//----------------------------------------------------
// Window event handler
int MainWindow::handle(int event) {
	// Use this to print readable form of event
	//printf("Event was %s (%d)\n", fl_eventnames[event], event);
	switch (event) {
		// Window close raises an FL_HIDE event?
	case FL_HIDE: {
		// Save prefs
		p->save();
		// Close radio windows if active
		if (Radio2_Win != NULL) Radio2_Win->close();
		if (Radio3_Win != NULL) Radio3_Win->close();
		break;
	}
	}
	// Pass all events down
	return Fl_Window::handle(event);
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
			if (Radio2_Win == NULL) Radio2_Win = new RadioWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
			// Close radio 3 if active
			if (Radio3_Win != NULL) {
				Radio3_Win->close();
				Radio3_Win = NULL;
			}
		}
		else if (value == 2) {
			// Three radios
			if (Radio2_Win == NULL) Radio2_Win = new RadioWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
			if (Radio3_Win == NULL) Radio3_Win = new RadioWindow(3, p->get_radio3_x(), p->get_radio3_y(), p->get_radio3_w(), p->get_radio3_h());
		}
		// Reset all radio state
		if (success) r_i->reset_radio_state();
	}
}

//----------------------------------------------------
// Callbacks from widget event handlers

//----------------------------------------------------
// Ctrl handler
int MainWindow::ctrl_handle_event(int state) {
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
	return true;
}

//----------------------------------------------------
// CAT handler
int MainWindow::cat_handle_event(int state) {
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
int MainWindow::tx_handle_event(int state) {
	if (state)
		TX_Win->show();
	else
		TX_Win->hide();
	return true;
}

//----------------------------------------------------
// Audio handler
int MainWindow::audio_handle_event(int state) {
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
int MainWindow::mode_handle_event(int state) {
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
int MainWindow::filt_handle_event(int state) {
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
}

//==============================================================================
// Helpers
void MainWindow::set_location() {
	w_loc.x = p->get_window_x();
	w_loc.y = p->get_window_y();
	w_loc.w = p->get_window_w();
	w_loc.h = p->get_window_h();
}

//==============================================================================
// Discover button
DiscoverButton::DiscoverButton(MainWindow* parent_widget, char* button_label, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Button(x, y, w, h, button_label) {
	myparent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int DiscoverButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
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
		return 1;	
		}
	default:
		return Fl_Widget::handle(event);
	}
}

//==============================================================================
// Exit button
ExitButton::ExitButton(MainWindow* parent_widget, char* button_label, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Button(x, y, w, h, button_label) {
	myparent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int ExitButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		myparent->hide();
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}
