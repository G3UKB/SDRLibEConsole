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
GridLayout::GridLayout(int x, int y, int w, int h, int rows, int cols, int margins) {
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
	cell_margins = margins;
	// Calculate the default coordinates and position of each cell.
	calculate_cell_metrics();
}

//==============================================================================
// Return cell metrics for a single cell
metrics GridLayout::get_cell_metrics(int row, int col) {
	m.x = (origin_x + grid_metrics.cells[row][col].w * col) + grid_metrics.cells[row][col].left;
	m.y = (origin_y + grid_metrics.cells[row][col].h * row) + grid_metrics.cells[row][col].top;
	m.w = grid_metrics.cells[row][col].w - grid_metrics.cells[row][col].left - grid_metrics.cells[row][col].right;
	m.h = grid_metrics.cells[row][col].h - grid_metrics.cells[row][col].top - grid_metrics.cells[row][col].bottom;

	return m;
}

// Return cell metrics for a spanned cell
// Note, this works for now but when weighting is applied will need to accumulate cols and rows size
metrics GridLayout::get_cell_metrics(int start_row, int start_col, int num_rows, int num_cols) {
	m.x = (origin_x + grid_metrics.cells[start_row][start_col].w * start_col) + grid_metrics.cells[start_row][start_col].left;
	m.y = (origin_y + grid_metrics.cells[start_row][start_col].h * start_row) + grid_metrics.cells[start_row][start_col].top;
	m.w = (grid_metrics.cells[start_row][start_col].w * num_cols) - grid_metrics.cells[start_row][start_col].left - grid_metrics.cells[start_row + num_rows - 1][start_col + num_cols - 1].right;
	m.h = (grid_metrics.cells[start_row][start_col].h * num_rows) - grid_metrics.cells[start_row][start_col].top - grid_metrics.cells[start_row + num_rows - 1][start_col + num_cols - 1].bottom;

	return m;
}

//==============================================================================
// PRIVATE

// Calculate metrics for current state
void GridLayout::calculate_cell_metrics() {
	// At present this is the simple form of unisize layout.
	// Margins and spanning are implemented
	// Weighting is not yet implemented but this structure allow for it.
	int row, col;

	for (row = 0 ; row < grid_rows ; row++) {
		for (col = 0 ; col < grid_cols ; col++) {
			grid_metrics.cells[row][col].w = area_width / grid_cols;
			grid_metrics.cells[row][col].h = area_height / grid_rows;
			// All margins set to the common value for now. Default 0;
			grid_metrics.cells[row][col].left = cell_margins;
			grid_metrics.cells[row][col].right = cell_margins;
			grid_metrics.cells[row][col].top = cell_margins;
			grid_metrics.cells[row][col].bottom = cell_margins;
		}
	}
}