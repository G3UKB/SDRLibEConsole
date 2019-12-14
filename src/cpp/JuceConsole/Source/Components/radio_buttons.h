/*
radio_buttons.h

Header for radio_buttons component for the Juce Console

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

//==============================================================================

enum class BUTTON_TYPE {
	START_STOP,
	DISCOVER
};

/*
	A start/stop button component
*/
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

	RadioButton *get_start_button();

	//==============================================================================

private:
	//==============================================================================
	// State variables

	// Button vars
	RadioButton *StartButton;
	RadioButton *DiscoverButton;

	//==============================================================================
	// Method prototypes
	void create_buttons();
	void layout_buttons_in_grid();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RadioPanel)
};
