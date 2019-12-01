/*
display_panel.cpp

Display panel component for the Juce Console

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

#include "display_panel.h"
#include "../Common/extern.h"
#include "../RadioInterface/radio_interface.h"

//==============================================================================
// Display Panel
//==============================================================================

//==============================================================================
// Public

//==============================================================================
// Constructor/Destructor
DisplayPanel::DisplayPanel(int p_display_id)
{
	// Local vars
	display_id = p_display_id;
}

DisplayPanel::~DisplayPanel()
{
}

//==============================================================================
// GUI Events
void DisplayPanel::paint(Graphics& g)
{
	// Our component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::black);
	draw_grid(g);
}

void DisplayPanel::resized()
{
	// This is called when the display is resized.
}

//==============================================================================
// Private

void DisplayPanel::draw_grid(Graphics& g) {
	// Draw grid with labels within our component bounds
	g.setColour(Colours::green);
	draw_horiz(g);
	draw_vert(g);
}

void DisplayPanel::draw_horiz(Graphics& g) {
	// One horizontal line per 20 db
	int i, j;
	int db_divs = (abs(LOW_DB) - abs(HIGH_DB)) / 20;
	int db_pixels_per_div = ((getHeight() - T_MARGIN - B_MARGIN) / db_divs);
	for (i = 0, j = HIGH_DB; i <= db_divs; i++, j -= 20) {
		g.drawText(String(j), 5, Y_V_LABEL_ADJ + (i * db_pixels_per_div), 40, 20, Justification(Justification::left));
		g.drawLine(
			(float)(L_MARGIN),
			(float)(T_MARGIN + (i * db_pixels_per_div)),
			(float(getWidth() - R_MARGIN)),
			(float)(T_MARGIN + (i * db_pixels_per_div))
		);
	}
}

void DisplayPanel::draw_vert(Graphics& g) {
	// DIVS number of vertical bars
	int i, j;
	String sfreq;
	// Get current frequency
	int freq = RadioInterface::getInstance()->get_current_frequency();
	int start_freq = freq - (SPAN_FREQ / 2);
	int freq_inc = SPAN_FREQ / 7;

	int pixels_per_div = ((getWidth() - L_MARGIN - R_MARGIN) / DIVS);
	for (i = 0, j = start_freq; i <= DIVS; i++, j += freq_inc) {
		sfreq = String((float)(j / 1000000), 3);
		g.drawText(sfreq, 30 + (i * pixels_per_div), getHeight() - B_MARGIN + 50, 40, 20, Justification(Justification::left));
		g.drawLine(
			(float)(L_MARGIN + (i * pixels_per_div)),
			(float)(T_MARGIN),
			(float)(L_MARGIN + (i * pixels_per_div)),
			(float)(getHeight() - B_MARGIN)
		);
	}
}