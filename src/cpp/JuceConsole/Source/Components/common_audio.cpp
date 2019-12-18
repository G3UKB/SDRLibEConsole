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

#pragma once

#include "../Common/extern.h"
#include "../RadioInterface/radio_interface.h"
#include "common_audio.h"

//==============================================================================
// Audio Model
AudioModel::AudioModel(AudioType p_type) {
	
	table = new TableListBox();
	table->setModel(this);
	
	table->getHeader().addColumn(
		"Device",
		1,
		100,
		50, 400,
		TableHeaderComponent::defaultFlags);
}

int AudioModel::getNumRows() {
	return 0;
}

void AudioModel::paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) {

}

void AudioModel::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {

}

TableListBox *AudioModel::get_table() {
	return table;
}

//==============================================================================
// Audio Table
//AudioTable::AudioTable(AudioType p_type) {
//	TableListBox::TableListBox("Table");


//}