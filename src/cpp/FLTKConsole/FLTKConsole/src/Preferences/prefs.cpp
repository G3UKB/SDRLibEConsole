/*
prefs.cpp

Preferences for the FLTK Console

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
	The one and only preferences class
*/
#define DEFAULT_X 100
#define DEFAULT_Y 100
#define DEFAULT_W 331
#define DEFAULT_H 124

//==============================================================================
// PUBLIC
//----------------------------------------------------
// Constructor
Preferences::Preferences() {
	restore();
}

//----------------------------------------------------
// Save
void Preferences::save() {
	// Save the application data
	// Root database
	Fl_Preferences root(Fl_Preferences::USER, project, application);
	// APP
	Fl_Preferences app(root, "APP");
	app.set("window_x", window_x);
	app.set("window_y", window_y);
	app.set("window_w", window_w);
	app.set("window_h", window_h);
	app.set("radio2_x", radio2_x);
	app.set("radio2_y", radio2_y);
	app.set("radio2_w", radio2_w);
	app.set("radio2_h", radio2_h);
	app.set("radio3_x", radio3_x);
	app.set("radio3_y", radio3_y);
	app.set("radio3_w", radio3_w);
	app.set("radio3_h", radio3_h);
	app.set("tx_x", tx_x);
	app.set("tx_y", tx_y);
	app.set("tx_w", tx_w);
	app.set("tx_h", tx_h);

	// RADIO
	Fl_Preferences radio(root, "RADIO");
	radio.set("num_radios", num_radios);
	radio.set("freq-1", radio_freq[0]);
	radio.set("freq-2", radio_freq[1]);
	radio.set("freq-3", radio_freq[2]);
	radio.set("mode-1", radio_mode[0]);
	radio.set("mode-2", radio_mode[1]);
	radio.set("mode-3", radio_mode[2]);
	radio.set("filter-1", radio_filter[0]);
	radio.set("filter-2", radio_filter[1]);
	radio.set("filter-3", radio_filter[2]);
	radio.set("audio-1", radio_audio_path_1);
	radio.set("audio-2", radio_audio_path_2);
	radio.set("audio-3", radio_audio_path_3);
}

//==============================================================================
// API

