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
void idle_cb(void* data) {
	MainWindow* w = (MainWindow*)data;
	//Fl::add_timeout(0.2, idle_cb);
	w->handle_idle_timeout();
}

/*
	The one and only main window
*/
//----------------------------------------------------
// Constructor
MainWindow::MainWindow(int w, int h) : Fl_Double_Window(w, h) {

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

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
	StartBtn = new ControlButton(this, start_str, stop_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80, (Fl_Color)67);
	m = grid->get_cell_metrics(0, 1);
	DiscoverBtn = new DiscoverButton(this, discover_str, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	m = grid->get_cell_metrics(1, 0, 2, 3);
	VFOComponent *c = new VFOComponent(radio_id, 0, m.x, m.y, m.w, m.h);

	// Add mode trigger
	m = grid->get_cell_metrics(1, 3);
	ModeBtn = new ModeTrigger(this, mode_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);

	// Add filter trigger
	m = grid->get_cell_metrics(2, 3);
	FilterBtn = new FilterTrigger(this, filter_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);

	// Close up and display
	top_group->end();
	// Finish up
	end();
	// Initially deactivate all buttons
	StartBtn->deactivate();
	DiscoverBtn->deactivate();
	ModeBtn->deactivate();
	FilterBtn->deactivate();

	// Display main window
	show();

	// Create the modes panel hidden
	modes = new Modes(230, 80);
	modes->hide();

	// Create the filters panel hidden
	filters = new Filters(230, 80);
	filters->hide();

	// Set an idle timeout
	Fl::add_timeout(0.2, idle_cb, (void*)this);
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
// Callbacks
//----------------------------------------------------
// Handle idle timeout
void MainWindow::handle_idle_timeout() {
	// Handle enable/disable of controls here
	bool discovered = RSt::inst().get_discovered();
	
	if(discovered) {
		StartBtn->activate();
		DiscoverBtn->deactivate();
		ModeBtn->activate();
		FilterBtn->activate();
	}
	else {
		StartBtn->deactivate();
		DiscoverBtn->activate();
		ModeBtn->deactivate();
		FilterBtn->deactivate();
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

//----------------------------------------------------
// Show or hide the filter panel
void  MainWindow::manage_filter_panel(bool show) {

	if (show) {
		int x = p->get_window_x() + p->get_window_w() + 5;
		int y = p->get_window_y();
		filters->position(x, y);
		filters->show();
	}
	else {
		filters->hide();
	}
}

//==============================================================================
// Control button (start/stop)
ControlButton::ControlButton(MainWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col) : ToggleButtonBase(button_up_label, x, y, w, h, back_col, button_up_col) {
	myparent = parent_widget;
	up_label = button_up_label;
	down_label = button_down_label;
	up_col = button_up_col;
	down_col = button_down_col;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
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

//==============================================================================
// Mode button
ModeTrigger::ModeTrigger(MainWindow* parent_widget, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(button_label, x, y, w, h, back_col, label_col) {
	parent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
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

//==============================================================================
// Filter button
FilterTrigger::FilterTrigger(MainWindow* parent_widget, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : ToggleButtonBase(button_label, x, y, w, h, back_col, label_col) {
	parent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	id = button_id;
}

//----------------------------------------------------
// Handle click event
int FilterTrigger::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (value()) {
			// Hide 
			parent->manage_filter_panel(false);
			// Button release
			clear();
		}
		else {
			// Show
			parent->manage_filter_panel(true);
			// Button depressed
			set();
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}