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

#include <iostream>
#include <string>
#include <map>
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl.H"
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Group.H"
#include "../../Includes/extern.h"
#include "../../Includes/radio_interface.h"
#include "../../Includes/vfo.h"

//==============================================================================
// Main VFO Component Panel
//==============================================================================

//==============================================================================
// Constructor/Destructor
VFOComponent::VFOComponent(std::string p_radio_id, int p_vfo_type, int x, int y, int w, int h) : Fl_Group(x, y, w, h)
{
	// Local vars
	vfo_type = p_vfo_type;
	radio_id = p_radio_id;
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
	int freq = 7100000;
	current_freq = freq;

	convertFreq(freq);
	set_display_freq(convertFreq(7100000));
	//set_radio_freq();

}

VFOComponent::~VFOComponent()
{
}

void VFOComponent::set_freq_inc(int id) {
	freq_inc = freq_inc_map.at(id);
}

void VFOComponent::reset_freq_inc() {
	freq_inc = -1;
}

void VFOComponent::freq_plus() {
	if (freq_inc > 0) {
		int ifreq = current_freq + freq_inc;
		if (ifreq <= MAX_FREQ) {
			current_freq = ifreq;
			set_display_freq(convertFreq(current_freq));
			//set_radio_freq();
		}
	}
}

void VFOComponent::freq_minus() {
	if (freq_inc > 0) {
		int ifreq = current_freq - freq_inc;
		if (ifreq >= MIN_FREQ) {
			current_freq = ifreq;
			set_display_freq(convertFreq(current_freq));
			//set_radio_freq();
		}
	}
}

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

void VFOComponent::set_display_freq(std::string freq) {
	// The string passed to label is not copied so it must be
	// unique and not go out of scope.
	static char c0[] = "\0\0";
	static char c1[] = "\0\0";
	static char c2[] = "\0\0";
	static char c3[] = "\0\0";
	static char c4[] = "\0\0";
	static char c5[] = "\0\0";
	static char c6[] = "\0\0";
	static char c7[] = "\0\0";
	static char c8[] = "\0\0";
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

void VFOComponent::set_freq_from_hz(int freq) {
	//set_display_freq(convertFreq(freq));
}

//==============================================================================
// GUI Events


//==============================================================================
// Private
void VFOComponent::create_digits() {
	
	// Create digits
	d_100MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 20, 58, 30, 37);
	d_100MHz->argument(1);
	d_10MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 50, 58, 30, 37);
	d_10MHz->argument(2);
	d_1MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 80, 58, 30, 37);
	d_1MHz->argument(3);
	d_100KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 125, 58, 30, 37);
	d_100KHz->argument(4);
	d_10KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 155, 58, 30, 37);
	d_10KHz->argument(5);
	d_1KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 185, 58, 30, 37);
	d_1KHz->argument(6);
	d_100Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 230, 60, 30, 30);
	d_100Hz->argument(7);
	d_10Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 255, 60, 30, 30);
	d_10Hz->argument(8);
	d_1Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 280, 60, 30, 30);
	d_1Hz->argument(9);

	// Create and place two dot spacers
	sep_1 = new Fl_Box(110, 58, 15, 32, ".");
	sep_1->labelsize(36);
	sep_1->labelcolor((Fl_Color)90);
	sep_2 = new Fl_Box(215, 61, 20, 29, ".");
	sep_2->labelsize(36);
	sep_2->labelcolor((Fl_Color)90);

	// Close off the group
	end();
}

void VFOComponent::set_radio_freq() {
	if (radio_id == "radio-1")
		;
	else if (radio_id == "radio-2")
		;
	else if (radio_id == "radio-3")
		;
}

//==============================================================================
// A VFO Digit
//==============================================================================

VFODigit::VFODigit(VFOComponent* parent, Fl_Color label_colour, float font_size, int x, int y, int w, int h) : Fl_Box(FL_NO_BOX, x, y, w, h, "0") {

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

void VFODigit::draw() {
	Fl_Widget::draw_label();
}

// Handle all events
int VFODigit::handle(int event) {
	switch (event) {
		case FL_ENTER: {
			// Grow the digit a little
			if (argument() > 6)
				labelsize(HZ_FONT_OVER);
			else if (argument() > 3)
				labelsize(KHZ_FONT_OVER);
			else
				labelsize(MHZ_FONT_OVER);
			// Necessary to make the label redraw correctly
			label(label());
			// Remember where we are
			active_digit = argument();
			my_parent->set_freq_inc(active_digit);
			redraw();
			return 1;
		}
		case FL_LEAVE: {
			// Shrink back to normal size
			if (argument() > 6)
				labelsize(HZ_FONT);
			else if (argument() > 3)
				labelsize(KHZ_FONT);
			else
				labelsize(MHZ_FONT);
			// Necessary to make the label redraw correctly
			label(label());
			// Nothing active
			active_digit = -1;
			my_parent->reset_freq_inc();
			redraw();
			return 1;
		}
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