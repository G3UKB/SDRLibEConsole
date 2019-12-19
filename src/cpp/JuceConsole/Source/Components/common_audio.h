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

#include "../Common/extern.h"
#include "../../JuceLibraryCode/JuceHeader.h"

//===================================================================================
// Listbox for audio selection
enum class AudioType {
	INPUT,
	OUTPUT
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
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;

	TableListBox *AudioModel::get_table();
private:
	//==============================================================================
	// State variables
	TableListBox *table;
	DeviceEnumList *audio_outputs;
	int num_rows;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModel)
};

class RxColumnCustomComponent : public Component
{
public:
	RxColumnCustomComponent(AudioModel& td) : owner(td)
	{
		// just put a combo box inside this component
		addAndMakeVisible(toggleButton);
	}

	void resized() override
	{
		toggleButton.setBoundsInset(BorderSize<int>(2));
	}

	// Our demo code will call this when we may need to update our contents
	//void setRowAndColumn(int newRow, int newColumn)
	//{
	//	row = newRow;
	//	columnId = newColumn;
	//	comboBox.setSelectedId(owner.getRating(row), dontSendNotification);
	//}

private:
	AudioModel& owner;
	ToggleButton toggleButton;
	int row, columnId;
};

class ToColumnCustomComponent : public Component
{
public:
	ToColumnCustomComponent(AudioModel& td) : owner(td)
	{
		// just put a combo box inside this component
		addAndMakeVisible(comboBox);
		comboBox.addItem("HPSDR", 1);
		comboBox.addItem("LOCAL", 2);
		comboBox.addItem("EXT-IQ", 3);
		comboBox.addItem("EXT-Aud", 4);
	}

	void resized() override
	{
		comboBox.setBoundsInset(BorderSize<int>(2));
	}

	// Our demo code will call this when we may need to update our contents
	//void setRowAndColumn(int newRow, int newColumn)
	//{
	//	row = newRow;
	//	columnId = newColumn;
	//	comboBox.setSelectedId(owner.getRating(row), dontSendNotification);
	//}

private:
	AudioModel& owner;
	ComboBox comboBox;
	int row, columnId;
};

class SetColumnCustomComponent : public Component
{
public:
	SetColumnCustomComponent(AudioModel& td) : owner(td)
	{
		// just put a combo box inside this component
		textButton.setButtonText("Set");
		addAndMakeVisible(textButton);
	}

	void resized() override
	{
		textButton.setBoundsInset(BorderSize<int>(2));
	}

	// Our demo code will call this when we may need to update our contents
	//void setRowAndColumn(int newRow, int newColumn)
	//{
	//	row = newRow;
	//	columnId = newColumn;
	//	comboBox.setSelectedId(owner.getRating(row), dontSendNotification);
	//}

private:
	AudioModel& owner;
	TextButton textButton;
	int row, columnId;
};

