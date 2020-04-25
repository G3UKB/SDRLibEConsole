/*
cat.cpp

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

//==============================================================================
// Defines

// Vars
// Create the CAT thread
CATThrd* cat_thrd = new CATThrd();

//==============================================================================
// Thread start callable function
void CATStart(Preferences* prefs, std::string port) {
	// Put ref in the repos
	RSt::inst().put_obj("CAT", (void*)cat_thrd);
	// Run the thread
	cat_thrd->run(prefs, port);
}

//==============================================================================
// PUBLIC

//----------------------------------------------------
// Enable/disable thread
void CATThrd::enable(bool enable) {
	cat_enable = enable;
}

//----------------------------------------------------
// Ask thread to terminate
void CATThrd::terminate() {
	cat_term = true;
}

//----------------------------------------------------
// Thread entry pont
void CATThrd::run(Preferences* prefs, std::string port)
{
	serial_port = port;
	p = prefs;

	enumerate_ports();
	while (!cat_term) {
		if (cat_enable) {
			// Open port
			if (!open()) {
				break;
			}
			// Process requests
			while (cat_enable && !cat_term) {
				process();
				Sleep(100);
			}
		}
		else {
			Sleep(100);
		}
	}
	close();
	printf("CAT thread exiting...\n");
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Enumerate ports
void CATThrd::enumerate_ports()
{
	std::vector<serial::PortInfo> devices_found = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;
		printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
			device.hardware_id.c_str());
	}
}

//----------------------------------------------------
// Open COM/dev port
bool CATThrd::open()
{
	if (!port_open) {
		cat_serial = new serial::Serial(serial_port, desc.serial.baud, serial::Timeout::simpleTimeout(1000));
		if (cat_serial->isOpen()) {
			std::cout << "Opened CAT port" << std::endl;
			cat_serial->setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
			port_open = true;
			return true;
		}
		else {
			std::cout << "Failed to open CAT port!" << std::endl;
			return false;
		}
	}
	return true;
}

//----------------------------------------------------
// Close COM/dev port
void CATThrd::close()
{
	if (port_open) {
		cat_serial->close();
	}
}

//----------------------------------------------------
// Process CAT commands
void CATThrd::process()
{
	if (port_open) {
		// Commands consist of 5 bytes, 4 parameters and the command byte
		std::string data;
		data = cat_serial->read(5);
		bytes = data.c_str();
		if (data.length() > 0) {
			// Valid data
			switch (bytes[4]) {
			case LOCK_ON:
				break;
			case LOCK_OFF:
				break;
			case PTT_ON:
				break;
			case PTT_OFF:
				break;
			case SET_FREQ:
				break;
			case SET_MODE:
				break;
			case FREQ_MODE_GET:
				freq_mode_get(bytes);
				break;
			}
		}
	}
}

//----------------------------------------------------
// Return current freq and mode
void CATThrd::freq_mode_get(const char* bytes) {
	// Construct a frequency and mode response
	// Return the radio 1 frequency and mode as a 5 byte packet
	int f = p->get_freq(1);
	// 0-3 is frequency MSB first, 4 is the current mode
	// e.g. 01, 42, 34, 56, [ 01 ] = 14.23456 MHz, mode 1 (USB)
	// Frequency is in Hz
	std::string fs = zero_pad_number(f);
	printf("%s\n", fs.c_str());
}

//----------------------------------------------------
// Convert int Hz into 9 digit zero filled string
std::string CATThrd::zero_pad_number(int num)
{
	std::ostringstream ss;
	ss << std::setw(9) << std::setfill('0') << num;
	return ss.str();
}