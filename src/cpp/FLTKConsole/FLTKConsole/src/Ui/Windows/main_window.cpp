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

/*
	The one and only main window
*/
//----------------------------------------------------
// Constructor
MainWindow::MainWindow(int x, int y, int w, int h) : WindowBase(1, x, y, w, h) {
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Get num radios
	int num = p->get_num_radios();

	// Add start/stop buttons to the group
	m = grid->get_cell_metrics(0, 0);
	StartBtn = new ControlButton(this, start_str, stop_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80, (Fl_Color)67);
	top_group->add(StartBtn);
	// Add discover button
	m = grid->get_cell_metrics(0, 1);
	DiscoverBtn = new DiscoverButton(this, discover_str, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
	top_group->add(DiscoverBtn);
	// Add radio choice
	m = grid->get_cell_metrics(0, 2);
	SelectRadio = new Fl_Choice(m.x+20, m.y, m.w-20, m.h, "RX:");
	SelectRadio->add("1");
	SelectRadio->add("2");
	SelectRadio->add("3");
	SelectRadio->color((Fl_Color)33);
	SelectRadio->labelcolor((Fl_Color)80);
	SelectRadio->callback((Fl_Callback*)radio_cb, (void*)this);
	top_group->add(SelectRadio);
	SelectRadio->value(num - 1);

	// Initially deactivate all buttons
	StartBtn->deactivate();
	DiscoverBtn->deactivate();
	SelectRadio->deactivate();

	// Display main window
	show();

	// Show radio windows as appropriate
	if (num > 1) {
		Radio2_Win = new RadioWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
	}
	if (num > 2) {
		Radio3_Win = new RadioWindow(3, p->get_radio3_x(), p->get_radio3_y(), p->get_radio3_w(), p->get_radio3_h());
	}

	// Set an idle timeout
	Fl::add_timeout(0.2, main_window_idle_cb, (void*)this);
}

//===================================================
// Event handlers

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
			// Close radio windows if active
			if (Radio2_Win != NULL) Radio2_Win->close();
			if (Radio3_Win != NULL) Radio3_Win->close();
			break;
		}
	}
	// Pass all events down
	return Fl_Window::handle(event);
}

//===================================================
// Callbacks
//----------------------------------------------------
// Handle idle timeout
void MainWindow::handle_idle_timeout() {
	// Handle enable/disable of controls here
	static int last_discovered = -1;
	bool discovered = RSt::inst().get_discovered();
	if (discovered != last_discovered) {
		if (discovered) {
			StartBtn->activate();
			SelectRadio->activate();
			DiscoverBtn->deactivate();
		}
		else {
			StartBtn->deactivate();
			SelectRadio->deactivate();
			DiscoverBtn->activate();
		}
	}
	last_discovered = discovered;

	WindowBase::handle_idle_timeout();
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
		StartBtn->clear();
		// Rejig the UI
		if (value == 0) {
			// One radio
			// Close radios 2/3 if active
			if (Radio2_Win != NULL) {
				Radio2_Win->close();
				Radio2_Win = NULL;
			}
			if (Radio3_Win != NULL ) {
				Radio3_Win->close();
				Radio3_Win = NULL;
			}
		}
		else if (value == 1) {;
			// Two radios
			if (Radio2_Win == NULL) Radio2_Win = new RadioWindow(2, p->get_radio2_x(), p->get_radio2_y(), p->get_radio2_w(), p->get_radio2_h());
			// Close radio 3 if active
			if (Radio3_Win != NULL ) {
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

//==============================================================================
// Control button (start/stop)
ControlButton::ControlButton(MainWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col) : Fl_Toggle_Button(x, y, w, h, button_up_label) {
	myparent = parent_widget;
	up_label = button_up_label;
	down_label = button_down_label;
	up_col = button_up_col;
	down_col = button_down_col;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)button_up_col);
	id = button_id;
}

//----------------------------------------------------
// Handle click event
int ControlButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (id == 0) {
			// Start/stop events
			bool running = RSt::inst().get_radio_running();
			if (running) {
				// Stop event
				r_i->ri_radio_stop();
				RSt::inst().set_radio_running(false);
				clear();
				label(up_label);
				labelcolor(up_col);
			}
			else {
				// Start event
				r_i->ri_radio_start(0);
				RSt::inst().set_radio_running(true);
				set();
				label(down_label);
				labelcolor(down_col);
			}
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
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
