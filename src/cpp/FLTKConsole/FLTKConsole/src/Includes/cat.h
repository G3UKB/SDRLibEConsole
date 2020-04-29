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

// Request types
#define PTT_ON 0x08
#define PTT_OFF 0x88
#define SET_FREQ 0x01
#define SET_MODE 0x07
#define FREQ_MODE_GET 0x03
#define READ_EEPROM_DATA 0xbb
#define READ_TX_STATUS 0xf7
#define TOGGLE_VFO 0x81

//==============================================================================
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

// CATthrd startup function
void CATStart(Preferences* prefs, std::string port);

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
	void run(Preferences* prefs, std::string port);
	void terminate();
	void enable(bool enable);
	
private:
	//==============================================================================
	// State variables
	Preferences* p;
	char const *bytes;
	std::string serial_port;
	serial::Serial* cat_serial = NULL;
	bool cat_term = false;
	bool cat_enable = false;
	bool port_open = false;
	byte response[5];
	bool mox;

	//==============================================================================
	// Method prototypes
	void enumerate_ports();
	bool open();
	void close();
	void process();

	void read_eeprom(const char* bytes);
	void toggle_vfo(const char* bytes);
	void ptt_on(const char* bytes);
	void ptt_off(const char* bytes);
	void freq_mode_get(const char* bytes);
	void read_tx_status(const char* bytes);
	void set_freq(const char* bytes);
	void set_mode(const char* bytes);

	std::string zero_pad_number(int num);
};