//----------------------------------------------------
// APP
// Main window
int Preferences::get_window_x() {
	return window_x;
}
int Preferences::get_window_y() {
	return window_y;
}
void Preferences::set_window_x(int x) {
	window_x = x;
}
void Preferences::set_window_y(int y) {
	window_y = y;
}
int Preferences::get_window_w() {
	return window_w;
}
int Preferences::get_window_h() {
	return window_h;
}
void Preferences::set_window_w(int w) {
	window_w = w;
}
void Preferences::set_window_h(int h) {
	window_h = h;
}
// Radio2 window
int Preferences::get_radio2_x() {
	return radio2_x;
}
int Preferences::get_radio2_y() {
	return radio2_y;
}
void Preferences::set_radio2_x(int x) {
	radio2_x = x;
}
void Preferences::set_radio2_y(int y) {
	radio2_y = y;
}
int Preferences::get_radio2_w() {
	return radio2_w;
}
int Preferences::get_radio2_h() {
	return radio2_h;
}
void Preferences::set_radio2_w(int w) {
	radio2_w = w;
}
void Preferences::set_radio2_h(int h) {
	radio2_h = h;
}
// Radio3 window
int Preferences::get_radio3_x() {
	return radio3_x;
}
int Preferences::get_radio3_y() {
	return radio3_y;
}
void Preferences::set_radio3_x(int x) {
	radio3_x = x;
}
void Preferences::set_radio3_y(int y) {
	radio3_y = y;
}
int Preferences::get_radio3_w() {
	return radio3_w;
}
int Preferences::get_radio3_h() {
	return radio3_h;
}
void Preferences::set_radio3_w(int w) {
	radio3_w = w;
}
void Preferences::set_radio3_h(int h) {
	radio3_h = h;
}
// TX window
int Preferences::get_tx_x() {
	return tx_x;
}
int Preferences::get_tx_y() {
	return tx_y;
}
void Preferences::set_tx_x(int x) {
	tx_x = x;
}
void Preferences::set_tx_y(int y) {
	tx_y = y;
}
int Preferences::get_tx_w() {
	return tx_w;
}
int Preferences::get_tx_h() {
	return tx_h;
}
void Preferences::set_tx_w(int w) {
	tx_w = w;
}
void Preferences::set_tx_h(int h) {
	tx_h = h;
}
//----------------------------------------------------
// RADIO
int Preferences::get_num_radios() {
	return num_radios;
}
void Preferences::set_num_radios(int num) {
	num_radios = num;
}
int Preferences::get_freq(int radio) {
	return radio_freq[radio - 1];
}
void Preferences::set_freq(int radio, int freq) {
	radio_freq[radio - 1] = freq;
}
int Preferences::get_mode(int radio) {
	return radio_mode[radio - 1];
}
void Preferences::set_mode(int radio, int mode) {
	radio_mode[radio - 1] = mode;
}
int Preferences::get_filter(int radio) {
	return radio_filter[radio - 1];
}
void Preferences::set_filter(int radio, int filter) {
	radio_filter[radio - 1] = filter;
}
char* Preferences::get_audio_path(int radio) {
	if (radio == 1)
		return radio_audio_path_1;
	else if (radio == 2)
		return radio_audio_path_2;
	else
		return radio_audio_path_3;
}
void Preferences::set_audio_path(int radio, char* path) {
	if (radio == 1)
		strcpy_s(radio_audio_path_1, path);
	else if (radio == 2)
		strcpy_s(radio_audio_path_2, path);
	else
		strcpy_s(radio_audio_path_3, path);
}
// Audio tokens
struct struct_audio_desc Preferences::get_audio_desc(int radio) {
	return parse_audio_desc(radio);
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Restore
void Preferences::restore() {
	// Restore the database
	Fl_Preferences root(Fl_Preferences::USER, project, application);

	// Read application data
	Fl_Preferences app(root, "APP");
	app.get("window_x", window_x, DEFAULT_X);
	app.get("window_y", window_y, DEFAULT_Y);
	app.get("window_w", window_w, DEFAULT_W);
	app.get("window_h", window_h, DEFAULT_H);
	app.get("radio2_x", radio2_x, DEFAULT_X);
	app.get("radio2_y", radio2_y, DEFAULT_Y);
	app.get("radio2_w", radio2_w, DEFAULT_W);
	app.get("radio2_h", radio2_h, DEFAULT_H);
	app.get("radio3_x", radio3_x, DEFAULT_X);
	app.get("radio3_y", radio3_y, DEFAULT_Y);
	app.get("radio3_w", radio3_w, DEFAULT_W);
	app.get("radio3_h", radio3_h, DEFAULT_H);
	app.get("tx_x", tx_x, DEFAULT_X);
	app.get("tx_y", tx_y, DEFAULT_Y);
	app.get("tx_w", tx_w, DEFAULT_W);
	app.get("tx_h", tx_h, DEFAULT_H);

	// Read radio data
	Fl_Preferences radio(root, "RADIO");
	radio.get("num_radios", num_radios, 1);
	radio.get("freq-1", radio_freq[0], radio_freq[0]);
	radio.get("freq-2", radio_freq[1], radio_freq[1]);
	radio.get("freq-3", radio_freq[2], radio_freq[2]);
	radio.get("mode-1", radio_mode[0], radio_mode[0]);
	radio.get("mode-2", radio_mode[1], radio_mode[1]);
	radio.get("mode-3", radio_mode[2], radio_mode[2]);
	radio.get("filter-1", radio_filter[0], radio_filter[0]);
	radio.get("filter-2", radio_filter[1], radio_filter[1]);
	radio.get("filter-3", radio_filter[2], radio_filter[2]);
	// Audio route data
	radio.get("audio-1", radio_audio_path_1, "", 100);
	radio.get("audio-2", radio_audio_path_2, "", 100);
	radio.get("audio-3", radio_audio_path_3, "", 100);
}

//----------------------------------------------------
// Parse the audio descriptor into tokens
struct struct_audio_desc Preferences::parse_audio_desc(int radio) {
	// Retrieve audio path
	char* p_audio_path = get_audio_path(radio);
	static char audio_path[100];
	strcpy_s(audio_path, p_audio_path);
	audio_desc.valid = false;
	if (strlen(audio_path) > 0) {
		// We have a path
		// Extract tokens
		char* next_token;
		// Split into tokens
		audio_desc.sink_part = strtok_s(audio_path, ":", &next_token);
		audio_desc.dev_part = strtok_s(NULL, ":", &next_token);
		audio_desc.api_part = strtok_s(NULL, ":", &next_token);
		audio_desc.ch_part = strtok_s(NULL, ":", &next_token);
		audio_desc.valid = true;
	}
	return audio_desc;
}