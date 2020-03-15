/*
running_state.h

Cache and state header for the FLTK Console

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

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only running state.
	This is a singleton pattern so easily accessible and contains all instance cache
	data and running state.
*/

class RSt
{
public:
	static RSt& getInstance()
	{
		static RSt instance;	// Guaranteed to be destroyed.
										// Instantiated on first use.
		return instance;
	}
private:
	RSt() {}                    // Constructor? (the {} brackets) are needed here.

public:
	// Delete methods we don't need
	RSt(RSt const&) = delete;
	void operator=(RSt const&) = delete;


};