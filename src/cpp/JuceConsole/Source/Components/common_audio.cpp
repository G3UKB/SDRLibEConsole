/*
common_audio.cpp

All common controls audio component for the Juce Console

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

#include "../RadioInterface/radio_interface.h"
#include "../Common/extern.h"
#include "common_audio.h"

//==============================================================================
// Audio Model
// Constructor
AudioModel::AudioModel(AudioType p_type) {
	
	table = new TableListBox();
	table->setModel(this);
	
	table->getHeader().addColumn("Device", 1, 150, 100, 200, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-1", 2, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-2", 3, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx-3", 4, 30, 30, 30, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("To", 5, 80, 80, 80, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Host", 6, 100, 100, 150, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Set", 7, 50, 50, 50, TableHeaderComponent::defaultFlags);

	// Get the output enumeration
	audio_outputs = c_server_enum_audio_outputs();
	// We now copy this into a state structure to drive our table
	dsl = new DeviceStateList;
	dsl->entries = audio_outputs->entries;
	for (int i = 0; i < audio_outputs->entries; i++) {
		dsl->devices[i].name = audio_outputs->devices[i].name;
		dsl->devices[i].host_api = audio_outputs->devices[i].host_api;
		dsl->devices[i].active = false;
		dsl->devices[i].dest = "NONE";
		dsl->devices[i].rx_1 = false;
		dsl->devices[i].rx_2 = false;
		dsl->devices[i].rx_3 = false;
	}

	num_rows = dsl->entries;
}

//=====================================================================================================
// Overrides
int AudioModel::getNumRows() {
	return num_rows;
}

void AudioModel::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) {
	g.fillAll(Colours::darkgrey);
}

// Manage fixed text
void AudioModel::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	g.setFont(12);
	switch (columnId) {
	case 1 :
		g.drawText(dsl->devices[rowNumber].name, 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	case 6:
		g.drawText(dsl->devices[rowNumber].host_api, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
}

// Manage custom components
Component* AudioModel::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) {

	// Not custom components
	if (columnId == 1 || columnId == 6) {
		jassert(existingComponentToUpdate == nullptr);
		return nullptr;
	}

	// Destination component
	if (columnId == 5) {
		auto* to_col = static_cast<ToColumnCustomComponent*> (existingComponentToUpdate);
		if (to_col == nullptr)
			return new ToColumnCustomComponent(*this);
		else
			return to_col;
		to_col->setRowAndColumn(rowNumber, columnId);
	}

	// Set button
	if (columnId == 7) {
		auto* set_col = static_cast<SetColumnCustomComponent*> (existingComponentToUpdate);
		if (set_col == nullptr)
			return new SetColumnCustomComponent(*this);
		else
			return set_col;
		set_col->setRowAndColumn(rowNumber, columnId);
	}

	// Remainder are RX components
	int id;
	if (columnId == 2) id = 1;
	else if (columnId == 3) id = 2;
	else id = 3;
	auto* rx_col = static_cast<RxColumnCustomComponent*> (existingComponentToUpdate);
	if (rx_col == nullptr)
		return new RxColumnCustomComponent(*this, id);
	else
		return rx_col;
	rx_col->setRowAndColumn(rowNumber, columnId);
}

//=====================================================================================================
// Return our table
TableListBox *AudioModel::get_table() {
	return table;
}

//=====================================================================================================
// Get/set methods

bool AudioModel::get_active(int row) {
	return dsl->devices[row].active;
}
void AudioModel::set_active(int row, bool state) {
	dsl->devices[row].active = state;
}

String AudioModel::get_dest(int row) {
	return dsl->devices[row].dest;
}
void AudioModel::set_dest(int row, String value) {
	dsl->devices[row].dest = value;
}

bool AudioModel::get_rx_1(int row) {
	return dsl->devices[row].rx_1;
}
void AudioModel::set_rx_1(int row, bool state) {
	dsl->devices[row].rx_1 = state;
}

bool AudioModel::get_rx_2(int row) {
	return dsl->devices[row].rx_2;
}
void AudioModel::set_rx_2(int row, bool state) {
	dsl->devices[row].rx_2 = state;
}

bool AudioModel::get_rx_3(int row) {
	return dsl->devices[row].rx_3;
}
void AudioModel::set_rx_3(int row, bool state) {
	dsl->devices[row].rx_3 = state;
}
