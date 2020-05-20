/*
tx_window.cpp

Transmit window for the FLTK Console

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
void tx_window_idle_cb(void* data) {
	TxWindow* w = (TxWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, tx_window_idle_cb, data);
}

//----------------------------------------------------
// Constructor
TxWindow::TxWindow(int radio, int x, int y, int w, int h) : Fl_Double_Window(w, h) {

	// Stash paramaters
	width = w;
	height = h;
	radio_id = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Register ourselves
	RSt::inst().put_obj("TX-WINDOW", (void*)this);

	// Set window attributes
	color((Fl_Color)24);
	align(Fl_Align(65));
	char label[20];
	sprintf_s(label, "Transmitter");
	copy_label(label);

	// Set window position
	position(x, y);

	// Populate
	do_layout();

	// Create the audio panel hidden
	audio_in = new AudioInput(radio_id, 350, 130);
	audio_in->hide();

	// Create the modes panel hidden
	modes = new Modes(radio_id, 230, 80);
	modes->hide();

	// Create the filters panel hidden
	filters = new Filters(radio_id, 230, 80);
	filters->hide();

	// Deactivate buttons
	// Get re-activated as required in idle processing
	ModeBtn->deactivate();
	FilterBtn->deactivate();

	// Display main window
	show();
	
	// Start the idle timeout
	Fl::add_timeout(0.2, tx_window_idle_cb, (void*)this);
}

//----------------------------------------------------
// Populate window
void TxWindow::do_layout() {

	// Layout params
	int rows = 4;
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

	// Add Duplex button to the group
	std::function< int(int) > f1 = std::bind(&TxWindow::duplex_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("DUPLEX_CB", f1);
	m = grid->get_cell_metrics(0, 2);
	DuplexBtn = new C_ToggleButton(std::string("DUPLEX_CB"), duplex_str_up, duplex_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(DuplexBtn);

	// Add MOX button to the group
	std::function< int(int) > f2 = std::bind(&TxWindow::mox_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("MOX_CB", f2);
	m = grid->get_cell_metrics(0, 3);
	MOXBtn = new C_ToggleButton(std::string("MOX_CB"), mox_str_up, mox_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(MOXBtn);

	// Add the VFO component
	m = grid->get_cell_metrics(1, 0, 2, 3);
	VFOComponent *vfo = new VFOComponent(radio_id, 0, m.x, m.y, m.w, m.h);
	top_group->add(vfo);

	// Add RF drive to the group
	std::function< int(int) > f3 = std::bind(&TxWindow::rf_gain_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("GAIN_CB", f3);
	m = grid->get_cell_metrics(3, 0, 1, 3);
	RFGain = new RFSlider(std::string("GAIN_CB"), rf_gain_str, m.x, m.y, m.w, (2 * m.h) / 3, (Fl_Color)10);
	top_group->add(RFGain);

	// Add audio button
	std::function< int(int) > f4 = std::bind(&TxWindow::audio_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("AUDIO_TX_CB", f4);
	m = grid->get_cell_metrics(1, 3);
	AudioBtn = new C_ToggleButton(std::string("AUDIO_TX_CB"), audio_str_up, audio_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	top_group->add(AudioBtn);

	// We place the radio buttons into another grid
	// Get metrics from grid
	m = grid->get_cell_metrics(2, 3, 2, 1);
	// Create grid_1 with the new metrics
	GridLayout *grid_1 = new GridLayout(m.x, m.y, m.w, m.h, 2, 1, 2);

	// Add mode trigger in grid_1
	std::function< int(int) > f5 = std::bind(&TxWindow::mode_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("MODE_TX_CB", f5);
	m = grid_1->get_cell_metrics(0, 0);
	ModeBtn = new C_ToggleButton(std::string("MODE_TX_CB"), mode_str_up, mode_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Add filter trigger in grid_1
	std::function< int(int) > f6 = std::bind(&TxWindow::filt_handle_event, this, std::placeholders::_1);
	RSt::inst().put_cb("FILT_TX_CB", f6);
	m = grid_1->get_cell_metrics(1, 0);
	FilterBtn = new C_ToggleButton(std::string("FILT_TX_CB"), filt_str_up, filt_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);

	// Close up and display
	top_group->end();
	// Finish up
	end();
}

//----------------------------------------------------
// Close window
void TxWindow::close() {
	modes->close();
	filters->close();
	Fl::remove_timeout(tx_window_idle_cb);
	Fl::delete_widget(this);
}

//===================================================
// Event handlers

//----------------------------------------------------
// Window event handlers
//----------------------------------------------------
// Resize event
void  TxWindow::resize(int x, int y, int w, int h) {
	// Tell window to resize all widgets
	Fl_Double_Window::resize(x, y, w, h);
	// Save position and size
	p->set_tx_x(x);
	p->set_tx_y(y);
	p->set_tx_w(w);
	p->set_tx_h(h);
}

//----------------------------------------------------
// General event handler
int TxWindow::handle(int event) {
	// Use this to print readable form of event
	//printf("Event was %s (%d)\n", fl_eventnames[event], event);
	switch (event) {
		// Window close raises an FL_HIDE event?
	case FL_HIDE: {
		// Save prefs
		p->save();
		break;
		}
	}
	// Pass all events down
	return Fl_Window::handle(event);
}

//----------------------------------------------------
// Callbacks from widget event handlers

//----------------------------------------------------
// Duplex handler
int TxWindow::duplex_handle_event(int state) {
	if (RSt::inst().get_duplex()) {
		// Was duplex
		RSt::inst().set_duplex(false);
		r_i->ri_server_cc_out_set_duplex(0);
	}
	else {
		// Was simplex
		RSt::inst().set_duplex(true);
		r_i->ri_server_cc_out_set_duplex(1);
	}
	return true;
}

//----------------------------------------------------
// MOX handler
int TxWindow::mox_handle_event(int state) {
	if (RSt::inst().get_mox()) {
		// Was TX
		RSt::inst().set_mox(false);
		r_i->ri_server_cc_out_set_mox(0);
	}
	else {
		// Was RX
		RSt::inst().set_mox(true);
		r_i->ri_server_cc_out_set_mox(1);
	}
	return true;
}

void TxWindow::mox_on() {
	RSt::inst().set_mox(true);
	r_i->ri_server_cc_out_set_mox(1);
	MOXBtn->button_down();
}

void TxWindow::mox_off() {
	RSt::inst().set_mox(false);
	r_i->ri_server_cc_out_set_mox(0);
	MOXBtn->button_up();
}

//----------------------------------------------------
// RF gain handler
int TxWindow::rf_gain_handle_event(int value) {
	c_server_set_rf_drive(value);
	return true;
}

//----------------------------------------------------
// Audio handler
int TxWindow::audio_handle_event(int state) {
	set_location();
	if (state) {
		audio_in->position(w_loc.x + w_loc.w + 5, w_loc.y);
		audio_in->show();
	}
	else {
		audio_in->hide();
	}
	return true;
}

//----------------------------------------------------
// Mode handler
int TxWindow::mode_handle_event(int state) {
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
int TxWindow::filt_handle_event(int state) {
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
void TxWindow::handle_idle_timeout() {
	// Handle enable/disable of controls here
	static int last_discovered = -1;
	bool discovered = RSt::inst().get_discovered();
	if (discovered != last_discovered) {
		if (discovered) {
			ModeBtn->activate();
			FilterBtn->activate();
		}
		else {
			ModeBtn->deactivate();
			FilterBtn->deactivate();
		}
	}
	last_discovered = discovered;
}

//==============================================================================
// Helpers
void TxWindow::set_location() {
	w_loc.x = p->get_tx_x();
	w_loc.y = p->get_tx_y();
	w_loc.w = p->get_tx_w();
	w_loc.h = p->get_tx_h();
}

//==============================================================================
// RF Gain
RFSlider::RFSlider(std::string cb_key, char* label, int x, int y, int w, int h, Fl_Color back_col) : Fl_Value_Slider(x, y, w, h, label) {
	
	// Retrieve our callback function from the callback cache
	key = cb_key;
	cb = RSt::inst().get_cb(key);
	// Radio interface
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	// Set attributes
	type(5);
	labelsize(10);
	color((Fl_Color)back_col);
	selection_color((Fl_Color)1);
}

//----------------------------------------------------
// Handle events
void RFSlider::handle_event() {
	// Set R drive value 0.0-1.0
	c_server_set_rf_drive(value());
	cb(value());
}