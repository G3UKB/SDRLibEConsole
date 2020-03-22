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

	// RADIO
	Fl_Preferences radio(root, "RADIO");
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

//----------------------------------------------------
// RADIO
int Preferences::get_freq(int radio) {
	return radio_freq[radio];
}
void Preferences::set_freq(int radio, int freq) {
	radio_freq[radio] = freq;
}
int Preferences::get_mode(int radio) {
	return radio_mode[radio];
}
void Preferences::set_mode(int radio, int mode) {
	radio_mode[radio] = mode;
}
int Preferences::get_filter(int radio) {
	return radio_filter[radio];
}
void Preferences::set_filter(int radio, int filter) {
	radio_filter[radio] = filter;
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

	// Read radio data
	Fl_Preferences radio(root, "RADIO");
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
	char* temp = 0;
	radio.get("audio-1", temp, "", 100);
	strcpy_s(radio_audio_path_1, temp);
	free(temp);
	radio.get("audio-2", temp, "", 100);
	strcpy_s(radio_audio_path_2, temp);
	free(temp);
	radio.get("audio-3", temp, "", 100);
	strcpy_s(radio_audio_path_3, temp);
	free(temp);
}
