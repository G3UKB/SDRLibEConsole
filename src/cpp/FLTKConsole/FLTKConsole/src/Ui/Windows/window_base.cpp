/*
window_base.cpp

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
void base_idle_cb(void* data) {
	MainWindow* w = (MainWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, base_idle_cb, data);
}

/*
	The base class for main and radio windows.
*/
//----------------------------------------------------
// Constructor
WindowBase::WindowBase(int radio, int w, int h) : Fl_Double_Window(w, h) {
	// Radio inst
	r = radio;

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
	grid = new GridLayout(5, 5, w - 10, h - 10, 3, 4, 5);

	// Add audio buton
	m = grid->get_cell_metrics(0, 3);
	AudioBtn = new AudioTriggerBase(this, audio_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80);

	// Add the VFO component
	// This extends Fl_Group so we place the group below the buttons
	m = grid->get_cell_metrics(1, 0, 2, 3);
	VFOComponent *c = new VFOComponent(r, 0, m.x, m.y, m.w, m.h);

	// We place the radio buttons into another grid
	// Get metrics from grid
	m = grid->get_cell_metrics(1, 3, 2, 1);
	// Create grid_1 with the new metrics
	GridLayout *grid_1 = new GridLayout(m.x, m.y, m.w, m.h, 2, 1, 2);

	// Add mode trigger in grid_1
	m = grid_1->get_cell_metrics(0, 0);
	ModeBtn = new ModeTriggerBase(this, mode_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);

	// Add filter trigger in grid_1
	m = grid_1->get_cell_metrics(1, 0);
	FilterBtn = new FilterTriggerBase(this, filter_str, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)67);

	// Close up and display
	top_group->end();
	// Finish up
	end();

	// Initially deactivate all buttons
	ModeBtn->deactivate();
	FilterBtn->deactivate();

	// Create the audio panel hidden for radio 1
	audio = new Audio(r, 350, 130);
	audio->hide();

	// Create the modes panel hidden
	modes = new Modes(r, 230, 80);
	modes->hide();

	// Create the filters panel hidden
	filters = new Filters(r, 230, 80);
	filters->hide();

	// Set an idle timeout
	Fl::add_timeout(0.2, base_idle_cb, (void*)this);
}

//===================================================
// Callbacks
//----------------------------------------------------
// Handle idle timeout
void WindowBase::handle_idle_timeout() {
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

//----------------------------------------------------
// Show or hide the audio panel
void  WindowBase::manage_audio_panel(bool show) {

	if (show) {
		int x = p->get_window_x() + p->get_window_w() + 5;
		int y = p->get_window_y();
		audio->position(x, y);
		audio->show();
	}
	else {
		audio->hide();
	}
}

//----------------------------------------------------
// Show or hide the modes panel
void  WindowBase::manage_mode_panel(bool show) {

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
void  WindowBase::manage_filter_panel(bool show) {

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
// Mode button
ModeTriggerBase::ModeTriggerBase(WindowBase* parent_widget, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Toggle_Button(x, y, w, h, button_label) {
	parent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	id = button_id;
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int ModeTriggerBase::handle(int event) {
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
FilterTriggerBase::FilterTriggerBase(WindowBase* parent_widget, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Toggle_Button(x, y, w, h, button_label) {
	parent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	id = button_id;
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int FilterTriggerBase::handle(int event) {
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

//==============================================================================
// Audio button
AudioTriggerBase::AudioTriggerBase(WindowBase* parent_widget, char* button_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col) : Fl_Toggle_Button(x, y, w, h, button_label) {
	parent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	id = button_id;
	color((Fl_Color)back_col);
	labelcolor((Fl_Color)label_col);
}

//----------------------------------------------------
// Handle click event
int AudioTriggerBase::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (value()) {
			// Hide 
			parent->manage_audio_panel(false);
			// Button release
			clear();
		}
		else {
			// Show
			parent->manage_audio_panel(true);
			// Button depressed
			set();
		}
		return 1;
	}
	default:
		return Fl_Widget::handle(event);
	}
}