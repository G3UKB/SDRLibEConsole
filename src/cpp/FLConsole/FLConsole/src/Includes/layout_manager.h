/*
layout_manager.h

Header for layout managers

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

// Cell metric 
typedef struct grid_cell {
	// Width/height of cell
	int w;
	int h;
	// Margins
	int left;
	int right;
	int top;
	int bottom;
};

typedef struct grid_cells {
	// Allow 10 rows and 10 cols for 100 cells
	grid_cell cells[10][10];
};

// Return from get_cell_metrics()
typedef struct metrics {
	int x;
	int y;
	int w;
	int h;
};

/*
	A grid layout manager
*/
class GridLayout
{
public:
	//==============================================================================
	GridLayout(int x, int y, int w, int h, int rows, int cols, int margins = 0);
	~GridLayout() {};

	// Return cell metrics for single or spanned cells
	metrics get_cell_metrics(int row, int col);
	metrics get_cell_metrics(int start_row, int start_col, int num_rows, int num_cols);

	//==============================================================================

private:
	//==============================================================================
	// State variables
	// Parameters
	int origin_x;
	int origin_y;
	int area_width;
	int area_height;
	int grid_rows;
	int grid_cols;
	int cell_margins;

	// Intermediates
	int row_width;
	int row_height;

	// Metrics
	metrics m;
	// The grid_metrics instance
	grid_cells grid_metrics;

	//==============================================================================
	// Method prototypes
	void calculate_cell_metrics();
};