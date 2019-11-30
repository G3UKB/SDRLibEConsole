/*
mode_panel.cpp

Mode buttons component for the Juce Console

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
#include "../RadioInterface/radio_interface.h"

//==============================================================================

ModeButton::ModeButton(String label) {

	setButtonText(label);
	setClickingTogglesState(true);
}

ModeButton::~ModeButton() {}

void ModeButton::clicked() {

	if (getToggleState()) {
		int id = getComponentID().getIntValue();
		RadioInterface::getInstance()->ri_server_set_rx_mode(0, id);
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

void ModePanel::resized() {
	// This is called when the mode panel is resized.
	layout_buttons_in_grid();
}

//==============================================================================
// Private
void ModePanel::create_buttons() {

	LSBButton = new ModeButton("LSB");
	LSBButton->setComponentID("0");
	LSBButton->setRadioGroupId(1);
	addAndMakeVisible(LSBButton);

	USBButton = new ModeButton("USB");
	USBButton->setComponentID("1");
	USBButton->setRadioGroupId(1);
	addAndMakeVisible(USBButton);

	DSBButton = new ModeButton("DSB");
	DSBButton->setComponentID("2");
	DSBButton->setRadioGroupId(1);
	addAndMakeVisible(DSBButton);

	CWLButton = new ModeButton("CW-L");
	CWLButton->setComponentID("3");
	CWLButton->setRadioGroupId(1);
	addAndMakeVisible(CWLButton);

	CWUButton = new ModeButton("CW-U");
	CWUButton->setComponentID("4");
	CWUButton->setRadioGroupId(1);
	addAndMakeVisible(CWUButton);

	FMButton = new ModeButton("FM");
	FMButton->setComponentID("5");
	FMButton->setRadioGroupId(1);
	addAndMakeVisible(FMButton);

	AMButton = new ModeButton("AM");
	AMButton->setComponentID("6");
	AMButton->setRadioGroupId(1);
	addAndMakeVisible(AMButton);

	DIGUButton = new ModeButton("DIG-U");
	DIGUButton->setComponentID("7");
	DIGUButton->setRadioGroupId(1);
	addAndMakeVisible(DIGUButton);

	SPECButton = new ModeButton("SPEC");
	SPECButton->setComponentID("8");
	SPECButton->setRadioGroupId(1);
	addAndMakeVisible(SPECButton);

	DIGLButton = new ModeButton("DIG-L");
	DIGLButton->setComponentID("9");
	DIGLButton->setRadioGroupId(1);
	addAndMakeVisible(DIGLButton);

	SAMButton = new ModeButton("DIG-L");
	SAMButton->setComponentID("10");
	SAMButton->setRadioGroupId(1);
	addAndMakeVisible(SAMButton);

	DRMButton = new ModeButton("DRM");
	DRMButton->setComponentID("11");
	DRMButton->setRadioGroupId(1);
	addAndMakeVisible(DRMButton);
}

void ModePanel::layout_buttons_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;

	// Layout in 4 cols by 3 rows
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(1_fr),  Track(1_fr) };

	// Add items to the grid
	grid.items = {
		GridItem(LSBButton),
		GridItem(USBButton),
		GridItem(DSBButton),
		GridItem(CWLButton),
		GridItem(CWUButton),
		GridItem(FMButton),
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