/*
tx_window.cpp

Single TX window implementation for the FLTK Console

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

//----------------------------------------------------
// Idle time callback
// We call back to the window to do housekeeping
void tx_window_idle_cb(void* data) {
	TxWindow* w = (TxWindow*)data;
	w->handle_idle_timeout();
	Fl::repeat_timeout(0.2, tx_window_idle_cb, data);
}

//----------------------------------------------------
// Callback to set RF gain
static void rf_gain_cb(Fl_Widget* o, void* data) {
	RFSlider* s = (RFSlider*)data;
	s->handle_event();
}

/*
	The one and only main window
*/
//----------------------------------------------------
// Constructor
TxWindow::TxWindow(int radio, int x, int y, int w, int h) : WindowBase(radio, x, y, w, h, 4, 4, 1) {
	r = radio;
	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Add Duplex button to the group
	m = grid->get_cell_metrics(0, 2);
	DuplexBtn = new DuplexButton(this, duplex_str_on, duplex_str_off, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80, (Fl_Color)67);
	top_group->add(DuplexBtn);
	// Add MOX button to the group
	m = grid->get_cell_metrics(0, 3);
	MOXBtn = new MOXButton(this, mox_str_on, mox_str_off, 0, m.x, m.y, m.w, m.h, (Fl_Color)33, (Fl_Color)80, (Fl_Color)67);
	top_group->add(MOXBtn);
	// Add RF drive to the group
	m = grid->get_cell_metrics(3, 0, 1, 3);
	RFGain = new RFSlider(this, rf_gain_str, m.x, m.y, m.w, (2*m.h)/3, (Fl_Color)10);
	top_group->add(RFGain);
	// Display main window
	show();

	// Set an idle timeout
	Fl::add_timeout(0.2, tx_window_idle_cb, (void*)this);
}

//----------------------------------------------------
// Close window
void TxWindow::close() {
	Fl::remove_timeout(tx_window_idle_cb);
	Fl::delete_widget(this);
}

//===================================================
// Callbacks

//----------------------------------------------------
// Handle idle timeout
void TxWindow::handle_idle_timeout() {
	// Nothing todo here
	// Just call base class
	WindowBase::handle_idle_timeout();
}

//==============================================================================
// Duplex button
DuplexButton::DuplexButton(TxWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col) : Fl_Toggle_Button(x, y, w, h, button_up_label) {
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
int DuplexButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (RSt::inst().get_duplex()) {
			// Was duplex
			clear();
			label(up_label);
			labelcolor(up_col);
			RSt::inst().set_duplex(false);
			r_i->ri_server_cc_out_set_duplex(0);
		}
		else {
			// Was simplex
			set();
			label(down_label);
			labelcolor(down_col);
			RSt::inst().set_duplex(true);
			r_i->ri_server_cc_out_set_duplex(1);
		}
		return 1;
		}
		
	default:
		return Fl_Widget::handle(event);
	}
}

//==============================================================================
// MOX button
MOXButton::MOXButton(TxWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col) : Fl_Toggle_Button(x, y, w, h, button_up_label) {
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
int MOXButton::handle(int event) {
	switch (event) {
	case FL_PUSH: {
		if (RSt::inst().get_mox()) {
			// Was TX, goint to RX
			clear();
			label(up_label);
			labelcolor(up_col);
			RSt::inst().set_mox(false);
			r_i->ri_server_cc_out_set_mox(0);
		}
		else {
			// Was RX, going to TX
			set();
			label(down_label);
			labelcolor(down_col);
			RSt::inst().set_mox(true);
			r_i->ri_server_cc_out_set_mox(1);
		}
		return 1;
	}

	default:
		return Fl_Widget::handle(event);
	}
}

//==============================================================================
// RF Gain
RFSlider::RFSlider(TxWindow* parent_widget, char* label, int x, int y, int w, int h, Fl_Color back_col) : Fl_Value_Slider(x, y, w, h, label) {
	myparent = parent_widget;
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	type(5);
	labelsize(10);
	color((Fl_Color)back_col);
	selection_color((Fl_Color)1);
	//callback((Fl_Callback*)rf_gain_cb, (void*)this);
}

//----------------------------------------------------
// Handle events
void RFSlider::handle_event() {
	// Set R drive value 0.0-1.0
	c_server_set_rf_drive(value());
}

