/*
tx_window.h

Single TX window header for the FLTK Console

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
class DuplexButton;
class MOXButton;
class RFSlider;
class TxWindow : public WindowBase
{
public:
	//==============================================================================
	TxWindow(int radio, int x, int y, int w, int h);
	~TxWindow() {};
	void handle_idle_timeout();
	void close();
	void do_mox();
	void mox_off();
	void mox_on();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// labels must be fixed storage
	// Radio inst
	int r;
	// Buttons
	char duplex_str_on[10] = "Duplex";
	char duplex_str_off[10] = "Simplex";
	char mox_str_on[5] = "TX";
	char mox_str_off[5] = "RX";
	char rf_gain_str[5] = "Gain";
	
	// Preferences
	Preferences* p;

	// Ref to set radio parameters
	RadioInterface* r_i;
	// Additional TX only buttons
	DuplexButton* DuplexBtn;
	MOXButton* MOXBtn;
	RFSlider* RFGain;

	// Components

	//==============================================================================
	// Method prototypes

};

//==============================================================================
// The duplex button
class DuplexButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	DuplexButton(TxWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~DuplexButton() {};
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
	TxWindow* myparent;

	//==============================================================================
	// Method prototypes
};

//==============================================================================
// The MOX button
class MOXButton : public Fl_Toggle_Button
{
public:
	//==============================================================================
	MOXButton(TxWindow* parent_widget, char* button_up_label, char* button_down_label, int button_id, int x, int y, int w, int h, Fl_Color back_col, Fl_Color button_up_col, Fl_Color button_down_col);
	~MOXButton() {};
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
	TxWindow* myparent;

	//==============================================================================
	// Method prototypes
};

//==============================================================================
// The RF gain slider
class RFSlider : public Fl_Value_Slider
{
public:
	//==============================================================================
	RFSlider(TxWindow* parent_widget, char* label, int x, int y, int w, int h, Fl_Color back_col);
	~RFSlider() {};
	void handle_event();

	//==============================================================================

private:
	//==============================================================================
	// State variables
	RadioInterface* r_i;
	TxWindow* myparent;

	//==============================================================================
	// Method prototypes
};