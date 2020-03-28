/*
vfo.h

VFO header for the FLTK Console

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

#pragma once

#include "includes.h"

//==============================================================================
// Defines
#define MHZ_FONT 35.0f
#define KHZ_FONT 35.0f
#define HZ_FONT 25.0f
#define MHZ_FONT_OVER 40.0f
#define KHZ_FONT_OVER 40.0f
#define HZ_FONT_OVER 30.0f
#define MHZ_COLOR (Fl_Color)fl_rgb_color(228,228,228)
#define KHZ_COLOR (Fl_Color)fl_rgb_color(228,228,228)
#define HZ_COLOR (Fl_Color)fl_rgb_color(237,100,41)
#define RX 0
#define TX 1
#define MIN_FREQ 1000
#define MAX_FREQ 500000000

class VFOComponent;

/*
	A VFO Digit component
*/
class VFODigit : public Fl_Box
{
public:
	//==============================================================================
	VFODigit(VFOComponent* parent, Fl_Color label_colour, float font_size, int x, int y, int w, int h);
	~VFODigit();

	//==============================================================================
	int handle(int event) override;
	void draw() override;

private:
	//==============================================================================
	// State variables
	VFOComponent *my_parent;
	int active_digit;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
/*
	Multi-instance VFO component
*/
class VFOComponent : public Fl_Group
{
public:
	//==============================================================================
	VFOComponent(int radio, int p_vfo_type, int x, int y, int w, int h);
	~VFOComponent();

	//==============================================================================
	void set_freq_inc(int inc);
	void reset_freq_inc();
	void freq_plus();
	void freq_minus();
	std::string convertFreq(int freq);
	void set_display_freq(std::string freq);

private:
	//==============================================================================
	// Containers

	// State variables
	// Container
	int x_ord;
	int y_ord;
	int width;
	int height;

	Preferences* p;
	RadioInterface* r_i;
	int vfo_type = RX;			// RX|TX
	int r;						// Radio id of VFO instance
	int freq_inc = -1;			// Last frequence increment in MHz
	int current_freq = 7100000;	// Current frequency in MHz

	// Map for lookup of increments
	std::map<int, int> freq_inc_map;

	// Digits
	VFODigit *d_100MHz;
	VFODigit *d_10MHz;
	VFODigit *d_1MHz;
	VFODigit *d_100KHz;
	VFODigit *d_10KHz;
	VFODigit *d_1KHz;
	VFODigit *d_100Hz;
	VFODigit *d_10Hz;
	VFODigit *d_1Hz;
	Fl_Box *sep_1;
	Fl_Box *sep_2;

	// Label storage
	char c0[2] = { 0,0 };
	char c1[2] = { 0,0 };
	char c2[2] = { 0,0 };
	char c3[2] = { 0,0 };
	char c4[2] = { 0,0 };
	char c5[2] = { 0,0 };
	char c6[2] = { 0,0 };
	char c7[2] = { 0,0 };
	char c8[2] = { 0,0 };


	//==============================================================================
	// Method prototypes
	void create_digits();
	void set_radio_freq();
};

