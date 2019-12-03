/*
display_panel.h

Header for display panel component for the Juce Console

Copyright (C) 2019 by G3UKB Bob Cowdery

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

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// Defines
// Panadapter and waterfall definitions

// ----------------------------------
// Divisions and margins
// Panadapter grid divs
#define	DIVS 6
// Left, right, top, bottom margins(limits of grid and display)
#define	L_MARGIN 35
#define	R_MARGIN 15
#define	T_MARGIN 10
#define	B_MARGIN 100
// Adjustments for label data to correctly align
#define	X_H_LABEL_ADJ + 5
#define	Y_H_LABEL_ADJ - 10
#define	Y_V_LABEL_ADJ - 3

// Fixed height for waterfall display
#define	WF_HEIGHT 75

// ----------------------------------
// Panadapter db scale to scale the Y axis display
#define	LOW_DB - 140
#define	HIGH_DB - 20

// ----------------------------------
// Frequency
// Panadapter span i.e sample rate
#define	SPAN_FREQ 48000
#define MAX_SIZE_OF_DISPLAY 1920

// ===========================================================
/*
	Multi-instance display component
*/
class DisplayPanel : public Component, public Timer
{
public:
	//==============================================================================
	DisplayPanel(int p_display_id);
	~DisplayPanel();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void timerCallback();

private:
	//==============================================================================
	// State variables
	int display_id;
	float *buf;
	Path path;
	bool display_set = false;

	//==============================================================================
	// Method prototypes
	void draw_all(Graphics& g);
	void draw_horiz(Graphics& g);
	void draw_vert(Graphics& g);
	void draw_filter(Graphics& g);
	void draw_pan(Graphics& g);
	float val_to_coord(float val);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DisplayPanel)
};



