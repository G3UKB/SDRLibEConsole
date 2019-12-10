/*
gui_cache.cpp

Radio interface for the Juce Console

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

#include "gui_cache.h"
#include "../Common/extern.h"

//==============================================================================
// Radio Interface Singleton class
//==============================================================================

//==============================================================================
// Public

//Macro for Singleton
juce_ImplementSingleton(GUICache)

//==============================================================================
// Get/Set methods

void GUICache::setVFOInst(Component* pvfo) {
	vfo = pvfo;
}

Component* GUICache::getVFOInst() {
	return vfo;
}

void GUICache::setMainInst(Component* pmain) {
	main = pmain;
}

Component* GUICache::getMainInst() {
	return main;
}