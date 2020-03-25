/*
main_window.h

Window header for the FLTK Console

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
class ControlButton;
class DiscoverButton;

class MainWindow : public WindowBase
{
public:
	//==============================================================================
	MainWindow(int w, int h);
	~MainWindow() {};
	void resize(int x, int y, int w, int h);
	int handle(int event);
	void handle_idle_timeout();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// labels must be fixed storage
	char start_str[10] = "Start";
	char stop_str[10] = "Stop";
	char discover_str[12] = "Discover>>";

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components
	ControlButton* StartBtn;
	DiscoverButton* DiscoverBtn;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The start/stop button
class ControlButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	ControlButton(MainWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~ControlButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	int id;
	char* up_label;
	char* down_label;
	Fl_Color up_col;
	Fl_Color down_col;
	MainWindow* myparent;

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The discover button
class DiscoverButton : public Fl_Button
{
public:
	//==============================================================================
	DiscoverButton(MainWindow* parent_widget, char* button_label, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_col);
	~DiscoverButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	MainWindow* myparent;

	//==============================================================================
	// Method prototypes

};