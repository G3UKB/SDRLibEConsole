/*
radio_buttons.cpp

Start/Stop buttons component for the Juce Console

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
#include "radio_buttons.h"
#include "../Common/extern.h"

//==============================================================================

StartButton::StartButton() {

	setButtonText("Start");
	setClickingTogglesState(true);
}

StartButton::~StartButton() {}

void StartButton::clicked() {

	int direction;
	char* host_api;
	char* dev;

	if (getToggleState()) {
		// Switching everything on
		setButtonText("Stop");
		if (!audio_set) {
			audio_set = true;
			// Set up a default route for RX1 to Speaker output
			DeviceEnumList* l = c_server_enum_audio_outputs();
			for (int i = 0; i < l->entries; i++) {
				if (String(l->devices[i].name).startsWith("Speakers")) {
					if (String(l->devices[i].host_api) == "MME") {
						direction = l->devices[i].direction;
						host_api = l->devices[i].host_api;
						dev = l->devices[i].name;
						break;
					}
				}
			}
			c_server_set_audio_route(direction, LOCAL, 1, host_api, dev, BOTH);
		}

		if (!discovered) {
			discovered = true;
			// Discover radio
			if (!c_radio_discover()) {
				printf("Radio hardware not found!");
				return;
			}
			Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
		}

		if (!server_running) {
			server_running = true;
			// Start server
			if (!c_server_start()) {
				printf("Failed to start server!");
				return;
			}
			Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 1000);
		}

		// Start radio
		if (!c_radio_start(0)) {
			printf("Failed to start radio!");
		}
	}
	else {
		// Switching off
		// Stop radio
		setButtonText("Start");
		if (!c_radio_stop()) {
			printf("Failed to stop radio!");
		}
	}
}