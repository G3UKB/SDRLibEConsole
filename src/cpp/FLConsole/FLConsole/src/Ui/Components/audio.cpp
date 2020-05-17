/*
audio.cpp

Audio selection window implementation for the FLTK Console

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

#include "../../Includes/includes.h"

//==============================================================================
// PUBLIC

//==============================================================================
// Audio Output Component Window
//==============================================================================

void apply_output_cb(Fl_Widget* w, void* user_data) {
	// Call down to our Audio instance
	((AudioOutput*)user_data)->handle_apply();
}

//----------------------------------------------------
// Constructor/Destructor
AudioOutput::AudioOutput(int radio, int w, int h) : Fl_Window(w, h) {

	// Our radio instance
	r = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");
	
	// Set up window
	char label[30];
	sprintf_s(label, "Audio Output (radio-%d", r);
	copy_label(label);
	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));
	
	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 5, w - 10, h - 10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);
	
	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w - 10, h - 10, 4, 5, 5);
	metrics m;
	// Add labels
	m = grid->get_cell_metrics(0, 0);
	Fl_Box* l_sink = new Fl_Box(m.x, m.y, m.w, m.h, "Sink");
	m = grid->get_cell_metrics(1, 0);
	Fl_Box* l_dev = new Fl_Box(m.x, m.y, m.w, m.h, "Dev");
	m = grid->get_cell_metrics(2, 0);
	Fl_Box* l_ch = new Fl_Box(m.x, m.y, m.w, m.h, "Ch");
	
	// Add choices
	m = grid->get_cell_metrics(0, 1, 1, 4);
	// Sink selection
	sink = new Fl_Choice(m.x, m.y, m.w, m.h);
	sink->add("HPSDR");
	sink->add("Local-AF");
	sink->add("Local-IQ");
	sink->value(0);
	m = grid->get_cell_metrics(1, 1, 1, 4);
	// Device selection
	device = new Fl_Choice(m.x, m.y, m.w, m.h);
	// Get the output enumeration
	DeviceEnumList* audio_outputs = c_server_enum_audio_outputs();
	// Populate the list
	char str[100];
	for (int i = 0; i < audio_outputs->entries; i++) {
		strcpy_s(str, 100, audio_outputs->devices[i].name);
		strcat_s(str, 100, ":");
		strcat_s(str, 100, audio_outputs->devices[i].host_api);
		device->add(str);
	}
	
	// Add channel selection
	m = grid->get_cell_metrics(2, 1, 1, 4);
	Fl_Group *ch_group = new Fl_Group(m.x, m.y, m.w, m.h);
	m = grid->get_cell_metrics(2, 1);
	left = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Left");
	m = grid->get_cell_metrics(2, 2);
	right = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Right");
	m = grid->get_cell_metrics(2, 3);
	both = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "Both");
	m = grid->get_cell_metrics(2, 4);
	none = new Fl_Radio_Light_Button(m.x, m.y, m.w, m.h, "None");
	ch_group->end();
	
	// Add apply button
	m = grid->get_cell_metrics(3, 4);
	apply = new Fl_Button(m.x, m.y, m.w, m.h, "Apply");
	apply->callback((Fl_Callback*)apply_output_cb, (void*)this);
	
	// Close up and display
	top_group->end();
	end();
	border(false);
	
	// Retrieve and reset audio path
	struct_audio_desc desc = p->get_audio_desc(r);
	if (desc.valid) {
		// Set the widget state
		set_widget_state(desc.loc_part, desc.api_part, desc.dev_part, desc.ch_part);
		// Set audio path
		r_i->set_audio_paths();
	}

	// Finally show window
	show();
}

//----------------------------------------------------
// Apply current state as a new audio path
void AudioOutput::handle_apply() {
	// Gather all the required information
	char sink_str[10];
	char dev_str[100];
	char *dev_part;
	char *api_part;
	char ch_str[10];

	// Set sink type
	if (strcmp(((Fl_Choice*)sink)->text(), "LOCAL-AF")) {
		strcpy_s(sink_str, 9, LOCAL_AF);
	} else if (strcmp(((Fl_Choice*)sink)->text(), "LOCAL-IQ")) {
		strcpy_s(sink_str, 9, LOCAL_IQ);
	} else if (strcmp(((Fl_Choice*)sink)->text(), "HPSDR")) {
		strcpy_s(sink_str, 9, HPSDR);
	}

	// Set dev type
	strcpy_s(dev_str, 100, ((Fl_Choice*)device)->text());
	// Split the dev part
	dev_part = strtok_s(dev_str, ":", &api_part);

	// Set channel type
	if (((Fl_Radio_Light_Button*)left)->value()) {
		strcpy_s(ch_str, 10, LEFT);
	}
	else if (((Fl_Radio_Light_Button*)right)->value()) {
		strcpy_s(ch_str, 10, RIGHT);
	}
	else {
		strcpy_s(ch_str, 10, BOTH);
	}

	// Save the new route
	save_route(r, sink_str, api_part, dev_part, ch_str);

	// Reset the audio path for all receivers 
	r_i->set_audio_paths();
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Set a new audio path
void AudioOutput::set_widget_state(char* vsink, char* vapi, char* vdev, char* vch) {
	char str[100];

	// Set sink
	if (strcmp(vsink, LOCAL_AF) == 0) {
		((Fl_Choice*)sink)->value(((Fl_Choice*)sink)->find_index("Local-AF"));
	}
	else if (strcmp(vsink, LOCAL_IQ) == 0) {
		((Fl_Choice*)sink)->value(((Fl_Choice*)sink)->find_index("Local-IQ"));
	}
	else if (strcmp(vsink, HPSDR) == 0) {
		((Fl_Choice*)sink)->value(((Fl_Choice*)sink)->find_index("HPSDR"));
	}
	// Set device
	strcpy_s(str, 100, vdev);
	strcat_s(str, 100, ":");
	strcat_s(str, 100, vapi);
	((Fl_Choice*)device)->value(((Fl_Choice*)device)->find_index(str));
	// Set channel
	if (strcmp(vch, LEFT) == 0) {
		((Fl_Radio_Light_Button*)left)->set();
	}
	else if (strcmp(vch, RIGHT) == 0) {
		((Fl_Radio_Light_Button*)right)->set();
	}
	else if (strcmp(vch, BOTH) == 0) {
		((Fl_Radio_Light_Button*)both)->set();
	}
}

//----------------------------------------------------
// Save the audio route
void AudioOutput::save_route(int radio, char* sink, char* api, char* dev, char* ch) {
	// Save the current route
	char current_route[100];
	strcpy_s(current_route, 100, sink);
	strcat_s(current_route, 100, ":");
	strcat_s(current_route, 100, dev);
	strcat_s(current_route, 100, ":");
	strcat_s(current_route, 100, api);
	strcat_s(current_route, 100, ":");
	strcat_s(current_route, 100, ch);
	p->set_audio_path(radio, current_route);
}

//==============================================================================
// Audio Input Component Window
//==============================================================================

void apply_input_cb(Fl_Widget* w, void* user_data) {
	// Call down to our Audio instance
	((AudioInput*)user_data)->handle_apply();
}

//----------------------------------------------------
// Constructor/Destructor
AudioInput::AudioInput(int radio, int w, int h) : Fl_Window(w, h) {

	// Our radio instance
	r = radio;

	// Get dependent objects from the cache
	r_i = (RadioInterface*)RSt::inst().get_obj("RADIO-IF");
	p = (Preferences*)RSt::inst().get_obj("PREFS");

	// Set up window
	copy_label("Audio Input (transmitter)");
	resizable(this);
	color((Fl_Color)24);
	align(Fl_Align(65));

	// Add a group box
	Fl_Group *top_group = new Fl_Group(5, 5, w - 10, h - 10);
	top_group->box(FL_GTK_THIN_UP_BOX);
	top_group->color((Fl_Color)24);

	// Create a grid layout handler
	GridLayout *grid = new GridLayout(5, 5, w - 10, h - 10, 4, 5, 5);
	metrics m;
	// Add labels
	m = grid->get_cell_metrics(0, 0);
	Fl_Box* l_sink = new Fl_Box(m.x, m.y, m.w, m.h, "Source");
	m = grid->get_cell_metrics(1, 0);
	Fl_Box* l_dev = new Fl_Box(m.x, m.y, m.w, m.h, "Dev");

	// Add choices
	m = grid->get_cell_metrics(0, 1, 1, 4);
	// Source selection
	source = new Fl_Choice(m.x, m.y, m.w, m.h);
	source->add("HPSDR");
	source->add("Local");
	source->value(0);
	m = grid->get_cell_metrics(1, 1, 1, 4);
	// Device selection
	device = new Fl_Choice(m.x, m.y, m.w, m.h);
	// Get the output enumeration
	DeviceEnumList* audio_inputs = c_server_enum_audio_inputs();
	// Populate the list
	char str[100];
	for (int i = 0; i < audio_inputs->entries; i++) {
		strcpy_s(str, 100, audio_inputs->devices[i].name);
		strcat_s(str, 100, ":");
		strcat_s(str, 100, audio_inputs->devices[i].host_api);
		device->add(str);
	}

	// Add apply button
	m = grid->get_cell_metrics(3, 4);
	apply = new Fl_Button(m.x, m.y, m.w, m.h, "Apply");
	apply->callback((Fl_Callback*)apply_input_cb, (void*)this);

	// Close up and display
	top_group->end();
	end();
	border(false);

	// Retrieve and reset audio path
	struct_audio_desc desc = p->get_audio_desc(r);
	if (desc.valid) {
		// Set the widget state
		set_widget_state(desc.loc_part, desc.api_part, desc.dev_part);
		// Set audio path
		r_i->set_audio_paths();
	}

	// Finally show window
	show();
}

//----------------------------------------------------
// Apply current state as a new audio path
void AudioInput::handle_apply() {
	// Gather all the required information
	char source_str[10];
	char dev_str[100];
	char *dev_part;
	char *api_part;

	// Set source type
	if (strcmp(((Fl_Choice*)source)->text(), "LOCAL")) {
		strcpy_s(source_str, 9, LOCAL_AF);
	}
	else if (strcmp(((Fl_Choice*)source)->text(), "HPSDR")) {
		strcpy_s(source_str, 9, HPSDR);
	}

	// Set dev type
	strcpy_s(dev_str, 100, ((Fl_Choice*)device)->text());
	// Split the dev part
	dev_part = strtok_s(dev_str, ":", &api_part);

	// Save the new route
	save_route(r, source_str, api_part, dev_part);

	// Reset the audio path for all receivers 
	r_i->set_audio_paths();
}

//==============================================================================
// PRIVATE

//----------------------------------------------------
// Set a new audio path
void AudioInput::set_widget_state(char* vsource, char* vapi, char* vdev) {
	char str[100];

	// Set source
	if (strcmp(vsource, LOCAL) == 0) {
		((Fl_Choice*)source)->value(((Fl_Choice*)source)->find_index("Local-IQ"));
	}
	else if (strcmp(vsource, HPSDR) == 0) {
		((Fl_Choice*)source)->value(((Fl_Choice*)source)->find_index("HPSDR"));
	}
	// Set device
	strcpy_s(str, 100, vdev);
	strcat_s(str, 100, ":");
	strcat_s(str, 100, vapi);
	((Fl_Choice*)device)->value(((Fl_Choice*)device)->find_index(str));
}

//----------------------------------------------------
// Save the audio route
void AudioInput::save_route(int radio, char* source, char* api, char* dev) {
	// Save the current route
	char current_route[100];
	strcpy_s(current_route, 100, source);
	strcat_s(current_route, 100, ":");
	strcat_s(current_route, 100, dev);
	strcat_s(current_route, 100, ":");
	strcat_s(current_route, 100, api);
	strcat_s(current_route, 100, ":BOTH");
	p->set_audio_path(radio, current_route);
}
