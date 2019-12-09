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

ModeButton::ModeButton(int p_radio_id, String label) {

	setButtonText(label);
	setClickingTogglesState(true);
	radio_id = p_radio_id;
}

ModeButton::~ModeButton() {}

void ModeButton::clicked() {

	if (getToggleState()) {
		int mode_id = getComponentID().getIntValue();
		RadioInterface::getInstance()->ri_server_set_rx_mode(radio_id, mode_id);
	}
}

//==============================================================================
// The panel
ModePanel::ModePanel(String p_radio_id) {

	// Local vars
	radio_id = p_radio_id;

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

	int radio;

	if (radio_id == "radio-1") radio = 0;
	else if (radio_id == "radio-2") radio = 1;
	else radio = 2;

	LSBButton = new ModeButton(radio, "LSB");
	LSBButton->setComponentID("0");
	LSBButton->setRadioGroupId(1);
	addAndMakeVisible(LSBButton);

	USBButton = new ModeButton(radio, "USB");
	USBButton->setComponentID("1");
	USBButton->setRadioGroupId(1);
	addAndMakeVisible(USBButton);

	DSBButton = new ModeButton(radio, "DSB");
	DSBButton->setComponentID("2");
	DSBButton->setRadioGroupId(1);
	addAndMakeVisible(DSBButton);

	CWLButton = new ModeButton(radio, "CW-L");
	CWLButton->setComponentID("3");
	CWLButton->setRadioGroupId(1);
	addAndMakeVisible(CWLButton);

	CWUButton = new ModeButton(radio, "CW-U");
	CWUButton->setComponentID("4");
	CWUButton->setRadioGroupId(1);
	addAndMakeVisible(CWUButton);

	FMButton = new ModeButton(radio, "FM");
	FMButton->setComponentID("5");
	FMButton->setRadioGroupId(1);
	addAndMakeVisible(FMButton);

	AMButton = new ModeButton(radio, "AM");
	AMButton->setComponentID("6");
	AMButton->setRadioGroupId(1);
	addAndMakeVisible(AMButton);

	DIGUButton = new ModeButton(radio, "DIG-U");
	DIGUButton->setComponentID("7");
	DIGUButton->setRadioGroupId(1);
	addAndMakeVisible(DIGUButton);

	SPECButton = new ModeButton(radio, "SPEC");
	SPECButton->setComponentID("8");
	SPECButton->setRadioGroupId(1);
	addAndMakeVisible(SPECButton);

	DIGLButton = new ModeButton(radio, "DIG-L");
	DIGLButton->setComponentID("9");
	DIGLButton->setRadioGroupId(1);
	addAndMakeVisible(DIGLButton);

	SAMButton = new ModeButton(radio, "DIG-L");
	SAMButton->setComponentID("10");
	SAMButton->setRadioGroupId(1);
	addAndMakeVisible(SAMButton);

	DRMButton = new ModeButton(radio, "DRM");
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