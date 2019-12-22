/*
common_controls.cpp

All common controls for the Juce Console, i.e. not radio dependent

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

#include "common_controls.h"
#include "../Common/extern.h"
#include "../RadioInterface/radio_interface.h"
#include "../RadioWindows/radio_window.h"
#include "../RadioWindows/radio_win_cache.h"
#include "common_audio.h"

//==============================================================================
// Radio control buttons
RadioButton::RadioButton(BUTTON_TYPE p_type, String p_label, String p_label_down, Colour p_normal, Colour p_down) {

	type = p_type;
	label = p_label;
	label_down = p_label_down;

	setButtonText(p_label);
	if (type == BUTTON_TYPE::START_STOP) {
		setClickingTogglesState(true);
		setColour(textColourOffId, p_normal);
		setColour(textColourOnId, p_down);
		if (RadioInterface::getInstance()->is_radio_discovered())
			setEnabled(true);
		else
			setEnabled(false);
	} else if (type == BUTTON_TYPE::DISCOVER) {
		setColour(textColourOffId, p_normal);
		setColour(textColourOnId, p_normal);
		if (RadioInterface::getInstance()->is_radio_discovered())
			setEnabled(false);
		else
			setEnabled(true);
	}
}

RadioButton::~RadioButton() {}

void RadioButton::clicked() {

	if (type == BUTTON_TYPE::START_STOP) {
		if (getToggleState()) {
			setButtonText(label_down);
			if (!RadioInterface::getInstance()->ri_radio_start(0)) return;
		} else {
			// Switching off
			// Stop radio
			setButtonText(label);
			RadioInterface::getInstance()->ri_radio_stop();
		}
	} else if (type == BUTTON_TYPE::DISCOVER) {
		if (!RadioInterface::getInstance()->ri_radio_discover()) {
			// Radio hardware not running or comms problem
			std::cout << "Failed to discover radio, please check hardware and try again." << std::endl;
		}
		else {
			if (!RadioInterface::getInstance()->ri_server_start()) {
				// Something also kinda wrong here, cant continue
				std::cout << std::endl << "Failed to start server processes, unable to continue!" << std::endl;
				JUCEApplicationBase::quit();
			} else {
				// Start radio 1 instance
				setEnabled(false);
				RadioPanel *p = (RadioPanel *)getParentComponent();
				p->get_start_button()->setEnabled(true);
				std::cout << "Starting radio 1" << std::endl;
				RadioWindow * w1 = RadioWinCache::getInstance()->create_window("radio-1");
				w1->get_component()->start_ui();
			}
		}
	}
}

//==============================================================================
// App control buttons
ExitButton::ExitButton(String label) {

	setButtonText(label);
}

void ExitButton::clicked() {
	JUCEApplicationBase::quit();
}

//==============================================================================
// Radio select buttons
// Frame
SelectFrame::SelectFrame(String label) {

	// Create buttons
	setText(label);
	

	select_button_r1 = new SelectButton("radio-1", "1");
	select_button_r2 = new SelectButton("radio-2", "2");
	select_button_r3 = new SelectButton("radio-3", "3");
	addAndMakeVisible(select_button_r1);
	addAndMakeVisible(select_button_r2);
	addAndMakeVisible(select_button_r3);
}

void SelectFrame::resized() {
	
	// Local grid as its just a bag of behaviour
	Grid grid;

	// Layout in 1 row by 3 cols
	using Track = Grid::TrackInfo;
	grid.templateColumns = { Track(1_fr), Track(1_fr), Track(1_fr) };
	grid.templateRows = { Track(15_px) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::row;
	grid.justifyItems = Grid::JustifyItems::center;
	grid.alignItems = Grid::AlignItems::center;
	grid.justifyContent = Grid::JustifyContent::center;
	grid.alignContent = Grid::AlignContent::center;

	// Add items to the grid
	grid.items.addArray({
		GridItem(select_button_r1),
		GridItem(select_button_r2),
		GridItem(select_button_r3),
		});

	grid.performLayout(Rectangle<int>(10, 15, getWidth() - 20, getHeight() - 20));
}

//==============================================================================
// Apply buttons
ApplyButton::ApplyButton(String label) {

	setButtonText(label);
}

void ApplyButton::clicked() {
	

}
//==============================================================================
// Select buttons
SelectButton::SelectButton(String p_radio_id, String label) {

	// Make it a toggle in the select group and set its radio id
	radio_id = p_radio_id;
	setButtonText(label);
	setClickingTogglesState(true);
	setRadioGroupId(1);
	setComponentID(p_radio_id);
	// ToDo: should persist state
	if (p_radio_id == "radio-1")
		setToggleState(true, NotificationType::dontSendNotification);
}

void SelectButton::clicked() {
	
	if (getToggleState()) {
		// Button toggle on
		// Get number of receivers
		int num_rx;
		if (getComponentID() == "radio-1") num_rx = 1;
		else if (getComponentID() == "radio-2") num_rx = 2;
		else num_rx = 3;

		// Stop radio and reset interface
		RadioInterface::getInstance()->ri_radio_stop();
		RadioInterface::getInstance()->reset();

		// Turn off start button
		RadioPanel *p = (RadioPanel *)(getParentComponent()->getParentComponent());
		p->get_start_button()->setToggleState(false, NotificationType::dontSendNotification);
		p->get_start_button()->setButtonText("Start");

		// Stop any radio instances
		RadioWinCache::getInstance()->delete_window("radio-1");
		RadioWinCache::getInstance()->delete_window("radio-2");
		RadioWinCache::getInstance()->delete_window("radio-3");

		// Terminate server as we can't patch this in to a running server
		if (!c_server_terminate()) {
			printf("Failed to terminate server, unable to continue!");
			JUCEApplicationBase::quit();
		}
		// Wait a bit to ensure terminated
		Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
		
		// Now start up from scratch
		// Initialise server
		if (!c_server_init()) {
			printf("Failed to initialise server, unable to continue!");
			JUCEApplicationBase::quit();
		}
		// Set new number of receivers
		c_server_set_num_rx(num_rx);

		if (!RadioInterface::getInstance()->ri_set_default_audio()) {
			// Something is kinda wrong here, can't continue
			std::cout << std::endl << "Failed to configure audio, unable to continue!" << std::endl;
			JUCEApplicationBase::quit();
		}
		
		if (!RadioInterface::getInstance()->ri_radio_discover()) {
			// Radio hardware not running or comms problem
			std::cout << "Failed to discover radio, please check hardware and try again." << std::endl;
		}
		else {
			if (!RadioInterface::getInstance()->ri_server_start()) {
				// Something also kinda wrong here, cant continue
				std::cout << std::endl << "Failed to start server processes, unable to continue!" << std::endl;
				JUCEApplicationBase::quit();
			}
			else {
				// Start radio instances
				std::cout << "Starting radios" << std::endl;
				RadioWindow * w1 = RadioWinCache::getInstance()->create_window("radio-1");
				w1->get_component()->start_ui();
				if (num_rx > 1) {
					RadioWindow * w2 = RadioWinCache::getInstance()->create_window("radio-2");
					w2->get_component()->start_ui();
				}
				if (num_rx > 2) {
					RadioWindow * w3 = RadioWinCache::getInstance()->create_window("radio-3");
					w3->get_component()->start_ui();
				}
			}
		}
	}		
}

//==============================================================================
// The Panel
RadioPanel::RadioPanel() {

	// Local vars

	// Create all radio buttons
	create_components();
}

RadioPanel::~RadioPanel() {}

void RadioPanel::resized() {
	// This is called when the radio panel is resized.
	layout_components_in_grid();
}

RadioButton *RadioPanel::get_start_button() {
	return StartButton;
}

void RadioPanel::paint(Graphics& g)
{
	// The component is opaque, so we must completely fill the background with a solid colour
	g.fillAll(Colours::darkgrey);
	// Draw horizontal lines
	g.setColour(Colours::grey);
	g.fillRect(0, 45, getWidth(), 2);
}

//==============================================================================
// Private
void RadioPanel::create_components() {

	// Radio control buttons
	StartButton = new RadioButton(BUTTON_TYPE::START_STOP, "Start", "Stop", Colours::green, Colours::red);
	addAndMakeVisible(StartButton);
	DiscoverButton = new RadioButton(BUTTON_TYPE::DISCOVER, "Discover", "", Colours::orange, Colours::orange);
	addAndMakeVisible(DiscoverButton);
	// Exit app button
	exit_button = new ExitButton("Exit");
	addAndMakeVisible(exit_button);
	// Select radios buttons
	select_frame = new SelectFrame("Radios");
	addAndMakeVisible(select_frame);
	// Audio selection
	audio_table = new AudioModel(AudioType::OUTPUT);
	addAndMakeVisible(audio_table->get_table());
	// Apply button
	apply_button = new ApplyButton("Apply");
	addAndMakeVisible(apply_button);
}

void RadioPanel::layout_components_in_grid() {

	// Local grid as its just a bag of behaviour
	Grid grid;

	// Layout in 1 row by 4 cols so that exit stays right and other stay left
	using Track = Grid::TrackInfo;
	grid.templateColumns = { Track(80_px), Track(80_px), Track(20_px), Track(1_fr), Track(20_px), Track(80_px) };
	grid.templateRows = { Track(35_px),  Track(20_px), Track(1_fr), Track(25_px) };
	grid.autoColumns = Track(1_fr);
	grid.autoRows = Track(1_fr);
	grid.autoFlow = Grid::AutoFlow::row;
	grid.justifyItems = Grid::JustifyItems::center;

	// Add items to the grid
	grid.items.addArray({
		GridItem(StartButton).withJustifySelf(GridItem::JustifySelf::start),
		GridItem(DiscoverButton).withJustifySelf(GridItem::JustifySelf::start),
		GridItem(exit_button).withArea(1,6),
		GridItem(select_frame).withArea(1,4),
		GridItem(audio_table->get_table()).withArea(3,1,3,7),
		GridItem(apply_button).withArea(4,6)
	});

	grid.performLayout(getLocalBounds());
}