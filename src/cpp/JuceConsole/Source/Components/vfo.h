/*
vfo.h

Header for VFO component for the Juce Console

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
// Defines
#define MHZ_FONT 30
#define KHZ_FONT 30
#define HZ_FONT 20
#define MHZ_COLOR Colours::white
#define KHZ_COLOR Colours::white
#define HZ_COLOR Colours::orange
#define RX 0
#define TX 1

//==============================================================================
/*
	Multi-instance VFO component
*/
class VFOComponent : public Component
{
public:
	//==============================================================================
	VFOComponent(int p_vfo_type, int p_vfo_id);
	~VFOComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	//==============================================================================
	// State variables
	int vfo_type = RX;		// RX|TX
	int vfo_id = 0;			// Numerical id of VFO instance
	int freq_int = 0;		// Last frequence increment in MHz
	int current_freq = 0;	// Current frequency in MHz

	// Map for lookup of increments
	std::map<String, float> freq_inc_map {
		{String("100MHz"), 100.0f},
		{String("10MHz"), 10.0f},
		{String("1MHz"), 1.0f},
		{String("100KHz"), 0.1f},
		{String("10KHz"), 0.01f},
		{String("1KH"), 0.001f},
		{String("100Hz"), 0.0001f},
		{String("10Hz"), 0.00001f},
		{String("1H"), 0.000001f},
	};

	//==============================================================================
	// Method prototypes
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VFOComponent)
};
