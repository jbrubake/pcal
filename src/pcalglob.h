/*
 * pcalglob.h - definitions and declarations of globals used throughout Pcal
 *
 * Revision history:
 *
 *	4.8.0	B.Marr	2004-12-04	Create separate variables for X/Y 
 *					scaling and X/Y translation done by
 *					the program to distinguish from the
 *					X/Y scaling and X/Y translation
 *					specified by the user.  Remove spaces
 *					embedded within tab fields.
 * 
 *	4.7.1	SF	01/06/2003	added support for -q flag
 *
 *	4.7	AWR	03/08/1998	redefine several globals as numeric
 *					instead of string (cf. pcaldefs.h)
 *
 *			07/27/1997	replace calendar_out global with
 *					output_type (PostScript, HTML, or
 *					Un*x 'calendar' utility output)
 *
 *	4.6	AWR	10/03/1995	Add globals "init_month", "init_year",
 *					"nmonths", "final_month", "final_year"
 *
 *	4.5	AWR	02/05/1993	Added support for -# flag (multiple
 *					copies of output page)
 *
 *		AWR	11/16/1992	Use common "protos.h" file (q.v.)
 *
 *	4.4	AWR	01/20/1992	Added support for -z flag and revised
 *					-[bgGO]
 *
 *	4.3	AWR	12/05/1991	Add global "progpath" (used when
 *					searching for moon file)
 *
 *		AWR	12/03/1991	Added support for -s flag
 *
 *	4.2	AWR	10/08/1991	Added support for -[kK] flags
 *
 *		AWR	10/02/1991	Added support for -c, -N, -S flags
 *
 *	4.1	AWR	07/16/1991	Added support for -G flag
 *
 *	4.0	AWR	01/28/1991	Added support for -b and -w flags
 *
 *		AWR	01/15/1991	Extracted from pcal.c
 *
 */

#include "protos.h"

/*
 * Definitions and/or declarations of globals
 */

#ifdef MAIN_MODULE

/*
 * Misc. globals
 */

year_info *head = NULL;		/* head of internal data structure */
int curr_year;			/* current default year for date file entries */
int init_month;			/* starting month */
int init_year;			/* starting year */
int nmonths;			/* number of months */
int final_month;		/* ending month (calculated) */
int final_year;			/* ending year (calculated) */
double xsval_pgm;		/* X scaling value calculated by program */
double ysval_pgm;		/* Y scaling value calculated by program*/
int xtval_pgm;			/* X translation value calculated by program */
int ytval_pgm;			/* Y translation value calculated by program */
char *words[MAXWORD];		/* maximum number of words per date file line */
char lbuf[LINSIZ];		/* date file source line buffer */
char progname[STRSIZ];		/* program name (for error messages) */
char progpath[STRSIZ];		/* directory where executable lives */
char version[20];		/* program version (for info messages) */

