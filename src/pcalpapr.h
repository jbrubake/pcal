/*
 * pcalpapr.h - 'paper size' and related definitions for Pcal program
 *
 * Revision history:
 *
 *	4.8.0	B.Marr		2004-12-03	Created.
 *
 */

/*
 * Define enumerations for the supported paper sizes.  The order of entries in
 * the 'paper_info[]' array must correspond to these enumerations.
 */ 
#define NUM_PAPER_SIZES		4	/* *** Change this if adding a new paper size. *** */
#define PAPERSIZE_LETTER	0
#define PAPERSIZE_LEGAL		1
#define PAPERSIZE_A4		2
#define PAPERSIZE_TABLOID	3

#define PAPERSIZE_DEFAULT	PAPERSIZE_LETTER

/* 
 * Define the distance (in typographic points) from the top edge of the paper
 * to the top of the calendar boxes for a monthly calendar in landscape
 * orientation.
 * 
 * The value defined here includes the top margin, rather than defining proper
 * margins and using them generically.  This is rather a inelegant way of
 * doing it, but it's been done that way for a long time.
 */
#define TOP_OF_CAL_BOXES_PTS	(-85)

/* 
 * Define the default left, right, top, and bottom margins (in typographic
 * points).
 * 
 * Some of these values are not currently used, but are left here for a time
 * when 'pcal' might be modified to provide proper, flexible margins.
 */
#define DEFAULT_MARGIN_LEFT_PTS		46
#define DEFAULT_MARGIN_RIGHT_PTS	46
#define DEFAULT_MARGIN_TOP_PTS		46
#define DEFAULT_MARGIN_BOTTOM_PTS	46

/*
 * Define the structure which holds the information about the various settings
 * for the various paper sizes.
 */ 
typedef struct {
	char *name;
	int page_dim_short_axis_pts;
	int page_dim_long_axis_pts;
	int daybox_width_pts;
	int daybox_height_pts;
} paper_info_str_typ;

/*
 * External references to variables defined in 'pcalpapr.c'
 */
extern paper_info_str_typ paper_info[];
extern int paper_size;
extern int page_dim_short_axis_pts, page_dim_long_axis_pts;
extern int daybox_width_pts, daybox_height_pts;
