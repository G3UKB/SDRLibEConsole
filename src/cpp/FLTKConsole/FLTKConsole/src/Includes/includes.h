/*
includes.h

Header of headers for the FLTK Console

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

// System includes
#include <iostream>
#include <string>
#include <map>

// FLTK includes
#ifdef linux
	#include <FL/Fl.H>
	#include <FL/Fl_Window.H>
	#include <FL/Fl_Double_Window.H>
	#include <FL/Fl_Group.H>
	#include <FL/Fl_Box.H>
	#include <FL/Fl_Button.H>
	#include <FL/Fl_Toggle_Button.H>
#else
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/names.h"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Window.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Double_Window.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Group.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Box.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Button.H"
	#include "E:/DevelopmentResources/FLTK/fltk-1.3.5/FL/Fl_Toggle_Button.H"
#endif

// Application includes
#include "extern.h"
#include "radio_interface.h"
#include "toggle_base.h"
#include "vfo.h"
#include "layout_manager.h"
#include "modes.h"
#include "main_window.h"
