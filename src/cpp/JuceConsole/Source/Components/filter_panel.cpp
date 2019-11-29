/*
filter_panel.cpp

Filter buttons component for the Juce Console

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
#include "filter_panel.h"
#include "../Common/extern.h"

//==============================================================================

FilterButton::FilterButton(String label) {

	setButtonText(label);
	setClickingTogglesState(true);
}

FilterButton::~FilterButton() {}

void FilterButton::clicked() {
	int low;
	int high;

	if (getToggleState()) {
		int id = getComponentID().getIntValue();
		switch (id) {
			case 0: low = 100; high = 6100; break;
			case 1: low = 100; high = 4100; break;
			case 2: low = 300; high = 3000; break;
			case 3: low = 300; high = 2700; break;
			case 4: low = 300; high = 2400; break;
			case 5: low = 300; high = 1300; break;
			case 6: low = 500; high = 1000; break;
			case 7: low = 600; high = 850; break;
			case 8: low = 700; high = 800; break;
		}
		c_server_set_rx_filter_freq(0, low, high);
	}
}

//==============================================================================
// The panel
FilterPanel::FilterPanel(int p_filter_id) {

	// Local vars
	filter_id = p_filter_id;

	// Create all mode buttons
	create_buttons();
}

FilterPanel::~FilterPanel() {}

void FilterPanel::resized() {
	// This is called when the filter panel is resized.
	layout_buttons_in_grid();
}

//==============================================================================
// Private
void FilterPanel::create_buttons() {

	Filt6K0Button = new FilterButton("6.0 KHz");
	Filt6K0Button->setComponentID("0");
	Filt6K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt6K0Button);

	Filt4K0Button = new FilterButton("4.0 KHz");
	Filt4K0Button->setComponentID("1");
	Filt4K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt4K0Button);

	Filt2K7Button = new FilterButton("2.7 KHz");
	Filt2K7Button->setComponentID("2");
	Filt2K7Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K7Button);

	Filt2K4Button = new FilterButton("2.4KHz");
	Filt2K4Button->setComponentID("3");
	Filt2K4Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K4Button);

	Filt2K1Button = new FilterButton("2.1KHz");
	Filt2K1Button->setComponentID("4");
	Filt2K1Button->setRadioGroupId(1);
	addAndMakeVisible(Filt2K1Button);

	Filt1K0Button = new FilterButton("1.0KHz");
	Filt1K0Button->setComponentID("5");
	Filt1K0Button->setRadioGroupId(1);
	addAndMakeVisible(Filt1K0Button);

	Filt500Button = new FilterButton("500 Hz");
	Filt500Button->setComponentID("6");
	Filt500Button->setRadioGroupId(1);
	addAndMakeVisible(Filt500Button);

	Filt250Button = new FilterButton("250 Hz");
	Filt250Button->setComponentID("7");
	Filt250Button->setRadioGroupId(1);
	addAndMakeVisible(Filt250Button);

	Filt100Button = new FilterButton("100 Hz");
	Filt100Button->setComponentID("8");
	Filt100Button->setRadioGroupId(1);
	addAndMakeVisible(Filt100Button);
}

void FilterPanel::layout_buttons_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;

	// Layout in 3 cols by 3 rows
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(1_fr),  Track(1_fr),  Track(1_fr) };

	// Add items to the grid
	grid.items = {
		GridItem(Filt6K0Button),
		GridItem(Filt4K0Button),
		GridItem(Filt2K7Button),
		GridItem(Filt2K4Button),
		GridItem(Filt2K1Button),
		GridItem(Filt1K0Button),
		GridItem(Filt500Button),
		GridItem(Filt250Button),
		GridItem(Filt100Button)
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}