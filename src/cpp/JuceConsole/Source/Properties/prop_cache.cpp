/*
prop_cache.cpp

Property cache for the Juce Console

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

#include "prop_cache.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public
PropCache::PropCache() {
	radio_1 = new Properties("radio-1", "r1_config.cfg");
}

//Macro for Singleton
juce_ImplementSingleton(PropCache)

//==============================================================================
// Get/Set methods

Properties* PropCache::get_prop_inst(String name) {
	if (name == "radio-1") return radio_1;
	else if (name == "radio-2") return radio_2;
	else return radio_3;
}