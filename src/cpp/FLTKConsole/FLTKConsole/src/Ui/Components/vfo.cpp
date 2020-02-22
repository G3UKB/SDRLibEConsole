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
#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Pack.H"
//#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Draw.H"
#include "../../Includes/extern.h"
#include "../../Includes/radio_interface.h"
#include "../../Includes/vfo.h"

//==============================================================================
// Main VFO Component Panel
//==============================================================================

//==============================================================================
// Constructor/Destructor
VFOComponent::VFOComponent(std::string p_radio_id, int p_vfo_type, int x, int y, int w, int h) : Fl_Widget(x, y, w, h)
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

	// WE make content here but arrange it in resized ??
	layout_digits();

	// Update frequency
	int freq = 7100000;
	//set_freq(convertFreq(7100000));

	current_freq = freq;
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
			set_freq(convertFreq(current_freq));
			set_radio_freq();
		}
	}
}

void VFOComponent::freq_minus() {
	if (freq_inc > 0) {
		int ifreq = current_freq - freq_inc;
		if (ifreq >= MIN_FREQ) {
			current_freq = ifreq;
			set_freq(convertFreq(current_freq));
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

void VFOComponent::set_freq(std::string freq) {
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
	set_freq(convertFreq(freq));
}

//==============================================================================
// GUI Events
void VFOComponent::draw()
{
	// Our component is opaque, so we must completely fill the background with a solid colour
	Fl::background(100,100,100);
}

void VFOComponent::resize()
{
	// This is called when the VFOComponent is resized.
	//layout_digits();
}

int VFOComponent::handle(int event) {
	printf("Handle component\n");
	return 0;
}

//==============================================================================
// Private
void VFOComponent::create_digits() {

	// Create digits
	d_100MHz = new VFODigit(this, std::string("0"), MHZ_COLOR, MHZ_FONT);
	d_100MHz->argument(1);
	d_10MHz = new VFODigit(this, std::string("0"), MHZ_COLOR, MHZ_FONT);
	d_10MHz->argument(2);
	d_1MHz = new VFODigit(this, std::string("0"), MHZ_COLOR, MHZ_FONT);
	d_1MHz->argument(3);
	d_100KHz = new VFODigit(this, std::string("0"), KHZ_COLOR, KHZ_FONT);
	d_100KHz->argument(4);
	d_10KHz = new VFODigit(this, std::string("0"), KHZ_COLOR, KHZ_FONT);
	d_10KHz->argument(5);
	d_1KHz = new VFODigit(this, std::string("0"), KHZ_COLOR, KHZ_FONT);
	d_1KHz->argument(6);
	d_100Hz = new VFODigit(this, std::string("0"), HZ_COLOR, HZ_FONT);
	d_100Hz->argument(7);
	d_10Hz = new VFODigit(this, std::string("0"), HZ_COLOR, HZ_FONT);
	d_10Hz->argument(8);
	d_1Hz = new VFODigit(this, std::string("0"), HZ_COLOR, HZ_FONT);
	d_1Hz->argument(9);

	// Create two spacers
	sep_1 = new Fl_Output(0,0,0,0, ".");
	sep_1->textcolor(FL_LIGHT1);
	sep_2 = new Fl_Output(0, 0, 0, 0, ".");
	sep_2->textcolor(FL_LIGHT1);

	set_freq(convertFreq(current_freq));
}

void VFOComponent::layout_digits() {

	// Create containers
	group = new Fl_Group(0,0, 170, 330);
	group->box(FL_EMBOSSED_FRAME);
	pack = new Fl_Pack(0,0, 170, 330);
	// Pack digits
	create_digits();
	pack->end();
	group->end();
	
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

VFODigit::VFODigit(VFOComponent *parent, std::string text, Fl_Color colour, float size) : Fl_Output(20,20,50,20) {

	my_parent = parent;
	// Create a digit ready to add to the grid
	label(text.data());
	textcolor(colour);
	//labelfont(1);
	labelsize(size);
};

VFODigit::~VFODigit() {

}

void VFODigit::draw() {

}

void VFODigit::resize() {

}

int VFODigit::handle(int event) {
	printf("Handle digit\n");
	return 0;
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