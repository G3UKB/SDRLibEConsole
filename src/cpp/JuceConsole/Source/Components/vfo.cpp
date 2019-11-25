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

	// Add digits to the grid
	grid = new Grid();
	add_digits(grid);
	grid->performLayout(getLocalBounds());
}

VFOComponent::~VFOComponent()
{
}

//==============================================================================
// GUI Events
void VFOComponent::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::darkgrey);
}

void VFOComponent::resized()
{
	// This is called when the VFOComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
}

//==============================================================================
// Private
void VFOComponent::add_digits(Grid *thegrid) {

	// Create digits
	d_100MHz = new VFODigit( String("0"), MHZ_COLOR, MHZ_FONT );
	d_10MHz = new VFODigit(String("0"), MHZ_COLOR, MHZ_FONT);
	d_1MHz = new VFODigit(String("0"), MHZ_COLOR, MHZ_FONT);
	d_100KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_10KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_1KHz = new VFODigit(String("0"), KHZ_COLOR, KHZ_FONT);
	d_100Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);
	d_10Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);
	d_1Hz = new VFODigit(String("0"), HZ_COLOR, HZ_FONT);

	// Add digits in one horizontal row
	using Track = Grid::TrackInfo;
	thegrid->templateRows = { Track(1_fr) };
	thegrid->templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr), Track(1_fr) };
	
	thegrid->items = { GridItem(d_100MHz), GridItem(d_10MHz), GridItem(d_1MHz), GridItem(d_100KHz), GridItem(d_10KHz), GridItem(d_1KHz), GridItem(d_100Hz), GridItem(d_10Hz), GridItem(d_1Hz) };
}

//==============================================================================
// A VFO Digit
//==============================================================================

VFODigit::VFODigit(String text, Colour colour, float size) {
	// Create a digit ready to add to the grid
	setText(text, dontSendNotification);
	setJustificationType(Justification::centred);
	setColour(Label::textColourId, colour);
	setFont(Font(size, Font::bold));
};

VFODigit::~VFODigit() {

}

void VFODigit::paint(Graphics& g) {
	g.fillAll(Colours::blue);
};

void VFODigit::resized() {

};
