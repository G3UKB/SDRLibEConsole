/*
MainComponent.cpp

Main Component for the Juce Console

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

#include "MainComponent.h"
#include "Common/gui_cache.h"

//==============================================================================
MainComponent::MainComponent() {
}

MainComponent::~MainComponent() {
}

void MainComponent::start_ui()
{
	start_button = new StartButton();
	addAndMakeVisible(start_button);

	/*
	vfo_component = new VFOComponent("radio-1", RX);
	addAndMakeVisible(vfo_component);
	GUICache::getInstance()->setVFOInst(vfo_component);

	mode_panel = new ModePanel("radio-1");
	addAndMakeVisible(mode_panel);

	filter_panel = new FilterPanel("radio-1");
	addAndMakeVisible(filter_panel);

	display_panel = new DisplayPanel("radio-1");
	addAndMakeVisible(display_panel);
	*/
	setSize(300, 200);
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // Resize children
	start_button->setBounds(10, 10, 60, 40);
	//vfo_component->setBounds(10, 50, getWidth() - 20, 80);
	//mode_panel->setBounds(10,140, (getWidth()/2) + 20, 100);
	//filter_panel->setBounds((getWidth()/2) + 40, 140, (getWidth()/2) - 50, 100);
	//display_panel->setBounds(10, 245, getWidth() - 20, getHeight() - 255);
}
