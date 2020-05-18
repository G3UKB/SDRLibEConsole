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
MainWindow::MainWindow(int x, int y, int w, int h) : WindowBase(1, x, y, w, h, 5, 4, 2) {
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Get num radios
	int num = p->get_num_radios();

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

	// Add Exit button to the group
	m = grid->get_cell_metrics(0, 3);
	ExitBtn = new ExitButton(this, exit_str, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);
	top_group->add(ExitBtn);

	// Deactivate buttons
	// Get re-activated as required in idle processing
	CtrlBtn->deactivate();
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

	// Create and hide TX window
	TX_Win = new TxWindow(4, p->get_tx_x(), p->get_tx_y(), p->get_tx_w(), p->get_tx_h());
	TX_Win->hide();

	// Set an idle timeout
	Fl::add_timeout(0.2, main_window_idle_cb, (void*)this);
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
		}
		else {
			CtrlBtn->deactivate();
			SelectRadio->deactivate();
			DiscoverBtn->activate();
		}
	}
	last_discovered = discovered;

	WindowBase::handle_idle_timeout();
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
