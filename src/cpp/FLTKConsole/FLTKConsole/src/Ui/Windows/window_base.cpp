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
// PUBLIC

/*
	The base class for main and radio windows.
*/
//----------------------------------------------------
// Constructor
WindowBase::WindowBase(int radio, int x, int y, int w, int h) : Fl_Double_Window(w, h) {
	// Radio inst
	r = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));
	char label[20];
	sprintf_s(label, "Receiver-%d", r);
	copy_label(label);

	// Set window position
	position(x, y);

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
}

//===================================================
// Event handlers
//----------------------------------------------------
// Close window
void WindowBase::close() {
	Fl::delete_widget(this);
	Fl::delete_widget(audio);
	Fl::delete_widget(modes);
	Fl::delete_widget(filters);
}

//----------------------------------------------------
// Resize event
void  WindowBase::resize(int x, int y, int w, int h) {
	// Tell window to resize all widgets
	Fl_Double_Window::resize(x, y, w, h);
	// Save position and size
	if (r == 1) {
		p->set_window_x(x);
		p->set_window_y(y);
		p->set_window_w(w);
		p->set_window_h(h);
	}
	else if (r == 2) {
		p->set_radio2_x(x);
		p->set_radio2_y(y);
		p->set_radio2_w(w);
		p->set_radio2_h(h);
	}
	else if (r == 3) {
		p->set_radio3_x(x);
		p->set_radio3_y(y);
		p->set_radio3_w(w);
		p->set_radio3_h(h);
	}
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
	struct struct_w_loc loc = get_location(r);
	if (show) {
		audio->position(loc.x + loc.w + 5, loc.y);
		audio->show();
	}
	else {
		audio->hide();
	}
}

//----------------------------------------------------
// Show or hide the modes panel
void  WindowBase::manage_mode_panel(bool show) {
	struct struct_w_loc loc = get_location(r);
	if (show) {
		modes->position(loc.x + loc.w + 5, loc.y);
		modes->show();
	}
	else {
		modes->hide();
	}
}

//----------------------------------------------------
// Show or hide the filter panel
void  WindowBase::manage_filter_panel(bool show) {
	struct struct_w_loc loc = get_location(r);
	if (show) {
		filters->position(loc.x + loc.w + 5, loc.y);
		filters->show();
	}
	else {
		filters->hide();
	}
}

//==============================================================================
// PRIVATE

struct struct_w_loc WindowBase::get_location(int radio) {
	if (radio == 1) {
		w_loc.x = p->get_window_x();
		w_loc.y = p->get_window_y();
		w_loc.w = p->get_window_w();
		w_loc.h = p->get_window_h();
	}
	else if (radio == 2) {
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
	return w_loc;
}

//==============================================================================
// TRIGGER CLASSES

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