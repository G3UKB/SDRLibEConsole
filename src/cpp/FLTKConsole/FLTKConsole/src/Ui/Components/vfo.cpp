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
	freq_inc_map.insert (std::pair <std::string, int> ("100MHz", 100000000));
	freq_inc_map.insert (std::pair <std::string, int> ("10MHz", 10000000));
	freq_inc_map.insert (std::pair <std::string, int> ("1MHz", 1000000));
	freq_inc_map.insert (std::pair <std::string, int> ("100KHz", 100000));
	freq_inc_map.insert (std::pair <std::string, int> ("10KHz", 10000));
	freq_inc_map.insert (std::pair <std::string, int> ("1KHz", 1000));
	freq_inc_map.insert (std::pair <std::string, int> ("100Hz", 100));
	freq_inc_map.insert (std::pair <std::string, int> ("10Hz", 10));
	freq_inc_map.insert (std::pair <std::string, int> ("1Hz", 1));

	// Make it look decent
	box(FL_GTK_THIN_UP_BOX);
	color((Fl_Color)24);

	// Create VFO digits
	create_digits();

	// Update frequency
	int freq = 7100000;
	current_freq = freq;

	//set_display_freq(convertFreq(7100000));
	//set_radio_freq();

}

VFOComponent::~VFOComponent()
{
}

void VFOComponent::set_freq_inc(std::string id) {
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
			set_radio_freq();
		}
	}
}

void VFOComponent::freq_minus() {
	if (freq_inc > 0) {
		int ifreq = current_freq - freq_inc;
		if (ifreq >= MIN_FREQ) {
			current_freq = ifreq;
			set_display_freq(convertFreq(current_freq));
			set_radio_freq();
		}
	}
}

std::string VFOComponent::convertFreq(int freq) {
	// Convert to a string representation of the frequency in Hz
	std::string sfreq = std::to_string(freq);
	// Add leading zeros to make it a 9 digit string
	// Number of leading zeros to add
	int l = 9 - sfreq.length();
	std::string leading_zeros;
	for (int i = 0; i < l; i++) {
		leading_zeros += "0";
	}
	return (leading_zeros += sfreq);
}

void VFOComponent::set_display_freq(std::string freq) {
	char c[] = "\0\0";
	c[0] = freq[0]; d_100MHz->label(c);
	c[0] = freq[1]; d_10MHz->label(c);
	c[0] = freq[2]; d_1MHz->label(c);
	c[0] = freq[3]; d_100KHz->label(c);
	c[0] = freq[4]; d_10KHz->label(c);
	c[0] = freq[5]; d_1KHz->label(c);
	c[0] = freq[6]; d_100Hz->label(c);
	c[0] = freq[7]; d_10Hz->label(c);
	c[0] = freq[8]; d_1Hz->label(c);
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
	d_100MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 35, 63, 35, 17);
	d_100MHz->argument(1);
	d_10MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 60, 63, 35, 17);
	d_10MHz->argument(2);
	d_1MHz = new VFODigit(this, MHZ_COLOR, MHZ_FONT, 85, 63, 35, 17);
	d_1MHz->argument(3);
	d_100KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 130, 63, 35, 17);
	d_100KHz->argument(4);
	d_10KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 155, 63, 35, 17);
	d_10KHz->argument(5);
	d_1KHz = new VFODigit(this, KHZ_COLOR, KHZ_FONT, 180, 63, 35, 17);
	d_1KHz->argument(6);
	d_100Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 220, 63, 35, 17);
	d_100Hz->argument(7);
	d_10Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 245, 63, 35, 17);
	d_10Hz->argument(8);
	d_1Hz = new VFODigit(this, HZ_COLOR, HZ_FONT, 270, 63, 35, 17);
	d_1Hz->argument(9);

	// Create and place two dot spacers
	sep_1 = new Fl_Box(105, 63, 35, 17, ".");
	sep_1->labelsize(36);
	sep_1->labelcolor((Fl_Color)90);
	sep_2 = new Fl_Box(200, 63, 35, 17, ".");
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

VFODigit::VFODigit(VFOComponent* parent, Fl_Color label_colour, float font_size, int x, int y, int w, int h) : Fl_Box(x, y, w, h, "0") {

	my_parent = parent;

	// Set attributes
	labelsize(font_size);
	labelcolor((Fl_Color)label_colour);
};

VFODigit::~VFODigit() {

}

// Handle all events
int VFODigit::handle(int event) {
	switch (event) {
		case FL_ENTER: {
			printf("Enter\n");
			labelsize(MHZ_FONT_OVER);
			//labelcolor((Fl_Color)91);
			redraw();
			return 1;
		}
		case FL_LEAVE: {
			printf("Leave\n");
			labelsize(HZ_FONT);
			//labelcolor((Fl_Color)40);
			redraw();
			return 1;
		}
		default:
			return Fl_Widget::handle(event);
	}
}

/*
void VFODigit::paint(Graphics& g) {
	// Add any additional painting here
	Label::paint(g);
};

void VFODigit::resized() {

};

void VFODigit::mouseEnter(const MouseEvent& event) {
	String id = event.eventComponent->getComponentID();
	my_parent->set_freq_inc(id);

	// Grow font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h + 5, Font::plain));
}

void VFODigit::mouseExit(const MouseEvent& event) {
	my_parent->reset_freq_inc();

	// Shrink font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h - 5, Font::plain));
}

void VFODigit::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	float dir = wheel.deltaY;
	if (dir < 0) {
		// Freq going down
		my_parent->freq_minus();
	}
	else {
		// Freq going up
		my_parent->freq_plus();
	}
}
*/