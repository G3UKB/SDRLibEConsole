/*
common_controls.h

Header for common_controls component for the Juce Console

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
#include "common_audio.h"

//===================================================================================
// A start/stop button component
enum class BUTTON_TYPE {
	START_STOP,
	DISCOVER
};

class RadioButton : public TextButton
{
public:
	//==============================================================================
	RadioButton(BUTTON_TYPE type, String label, String label_down, Colour normal, Colour down);
	~RadioButton();

	//==============================================================================
	void clicked();

private:
	//==============================================================================
	// State variables
	BUTTON_TYPE type;
	String label;
	String label_down;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioButton)
};

//===================================================================================
// Terminate application button
class ExitButton : public TextButton
{
public:
	//==============================================================================
	ExitButton(String label);
	~ExitButton() {}

	//==============================================================================
	void clicked();

private:
	//==============================================================================
	// State variables

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExitButton)
};

//===================================================================================
// Select number of radios button
class SelectButton : public TextButton
{
public:
	//==============================================================================
	SelectButton(String p_radio_id, String label);
	~SelectButton() {}

	//==============================================================================
	void clicked();

private:
	//==============================================================================
	// State variables
	String radio_id;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectButton)
};

//===================================================================================
// The frame to assemble everything in
class SelectFrame : public GroupComponent
{
public:
	//==============================================================================
	SelectFrame(String label);
	~SelectFrame() {}

	//==============================================================================
	void resized() override;

private:
	//==============================================================================
	// State variables
	SelectButton *select_button_r1;
	SelectButton *select_button_r2;
	SelectButton *select_button_r3;

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectFrame)
};

//===================================================================================
// Terminate application button
class ApplyButton : public TextButton
{
public:
	//==============================================================================
	ApplyButton(String label);
	~ApplyButton() {}

	//==============================================================================
	void clicked();

private:
	//==============================================================================
	// State variables

	//==============================================================================
	// Method prototypes

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ApplyButton)
};

/*
	A radio panel component
*/
class RadioPanel : public Component
{
public:
	//==============================================================================
	RadioPanel();
	~RadioPanel();

	void resized() override;
	void paint(Graphics& g) override;

	RadioButton *get_start_button();

	//==============================================================================

private:
	//==============================================================================
	// State variables

	// Button vars
	RadioButton *StartButton;
	RadioButton *DiscoverButton;
	ExitButton *exit_button;
	GroupComponent *select_frame;
	AudioModel *audio_table;
	ApplyButton *apply_button;

	//==============================================================================
	// Method prototypes
	void create_components();
	void layout_components_in_grid();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioPanel)
};
