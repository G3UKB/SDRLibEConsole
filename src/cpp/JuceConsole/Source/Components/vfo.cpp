/*
vfo.cpp

VFO component for the Juce Console

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

#include "vfo.h"

//==============================================================================
// Main VFO Component Panel
//==============================================================================

//==============================================================================
// Constructor/Destructor
VFOComponent::VFOComponent(int p_vfo_type, int p_vfo_id, int x, int y, int w, int h)
{
	// Local vars
	vfo_type = p_vfo_type;
	vfo_id = p_vfo_id;

	// Bounds given by caller to position within callers container
	setBounds(x, y, w, h);

	// WE make content here but arrange it in resized
	create_digits();
}

VFOComponent::~VFOComponent()
{
}

//==============================================================================
// GUI Events
void VFOComponent::paint(Graphics& g)
{
	// Our component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
}

void VFOComponent::resized()
{
	// This is called when the VFOComponent is resized.
	layout_digits_in_grid();
}

//==============================================================================
// Private
void VFOComponent::create_digits() {

	// Create digits
	d_100MHz = new VFODigit(String("0"), MHZ_COLOR, MHZ_FONT);
	d_100MHz->setComponentID("0");
	addAndMakeVisible(d_100MHz);
	d_10MHz = new VFODigit(String("0"), MHZ_COLOR, MHZ_FONT);
	d_10MHz->setComponentID("1");
	addAndMakeVisible(d_10MHz);
	d_1MHz = new VFODigit(String("0"), MHZ_COLOR, MHZ_FONT);
	d_1MHz->setComponentID("2");
	addAndMakeVisible(d_1MHz);
	d_100KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_100KHz->setComponentID("3");
	addAndMakeVisible(d_100KHz);
	d_10KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_10KHz->setComponentID("4");
	addAndMakeVisible(d_10KHz);
	d_1KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_1KHz->setComponentID("5");
	addAndMakeVisible(d_1KHz);
	d_100Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);
	d_100Hz->setComponentID("6");
	addAndMakeVisible(d_100Hz);
	d_10Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);
	d_10Hz->setComponentID("7");
	addAndMakeVisible(d_10Hz);
	d_1Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);
	d_1Hz->setComponentID("8");
	addAndMakeVisible(d_1Hz);
}

void VFOComponent::layout_digits_in_grid() {

	Grid grid;

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;
	
	grid.templateRows = { Track(1_fr) };
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	
	grid.items = {
		GridItem(d_100MHz),
		GridItem(d_10MHz), 
		GridItem(d_1MHz), 
		GridItem(d_100KHz), 
		GridItem(d_10KHz), 
		GridItem(d_1KHz), 
		GridItem(d_100Hz), 
		GridItem(d_10Hz), 
		GridItem(d_1Hz) 
	};

	grid.justifyItems = Grid::JustifyItems::stretch;
	// Add gaps if required
	//grid.rowGap = Grid::Px::Px(10.0f);
	//grid.columnGap = Grid::Px::Px(10.0f);
	grid.performLayout(getLocalBounds());
}

//==============================================================================
// A VFO Digit
//==============================================================================

VFODigit::VFODigit(String text, Colour colour, float size) {
	
	Label::Label();
	// Create a digit ready to add to the grid
	setText(text, dontSendNotification);
	setJustificationType(Justification::centred);
	setColour(Label::textColourId, colour);
	setFont(Font(size, Font::plain));
};

VFODigit::~VFODigit() {

}

void VFODigit::paint(Graphics& g) {
	// Add any additional painting here
	Label::paint(g);
};

void VFODigit::resized() {
	
};

void VFODigit::mouseEnter(const MouseEvent& event) {
	String id = event.eventComponent->getComponentID();
	
	// Grow font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h + 5, Font::plain));
}

void VFODigit::mouseExit(const MouseEvent& event) {
	String id = event.eventComponent->getComponentID();

	// Shrink font by 5 points
	Font f = getFont();
	int h = f.getHeight();
	setFont(Font(h - 5, Font::plain));
}

void VFODigit::mouseWheelMove(const MouseEvent& event, const MouseWheelDetails& wheel) {
	printf("Wheel\n");
}