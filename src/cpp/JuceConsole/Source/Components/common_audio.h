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
// Model class for Audio selection
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
	// Overrides
	int getNumRows() override;
	void paintRowBackground(Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
	void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	Component* refreshComponentForCell(int rowNumber, int columnId, bool /*isRowSelected*/, Component* existingComponentToUpdate) override;
	// Our table component
	TableListBox *AudioModel::get_table();
	int getRows() {
		return num_rows;
	}

	// Methods to get/set attributes per row for all fields as these cannot be held
	// by the custom components themselves as they are reused when scrolling. This is
	// an efficiency measure as there could be thousands of rows.
	bool get_active(int row);
	void set_active(int row, bool state);
	int get_dest(int row);
	void set_dest(int row, int index);
	bool get_rx_1(int row);
	void set_rx_1(int row, bool state);
	bool get_rx_2(int row);
	void set_rx_2(int row, bool state);
	bool get_rx_3(int row);
	void set_rx_3(int row, bool state);

private:
	//==============================================================================
	// State variables
	TableListBox *table;
	DeviceEnumList *audio_outputs;
	int num_rows = 0;

	// Structure to hold all audio info and current state
	typedef struct DeviceState {
		String name;
		String host_api;
		bool rx_1;
		bool rx_2;
		bool rx_3;
		int dest;
		bool active;
	}DeviceState;

	typedef struct DeviceStateList {
		int entries;
		DeviceState devices[50];
	}DeviceStateList;

	DeviceStateList *dsl;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioModel)
};

//===================================================================================
// RX custom component
class RxColumnCustomComponent : public Component,
								public Button::Listener
{
public:
	RxColumnCustomComponent(AudioModel& td, int p_id) : owner(td)
	{
		// just put a combo box inside this component
		addAndMakeVisible(toggleButton);
		//toggleButton.onClick = std::function<Button>(doToggled());
		rx_id = p_id;
		toggleButton.addListener(this);
	}

	void resized() override
	{
		toggleButton.setBoundsInset(BorderSize<int>(2));
	}

	// Called when a button state changes to clicked
	void buttonClicked(Button *button) override {
		if (row != -1) {
			if (rx_id == 1) owner.set_rx_1(row, toggleButton.getToggleState());
			else if (rx_id == 2) owner.set_rx_2(row, toggleButton.getToggleState());
			else owner.set_rx_3(row, toggleButton.getToggleState());
		}
	}

	// We set the current row and column on every invocation
	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		// Here we need to set the current state for the component
		// for the current row and column. Note that if we scroll
		// the table components are reused. There is not one component
		// for every row in the table unless the whole table is visible.
		if (rx_id == 1) toggleButton.setToggleState(owner.get_rx_1(row), NotificationType::dontSendNotification);
		else if (rx_id == 2) toggleButton.setToggleState(owner.get_rx_2(row), NotificationType::dontSendNotification);
		else toggleButton.setToggleState(owner.get_rx_3(row), NotificationType::dontSendNotification);
	}

private:
	AudioModel& owner;
	ToggleButton toggleButton;
	int row = -1;
	int columnId = -1;
	int rx_id;
};

//===================================================================================
// Destination custom component
class ToColumnCustomComponent : public Component,
								public ComboBox::Listener
{
public:
	ToColumnCustomComponent(AudioModel& td) : owner(td)
	{
		// just put a combo box inside this component
		addAndMakeVisible(comboBox);
		comboBox.addItem("NONE", 1);
		comboBox.addItem("HPSDR", 2);
		comboBox.addItem("LOCAL", 3);
		comboBox.addItem("EXT-IQ", 4);
		comboBox.addItem("EXT-Aud", 5);

		comboBox.addListener(this);
	}

	void resized() override
	{
		comboBox.setBoundsInset(BorderSize<int>(2));
	}

	// We set the current row and column on every invocation
	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		// Here we need to set the current state for the component
		// for the current row and column. Note that if we scroll
		// the table components are reused. There is not one component
		// for every row in the table unless the whole table is visible.
		comboBox.setSelectedItemIndex(owner.get_dest(row), NotificationType::dontSendNotification);	
	}

	// Called when the selected item is changed
	void comboBoxChanged(ComboBox* cb) override {
		//printf("%s\n", comboBox.getItemText(comboBox.getSelectedItemIndex()));
		owner.set_dest(row, comboBox.getSelectedItemIndex());
	}

private:
	AudioModel& owner;
	ComboBox comboBox;
	int row, columnId;
};

//===================================================================================
// Action custom component
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

	// We set the current row and column on every invocation
	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		// As this is an action button we need to state that the
		// row is active, i.e the audio routing is active. Most
		// probably set the row to a different colour.
	}

private:
	AudioModel& owner;
	TextButton textButton;
	int row, columnId;
};

//===================================================================================
// Action custom component
class RemColumnCustomComponent : public Component
{
public:
	RemColumnCustomComponent(AudioModel& td) : owner(td)
	{
		// just put a button inside this component
		textButton.setButtonText("Rem");
		addAndMakeVisible(textButton);
	}

	void resized() override
	{
		textButton.setBoundsInset(BorderSize<int>(2));
	}

	// We set the current row and column on every invocation
	void setRowAndColumn(int newRow, int newColumn)
	{
		row = newRow;
		columnId = newColumn;
		// As this is an action button we need to state that the
		// row is active, i.e the audio routing is active. Most
		// probably set the row to a different colour.
	}

private:
	AudioModel& owner;
	TextButton textButton;
	int row, columnId;
};
