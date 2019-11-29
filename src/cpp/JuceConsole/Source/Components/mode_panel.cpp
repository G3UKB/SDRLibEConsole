/*
mode_panel.cpp

Mode Bbuttons component for the Juce Console

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
#include "mode_panel.h"
#include "../Common/extern.h"

//==============================================================================

ModeButton::ModeButton(String label) {

	setButtonText(label);
	setClickingTogglesState(true);
}

ModeButton::~ModeButton() {}

void ModeButton::clicked() {

	if (getToggleState()) {
		printf("ON: %s\n", getComponentID());
	}
	else {
		printf("OFF: %s\n", getComponentID());
	}
}

//==============================================================================
// The panel
ModePanel::ModePanel(int p_mode_id) {

	// Local vars
	mode_id = p_mode_id;

	// Create all mode buttons
	create_buttons();
}

ModePanel::~ModePanel() {}

void ModePanel::resized()
{
	// This is called when the mode panel is resized.
	layout_buttons_in_grid();
}

//==============================================================================
// Private
void ModePanel::make_mode_button(ModeButton *btn, String label, String id) {
	btn = new ModeButton(label);
	btn->setComponentID(id);
	btn->setRadioGroupId(1);
	addAndMakeVisible(btn);
}

void ModePanel::create_buttons() {
	make_mode_button(LSBButton, "LSB", "0");
	make_mode_button(USBButton, "USB", "1");
	make_mode_button(DSBButton, "DSB", "2");
	make_mode_button(CWLButton, "CW_L", "3");
	make_mode_button(CWUButton, "CW_U", "4");
	make_mode_button(FMButton, "FM", "5");
	make_mode_button(AMButton, "AM", "6");
	make_mode_button(DIGUButton, "DIG_U", "7");
	make_mode_button(SPECButton, "SPEC", "8");
	make_mode_button(DIGLButton, "DIG_L", "9");
	make_mode_button(SAMButton, "SAM", "10");
	make_mode_button(DRMButton, "DRM", "11");
}

void ModePanel::layout_buttons_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;

	// Layout in 4 cols by 3 rows
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(1_fr),  Track(1_fr) };

	// Ad items to the grid
	grid.items = {
		GridItem(LSBButton),
		GridItem(USBButton),
		GridItem(DSBButton),
		GridItem(CWLButton),
		GridItem(CWUButton),
		GridItem(AMButton),
		GridItem(DIGUButton),
		GridItem(SPECButton),
		GridItem(DIGLButton),
		GridItem(SAMButton),
		GridItem(DRMButton)
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}