/* lengths and offsets of months in common year */
char month_len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
short month_off[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

/* dispatch table for wildcard matching routines - cf. pcaldefs.h */
#ifdef PROTOS
int (*pdatefcn[])(int, int, int) = {
#else
int (*pdatefcn[])() = {
#endif
	is_anyday, is_weekday, is_workday, is_holiday, not_weekday, not_workday,
	not_holiday, is_newmoon, is_firstq, is_fullmoon, is_lastq
	};

/*
 * Default values for command-line options:
 */

char default_color[] = DAY_COLOR;	/* -b, -g, -G, -O */
char day_color[7];
int holiday_color;
int weekday_color = WEEKDAY_COLOR;

int datefile_type = SYS_DATEFILE;	/* -e, -f */
char datefile[STRSIZ] = "";

int rotate = ROTATE;			/* -l, -p */

int draw_moons = DRAW_MOONS;		/* -m, -M */

char datefont[STRSIZ] = DATEFONT;	/* -d, -t, -n */
char titlefont[STRSIZ] = TITLEFONT;
char notesfont[STRSIZ] = NOTESFONT;

int mapfonts = MAPFONTS;		/* -r */

char shading[STRSIZ] = SHADING;		/* -s */

char lfoot[STRSIZ] = LFOOT;             /* -L, -C, -R */
char cfoot[STRSIZ] = CFOOT;
char rfoot[STRSIZ] = RFOOT;

char notes_hdr[STRSIZ];			/* -N (initialized in main()) */

int first_day_of_week = FIRST_DAY;	/* -F */

int date_style = DATE_STYLE;		/* -A, -E */

char outfile[STRSIZ] = "";		/* -o */

double xsval_user = 1.0;		/* -x, -y, -X, -Y */
double ysval_user = 1.0;
int xtval_user = 0;
int ytval_user = 0;

int julian_dates = JULIAN_DATES;	/* -j */

int do_whole_year = DO_WHOLE_YEAR;	/* -w */

int output_type = OUTPUT_TYPE;		/* -c, -H */

int one_column = 0;			/* -q */

int blank_boxes = BLANK_BOXES;		/* -B */

int ncopy = NCOPY;			/* -# */

int small_cal_pos = SMALL_CAL_POS;	/* -k, -K, -S */
int prev_cal_box[4] = PREV_CAL_BOX;
int next_cal_box[4] = NEXT_CAL_BOX;

char time_zone[STRSIZ] = TIMEZONE;	/* -z */
double utc_offset;
int tz_flag = FALSE;

int debug_flags = 0;			/* -Z */

#else

/*
 * Misc. globals
 */

extern year_info *head;		/* head of internal data structure */
extern int curr_year;		/* current default year for date file entries */
extern int init_month;		/* starting month */
extern int init_year;		/* starting year */
extern int nmonths;		/* number of months */
extern int final_month;		/* ending month (calculated) */
extern int final_year;		/* ending year (calculated) */
extern double xsval_pgm;	/* X scaling value calculated by program */
extern double ysval_pgm;	/* Y scaling value calculated by program*/
extern int xtval_pgm;		/* X translation value calculated by program */
extern int ytval_pgm;		/* Y translation value calculated by program */
extern char *words[];		/* maximum number of words per date file line */
extern char lbuf[];		/* date file source line buffer */
extern char progname[];		/* program name (for error messages) */
extern char progpath[];		/* directory where executable lives */
extern char version[];		/* program version (for info messages) */

/* lengths and offsets of months in common year */
extern char month_len[];
extern short month_off[];

/* dispatch functions and table for wildcard processing */
#ifdef PROTOS
extern int (*pdatefcn[])(int, int, int);
#else
extern int (*pdatefcn[])(); 
#endif

/*
 * Default values for command-line options:
 */

extern char default_color[];		/* -b, -g, -G, -O */
extern char day_color[];
extern int holiday_color;
extern int weekday_color;

extern int datefile_type;		/* -e, -f */
extern char datefile[];

extern int rotate;			/* -l, -p */

extern int draw_moons;			/* -m, -M */

extern char datefont[];			/* -d, -t, -n */
extern char titlefont[];
extern char notesfont[];

extern int mapfonts;			/* -r */

extern char shading[];			/* -s */

extern char lfoot[];			/* -L, -C, -R */
extern char cfoot[];
extern char rfoot[];

extern char notes_hdr[];		/* -N */

extern int first_day_of_week;		/* -F */

extern int date_style;			/* -A, -E */

extern char outfile[];			/* -o */

extern double xsval_user;		/* -x, -y, -X, -Y */
extern double ysval_user;
extern int xtval_user;
extern int ytval_user;

extern int julian_dates;		/* -j */

extern int do_whole_year;		/* -w */

extern int output_type;			/* -c, -H */
	
extern int one_column;			/* -q */

extern int blank_boxes;			/* -B */

extern int ncopy;			/* -# */

extern int small_cal_pos;		/* -k, -K, -S */
extern int prev_cal_box[];
extern int next_cal_box[];

extern char time_zone[];		/* -z */
extern double utc_offset;
extern int tz_flag;

extern int debug_flags;			/* -Z */

#endif

