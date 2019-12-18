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
AudioModel::AudioModel(AudioType p_type) {
	
	table = new TableListBox();
	table->setModel(this);
	
	table->getHeader().addColumn("Device", 1, 250, 250, 250, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Rx", 2, 80, 80, 80, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("To", 3, 80, 80, 80, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Ch", 4, 80, 80, 80, TableHeaderComponent::defaultFlags);
	table->getHeader().addColumn("Host", 5, 150, 150, 150, TableHeaderComponent::defaultFlags);

	// Get the output enumeration
	audio_outputs = c_server_enum_audio_outputs();
	num_rows = audio_outputs->entries;
}

int AudioModel::getNumRows() {
	return num_rows;
}

void AudioModel::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) {
	g.fillAll(Colours::darkgrey);
}

void AudioModel::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
	switch (columnId) {
	case 1 :
		g.drawText(audio_outputs->devices[rowNumber].name, 2, 0, width - 4, height, Justification::centredLeft, true);
		break;
	case 5:
		g.drawText(audio_outputs->devices[rowNumber].host_api, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
}

Component* AudioModel::refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) {
	switch (columnId) {
	case 2 : 
		return new RxColumnCustomComponent(*this);
		break;
	case 3:
		return new ToColumnCustomComponent(*this);
		break;
	case 4:
		return new ChColumnCustomComponent(*this);
		break;
	}
	return nullptr;
}

TableListBox *AudioModel::get_table() {
	return table;
}
