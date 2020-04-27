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

//==============================================================================
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
				read_eeprom(bytes);
				break;
			case READ_TX_STATUS:
				read_tx_status(bytes);
				break;
			case LOCK_ON:
				break;
			case LOCK_OFF:
				break;
			case PTT_ON:
				break;
			case PTT_OFF:
				ptt_off(bytes);
				break;
			case SET_FREQ:
				set_freq(bytes);
				break;
			case SET_MODE:
				break;
			case TOGGLE_VFO:
				toggle_vfo(bytes);
				break;
			case FREQ_MODE_GET:
				freq_mode_get(bytes);
				break;
			default:
				printf("Unknown command: %x\n", bytes[4] & 0x000000ff);
				// Send a response so we don't stop processing
				// Unless it needs a multi-byte response
				uint8_t const b = 0x00;
				cat_serial->write(&b, 1);
			}
		}
	}
}

//==============================================================================
// Command execution

//----------------------------------------------------
// Return EEPROM data
void CATThrd::read_eeprom(const char* bytes) {
	// Of course we don't actually have an EEPROM
	// Just return what we think it asked for
	// http://www.ka7oei.com/ft817_meow.html has info not in manual
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
	// Get radio 1 frequency
	int f = p->get_freq(1);
	
	// 0-3 is frequency MSB first, 4 is the current mode
	// e.g. 01, 42, 34, 56, [ 01 ] = 14.23456 MHz, mode 1 (USB)
	// Frequency is in Hz
	// Convert to string and zero pad to 8 characters
	std::string fs = zero_pad_number(f);
	// Convert to a C string
	const char* fc = fs.c_str();
	// Construct response
	if (f < 100000000) {
		// < 100MHz
		char val[4];
		strncpy_s(val, fc, 1);
		response[0] = (byte)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 1, 2);
		response[1] = (int)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 3, 2);
		response[2] = (int)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 5, 2);
		response[3] = (int)strtol(val, (char **)NULL, 16);
		response[4] = 01;
		//printf("%d%d%d%d %d\n", response[0], response[1], response[2], response[3], response[4]);
	}
	else {
		// >= 100MHz
		char val[4];
		strncpy_s(val, fc, 2);
		response[0] = (byte)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 2, 2);
		response[1] = (int)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 4, 2);
		response[2] = (int)strtol(val, (char **)NULL, 16);
		strncpy_s(val, fc + 6, 2);
		response[3] = (int)strtol(val, (char **)NULL, 16);
		response[4] = 01;
		//printf("%d%d%d%d %d\n", response[0], response[1], response[2], response[3], response[4]);
	}
	// Return frequency
	cat_serial->write(response, 5);
}

//----------------------------------------------------
// Toggle VFO
void CATThrd::toggle_vfo(const char* bytes) {
	// No response required
	uint8_t const b = 0x00;
	cat_serial->write(&b, 1);
}

//----------------------------------------------------
// PTT off
void CATThrd::ptt_off(const char* bytes) {
	// ToDo - go into RX modei.e. turn TX off if on
	// and respond with approprtiate code
	// Unkeyed
	uint8_t const b = 0xF0;
	// Was keyed
	uint8_t const b1 = 0x00;
	cat_serial->write(&b, 1);
}

//----------------------------------------------------
// Read TX status
void CATThrd::read_tx_status(const char* bytes) {
	// Returns 1 status byte
	// ToDo return actual TX status if we are in TX mode
	// else return 0 as this is invalid in RX mode.
	uint8_t const b = 0x00;
	cat_serial->write(&b, 1);
}

//----------------------------------------------------
// Set freq
void CATThrd::set_freq(const char* bytes) {
	// For now set radio 1 freq but this may change
	// and in split mode might be difficult to handle
	// Presumably the RX/TX frequencies are set as and when in simplex mode.
	int MHz_100, MHz_10, MHz_1, KHz_100, KHz_10, KHz_1, Hz_100, Hz_10, Hz;
	MHz_100 = ((bytes[0] & 0xF0) >> 4) * 100000000;
	MHz_10 = (bytes[0] & 0x0F) * 10000000;
	MHz_1 = ((bytes[1] & 0xF0) >> 4) * 1000000;
	KHz_100 = (bytes[1] & 0x0F) * 100000;
	KHz_10 = ((bytes[2] & 0xF0) >> 4) * 10000;
	KHz_1 = (bytes[2] & 0x0F) * 1000;
	Hz_100 = ((bytes[3] & 0xF0) >> 4) * 100;
	Hz_10 = (bytes[3] & 0x0F) * 10;
	Hz = MHz_100 + MHz_10 + MHz_1 + KHz_100 + KHz_10 + KHz_1 + Hz_100 + Hz_10;
	// Set VFO for radio 1
	((VFOComponent*)RSt::inst().get_obj("RADIO-1"))->external_set_display(Hz);
	// Return an Ack. This is not documented anywhere but hamlib seems to
	// expect some resoponse and 0x00 works.
	uint8_t const b = 0x00;
	cat_serial->write(&b, 1);
}

//==============================================================================
// Helpers

//----------------------------------------------------
// Convert int Hz into 9 digit zero filled string
std::string CATThrd::zero_pad_number(int num)
{
	std::ostringstream ss;
	ss << std::setw(8) << std::setfill('0') << num;
	return ss.str();
}

