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
	((MainWindow*)user_data)->handle_radio();
}

/*
	The one and only main window
*/
//----------------------------------------------------
// Constructor
MainWindow::MainWindow(int w, int h) : WindowBase(1, w, h) {
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

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

	// Initially deactivate all buttons
	StartBtn->deactivate();
	DiscoverBtn->deactivate();
	//SelectRadio->deactivate();

	// Display main window
	show();

	// Set an idle timeout
	Fl::add_timeout(0.2, main_window_idle_cb, (void*)this);
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
	static int last_discovered = -1;
	bool discovered = RSt::inst().get_discovered();
	if (discovered != last_discovered) {
		if (discovered) {
			StartBtn->activate();
			//SelectRadio->activate();
			DiscoverBtn->deactivate();
		}
		else {
			StartBtn->deactivate();
			//SelectRadio->deactivate();
			DiscoverBtn->activate();
		}
	}
	last_discovered = discovered;
}

// Handle number of radio instances
void MainWindow::handle_radio() {
	Radio2_Win = new RadioWindow(2, 331, 124);
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

/*
//==============================================================================
// Radio choice
RadioChoice::RadioChoice(MainWindow* parent_widget, int x, int y, int w, int h, Fl_Color back_col, Fl_Color text_col) : Fl_Choice(x, y, w, h) {
	myparent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)text_col);
}

//----------------------------------------------------
// Handle click event
int RadioChoice::handle(int event) {
	printf("Event %d\n");
	//switch (event) {
	
	//}
	//default:
		return Fl_Widget::handle(event);
	//}
}
*/