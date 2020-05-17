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
class RadioChoice;
class CATButton;
class TXButton;
class ExitButton;
class CATThrd;

class MainWindow : public WindowBase
{
public:
	//==============================================================================
	MainWindow(int x, int y, int w, int h);
	~MainWindow() {};
	int handle(int event);
	void handle_idle_timeout();
	void handle_radio(Fl_Widget* w);
	void show_tx(bool show);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// labels must be fixed storage
	char start_str[10] = "Start";
	char stop_str[10] = "Stop";
	char discover_str[12] = "Discover>>";
	char cat_str_on[12] = "CAT ON";
	char cat_str_off[12] = "CAT OFF";
	char tx_str_on[12] = "Show TX";
	char tx_str_off[12] = "Hide TX";
	char exit_str[10] = "EXIT";

	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;

	// Components
	ControlButton* StartBtn;
	DiscoverButton* DiscoverBtn;
	Fl_Choice* SelectRadio;
	CATButton* CATBtn;
	TXButton* TXBtn;
	ExitButton* ExitBtn;

	// RX windows
	RadioWindow* Radio2_Win = NULL;
	RadioWindow* Radio3_Win = NULL;
	// TX Window
	TxWindow* TX_Win = NULL;

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

//==============================================================================
// The CAT enable button
class CATButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	CATButton(MainWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~CATButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	Preferences* p;
	std::thread* cat;
	CATThrd* t;
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
// The TX button
class TXButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	TXButton(MainWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~TXButton() {};
	int handle(int event);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	Preferences* p;
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
// Exit button
class ExitButton : public Fl_Button
{
public:
	//==============================================================================
	ExitButton(MainWindow* parent_widget, char* button_label, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_col);
	~ExitButton() {};
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