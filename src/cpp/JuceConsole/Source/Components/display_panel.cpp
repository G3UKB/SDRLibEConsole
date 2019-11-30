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
	int i;
	g.setColour(Colours::green);
	// One horizontal line per 20 db
	int db_divs = (abs(LOW_DB) - abs(HIGH_DB))/20;
	int db_pixels_per_div = ((getHeight() - T_MARGIN - B_MARGIN) / db_divs);
	for (i = 0; i <= db_divs; i++) {
		g.drawLine(
			(float)(L_MARGIN),
			(float)(T_MARGIN + (i * db_pixels_per_div)),
			(float(getWidth() - L_MARGIN - R_MARGIN)),
			(float)(T_MARGIN + (i * db_pixels_per_div))
		);
	}
}