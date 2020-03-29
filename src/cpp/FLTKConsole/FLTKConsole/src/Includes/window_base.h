/*
window_base.h

Window base class header for the FLTK Console

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

#include "includes.h"

//==============================================================================
// Defines

//==============================================================================
/*
	The one and only main window
*/
class ModeTriggerBase;
class FilterTriggerBase;
class AudioTriggerBase;

// Return my location
typedef struct struct_w_loc {
	int x;
	int y;
	int w;
	int h;
};

class WindowBase : public Fl_Double_Window
{
public:
	//==============================================================================
	WindowBase(int radio, int x, int y, int w, int h);
	~WindowBase() {};
	void resize(int x, int y, int w, int h);
	void manage_mode_panel(bool show);
	void manage_filter_panel(bool show);
	void manage_audio_panel(bool show);
	virtual void handle_idle_timeout();

	//Layout
	GridLayout *grid;
	metrics m;
	// Widgets
	Fl_Group *top_group;

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Radio inst
	int r;
	// Structures
	struct_w_loc w_loc;
	// Labels must be fixed storage
	char audio_str[10] = "Audio>>";
	char mode_str[10] = "Mode>>";
	char filter_str[10] = "Filter>>";

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components
	AudioTriggerBase* AudioBtn;
	Audio* audio;
	ModeTriggerBase* ModeBtn;
	Modes *modes;
	FilterTriggerBase* FilterBtn;
	Filters *filters;

	//==============================================================================
	// Method prototypes
	struct struct_w_loc get_location(int radio);
};

//==============================================================================
// The mode button class
class ModeTriggerBase : public Fl_Toggle_Button
{
public:
	//==============================================================================
	ModeTriggerBase(WindowBase* parent_widget, char* label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col);
	~ModeTriggerBase() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int id;
	WindowBase* parent;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The filter button class
class FilterTriggerBase : public Fl_Toggle_Button
{
public:
	//==============================================================================
	FilterTriggerBase(WindowBase* parent_widget, char* label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col);
	~FilterTriggerBase() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int id;
	WindowBase* parent;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The audio button class
class AudioTriggerBase : public Fl_Toggle_Button
{
public:
	//==============================================================================
	AudioTriggerBase(WindowBase* parent_widget, char* label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color label_col);
	~AudioTriggerBase() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int id;
	WindowBase* parent;

	//==============================================================================
	// Method prototypes
};