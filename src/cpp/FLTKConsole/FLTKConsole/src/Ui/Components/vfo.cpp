/*
vfo.cpp

VFO implementation for the FLTK Console

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
// Main VFO Component Panel
//==============================================================================

//==============================================================================
// Constructor/Destructor
VFOComponent::VFOComponent(int radio, int p_vfo_type, int x, int y, int w, int h) : Fl_Group(x, y, w, h)
{
	r = radio;
	x_ord = x;
	y_ord = y;
	width = w;
	height = h;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Local vars
	vfo_type = p_vfo_type;
	// Create freq_inc_map
	freq_inc_map.insert (std::pair <int, int> (1, 100000000));
	freq_inc_map.insert (std::pair <int, int> (2, 10000000));
	freq_inc_map.insert (std::pair <int, int> (3, 1000000));
	freq_inc_map.insert (std::pair <int, int> (4, 100000));
	freq_inc_map.insert (std::pair <int, int> (5, 10000));
	freq_inc_map.insert (std::pair <int, int> (6, 1000));
	freq_inc_map.insert (std::pair <int, int> (7, 100));
	freq_inc_map.insert (std::pair <int, int> (8, 10));
	freq_inc_map.insert (std::pair <int, int> (9, 1));

	// Make it look decent
	box(FL_GTK_THIN_UP_BOX);
	color((Fl_Color)24);

	// Create VFO digits
	create_digits();

	// Update frequency
	current_freq = p->get_freq(r);

	convertFreq(current_freq);
	set_display_freq(convertFreq(current_freq));
	set_radio_freq();
}

VFOComponent::~VFOComponent()
{
}

//----------------------------------------------------
// As we enter digit we set the appropriate increment for the digit
void VFOComponent::set_freq_inc(int id) {
	freq_inc = freq_inc_map.at(id);
}

//----------------------------------------------------
//  As we leave digit we reset the increment
void VFOComponent::reset_freq_inc() {
	freq_inc = -1;
}

//----------------------------------------------------
// On mouse wheel up we increment the frequency by increment value
void VFOComponent::freq_plus() {
	if (freq_inc > 0) {
		int ifreq = current_freq + freq_inc;
		if (ifreq <= MAX_FREQ) {
			current_freq = ifreq;
			set_display_freq(convertFreq(current_freq));
			set_radio_freq();
			p->set_freq(r, current_freq);
		}
	}
}

//----------------------------------------------------
// On mouse wheel down we decrement the frequency by increment value
void VFOComponent::freq_minus() {
	if (freq_inc > 0) {
		int ifreq = current_freq - freq_inc;
		if (ifreq >= MIN_FREQ) {
			current_freq = ifreq;
			set_display_freq(convertFreq(current_freq));
			set_radio_freq();
			p->set_freq(r, current_freq);
		}
	}
}

//----------------------------------------------------
// Convert integer frequency in Hz to a 9 digit string
std::string VFOComponent::convertFreq(int freq) {
	// Convert to a string representation of the frequency in Hz
	std::string sfreq = std::to_string(freq);
	// Add leading zeros to make it a 9 digit string
	// Number of leading zeros to add
	int l = 9 - sfreq.length();
	std::string leading_zeros("");
	for (int i = 0; i < l; i++) {
		leading_zeros += "0";
	}
	return (leading_zeros += sfreq);
}

//----------------------------------------------------
// Set the display to the 9 digit string
void VFOComponent::set_display_freq(std::string freq) {
	// Update all the frequency digits
	c0[0] = freq[0]; d_100MHz->label(c0);
	c1[0] = freq[1]; d_10MHz->label(c1);
	c2[0] = freq[2]; d_1MHz->label(c2);
	c3[0] = freq[3]; d_100KHz->label(c3);
	c4[0] = freq[4]; d_10KHz->label(c4);
	c5[0] = freq[5]; d_1KHz->label(c5);
	c6[0] = freq[6]; d_100Hz->label(c6);
	c7[0] = freq[7]; d_10Hz->label(c7);
	c8[0] = freq[8]; d_1Hz->label(c8);
}

//==============================================================================
// GUI Events

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Create and layout the 9 digits and 2 spacers
void VFOComponent::create_digits() {
	
	// Create a grid layout handler
	GridLayout *grid = new GridLayout(x_ord+5, y_ord+5, width - 10, height - 10, 1, 11);
	metrics m;

	// Create digits
	m = grid->get_cell_metrics(0, 0);
	d_100MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, m.x, m.y, m.w, m.h);
	d_100MHz->argument(1);
	m = grid->get_cell_metrics(0, 1);
	d_10MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, m.x, m.y, m.w, m.h);
	d_10MHz->argument(2);
	m = grid->get_cell_metrics(0, 2);
	d_1MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, m.x, m.y, m.w, m.h);
	d_1MHz->argument(3);

	m = grid->get_cell_metrics(0, 3);
	sep_1 = new Fl_Box(m.x, m.y, m.w, m.h, ".");
	sep_1->labelsize(36);
	sep_1->labelcolor((Fl_Color)90);

	m = grid->get_cell_metrics(0, 4);
	d_100KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, m.x, m.y, m.w, m.h);
	d_100KHz->argument(4);
	m = grid->get_cell_metrics(0, 5);
	d_10KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, m.x, m.y, m.w, m.h);
	d_10KHz->argument(5);
	m = grid->get_cell_metrics(0, 6);
	d_1KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, m.x, m.y, m.w, m.h);
	d_1KHz->argument(6);

	m = grid->get_cell_metrics(0, 7);
	sep_2 = new Fl_Box(m.x, m.y, m.w, m.h, ".");
	sep_2->labelsize(36);
	sep_2->labelcolor((Fl_Color)90);

	m = grid->get_cell_metrics(0, 8);
	d_100Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, m.x, m.y, m.w, m.h);
	d_100Hz->argument(7);
	m = grid->get_cell_metrics(0, 9);
	d_10Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, m.x, m.y, m.w, m.h);
	d_10Hz->argument(8);
	m = grid->get_cell_metrics(0, 10);
	d_1Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, m.x, m.y, m.w, m.h);
	d_1Hz->argument(9);

	// Close off the group
	end();
}

//----------------------------------------------------
// Tune the radio-n to the current frequency
void VFOComponent::set_radio_freq() {
	if (r == 1)
		r_i->ri_server_cc_out_set_rx_1_freq(current_freq);
	else if (r == 2)
		r_i->ri_server_cc_out_set_rx_2_freq(current_freq);
	else if (r == 3)
		r_i->ri_server_cc_out_set_rx_3_freq(current_freq);
}

//==============================================================================
// A VFO Digit
//==============================================================================

//----------------------------------------------------
// Constructor/destructor
VFODigit::VFODigit(VFOComponent* parent, Fl_Color label_colour, float font_size, int x, int y, int w, int h) : Fl_Box(FL_FLAT_BOX, x, y, w, h, "0") {

	my_parent = parent;

	// Set attribute
	labelsize(font_size);
	labelcolor((Fl_Color)label_colour);
	Fl_Color parent_col = my_parent->color();
	color(parent_col);

	// No active digits
	active_digit = -1;
};

VFODigit::~VFODigit() {

}

//----------------------------------------------------
// Draw a single digit
void VFODigit::draw() {
	Fl_Box::draw_box();
	Fl_Widget::draw_label();
}

//==============================================================================
// Event Handlers

//----------------------------------------------------
// General handler
int VFODigit::handle(int event) {
	switch (event) {
		// Mouse enter digit
		case FL_ENTER: {
			// Grow the digit a little
			if (argument() > 6)
				labelsize(HZ_FONT_OVER);
			else if (argument() > 3)
				labelsize(KHZ_FONT_OVER);
			else
				labelsize(MHZ_FONT_OVER);
			// Remember where we are
			active_digit = argument();
			my_parent->set_freq_inc(active_digit);
			redraw();
			return 1;
		}
		// Mouse leave digit
		case FL_LEAVE: {
			// Shrink back to normal size
			if (argument() > 6)
				labelsize(HZ_FONT);
			else if (argument() > 3)
				labelsize(KHZ_FONT);
			else
				labelsize(MHZ_FONT);
			// Nothing active
			active_digit = -1;
			my_parent->reset_freq_inc();
			redraw();
			return 1;
		}
		// Mouse wheel over digit
		case FL_MOUSEWHEEL: {
			// Scrolling
			if (active_digit != -1) {
				// event_dy returns 1 or -1
				if (Fl::event_dy() < 0) {
					// Freq increasing
					my_parent->freq_plus();
				}
				else {
					// Freq decreasing
					my_parent->freq_minus();
				}
			}
			return 1;
		}
		default:
			return Fl_Widget::handle(event);
	}
}
