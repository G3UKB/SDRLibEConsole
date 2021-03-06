/*
running_state.cpp

Cache and current state for the FLTK Console

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

/*
	Provides a high level interface to save/restore all application prefs. The underlying
	mechanism uses Fl-Preferences with a group for each logically related dataset.

*/

#include "../Includes/includes.h"

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only running state class.
	This manages 
*/

//==============================================================================
// PUBLIC
//----------------------------------------------------
// Object cache
void RSt::put_obj(std::string name, void* obj) {
	obj_map.insert(std::pair<std::string, void*>(name, obj));
}

void* RSt::get_obj(std::string name) {
	if (obj_map.find(name) == obj_map.end())
		return NULL;
	else
		return obj_map.at(name);
}

//----------------------------------------------------
// Application state
void RSt::set_discovered(bool found) {
	app_state.discovered = found;
}

bool RSt::get_discovered() {
	return app_state.discovered;
}

void RSt::set_server_running(bool running) {
	app_state.server_running = running;
}

bool RSt::get_server_running() {
	return app_state.server_running;
}

void RSt::set_radio_running(bool running) {
	app_state.radio_running = running;
}

bool RSt::get_radio_running() {
	return app_state.radio_running;
}

void RSt::set_duplex(bool state) {
	app_state.duplex = state;
}

bool RSt::get_duplex() {
	return app_state.duplex;
}

void RSt::set_mox(bool state) {
	app_state.mox = state;
}

bool RSt::get_mox() {
	return app_state.mox;
}