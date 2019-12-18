/*
common_audio.h

Header for common_controls audio component for the Juce Console

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

//===================================================================================
// Listbox for audio selection
enum class AudioType {
	INPUT,
	OUTPUT
};

class AudioTable : public TableListBox
{
public:
	//==============================================================================
	AudioTable(AudioType p_type);
	~AudioTable() {}

	//==============================================================================


private:
	//==============================================================================
	// State variables

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTable)
};

class AudioModel : public Component, public TableListBoxModel
{
public:
	//==============================================================================
	AudioModel(AudioType p_type);
	~AudioModel() {}

	//==============================================================================
	int getNumRows() override;
	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

	TableListBox *AudioModel::get_table();
private:
	//==============================================================================
	// State variables
	TableListBox *table;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModel)
};
