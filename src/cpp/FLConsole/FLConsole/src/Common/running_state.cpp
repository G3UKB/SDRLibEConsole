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

//==============================================================================
// Cache

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
// Callback cache
void RSt::put_cb(std::string name, std::function< int(int, int) > f) {
	cb_map.insert(std::pair<std::string, std::function< int(int, int) >>(name, f));
}

std::function< int(int, int) > RSt::get_cb(std::string name) {
	if (cb_map.find(name) == cb_map.end())
		return NULL;
	else
		return cb_map.at(name);
}

//==============================================================================
// Application state

//----------------------------------------------------
// SDR type radio state
void RSt::set_type(int radio_type) {
	app_state.radio_type = radio_type;
}

int RSt::get_type() {
	return app_state.radio_type;
}

//----------------------------------------------------
// SDR type radio state
void RSt::set_discovered(bool found) {
	app_state.sdr_state.discovered = found;
}

bool RSt::get_discovered() {
	return app_state.sdr_state.discovered;
}

void RSt::set_server_running(bool running) {
	app_state.sdr_state.server_running = running;
}

bool RSt::get_server_running() {
	return app_state.sdr_state.server_running;
}

void RSt::set_radio_running(bool running) {
	app_state.sdr_state.radio_running = running;
}

bool RSt::get_radio_running() {
	return app_state.sdr_state.radio_running;
}

void RSt::set_duplex(bool state) {
	app_state.sdr_state.duplex = state;
}

bool RSt::get_duplex() {
	return app_state.sdr_state.duplex;
}

void RSt::set_mox(bool state) {
	app_state.sdr_state.mox = state;
}

bool RSt::get_mox() {
	return app_state.sdr_state.mox;
}

//----------------------------------------------------
// Analog type radio state
void RSt::set_analog_radio_running(bool running) {
	app_state.analog_state.radio_running = running;
}

bool RSt::get_analog_radio_running() {
	return app_state.analog_state.radio_running;
}

void RSt::set_analog_duplex(bool state) {
	app_state.analog_state.duplex = state;
}

bool RSt::get_analog_duplex() {
	return app_state.analog_state.duplex;
}

void RSt::set_analog_mox(bool state) {
	app_state.analog_state.mox = state;
}

bool RSt::get_analog_mox() {
	return app_state.analog_state.mox;
}