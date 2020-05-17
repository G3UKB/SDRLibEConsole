/*
hamlib_client.h

Header for CAT control client using hamlib library

Copyright (C) 2020 by G3UKB Bob Cowdery

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

#include "includes.h"

//==============================================================================
// Defines

//==============================================================================

class HamlibClient
{
public:
	//==============================================================================
	HamlibClient(char* port, rig_model_t model);
	~HamlibClient() {};
	bool init();
	bool open();
	bool set_mode();
	bool set_freq(int freq);

	//==============================================================================

private:
	//==============================================================================
	// State variables

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Rig state
	int retcode;				// generic return code from functions 
	char ser_port[20];			// Port name to open
	rig_model_t rig_model;		// Model as passed in
	hamlib_port_t current_port;	// serial port
	RIG *current_rig;			// handle to rig (nstance) 
	freq_t freq;				// frequency 
	rmode_t rmode;				// radio mode of operation 
	//vfo_t vfo;					// vfo selection 
	int strength;				// S-Meter level 
	int rit = 0;				// RIT status 
	int xit = 0;				// XIT status 

	//==============================================================================
	// Method prototypes

};
