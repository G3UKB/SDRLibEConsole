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
// Constructor/Destructor
VFOComponent::VFOComponent(int p_vfo_type, int p_vfo_id)
{
	vfo_type = p_vfo_type;
	vfo_id = p_vfo_id;
	setSize(600, 400);
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

	g.setFont(Font(16.0f));
	g.setColour(Colours::white);
}

void VFOComponent::resized()
{
	// This is called when the MainComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
}

//==============================================================================
// Private
