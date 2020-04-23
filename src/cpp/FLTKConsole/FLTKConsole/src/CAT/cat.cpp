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
void CATStart(std::string port) {
	// Put ref in the repos
	RSt::inst().put_obj("CAT", (void*)cat_thrd);
	// Run the thread
	cat_thrd->run(port);
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
void CATThrd::run(std::string port)
{
	serial_port = port;

	enumerate_ports();
	while (!cat_term) {
		if (cat_enable) {
			printf("Working\n");
			Sleep(1000);
		}
		else {
			printf("Sleeping\n");
			Sleep(1000);
		}
	}
	printf("Thread exiting\n");
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
void CATThrd::open()
{
	// port, baudrate, timeout in milliseconds
	//serial::Serial cat_serial(serial_port, baud, serial::Timeout::simpleTimeout(1000));
	cat_serial = new serial::Serial(serial_port, desc.serial.baud, serial::Timeout::simpleTimeout(1000));

	if (cat_serial->isOpen())
		std::cout << " Opened CAT port" << std::endl;
	else
		std::cout << " Failed to open CAT port!" << std::endl;
}

//----------------------------------------------------
// Close COM/dev port
void CATThrd::close()
{
}

//----------------------------------------------------
// Process CAT commands
void CATThrd::process()
{
}