/*
 * pcalpapr.c - 'paper size' and related routines for Pcal
 *
 * Contents:
 *
 *		recalc_paper_parameters
 *
 * Revision history:
 *
 *	4.8.0	B.Marr	2004-12-03	Created.
 *
 */

/*
 * Standard headers:
 */

#include <stdio.h>

/*
 * Pcal-specific definitions:
 */

#include "pcaldefs.h"
#include "pcalglob.h"
#include "pcalpapr.h"

/*
 * Local definitions:
 */

/*
 * Define the values associated with each of the supported paper sizes.  
 * 
 * All dimensions are provided in typographic points (72nds of an inch).
 * 
 * Values defined here include the common name associated with that paper
 * size, the page dimensions (shortest first), and the dimensions (width,
 * height) of a 'day box' on a monthly-format calendar printed in landscape
 * orientation.
 */
paper_info_str_typ paper_info[] = {
	{ "Letter",		/* name of this paper size */
	  612, 792,		/* U.S. 'Letter' = 8.5 x 11.0 inches */
	  100, 80		/* 'daybox' width and height */
	},
	{ "Legal",		/* name of this paper size */
	  612, 1008,		/* U.S. 'Legal' = 8.5 x 14.0 inches */
	  130, 80		/* 'daybox' width and height */
	},
	{ "A4",			/* name of this paper size */
	  595, 842,		/* International 'A4' = 210 x 297 mm */
	  108, 77		/* 'daybox' width and height */
	},
	{ "Tabloid",		/* name of this paper size */
	  792, 1224,		/* 'Tabloid'/'Ledger' = 11.0 x 17.0 inches */
	  161, 110		/* 'daybox' width and height */
	},
};

/* Define the default paper size. */
int paper_size = PAPERSIZE_DEFAULT;

/* These variables keep track of the page dimensions in typographic points
 * (72nds of an inch).
 * 
 * Note: Because of the possibility of different page orientations ('portrait'
 * and 'landscape'), to avoid confusion, these variables are named and used
 * independently of page orientation.  That is, they're not termed 'width' and
 * 'height' (which change depending on the orientation) but instead refer to
 * the 'long' and 'short' axes of the paper (which don't change with
 * orientation).
 */
int page_dim_short_axis_pts, page_dim_long_axis_pts;

/* Define the width and height of a 'day box' (i.e. a box which represents a
 * single day within a given month) in typographic points.
 * 
 * Note: These values change depending on whether the selected orientation is
 * 'portrait' or 'landscape'.
 */
int daybox_width_pts, daybox_height_pts;

/*
 * recalc_paper_parameters - set up all the variables which are associated
 * with a new setting of 'paper size'
 */
void
#ifdef PROTOS
recalc_paper_parameters(int paper_size_idx)
#else
recalc_paper_parameters(paper_size_idx)
	int paper_size_idx;
#endif
{
	page_dim_short_axis_pts = paper_info[paper_size_idx].page_dim_short_axis_pts;
	page_dim_long_axis_pts = paper_info[paper_size_idx].page_dim_long_axis_pts;
   
	daybox_width_pts = paper_info[paper_size_idx].daybox_width_pts;
	daybox_height_pts = paper_info[paper_size_idx].daybox_height_pts;

	xsval_pgm = ysval_pgm = (double)page_dim_short_axis_pts / (double)page_dim_long_axis_pts;
	xtval_pgm = (page_dim_long_axis_pts - (7 * daybox_width_pts)) / 2;
	
	if ((rotate == PORTRAIT) && do_whole_year) {
		ytval_pgm = page_dim_short_axis_pts + TOP_OF_CAL_BOXES_PTS + 
			6 * (daybox_height_pts - paper_info[paper_size_idx].daybox_height_pts);
	}
	else if ((rotate == PORTRAIT) && !do_whole_year) {
	/* 
	 * Copied from the 'ReadMe' file for v4.6:
	 * 
	 *    The date box height for single-month portrait-mode calendars has
	 *    been expanded from 80 points to 117.  If you prefer the original
	 *    date box proportions (basically a scaled-down landscape-mode
	 *    calendar), compile with -DORIG_BOX_HEIGHT.
	 */
#ifndef ORIG_BOX_HEIGHT
		daybox_height_pts = (page_dim_long_axis_pts - DEFAULT_MARGIN_BOTTOM_PTS + 
			TOP_OF_CAL_BOXES_PTS) / 6 / ysval_pgm;
#endif
		ytval_pgm = (page_dim_long_axis_pts + TOP_OF_CAL_BOXES_PTS) / ysval_pgm;
	}
	else {	/* landscape mode */
		xsval_pgm = ysval_pgm = 1.0;
		ytval_pgm = TOP_OF_CAL_BOXES_PTS;
	}
	return;
}
