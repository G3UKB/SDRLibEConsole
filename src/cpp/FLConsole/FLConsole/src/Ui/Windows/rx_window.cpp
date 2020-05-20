/*
rx_window.cpp

Auxiliary receivers window for the FLTK Console

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
void rx_window_idle_cb(void* data) {
	RxWindow* w = (RxWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, rx_window_idle_cb, data);
}

//----------------------------------------------------
// Constructor
RxWindow::RxWindow(int radio, int x, int y, int w, int h) : Fl_Double_Window(w, h) {

	// Stash paramaters
	width = w;
	height = h;
	radio_id = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Set window attributes
	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));
	char label[20];
	sprintf_s(label, "Receiver - %d", radio_id);
	copy_label(label);

	// Set window position
	position(x, y);

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
	ModeBtn->deactivate();
	FilterBtn->deactivate();

	// Display main window
	show();
	
	// Start the idle timeout
	Fl::add_timeout(0.2, rx_window_idle_cb, (void*)this);
}

//----------------------------------------------------
// Populate window
void RxWindow::do_layout() {

	// Layout params
	int rows = 3;
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
	m = grid->get_cell_metrics(0, 0, 2, 3);
	VFOComponent *vfo = new VFOComponent(radio_id, 0, m.x, m.y, m.w, m.h);
	top_group->add(vfo);

	// Add audio button
	m = grid->get_cell_metrics(0, 3);
	std::function< int(int) > f3 = std::bind(&RxWindow::audio_handle_event, this, std::placeholders::_1);
	if (radio_id == 2) {
		RSt::inst().put_cb("AUDIO_R2_CB", f3);
		AudioBtn = new C_ToggleButton(std::string("AUDIO_R2_CB"), audio_str_up, audio_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	else {
		RSt::inst().put_cb("AUDIO_R3_CB", f3);
		AudioBtn = new C_ToggleButton(std::string("AUDIO_R3_CB"), audio_str_up, audio_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	top_group->add(AudioBtn);

	// We place the radio buttons into another grid
	// Get metrics from grid
	m = grid->get_cell_metrics(1, 3, 2, 1);
	// Create grid_1 with the new metrics
	GridLayout *grid_1 = new GridLayout(m.x, m.y, m.w, m.h, 2, 1, 2);

	// Add mode trigger in grid_1
	m = grid_1->get_cell_metrics(0, 0);
	std::function< int(int) > f4 = std::bind(&RxWindow::mode_handle_event, this, std::placeholders::_1);
	if (radio_id == 2) {
		RSt::inst().put_cb("MODE_R2_CB", f4);
		ModeBtn = new C_ToggleButton(std::string("MODE_R2_CB"), mode_str_up, mode_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	else {
		RSt::inst().put_cb("MODE_R3_CB", f4);
		ModeBtn = new C_ToggleButton(std::string("MODE_R3_CB"), mode_str_up, mode_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	top_group->add(ModeBtn);

	// Add filter trigger in grid_1
	m = grid_1->get_cell_metrics(1, 0);
	std::function< int(int) > f5 = std::bind(&RxWindow::filt_handle_event, this, std::placeholders::_1);
	if (radio_id == 2) {
		RSt::inst().put_cb("FILT_R2_CB", f5);
		FilterBtn = new C_ToggleButton(std::string("FILT_R2_CB"), filt_str_up, filt_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	else {
		RSt::inst().put_cb("FILT_R3_CB", f5);
		FilterBtn = new C_ToggleButton(std::string("FILT_R3_CB"), filt_str_up, filt_str_dwn, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67, (Fl_Color)80);
	}
	top_group->add(FilterBtn);

	// Close up and display
	top_group->end();
	// Finish up
	end();
}

//----------------------------------------------------
// Close window
void RxWindow::close() {
	modes->close();
	filters->close();
	Fl::remove_timeout(rx_window_idle_cb);
	Fl::delete_widget(this);
}

//===================================================
// Event handlers

//----------------------------------------------------
// Window event handler

//----------------------------------------------------
// Resize event
void  RxWindow::resize(int x, int y, int w, int h) {
	// Tell window to resize all widgets
	Fl_Double_Window::resize(x, y, w, h);
	// Save position and size
	if (radio_id == 2) {
		p->set_radio2_x(x);
		p->set_radio2_y(y);
		p->set_radio2_w(w);
		p->set_radio2_h(h);
	}
	else if (radio_id == 3) {
		p->set_radio3_x(x);
		p->set_radio3_y(y);
		p->set_radio3_w(w);
		p->set_radio3_h(h);
	}
}

//----------------------------------------------------
// General event handler
int RxWindow::handle(int event) {
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
// Audio handler
int RxWindow::audio_handle_event(int state) {
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
int RxWindow::mode_handle_event(int state) {
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
int RxWindow::filt_handle_event(int state) {
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
void RxWindow::handle_idle_timeout() {
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
void RxWindow::set_location() {
	if (radio_id == 2) {
		w_loc.x = p->get_radio2_x();
		w_loc.y = p->get_radio2_y();
		w_loc.w = p->get_radio2_w();
		w_loc.h = p->get_radio2_h();
	}
	else {
		w_loc.x = p->get_radio3_x();
		w_loc.y = p->get_radio3_y();
		w_loc.w = p->get_radio3_w();
		w_loc.h = p->get_radio3_h();
	}
}

