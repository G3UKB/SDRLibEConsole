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

//----------------------------------------------------
// Constructor
MainWindow::MainWindow(Preferences* prefs, RadioInterface* radio_interface) : Fl_Double_Window(prefs->get_window_w(), prefs->get_window_h()) {

	r_i = radio_interface;
	p = prefs;
	int w = p->get_window_w();
	int h = p->get_window_h();

	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Set window position
	position(p->get_window_x(), p->get_window_y());

	// Add a group box
	top_group = new Fl_Group(5, 5, w - 10, h - 10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w - 10, h - 10, 3, 4, 5);
	metrics m;

	// Add start and stop buttons to the group
	m = grid->get_cell_metrics(0, 0);
	StartBtn = new ControlButton(this, r_i, start_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);
	m = grid->get_cell_metrics(0, 1);
	StopBtn = new ControlButton(this, r_i, stop_str, 1, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	m = grid->get_cell_metrics(1, 0, 2, 3);
	VFOComponent *c = new VFOComponent(r_i, radio_id, 0, m.x, m.y, m.w, m.h);

	// Add mode trigger
	m = grid->get_cell_metrics(1, 3);
	ModeBtn = new ModeTrigger(this, r_i, mode_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);

	// Close up and display
	top_group->end();
	end();
	show();

	// Create the modes panel hidden
	modes = new Modes(r_i, 270, 110);
	modes->hide();
}

//===================================================
// Event handlers
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
	//printf("Event was %s (%d)\n", fl_eventnames[event], event);
	switch (event) {
	case FL_HIDE: {
			// Save prefs
			p->save();
			break;
		}
	}
	// Pass all events down
	return Fl_Window::handle(event);
}

//===================================================
// Callbacks from children

//----------------------------------------------------
// Handle control button state
void MainWindow::handle_button_state(int id) {
	if (id == 0) {
		// Start button pressed
		StartBtn->set();
		StopBtn->clear();
	}
	else if (id == 1) {
		// Stop button pressed
		StartBtn->clear();
		StopBtn->set();
	}
}

//----------------------------------------------------
// Show or hide the modes panel
void  MainWindow::manage_mode_panel(bool show) {

	if (show) {
		int x = p->get_window_x() + p->get_window_w() + 5;
		int y = p->get_window_y();
		modes->position(x, y);
		modes->show();
	}
	else {
		modes->hide();
	}
}

//==============================================================================
// Control button (start/stop)
ControlButton::ControlButton(MainWindow* parent_widget, RadioInterface* radio_interface, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(radio_interface, button_label, x, y, w, h, back_col, label_col) {
	myparent = parent_widget;
	r_i = radio_interface;
	id = button_id;
}

//----------------------------------------------------
// Handle click event
int ControlButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (id == 0) {
			// Start events
			r_i->ri_radio_start(0);
			myparent->handle_button_state(id);
		}
		else if (id == 1) {
			// Stop events
			r_i->ri_radio_stop();
			myparent->handle_button_state(id);
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}

//==============================================================================
// Mode button
ModeTrigger::ModeTrigger(MainWindow* parent_widget, RadioInterface* radio_interface, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(radio_interface, button_label, x, y, w, h, back_col, label_col) {
	parent = parent_widget;
	r_i = radio_interface;
	id = button_id;
}

//----------------------------------------------------
// Handle click event
int ModeTrigger::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (value()) {
			// Hide 
			parent->manage_mode_panel(false);
			// Button release
			clear();
		}
		else {
			// Show
			parent->manage_mode_panel(true);
			// Button depressed
			set();
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}