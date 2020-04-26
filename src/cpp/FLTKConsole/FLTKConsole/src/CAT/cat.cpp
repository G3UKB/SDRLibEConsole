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
		cat_serial = new serial::Serial(serial_port, 9600, serial::Timeout::simpleTimeout(1000),serial::bytesize_t::eightbits,serial::parity_t::parity_none,serial::stopbits_t::stopbits_two);
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
		//if (data.length() > 0) {
		//	for (int i=0; i < 5; i++) {
		//		printf("R: %d, %x\n", i, data.c_str()[i] & 0x000000ff);
		//	}
		//}
		bytes = data.c_str();
		if (data.length() > 0) {
			// Valid data
			switch (bytes[4] & 0x000000ff) {
			case READ_EEPROM_DATA:
				printf("READ_EEPROM_DATA\n");
				read_eeprom(bytes);
				break;
			case READ_TX_STATUS:
				printf("READ_TX_STATUS\n");
				read_tx_status(bytes);
				break;
			case LOCK_ON:
				break;
			case LOCK_OFF:
				break;
			case PTT_ON:
				break;
			case PTT_OFF:
				printf("PTT_OFF\n");
				ptt_off(bytes);
				break;
			case SET_FREQ:
				break;
			case SET_MODE:
				break;
			case TOGGLE_VFO:
				printf("TOGGLE_VFO\n");
				toggle_vfo(bytes);
				break;
			case FREQ_MODE_GET:
				printf("FREQ_MODE_GET\n");
				freq_mode_get(bytes);
				break;
			}
		}
	}
}

//----------------------------------------------------
// Return EEPROM data
void CATThrd::read_eeprom(const char* bytes) {
	if (bytes[1] == 0x64) {
		// Request for CAT baud rate == 9600
		uint8_t const b1 = 0x40;
		// Returns 2 bytes of data
		cat_serial->write(&b1, 1);
		cat_serial->write(&b1, 1);
	}
}

//----------------------------------------------------
// Return current freq and mode
void CATThrd::freq_mode_get(const char* bytes) {
	// Construct a frequency and mode response
	// Return the radio 1 frequency and mode as a 5 byte packet
	/*
	int f = p->get_freq(1);
	printf("fi: %d\n", f);
	
	// 0-3 is frequency MSB first, 4 is the current mode
	// e.g. 01, 42, 34, 56, [ 01 ] = 14.23456 MHz, mode 1 (USB)
	// Frequency is in Hz
	std::string fs = zero_pad_number(f);
	printf("%d, %s\n", fs.length(),fs.c_str());
	const char* fc = fs.c_str();
	// Extract values
	char val[4];
	strncpy_s(val, fc, 2);
	response[0] = (byte)strtol(val, (char **)NULL, 10);
	strncpy_s(val, fc+2, 2);
	response[1] = (int)strtol(val, (char **)NULL, 10);
	strncpy_s(val, fc+4, 2);
	response[2] = (int)strtol(val, (char **)NULL, 10);
	strncpy_s(val, fc+6, 2);
	response[3] = (int)strtol(val, (char **)NULL, 10);
	response[4] = 01;
	printf("%d%d%d%d %d\n", response[0], response[1], response[2], response[3], response[4]);
	cat_serial->write(response, 5);
	*/
	Sleep(100);
	uint8_t const b1 = 0x01;
	uint8_t const b2 = 0x42;
	uint8_t const b3 = 0x34;
	uint8_t const b4 = 0x56;
	uint8_t const b5 = 0x01;

	cat_serial->write(&b1, 1);
	cat_serial->write(&b2, 1);
	cat_serial->write(&b3, 1);
	cat_serial->write(&b4, 1);
	cat_serial->write(&b5, 1);
}

//----------------------------------------------------
// Toggle VFO
void CATThrd::toggle_vfo(const char* bytes) {
	// No response required
}

//----------------------------------------------------
// PTT off
void CATThrd::ptt_off(const char* bytes) {
	// Report unkeyed
	// Unkeyed
	uint8_t const b = 0xF0;
	// Was keyed
	uint8_t const b1 = 0x00;
	Sleep(100);
	cat_serial->write(&b, 1);
}

//----------------------------------------------------
// Read TX status
void CATThrd::read_tx_status(const char* bytes) {
	// Returns 1 status byte
	// Split mode on
	uint8_t const b = 0x00;
	Sleep(100);
	cat_serial->write(&b, 1);
}

//----------------------------------------------------
// Convert int Hz into 9 digit zero filled string
std::string CATThrd::zero_pad_number(int num)
{
	std::ostringstream ss;
	ss << std::setw(8) << std::setfill('0') << num;
	return ss.str();
}

std::string CATThrd::string_to_hex(const std::string& in) {
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	for (size_t i = 0; in.length() > i; ++i) {
		ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
	}

	return ss.str();
}
