/*
layout_manager.cpp

Grid layout manager

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

/*
	FLTK does not have any layout managers. The declared way is to use Fluid to layout 
	and code the GUI classes. The reason is that it's impossible to manually code with
	absolute coodinates. One way to achieve this is to use Fluid to do the layout, generate
	the code and then cut and paste the coordinates. This becomes very tedious and difficult
	to maintain when things move around. However, a grid layout can take the tedium out of
	this by calculating the widget cooddinates. This class is just a calculator, it does
	nothing with FLTK itself. As FLTK already has good resize algorithms there is never a 
	need to recalculate after widgets are placed in the initial grid. 

	A grid layout manager is really all that is required as it can also do vertical and
	horizontal layouts by having one column or one row. 
*/

//==============================================================================
// PUBLIC
//==============================================================================
// Constructor
GridLayout::GridLayout(int x, int y, int w, int h, int rows, int cols) {
	// Start simple with the default which is all cells the same size and
	// allocation of one widget to one cell. Weights, row and column spanning
	// and margins can be set by additional calls after construction and 
	// before assigning widgets.
	// This is a calculator and does not place widgets itself but returns a
	// position and size for the caller to create the widget.
	origin_x = x;
	origin_y = y;
	area_width = w;
	area_height = h;
	grid_rows = rows;
	grid_cols = cols;
	// Calculate the default coordinates and position of each cell.
	calculate_cell_metrics();
}

//==============================================================================
// Return cell metrics
metrics GridLayout::get_cell_metrics(int row, int col) {
	m.x = origin_x + row_width * col;
	m.y = origin_y + row_height * row;
	m.w = row_width;
	m.h = row_height;
		
	return m;
}

//==============================================================================
// PRIVATE

// Calculate metrics for current state
void GridLayout::calculate_cell_metrics() {
	// At present this is the simple form of unisize layout with no margins
	// weighting or spanning.
	// Calculate in-line for now
	row_width = area_width / grid_cols;
	row_height = area_height / grid_rows;
}