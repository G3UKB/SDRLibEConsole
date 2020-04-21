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

//==============================================================================
/*
	CAT control class
*/

// Run the CAT thread
CATThrd* cat_thrd = new CATThrd();

void CATStart(int x) {
	RSt::inst().put_obj("CAT", (void*)cat_thrd);
	cat_thrd->run();
}

void CATThrd::terminate() {
	term = true;
}

// Thread entry pont
void CATThrd::run()
{
	while (!term) {
		Sleep(1000);
	}
	printf("Thread exiting\n");
}