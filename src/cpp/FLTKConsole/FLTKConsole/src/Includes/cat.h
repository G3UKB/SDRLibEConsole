/*
cat.h

CAT control

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

#include "../Includes/includes.h"
#pragma once

//==============================================================================
// Defines

//==============================================================================

// Descriptor for CAT FT817 emulation
// This is the simplest CAT interface so suits our minimal rquirement

// Serial interface parameters
typedef struct CATSerial {
	serial::parity_t PARITY = serial::parity_t::parity_none;
	serial::stopbits_t STOP_BITS = serial::stopbits_t::stopbits_one;
	int baud = 9600;
	int TIMEOUT = 2;
	int READ_SZ = 5;
};

// Minimal set of required commands
typedef struct CATCommands {
	unsigned char LOCK_ON = 0x00;
	unsigned char LOCK_OFF = 0x80;
	unsigned char PTT_ON = 0x08;
	unsigned char PTT_OFF = 0x88;
	unsigned char SET_FREQ = 0x01;
	unsigned char SET_MODE = 0x07;
	unsigned char FREQ_MODE_GET = 0x03;
};
// Modes setting
typedef struct CATModes {
	unsigned char MODE_LSB = 0x00;
	unsigned char MODE_USB = 0x01;
	unsigned char MODE_CW = 0x02;
	unsigned char MODE_CWR = 0x03;
	unsigned char MODE_AM = 0x04;
	unsigned char MODE_FM = 0x08;
	unsigned char MODE_DIG = 0x0A;
	unsigned char MODE_PKT = 0x0C;
};

typedef struct CAT_FT817_EM {
	CATSerial serial;
	CATCommands cmds;
	CATModes modes;
};

// CATthrd startup function
void CATStart(std::string port);

//==============================================================================
// CAT controller class
class CATThrd {
public:
	
	// Construct/destroy
	//==============================================================================
	CATThrd() {};
	~CATThrd() {};

	//==============================================================================
	// Method prototypes
	void run(std::string port);
	void terminate();
	void enable(bool enable);
	
private:
	//==============================================================================
	// State variables
	CAT_FT817_EM desc;
	std::string serial_port;
	serial::Serial* cat_serial = NULL;
	bool cat_term = false;
	bool cat_enable = false;
	bool port_open = false;

	//==============================================================================
	// Method prototypes
	void enumerate_ports();
	bool open();
	void close();
	void process();
};