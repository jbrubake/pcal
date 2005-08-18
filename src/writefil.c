/*
 * writefil.c - Pcal routines concerned with writing the PostScript output
 *
 * Contents:
 *
 *		def_footstring
 *		expand_fmt
 *		find_daytext
 *		find_images
 *		find_noteboxes
 *		multiple_months_html
 *		print_colors
 *		print_dates
 *		print_db_word
 *		print_html
 *		print_julian_info
 *		print_month
 *		print_moon_info
 *		print_pstext
 *		print_text
 *		print_word
 *		select_color
 *		set_rgb
 *		single_month_html
 *		single_month_one_column_html
 *		write_calfile
 *		write_htmlfile
 *		write_psfile
 *
 * Revision history:
 *
 *	4.9.0
 *		B.Marr		2005-08-10
 *		
 *		Fix a long-standing bug whereby a centered "footer"
 *		specification with 'strftime()'-like date specifiers used in
 *		an HTML yearly calendar was using/showing the correct date
 *		values for the HTML 'title' but the wrong date values for the
 *		centered header string at the start of the actual displayed
 *		(HTML) content.
 *		
 *		Eliminate the hack to support Esperanto via a custom,
 *		dedicated character encoding.  Esperanto is now handled
 *		generically by the 'Latin3' (ISO 8859-3) character encoding.
 *		
 *		B.Marr		2005-08-02
 *		
 *		Per a user's request, change the separator character from a
 *		space to a tab when using the '-c' option (to output text
 *		lines which are compatible with the Unix 'calendar' program).
 *		
 *		B.Marr		2005-01-04
 *		
 *		Add support for several new character mappings (PostScript
 *		encoding vectors) in order to support a wider variety of
 *		languages.  Rename enumerations for all encodings to be more
 *		consistent (and easily searchable).
 * 
 *	4.8.0
 *		B.Marr		2004-12-04
 *		
 *		Support new paper sizes.  Support specification of paper size
 *		via run-time option (command-line, etc).  Create and support
 *		concept of 'input' language versus 'output' language.  Use
 *		separate variables for X/Y scaling and X/Y translation done by
 *		the program to distinguish from the X/Y scaling and X/Y
 *		translation specified by the user.
 *		
 *		B.Marr		2004-11-19
 *		
 *		Support KOI8U character encodings (for Ukrainian language
 *		support) and provide abbreviated day-of-week names, both based
 *		on a patch from Volodymyr M. Lisivka.  Provide support for
 *		embedded EPS images (photos, icons, etc) for monthly
 *		PostScript calendars.  Fix bug since v4.7.1 whereby use of
 *		'-q' flag required '-F 1' to prevent wrong weekday display.
 *		Remove spaces embedded within tab fields.  Remove Ctl-L (page
 *		eject) characters from source file.
 *
 *	4.7.1	SF	01/06/2003	html output with one column per month
 *					(cf. single_month_one_column_html)
 *
 *	4.7	AWR	02/09/2000	document block of code responsible for
 *					forcing Letter or A4 paper tray
 *
 *			10/27/1999	rearrange comment block - 'sdtimage'
 *					tool (possibly others) doesn't recognize
 *					%%Orientation unless it precedes %%Pages
 *
 *			04/19/1999	support #if DIVIDE_BLANK_SPACE (cf.
 *					pcaldefs.h) to print each partial
 *					row of blank space at beginning/end
 *					of calendar as N one-column boxes
 *					instead	of one N-column box
 *
 *			06/19/1998	support landscape- and portrait-mode
 *					whole-year calendars in HTML mode
 *
 *			03/08/1998	write additional definitions to
 *					PostScript output for use with
 *					alternate (just A4 at present) paper
 *					sizes
 *
 *			12/21/1997	clean up gcc warnings in -Wall mode
 *
 *			07/27/1997	revise for -H support (generate HTML
 *					table as output; cf. write_htmlfile()
 *					and print_html()); delete obsolete
 *					FPR and PRT macros
 *
 *	4.6	AWR	04/30/1996	suppress "For:" and "Routing:" comments
 *					if account name is "nobody" (typically
 *					when pcal is run from a CGI script)
 *
 *			04/22/1996	use alternate (taller) date box
 *					height for single-month calendars in
 *					portrait mode
 *
 *			12/02/1995	drop asterisks from -c output (Un*x
 *					"calendar" utility interprets them
 *					as wildcards) unless KEEP_ASTERISKS
 *					is defined
 *
 *			11/14/1995	if OUTLINE_BLACK is defined, display
 *					-O dates as black outlines (not the
 *					specified color) in RGB mode (cf.
 *					pcalinit.ps)
 *
 *			10/03/1995	use globals for initial month, year,
 *					number of months (cf. pcalglob.h,
 *					pcal.c)
 *
 *			05/09/1995	add support for font style escape
 *					sequences (.[bir]; cf. pcalinit.ps)
 *
 *		AH	02/03/1995	add multiple language support
 *
 *
 *	4.5	AWR	11/15/1994	select gray/RGB independently for dates
 *					and fill boxes (cf. pcalinit.ps)
 *
 *			04/05/1994	select real vs. dummy PostScript code
 *					within write_psfile() (cf. pcalinit.ps)
 *
 *			11/30/1993	pre-scale all fonts used (as suggested
 *					by Andrew Houghton; cf. pcalinit.ps)
 *
 *			11/24/1993	replace find_holidays() with
 *					print_colors() (cf. drawnums{} in
 *					pcalinit.ps)
 *
 *			11/16/1993	Add set_rgb() to handle red:green:blue
 *					values for date/fill colors
 *
 *			09/23/1993	Support both ROMAN-8 and LATIN-8 font
 *					mappings
 *
 *		AWR	07/09/1993	Revised PostScript comment block
 *
 *		AWR	03/01/1993	add optional mapping of 8-bit fonts
 *
 *		AWR	02/05/1993	Support -# flag (multiple copies of
 *					each output page)
 *
 *		AWR	04/22/1992	use STRSIZ for temp buffer size
 *
 *	4.4	AWR	04/07/1992	revise to use new PUTCHAR and PUTSTR
 *					macros (cf. pcaldefs.h)
 *
 *			01/20/1992	support -z and revised -[bgGO] flags
 *
 *		AWR	01/13/1992	support optional font size in -d and
 *					-t flags; move initialization of fonts
 *					and sizes here (from pcalinit.ps)
 *
 *	4.3	AWR	12/03/1991	add support for -s flag (specify
 *					alternate date/fill box shading values)
 *
 *	4.2	AWR	10/08/1991	add support for -[kK] flags (change
 *					position of small calendars)
 *
 *			10/03/1991	add find_noteboxes(); revise to print
 *					text in multiple notes boxes
 *
 *					add support for -S flag
 *
 *			10/02/1991	modify def_footstring() to handle all
 *					types of strings; use it to print notes
 *					header (-N flag)
 *
 *			09/19/1991	add write_calfile(), print_dates(),
 *					and new print_text() to generate 
 *					input for Un*x "calendar" utility;
 *					renamed old print_text() as
 *					print_pstext() for clarity; revised
 *					to simplify setting working date
 *
 *	4.11	AWR	08/23/1991	revise expand_fmt() to write results
 *					to string instead of stdout; revise
 *					print_word() to avoid writing null
 *					strings
 *
 *		AWR	08/21/1991	use ABBR_DAY_LEN and ABBR_MONTH_LEN
 *					(cf. pcallang.h) to print abbreviated
 *					day/month names
 *
 *		AWR	08/21/1991	add %u and %w (calculate week number
 *					so that 1/1 is always week 1); support
 *					%[+-]<n>[DWMY] to adjust working date
 *					by +|- <n> days/weeks/months/years
 *
 *	4.1	AWR	08/16/1991	Support -G flag (outlined gray dates)
 *
 *	4.02	AWR	07/02/1991	Added "%" expansions in text strings
 *					(cf. expand_fmt())
 *
 *	4.0	AWR	01/28/1991	Support -B, -w flags and moon file
 *
 *			01/15/1991	Extracted from pcal.c
 *
 */

/*
 * Standard headers:
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/*
 * Pcal-specific definitions:
 */

#include "pcaldefs.h"
#include "pcalglob.h"
#define  WRITEFIL		/* to get ordinal_suffix() from pcallang.h */
#include "pcallang.h"
#include "pcalinit.h"		/* PostScript boilerplate, generated at
                                   compile time by 'pcalinit' */
#include "pcalpapr.h"

/*
 * Macros:
 */

/* make sure printf() doesn't round "ph" up to 1.0 when printing it */
#define PRT_TWEAK(ph)		((ph) >= 0.9995 ? 0.0 : (ph))

/* advance working date by n days */
#define SET_DATE(n)	do {						\
	MAKE_DATE(date, work_month, work_day + (n), work_year);		\
	normalize(&date);						\
	work_month = date.mm, work_day = date.dd, work_year = date.yy;	\
} while (0)

/* prescale a font and add its name to list */
#define ADDFONT(name, size, font, isarray)	do {			\
	char *p = alloc(strlen(name) + strlen(size) + 2);		\
	sprintf(allfonts[nfonts++] = p, "%s_%s", name, size);		\
	printf("/%s { %sfontsize ", p, font);				\
	if (isarray) printf("%s get ", size);				\
	printf("%sfont FontFind } def\n", font);			\
} while (0)

/* walk array of PostScript boilerplate strings and copy to stdout */
#define GEN_PSCODE(arr)							\
	for (ap = arr; *ap; ap++)					\
		printf("%s\n", *ap)

/* print PostScript string definition */
#define PRINT_DEF(name, value)	do {					\
	printf("/%s ", name);						\
	print_word(value);						\
	printf(" def\n");						\
} while (0)

/* add specified number of blank lines to HTML table entry */
#define BLANKLINES(n)  do {						\
	int i;								\
	for (i = 0; i < n; i++)						\
		printf("<br>&nbsp;");					\
} while(0)

#define FOOTSTRINGS()	(lfoot[0] || cfoot[0] || rfoot[0])

/* various PostScript comments and commands for start/end of page */

#ifdef EPS

/* macro definitions for generating EPS-like output */

#define PS_STARTPAGE()	do {						\
	page++;								\
	printf("%%%%Page: %d %d\n", page, page);				\
	printf("%%%%BeginPageSetup\n");					\
	printf("clear flush\n");						\
	printf("/PageNum { %d } def\n", page);				\
	printf("/PageState save def\n");					\
	printf("%%%%EndPageSetup\n");					\
} while (0)

#define PS_ENDPAGE()	do {						\
	printf("%%%%PageTrailer\n");					\
	printf("showpage\n");						\
	printf("clear flush\n");						\
	printf("PageState restore\n");					\
} while (0)

#define PS_TRAILER()	do {						\
	printf("%%%%Trailer\n");						\
	printf("clear flush\n");						\
	printf("%%%%EOF\n");						\
} while (0)

#else /* non-EPS flavors of above macros */
#define PS_STARTPAGE()
#define PS_ENDPAGE()		printf("showpage\n");
#define PS_TRAILER()
#endif

/* reset working date to original date */
#define RESET_DATE()	\
work_month = this_month, work_day = this_day, work_year = this_year

#define NEWFONT		"-8"	/* suffix for new 8-bit fonts */

#define MAXFONT		20	/* maximum number of fonts to prescale */

/*
 * Globals:
 */

/* order of following strings must conform to #define's in pcaldefs.h (q.v.) */
static char *cond[3] = {"false", "true", "(some)"};

static int this_day, this_month, this_year;	/* current day */
static int work_day, work_month, work_year;	/* working day (cf. expand_fmt()) */
static char *kw_note, *kw_opt, *kw_year;	/* keywords for -c output */

static int debug_text;				/* generate debug output */


/*
 * write_psfile - write PostScript code
 *
 * The actual output of the PostScript code is straightforward.  This routine
 * writes a PostScript header followed by declarations of all the PostScript
 * variables affected by command-line flags and/or language dependencies.  It
 * the generates the PostScript boilerplate generated from pcalinit.ps, and
 * finally calls print_month() to generate the PostScript code for each
 * requested month.
 *
 */
void
#ifdef PROTOS
write_psfile(void)
#else
write_psfile()
#endif
{
	int i, nfonts, nfsize, copies, dfltsize, color_dates, color_fill;
	char *p, **ap, tmp[STRSIZ], *allfonts[MAXFONT];
	struct tm *p_tm;
	time_t t;
#ifdef UN_X
	struct passwd *pw;
#endif
	/* default date, title, weekday font sizes (small/medium/large) */
	static int dsize[3] = DATEFONTSIZE;
	static int tsize[3] = TITLEFONTSIZE;
	static int wsize[3] = WEEKDAYFONTSIZE;
	static int fsize[3] = FOOTFONTSIZE;
	static int dmargin[3] = DATEMARGIN;
	static double gwidth[3] = GRIDLINEWIDTH;
	static char *calsize[3] = CALSIZE;

	debug_text = DEBUG(DEBUG_TEXT);		/* debug text output? */

	/*
	 * Write out PostScript prolog (including version/datefile stamp)
	 */

	/* comment block at top */

	printf("%%!%s\n", PS_RELEASE);		/* PostScript release */

	printf("%%%%CreationDate: ");		/* date/time stamp */
	p_tm = localtime((time(&t), &t));
	if (date_style == EUR_DATES)
		printf("%d.%d.", p_tm->tm_mday, p_tm->tm_mon + 1);
	else
		printf("%d/%d/", p_tm->tm_mon + 1, p_tm->tm_mday);
	printf("%02d %02d:%02d:%02d\n", p_tm->tm_year % 100, p_tm->tm_hour,
		p_tm->tm_min, p_tm->tm_sec);
	printf("%%%%Creator: %s %s %s\n", CREATED_MSG, progname, version);

#ifdef UN_X

	/* Generate "For" and "Routing" comments if user name is known */

	if ((pw = getpwuid(getuid())) != NULL &&
	    strcmp(pw->pw_name, NOBODY) != 0) {
		strcpy(tmp, pw->pw_gecos);
		if ((p = strchr(tmp, ',')) != NULL)
			*p = '\0';
		printf("%%%%For: %s\n", pw->pw_name);
		printf("%%%%Routing: %s\n", tmp);
	}
#endif

	/* Identify the output (month/year range and input file) */

	if (do_whole_year && init_month == JAN) {
		printf("%%%%Title: %s %d", TITLE_MSG, init_year);
		if (final_year > init_year)
			printf(" - %d", final_year);
	} else {
		char c = date_style == EUR_DATES ? '.' : '/';
		printf("%%%%Title: %s %d%c%02d", TITLE_MSG, init_month, c,
			init_year % 100);
		if (nmonths > 1)
			printf(" - %d%c%02d", final_month, c, final_year % 100);
	}
	if (*datefile)
		printf(DATEFILE_MSG, datefile);
	printf("\n");

	/* Miscellaneous other identification */

	printf("%%%%Orientation: %s\n", rotate == LANDSCAPE ? "Landscape" :
							   "Portrait");
	printf("%%%%Pages: %d\n", do_whole_year ? nmonths / 12 : nmonths);
	printf("%%%%PageOrder: Ascend\n");
	printf("%%%%BoundingBox: 0 0 %d %d\n", page_dim_short_axis_pts, page_dim_long_axis_pts);
	printf("%%%%DocumentPaperSizes: %s\n", paper_info[paper_size].name);
	printf("%%%%ProofMode: NotifyMe\n");
	printf("%%%%EndComments\n\n");

	/* number of copies (from -#<n> flag) */

	if ((copies = ncopy) > MAXCOPY)
		copies = MAXCOPY;		/* save a tree */
	if (copies > 1)
		printf("/#copies %d def\n", copies);

	/* paper size (force appropriate tray if known) */

	/* The following lines force the printer to select the specified paper
	 * tray (Letter or A4, as selected in the Makefile).  I (AWR) added it
	 * to v4.7 to fix a problem with one particular HP printer configured
	 * for both Letter and A4 size paper: under some conditions, previous
	 * A4 jobs would leave the printer in A4 mode unless I forced it back
	 * to Letter mode.  It is probably unnecessary for most users (and is
	 * known to confuse some previewers), so #ifdef it out if you like.
	 */
#if 1
	strcpy(tmp, paper_info[paper_size].name);
	tmp[0] = TOLOWER(tmp[0]);
	printf("statusdict /%stray known { statusdict begin %stray end } if\n",
		tmp, tmp);
#endif

	/* calendar sizes: to minimize number of pre-scaled fonts, whole-
	 * year calendars define 'medium' as 0 and the other sizes as -1
	 * (not used); single-month calendars define 'large' as 0, 'small'
	 * as 1, and 'medium' as -1 (not used)
	 */

	for (i = SMALL; i <= LARGE; i++)
		printf("/%s %d def\n", calsize[i],
			do_whole_year ? (i == MEDIUM ? 0 : -1) :
					(i == MEDIUM ? -1 : i == SMALL));

	/* font names and sizes */

	/* v4.4 supports user override of note and large date/title sizes */
	nfsize       = (p = strrchr(notesfont, '/')) ? *p++ = '\0', atoi(p) :
				atoi(strrchr(NOTESFONT, '/') + 1);
	dsize[LARGE] = (p = strrchr(datefont, '/')) ? *p++ = '\0', atoi(p) :
				atoi(strrchr(DATEFONT, '/') + 1);
	tsize[LARGE] = (p = strrchr(titlefont, '/')) ? *p++ = '\0', atoi(p) :
				atoi(strrchr(TITLEFONT, '/') + 1);

	/* enlarge footer strings in whole-year/portrait mode */
	if (do_whole_year && rotate == PORTRAIT)
		fsize[MEDIUM] = (int)((double)fsize[MEDIUM] * 1.25);

	/*
	 * if 8-bit remapping has been requested (-r flag), create new fonts
	 * with desired character remapping
	 */

	if (mapfonts != ENC_NONE) {

		/* include desired mapping */
		GEN_PSCODE(mapfonts == ENC_LATIN_1	? ps_iso8859_1 :
			   mapfonts == ENC_LATIN_2	? ps_iso8859_2 :
			   mapfonts == ENC_LATIN_3	? ps_iso8859_3 :
			   mapfonts == ENC_LATIN_4	? ps_iso8859_4 :
			   mapfonts == ENC_CYRILLIC	? ps_iso8859_5 :
			   /* mapfonts == ENC_ARABIC	? ps_iso8859_6 : */
			   mapfonts == ENC_GREEK	? ps_iso8859_7 :
			   /* mapfonts == ENC_HEBREW	? ps_iso8859_8 : */
			   mapfonts == ENC_LATIN_5	? ps_iso8859_9 :
			   mapfonts == ENC_LATIN_6	? ps_iso8859_10 :
			   mapfonts == ENC_THAI		? ps_iso8859_11 :
			   mapfonts == ENC_LATIN_7	? ps_iso8859_13 :
			   mapfonts == ENC_LATIN_8	? ps_iso8859_14 :
			   mapfonts == ENC_LATIN_9	? ps_iso8859_15 :
			   /* mapfonts == ENC_LATIN_10	? ps_iso8859_16 : */
			   mapfonts == ENC_KOI8_R	? ps_koi8_r :
			   mapfonts == ENC_KOI8_U	? ps_koi8_u :
							  ps_roman8);

		/* boilerplate to remap for 8-bit fonts */
		GEN_PSCODE(ps_remap);

		/* always generate code to remap title font */
		printf("/%s /%s%s remap_font\n", titlefont, titlefont, NEWFONT);
		strcat(titlefont, NEWFONT);

#if MAP_DATEFONT	/* any text printed in date font (cf. pcaldefs.h)? */
		/* generate code to remap date font if necessary */
		printf("FontDirectory /%s%s known not {\n", datefont, NEWFONT);
		printf("/%s /%s%s remap_font\n", datefont, datefont, NEWFONT);
		printf("} if\n");
		strcat(datefont, NEWFONT);
#endif

		/* generate code to remap notes font if necessary */
		printf("FontDirectory /%s%s known not {\n", notesfont, NEWFONT);
		printf("/%s /%s%s remap_font\n", notesfont, notesfont, NEWFONT);
		printf("} if\n");
		strcat(notesfont, NEWFONT);
	}

	/* define title, notes, and date fonts */
	printf("/titlefont /%s def\n", titlefont);
	printf("/datefont /%s def\n", datefont);
	printf("/notesfont /%s def\n", notesfont);

	/* typically defined in terms of above fonts - must define last */
	printf("/weekdayfont %s def\n", WEEKDAYFONT);
	printf("/footfont %s def\n", FOOTFONT);
	printf("/headingfont %s def\n", HEADINGFONT);

	/* print various font sizes and line/margin widths as PostScript arrays:
	 * one element for whole-year calendars; two (large, small) for single-
	 * month calendars
	 */

	if (do_whole_year) {
		printf("/datemargin [ %d ] def\n", dmargin[MEDIUM]);
		printf("/gridlinewidth [ %.1f ] def\n", gwidth[MEDIUM]);
		printf("/titlefontsize [ %d ] def\n", tsize[MEDIUM]);
		printf("/datefontsize [ %d ] def\n", dsize[MEDIUM]);
	} else {
		printf("/datemargin [ %d %d ] def\n", dmargin[LARGE], dmargin[SMALL]);
		printf("/gridlinewidth [ %.1f %.1f ] def\n", gwidth[LARGE], gwidth[SMALL]);
		printf("/titlefontsize [ %d %d ] def\n", tsize[LARGE], tsize[SMALL]);
		printf("/datefontsize [ %d %d ] def\n", dsize[LARGE], dsize[SMALL]);
	}

	dfltsize = do_whole_year ? MEDIUM : LARGE;

	printf("/weekdayfontsize %d def\n", wsize[dfltsize]);
	printf("/footfontsize %d def\n", fsize[dfltsize]);
	printf("/notesfontsize %d def\n", nfsize);
	printf("/headingfontsize %d def\n", HEADINGFONTSIZE);

	/* pre-scale all fonts used by PostScript code; try to be smart about
	 * skipping those that we know (at this point) won't be needed (whole-
	 * year calendars use either 3 or 4 fonts, while single-month calendars 
	 * can use anywhere from 3 to 8).  "FF" et. al. are indices into the
	 * font array (cf pcalinit.ps) for the different font types.
	 */

	printf("/FontFind { findfont exch scalefont } def\n");

	nfonts = 0;

	printf("/FF %d def\n", nfonts);			/* footers */
	if (FOOTSTRINGS())
		ADDFONT("ff", calsize[dfltsize], "foot", FALSE);

	if (do_whole_year) {
		printf("/TF %d def\n", nfonts);		/* month/year title */
		ADDFONT("tf", calsize[MEDIUM], "title", TRUE);

		printf("/DF %d def\n", nfonts);		/* dates */
		ADDFONT("df", calsize[MEDIUM], "date", TRUE);
	} else {
		printf("/HF %d def\n", nfonts);		/* 'Notes' heading */
		if (notes_hdr[0])
			ADDFONT("hf", calsize[LARGE], "heading", FALSE);

		/* large/small (if used) scalings of the same font must be
		 * contiguous and appear in that order
		 */

		printf("/TF %d def\n", nfonts);		/* large/small title */
		ADDFONT("tf", calsize[LARGE], "title", TRUE);
		if (small_cal_pos != SC_NONE)
			ADDFONT("tf", calsize[SMALL], "title", TRUE);

		printf("/DF %d def\n", nfonts);		/* large/small dates */
		ADDFONT("df", calsize[LARGE], "date", TRUE);
		if (small_cal_pos != SC_NONE)
			ADDFONT("df", calsize[SMALL], "date", TRUE);

		printf("/NF %d def\n", nfonts);		/* text in boxes */
		ADDFONT("nf", calsize[LARGE], "notes", FALSE);
	}

	printf("/WF %d def\n", nfonts);			/* weekdays */
	ADDFONT("wf", calsize[dfltsize], "weekday", FALSE);

	/* generate the font array (automatically in sync with above) */

	printf("/allfonts [\n\t");
	for (i = 0; i < nfonts; i++) {
		printf("%s ", allfonts[i]);
		free(allfonts[i]);
	}
	printf("\n] def\n");

	/*
	 * define various strings and numeric values used by Pcal
	 */

	/* month names */

	printf("/month_names [");
	for (i = JAN; i <= DEC; i++) {
		printf(i % 6 == 1 ? "\n\t" : " ");
		(void) print_word(months_ml[output_language][i-1]);
	}
	printf(" ] def\n");

	/* day names - abbreviate if printing entire year on page */

	printf("/day_names [");
	for (i = SUN; i <= SAT; i++) {
		printf((i % 6 == 0 && !do_whole_year) ? "\n\t" : " ");
		if (do_whole_year)
			strcpy(tmp, days_ml_short[output_language][(i + first_day_of_week) % 7]);
		else
			strcpy(tmp, days_ml[output_language][(i + first_day_of_week) % 7]);
		(void) print_word(tmp);
		}
	printf(" ] def\n");

	/* line separator and font change strings */

	PRINT_DEF("linesep", LINE_SEP);
	PRINT_DEF("boldfont", BOLD_FONT);
	PRINT_DEF("italicfont", ITALIC_FONT);
	PRINT_DEF("romanfont", ROMAN_FONT);

	/* page width and height (always referenced to 'portrait' mode) */

	printf("/pagewidth %d def\n", page_dim_short_axis_pts);
	printf("/pageheight %d def\n", page_dim_long_axis_pts);

	/* rotation, scaling, and translation factors */
 
	printf("/rval %d def\n", rotate);
	printf("/xsval %.3f def\n/ysval %.3f def\n", xsval_pgm * xsval_user, ysval_pgm * ysval_user);
	printf("/xtval %d def\n/ytval %d def\n", xtval_pgm + xtval_user, ytval_pgm + ytval_user);
	printf("/ytop %d def\n", -TOP_OF_CAL_BOXES_PTS);

	/* date box dimensions */

	printf("/daywidth %d def\n", daybox_width_pts);
	printf("/dayheight %d def\n", daybox_height_pts);

	/* moon, Julian date, and box fill flags */

	printf("/draw-moons %s def\n", cond[draw_moons]);
	printf("/julian-dates %s def\n", cond[julian_dates]);
	printf("/fill-boxes %s def\n", cond[! blank_boxes]);

	/* position of small calendars */

	printf("/prev_small_cal %d def\n", prev_cal_box[small_cal_pos]);
	printf("/next_small_cal %d def\n", next_cal_box[small_cal_pos]);

	/* date and fill box shading values */

	strcpy(tmp, shading);
	*(p = strchr(tmp, '/')) = '\0';
	printf("/dategray %s def\n", set_rgb(tmp));
	color_dates = strchr(tmp, RGB_CHAR) != NULL;
	printf("/fillgray %s def\n", set_rgb(++p));
	color_fill = strchr(p, RGB_CHAR) != NULL;

	/* PostScript boilerplate (part 1 of 1) */

	GEN_PSCODE(header);

	/* Additional PostScript code tailored to this calendar */

#ifdef OUTLINE_BLACK
	/* if OUTLINE_BLACK is defined, include an alternate prtday{}
	 * which prints outlined dates (-O) in black instead of the
	 * specified color; otherwise use the standard prtday{}
	 */
	GEN_PSCODE(color_dates ?
			ps_prtday_rgb_alt :	/* color prtday{} */
			ps_prtday_bw);		/* B&W prtday{} */
#else
	GEN_PSCODE(color_dates ?
			ps_prtday_rgb_orig :	/* color prtday{} */
			ps_prtday_bw);		/* B&W prtday{} */
#endif

	GEN_PSCODE(color_fill ?
			ps_setfill_rgb :	/* color setfill{} */
			ps_setfill_bw);		/* B&W setfill{} */

	GEN_PSCODE(FOOTSTRINGS() ?
			ps_footer :		/* at least one foot string */
			ps_nofooter);		/* no foot strings */

	GEN_PSCODE(blank_boxes ?
			ps_nofill :		/* blank fill boxes */
			ps_fill);		/* shaded fill boxes */

	if (do_whole_year) {
		GEN_PSCODE(rotate == LANDSCAPE ?
			   ps_year_l :		/* medium months (landscape) */
			   ps_year_p);		/* medium months (portrait)  */
		GEN_PSCODE(ps_nojulians);	/* no julians or moons */
		GEN_PSCODE(ps_nomoons);
	} else {
		GEN_PSCODE(julian_dates == NO_JULIANS ?
				ps_nojulians :		/* no julian dates */
				ps_julians);		/* some julian dates */
		GEN_PSCODE(draw_moons == NO_MOONS ?
				ps_nomoons :		/* no moons */
				ps_moons);		/* some or all moons */
		if (head)
			GEN_PSCODE(ps_text);		/* date text */
		GEN_PSCODE(ps_month);			/* single month */
	}

	/*
	 * Write out PostScript code to print calendars
	 */

	for (i = 0, this_month = init_month, this_year = init_year;
	     i < nmonths;
	     i++, BUMP_MONTH_AND_YEAR(this_month, this_year))
		print_month(this_month, this_year);

	/* generate trailer at end of PostScript output */
	PS_TRAILER();
}


/*
 * write_calfile - write dates in format suitable for Un*x "calendar" utility
 * (and subsequent use by Pcal)
 */
void
#ifdef PROTOS
write_calfile(void)
#else
write_calfile()
#endif
{
	KWD *k;
	int i;

	/* look up the Pcal keywords (assumed present) for the -c output file */
	for (k = keywds; k->name; k++) {
		if (k->code == DT_NOTE) kw_note = k->name;
		if (k->code == DT_OPT)  kw_opt  = k->name;
		if (k->code == DT_YEAR) kw_year = k->name;
	}

	/* print the date style for subsequent use by Pcal */
	printf("%s -%c\n", kw_opt, date_style == USA_DATES ? F_USA_DATES :
							  F_EUR_DATES);

	for (i = 0, this_month = init_month, this_year = init_year;
	     i < nmonths;
	     i++, BUMP_MONTH_AND_YEAR(this_month, this_year))
		print_dates(this_month, this_year);
}


/*
 * single_month_html - print the specified month and year as an HTML table
 */
void
#ifdef PROTOS
single_month_html(int this_month, int this_year)
#else
single_month_html(this_month, this_year)
	int this_month;
	int this_year;
#endif
{
	long holidays;
	int day, box, len;
	year_info *py;
	month_info *pm;
	register day_info *pd;

	/* table heading with month and year */
	printf("<a name=_%02d%02d>\n", this_month, this_year % 100);
	printf("<table width=100%% border=%d>\n", BORDER);
	printf("<tr><th colspan=7>%s", MONTHYEAR_PRE);
	print_html(months_ml[output_language][this_month-1]);
	printf(" %d%s</th></tr>\n", this_year, MONTHYEAR_POST);

	/* headings for weekday names */
	printf("<tr>");
	for (day = 0; day < 7; day++) {
		char buf[40];
		strcpy(buf, days_ml[output_language][(day + first_day_of_week) % 7]);
#if 0
		buf[ABBR_MONTH_LEN] = '\0';
#endif
		printf("<th width=14%%>");
		print_html(buf);
		printf("</th>");
	}
	printf("</tr>\n");

	/* blank space at beginning (if necessary) - divide into N one-column
	 * boxes (#if DIVIDE_BLANK_SPACE) or print as single N-column box
	 */
	printf("<tr>\n");
	if ((box = START_BOX(this_month, this_year)) > 0) {
#if DIVIDE_BLANK_SPACE
		int i;
		for (i = 0; i < box; i++) {
			printf("<td>");
			BLANKLINES(TEXTLINES);
			printf("</td>\n");
		}
#else
		printf("<td colspan=%d>", box);
		BLANKLINES(TEXTLINES);
		printf("</td>\n");
#endif
	}

	/* get pointer to text information for current month */
	pm = (py = find_year(this_year, FALSE)) != NULL ?
		py->month[this_month-1] : NULL;

	/* main loop for dates */
	len = LENGTH_OF(this_month, this_year);
	for (day = 1, holidays = pm ? pm->holidays : 0;
	     day <= len;
	     day++, box++, holidays >>= 1) {
		int nrows;

		if (box > 0 && box % 7 == 0)	/* start of row */
			printf("<tr>\n");
		printf("<td valign=top>");

		/* special HTML code for holidays (cf. pcaldefs.h) */
		if (holidays & 01)
			printf("%s%d%s", HOLIDAY_PRE, day, HOLIDAY_POST);
		else
			printf("%d", day);

		/* print associated text (count lines) */
		for (nrows = 0, pd = pm ? pm->day[day-1] : NULL;
		     pd;
		     pd = pd->next, nrows++) {
			/* Skip lines specifying an EPS image... */
			if (ci_strncmp(pd->text, "image:", 6) == 0) continue;
			printf("\n<br>");
			this_day = day;
			RESET_DATE();	/* reset working date */
			print_html(pd->text);
		}

		/* pad first box in each row with blank lines */
		if (box % 7 == 0 && nrows < TEXTLINES)
			BLANKLINES(TEXTLINES - nrows);

		printf("</td>\n");
		if (box % 7 == 6)		/* end of row */
			printf("</tr>\n");
	}

	/* blank space at end (if necessary) - divide into N one-column
	 * boxes (#if DIVIDE_BLANK_SPACE) or print as single N-column box
	 */
	if ((box %= 7) != 0) {
#if DIVIDE_BLANK_SPACE
		int i;
		for (i = box; i < 7; i++) {
			printf("<td>");
			BLANKLINES(TEXTLINES);
			printf("</td>\n");
		}
#else
		printf("<td colspan=%d>", 7 - box);
		BLANKLINES(TEXTLINES);
		printf("</td>\n");
#endif
	}
	printf("</tr>\n");
	printf("</table>\n");

	/* links to other months on page */
	if (nmonths > 2) {
		int j, m, y;
		char c = date_style == EUR_DATES ? '.' : '/';

		printf("<p>");
		for (j = 0, m = init_month, y = init_year;
		     j < nmonths;
		     j++, BUMP_MONTH_AND_YEAR(m, y))
			printf("<a href=#_%02d%02d>%02d%c%02d</a>%s\n",
				m, y % 100, m, c, y % 100,
				j % 12 == 11 ? "<br>" : "&nbsp;");
	}
}


/*
 * single_month_one_column_html - print the specified month and year as an
 * HTML table in one column
 */
void
#ifdef PROTOS
single_month_one_column_html(int this_month, int this_year)
#else
single_month_one_column_html(this_month, this_year)
	int this_month;
	int this_year;
#endif
{
	long holidays;
	int day, box = 0, len, i;
	year_info *py;
	month_info *pm;
	register day_info *pd;
	char *text_short;

	/* table heading with first 5 chars of the month */
	printf("<table border=%d>\n", BORDER);
	printf("<tr><th width=100px>%s</th></tr>\n", months_ml[output_language][this_month-1]);

	/* get pointer to text information for current month */
	pm = (py = find_year(this_year, FALSE)) != NULL ?
		py->month[this_month-1] : NULL;

	/* main loop for dates */
	len = LENGTH_OF(this_month, this_year);
	for (day = 1, holidays = pm ? pm->holidays : 0;
	     day <= len;
	     day++, box++, holidays >>= 1) {
		int nrows;

		// print only one column for the month
		printf("<tr>\n");
		printf("<td valign=top>");

		/* special HTML code for holidays (cf. pcaldefs.h) */
		if (holidays & 01)
			printf("%s%d%s %c", HOLIDAY_PRE, day, HOLIDAY_POST,
				days_ml[output_language][(day + START_BOX(this_month, this_year) + 
				first_day_of_week - 1) % 7][0]);
		else {
			/* print Sundays in bold */
			if ((day + START_BOX(this_month, this_year) + first_day_of_week - 1) % 7 == 0)
				printf("<B>%d %c</B>", day,
					days_ml[output_language][(day + START_BOX(this_month, this_year) + 
					first_day_of_week - 1) % 7][0]);
			/* print Saturdays in gray */
			else if ((day + START_BOX(this_month, this_year) + first_day_of_week - 1) % 7 == 6)
				printf("<B><FONT color=darkgray>%d</FONT> %c</B>", day,
					days_ml[output_language][(day + START_BOX(this_month, this_year) + 
					first_day_of_week - 1) % 7][0]);
			else
				printf("%d %c", day, days_ml[output_language][(day + 
					START_BOX(this_month, this_year) + first_day_of_week - 1) % 7][0]);
			/* print associated text (count lines),
			   not for holidays */
			for (nrows = 0, pd = pm ? pm->day[day-1] : NULL;
			     pd;
			     pd = pd->next, nrows++) {
				/* Skip lines specifying an EPS image... */
				if (ci_strncmp(pd->text, "image:", 6) == 0) continue;
				printf("\n<I>");
				this_day = day;
				RESET_DATE();	/* reset working date */
				text_short = pd->text;
				for  (i=0; i < 5; i++) {
					if(text_short[i] == '\0') break;
					// if text starts with < it is probably
					// an HTML tag (e.g. <I>) -> skip three
					// letters
					if(text_short[0] == '<' && text_short[i+3] == '\0') break;
					if(text_short[0] == '<')
						printf("%c",text_short[i+3]);
					else
						printf("%c",text_short[i]);
				}
				printf("</I>\n");
			}
		}

		// TR-tag to end the row
		printf("</td></TR>\n");
		if (box % 7 == 6)		/* end of row */
			printf("</tr>\n");
	}

	printf("</tr>\n");
	printf("</table>\n");

}


/*
 * multiple_months_html - print an HTML table in 'ncols' (assumed to be
 * either 3 or 4) columns, one column per month starting with the specified
 * month and year
 */
void
#ifdef PROTOS
multiple_months_html(int first_month, int first_year, int ncols)
#else
multiple_months_html(first_month, first_year, ncols)
	int first_month;
	int first_year;
	int ncols;
#endif
{
	long holidays;
	int day, box, len;
	year_info *py;
	month_info *pm;
	int i, m, d, y, w, wd1, wd2, nw;

	/* width of dates and spaces between months */
	wd1 = 12 / ncols;
	wd2 = (100 - (ncols * (7 * wd1))) / (ncols - 1);
	
	/* table heading with months and year */
	printf("<table width=100%% border=%d>\n", 0);
	
	printf("<tr>");
	for (i = 0, m = first_month, y = first_year;
	     i < ncols;
	     i++, BUMP_MONTH_AND_YEAR(m, y)) {
		if (i > 0)
			printf("<th><br></th>");
		printf("<th colspan=7>%s", MONTHYEAR_W_PRE);
		print_html(months_ml[output_language][m-1]);
		printf(" %d%s</th>\n", y, MONTHYEAR_W_POST);
	}
	printf("</tr>\n");

	/* headings for weekday names */
	printf("<tr>");
	for (i = 0; i < ncols; i++) {
		if (i > 0)
			printf("\n<th width=%d%%><br></th>", wd2);
		for (day = 0; day < 7; day++) {
			char buf[40];
			strcpy(buf, days_ml[output_language][(day + first_day_of_week) % 7]);
#if 0
			buf[ncols == 3 ? 3 : 2] = '\0';
#else
			buf[2] = '\0';		/* truncate to two characters */
#endif
			printf("<th width=%d%%>", wd1);
			print_html(buf);
			printf("</th>");
		}
	}
	printf("</tr>\n");

#ifdef CONSISTENT_SPACING
	/* always print 6 weeks (to maintain consistent vertical spacing) */
	nw = 6;
#else
	/* print 5 weeks unless at least one month requires a sixth week */
	nw = 5;
	for (i = 0, m = first_month, y = first_year;
	     i < ncols;
	     i++, BUMP_MONTH_AND_YEAR(m, y))
		if (START_BOX(m, y) + LENGTH_OF(m, y) > 35)
			nw = 6;
#endif

	/* main loop to print dates (one week at a time) */

	for (w = 0; w < nw; w++) {
		printf("<tr>");

		/* print one week of dates for each month */

		for (i = 0, m = first_month, y = first_year;
		     i < ncols;
		     i++, BUMP_MONTH_AND_YEAR(m, y)) {
			if (i > 0)
				printf("<td><br></td>");

			/* get starting box, month length, holiday info */
			box = START_BOX(m, y);
			len = LENGTH_OF(m, y);
			pm = (py = find_year(y, FALSE)) != NULL ?
			      py->month[m-1] : NULL;
			holidays = pm ? pm->holidays : 0;

			/* loop over days in current week and month */

			for (d = 1; d <= 7; d++) {
				day = (7 * w) + d - box;
				if (day < 1 || day > len)
					printf("<td><br></td>");
				else {
					printf("<td align=center>");
					if (holidays & (1L << (day - 1)))
						printf("%s%d%s", HOLIDAY_PRE,
							day, HOLIDAY_POST);
					else
						printf("%d", day);
					printf("</td>");
				}
			}
		}
		printf("</tr>\n");
	}

	/* end of table */
	printf("</table>\n");
}


/*
 * write_htmlfile - generate calendar in HTML format
 */
void
#ifdef PROTOS
write_htmlfile(void)
#else
write_htmlfile()
#endif
{
	int i;
	static struct {
		char *attribute;		/* attribute name */
		char *value;			/* attribute value */
		int   numeric;			/* TRUE if value is numeric */
	} *pb, body_attributes[] = {			/* cf. pcaldefs.h */
		{ "bgcolor",	BGCOLOR,	TRUE },
		{ "background",	BACKGROUND,	FALSE },
		{ "text",	TEXT,		TRUE },
		{ "link",	LINK,		TRUE },
		{ "alink",	ALINK,		TRUE },
		{ "vlink",	VLINK,		TRUE },
		{ NULL,		NULL,		FALSE }	/* must be last */
	};

	this_day = 1;			/* initialize working date */
	this_month = init_month;
	this_year = init_year;
	RESET_DATE();

	/* HTML boilerplate */
	printf("<html>\n");
	printf("<head>\n");

	/*
	 * if center footstring (-C) was specified, use it as the title;
	 * otherwise contrive title from beginning/ending month/year
	 */
	printf("<title>");
	if (cfoot[0]) {
		print_html(cfoot);
	}
	else {
		if (do_whole_year && init_month == JAN) {
			printf("%d", init_year);
			if (final_year > init_year)
				printf(" - %d", final_year);
		} else {
			char c = date_style == EUR_DATES ? '.' : '/';
			printf("%d%c%02d", init_month, c, init_year % 100);
			if (nmonths > 1)
				printf(" - %d%c%02d", final_month, c,
					final_year % 100);
		}
	}
	printf("</title>\n");
	printf("</head>\n");

	/* generate <body> definition with attributes */
	printf("<body");
	for (pb = body_attributes; pb->attribute; pb++) {
		char *p = pb->value;
		if (p && *p)
			printf(" %s=%s%s\"", pb->attribute,
				pb->numeric && p[0] != '#' ? "\"#" : "\"",
				pb->value);
	}
	printf(">\n");

	/* repeat center footstring (if specified) as heading */
	if (cfoot[0]) {
		RESET_DATE();
		printf("%s", HEADING_PRE);
		print_html(cfoot);
		printf("%s\n", HEADING_POST);
	}

	/* loop over all specified months, creating HTML table for each */
	this_month = init_month;
	this_year = init_year;

	if (do_whole_year) {
		/* whole-year mode - generate 3- or 4-column tables */
		int j, nc;

		nc = (rotate == LANDSCAPE) ? 4 : 3;
		for (i = 0; i < nmonths; i += nc) {
			if (i > 0)
				printf("<p>\n");
			multiple_months_html(this_month, this_year, nc);
			for (j = 0; j < nc; j++)
				BUMP_MONTH_AND_YEAR(this_month, this_year);
		}
	}
	else {
		/* single-month mode - generate one month per table
		 * either in one or several columns */
		if(one_column)
			printf("<TABLE><TR>");
		for (i = 0; i < nmonths; i++) {
			if(one_column) {
				printf("<TD valign=\"top\">");
				single_month_one_column_html(this_month, this_year);
				printf("</TD>");
			}
			else {
				printf("<TD valign=\"top\" width=\"%i%%\">",100/nmonths);
				if (i > 0)
					printf("<p><br>\n");
				single_month_html(this_month, this_year);
			}
			BUMP_MONTH_AND_YEAR(this_month, this_year);
		}
		printf("</TR></TABLE>");
	}

	/* boilerplate to terminate HTML file */
	printf("</body>\n");
	printf("</html>\n");
}

/*
 * low-level utilities for PostScript generation
 */

/*
 * set_rgb - convert "<r>:<g>:<b>" to [r g b] or "<gray>" to gray;
 * return pointer to static buffer containing converted string
 */
char *
#ifdef PROTOS
set_rgb(char *s)
#else
set_rgb(s)
	char *s;
#endif
{
	static char buf[STRSIZ];
	char *p1, *p2;
	double val[3];
	int n;

	val[0] = val[1] = val[2] = 0;		/* defaults */

	/* extract 1 - 3 floating-point values from string */
	for (n = 1, p1 = s; n <= 3; n++, p1 = p2 + 1) {
		val[n-1] = atof(p1);
		if ((p2 = strchr(p1, RGB_CHAR)) == NULL)
			break;
	}

	/* single value is gray scale; assume anything else is [r g b] */
	if (n > 1)
		sprintf(buf, "[%.3f %.3f %.3f]", val[0], val[1], val[2]);
	else
		sprintf(buf, "%.3f", val[0]);

	return buf; 
}

/*
 * select_color - if the holiday color has not been explicitly selected,
 * choose a color which contrasts with the majority of weekday colors
 */
int
#ifdef PROTOS
select_color(void)
#else
select_color()
#endif
{
	int i, min, index = BLACK;
	char count[NUM_COLORS];

	for (i = 0; i < NUM_COLORS; i++)		/* clear counts */
		count[i] = 0;

	for (i = SUN; i <= SAT; i++)			/* count colors */
		count[(int)day_color[i]]++;

	/* find smallest non-zero count; set its index and value */
	for (i = 0, min = 99; i < NUM_COLORS; i++)
		if (count[i] && count[i] < min)
			min = count[index = i];

	/* return least-used color (or pick one if only one color used) */
	return min == 7 ? index == BLACK ? GRAY : BLACK : index;
}


/*
 * expand_fmt - expand a strftime-like date format specifier; pcal supports
 * %[aAbBdjmUWyY] from strftime() plus %[luwDM] and prefixes [0o+-] (see
 * below); places expanded string in output buffer and returns pointer to
 * character following end of format specifier.  Assumes working date has
 * been initialized (via RESET_DATE() macro) prior to first call for a given
 * text string
 */
char *
#ifdef PROTOS
expand_fmt(char *buf,		/* output buffer (filled in)	    */
	   char *p)		/* character following percent sign */
#else
expand_fmt(buf, p)
	char *buf;		/* output buffer (filled in)	    */
	char *p;		/* character following percent sign */
#endif
{
	char c;
	static char *prefixes = "0o+-";
	int firstday, wkday;
	int adjust = 0, print_lz = FALSE, ordinal = FALSE, prev_num = -1;
	int num_present = FALSE, num_value = 0;
	DATE date;

	/* For compatibility with version 4.1, still support %[+-][bBdmY]
	 * (print the next/last month-name/day/month/year).  Version 4.11
	 * introduces %[+-]<n>[DWMY], which adjusts the working date by
	 * [+-]<n> days/weeks/months/years; this method is preferred due
	 * to its greater flexibility.
	 */

	buf[0] = '\0';		/* initialize output to null string */
	
	do {			/* loop until format character found */
		switch (c = *p++) {
		case 'a':	/* %a : abbreviated weekday */
			wkday = calc_weekday(work_month, work_day, work_year);
			strcpy(buf, days_ml_short[output_language][wkday]);
			break;

		case 'A':	/* %A : full weekday */
			wkday = calc_weekday(work_month, work_day, work_year);
			strcpy(buf, days_ml[output_language][wkday]);
			break;

		case 'b':	/* %b : abbreviated month name */
		case 'B':	/* %B : full month name */
			strcpy(buf, months_ml[output_language][(work_month + adjust + 11) % 12]);
			if (c == 'b')
				buf[ABBR_MONTH_LEN] = '\0';
			break;

		case 'd':	/* %d : day of month (01-31) */
			prev_num = work_day;
			sprintf(buf, print_lz ? "%02d" : "%d", prev_num);
			break;

		case 'D':	/* %D : adjust working date by <N> days (NEW) */
			if (!num_present || num_value == 0)
				RESET_DATE();
			else
				SET_DATE(adjust * num_value);
			break;

		case 'j':	/* %j : day of year (001-366) */
			prev_num = DAY_OF_YEAR(work_month, work_day,
					work_year);
			sprintf(buf, print_lz ? "%03d" : "%d", prev_num);
			break;

		case 'l':	/* %l : days left in year (000-365) (NEW) */
			prev_num = YEAR_LEN(work_year) - DAY_OF_YEAR(work_month,
					work_day, work_year);
			sprintf(buf, print_lz ? "%03d" : "%d", prev_num);
			break;

		case 'm':	/* %m : month (01-12) */
			prev_num = (work_month + adjust + 11) % 12 + 1;
			sprintf(buf, print_lz ? "%02d" : "%d", prev_num);
			break;

		case 'M':	/* %M : adjust date by <N> months (NEW) */
			if (!num_present || num_value == 0)
				RESET_DATE();
			else {
				int len;

				work_month += adjust * num_value;
				while (work_month > DEC) {
					work_month -= 12;
					work_year++;
				}
				while (work_month < JAN) {
					work_month += 12;
					work_year--;
				}

				/* make sure day of new month is legal */
				len = LENGTH_OF(work_month, work_year);
				if (work_day > len)
					work_day = len;
			}
			break;

		/* %u considers the week containing 1/1 to be week 1 and
		 * the next "logical Sunday" (the first day of the week
		 * as printed - cf. the -F option) to be the start of week
		 * 2; %U considers the first "logical Sunday" of the year to
		 * be the start of week 1.  %w and %W behave like %u and %U
		 * respectively, but use the first "logical Monday" instead.
		 */
		case 'W':	/* %W : week number (00-53)       */
			/* %W, if prefaced by [+-]N, adjusts the date by
			 * [+-]N weeks (resets if N == 0); check for this
			 * case first
			 */
			if (num_present) {
				if (num_value == 0)	/* N = 0: reset date */
					RESET_DATE();
				else
					SET_DATE(7 * adjust * num_value);
				break;
			}
			/* fall through */
		case 'u':	/* %u : week number (01-54) (NEW) */
		case 'U':	/* %U : week number (00-53)       */
		case 'w':	/* %w : week number (01-54) (NEW) */
			firstday = ((TOLOWER(c) == 'w' ? 15 : 14) -
					START_BOX(JAN, work_year)) % 7 + 1;
			prev_num = (DAY_OF_YEAR(work_month, work_day,
					work_year) - firstday + 7) / 7;
			if (islower(c) && firstday != 1)
				prev_num++;
			sprintf(buf, print_lz ? "%02d" : "%d", prev_num);
			break;

		case 'y':	/* %y : year w/o century (00-99) */
			prev_num = (work_year + adjust) % 100;
			sprintf(buf, "%02d", prev_num);
			break;

		case 'Y':	/* %Y : year w/century */
			/* %Y, if prefaced by [+-]N, adjusts the date by
			 * [+-]N years (resets if N == 0); check for this
			 * case first
			 */
			if (num_present) {
				if (num_value == 0)	/* N = 0: reset date */
					RESET_DATE();
				else {
					int len;

					work_year += adjust * num_value;

					/* make sure day is legal */
					len = LENGTH_OF(work_month, work_year);
					if (work_day > len)
						work_day = len;
				}
			} else {
				prev_num = work_year + adjust;
				sprintf(buf, "%d", prev_num);
			}
			break;

		/* prefix flags [o0+-] : set flags for next pass */

		case 'o':	/* %o : ordinal suffix (NEW) */
			ordinal = TRUE;
			break;

		case '0':	/* %0 : add leading zeroes (NEW) */
			print_lz = TRUE;
			break;

		case '+':	/* %+ : increment next value (NEW) */
		case '-':	/* %- : decrement next value (NEW) */
			adjust = c == '-' ? -1 : 1;
			if (isdigit(*p)) {		/* get the number */
				num_present = TRUE;
				while (isdigit(*p))
					num_value = num_value * 10 +
							(*p++ - '0');
			}
			break;

		case '\0':	/* accidental end-of-string */
		case ' ':
			return p - 1;

		default:	/* other - just copy it to output */
			sprintf(buf, "%c", c);
			break;
		};

	} while (strchr(prefixes, c) != NULL);

	/* append ordinal suffix if requested */
	if (ordinal && prev_num >= 0)
		strcat(buf, ordinal_suffix(prev_num));
	return p;

}


/*
 * print_word - print a single word, representing punctuation and non-ASCII
 * characters as octal literals and expanding format specifiers; return pointer
 * to character following word (NULL if no word follows)
 */
char *
#ifdef PROTOS
print_word(char *p)
#else
print_word(p)
	char *p;
#endif
{
	char c, buf[STRSIZ];
	int first = TRUE;	/* flag to avoid printing null strings */

	if (*p == '\0' || *(p += strspn(p, WHITESPACE)) == '\0')
		return NULL;

	while ((c = *p) && !isspace(c & CHAR_MSK)) {
		if (c == '%' && p[1] != '\0') {
			p = expand_fmt(buf, p + 1);
			if (*buf && first) {
				printf("(");
				first = FALSE;
			}
			PUTSTR(isalnum, buf, stdout);	
		} else {
			if (first)
				printf("(");
			first = FALSE;
			PUTCHAR(isalnum, c, stdout);
			p++;
		}
	}

	if (!first)
		printf(")");

	return p;
}


/*
 * print_db_word - debug version of print_word; omits parentheses, does not
 * convert punctuation to escape sequences, and writes results to stderr
 * (not stdout)
 */
char *
#ifdef PROTOS
print_db_word(char *p)
#else
print_db_word(p)
	char *p;
#endif
{
	char c, buf[STRSIZ];

	if (*p == '\0' || *(p += strspn(p, WHITESPACE)) == '\0')
		return NULL;

	while ((c = *p) && !isspace(c & CHAR_MSK)) {
		if (c == '%' && p[1] != '\0') {
			p = expand_fmt(buf, p + 1);
			PUTSTR(isprint, buf, stderr);
		} else {
			PUTCHAR(isprint, c, stderr);
			p++;
		}
	}

	return p;
}


/*
 * print_pstext - print tokens in text (assumed separated by single blank)
 * in PostScript format and as debugging information if requested
 */
void
#ifdef PROTOS
print_pstext(char *p)
#else
print_pstext(p)
	char *p;
#endif
{
	char *s = p;		/* save for possible second pass */

	while ((p = print_word(p)) != NULL)
		printf(" ");

	/* repeat to generate debugging info if requested */
	if (debug_text)
		while ((s = print_db_word(s)) != NULL)
			fprintf(stderr, " ");

}

/* 'clean' output */
#define PUTCHAR_CLEAN(_fc, _c, _fp) \
        fprintf((_fp), "%c", (_c) & CHAR_MSK)

#define PUTSTR_CLEAN(_fc, _s, _fp) \
	do { char *_p; for (_p = (_s); *_p; _p++) \
	PUTCHAR_CLEAN(_fc, *_p, _fp); } while (0)

/*
 * print_text - print text as supplied; expand format specifiers but
 * do not tokenize into words or translate punctuation to escape sequences
 */
void
#ifdef PROTOS
print_text(char *p)
#else
print_text(p)
	char *p;
#endif
{
	char c, buf[STRSIZ];

	while ((c = *p) != '\0')
		if (c == '%' && p[1] != '\0') {
			p = expand_fmt(buf, p + 1);
			/* AH: non-ascii for -c mode should pass clean! */
			PUTSTR_CLEAN(isprint, buf, stdout);
		} else {
			PUTCHAR_CLEAN(isprint, c, stdout);
			p++;
		}
}


/*
 * def_footstring - print definition for foot string, again converting 
 * all characters other than letters, digits, or space to octal escape
 * and expanding format specifiers
 */
void
#ifdef PROTOS
def_footstring(char *p,			/* definition */
	       char *str)		/* name of string */
#else
def_footstring(p, str)
	char *p;			/* definition */
	char *str;			/* name of string */
#endif
{
	char c, buf[STRSIZ];

	this_day = 1;			/* set default day in foot string */
	RESET_DATE();			/* reset working date */

	printf("/%s (", str);
	while ((c = *p) != '\0') {
		if (c == '%' && p[1] != '\0') {
			p = expand_fmt(buf, p + 1);
			PUTSTR(isalnum, buf, stdout);
		} else {
			PUTCHAR(isalnum, c, stdout);
			p++;
		}
	}
	printf(") def\n");
}


/*
 * Routines to extract and print data
 */


/*
 * find_daytext - find and print the day (including notes) or holiday text
 * for the specified month/year
 */
void
#ifdef PROTOS
find_daytext(int month,
	     int year,
	     int is_holiday)	/* TRUE: print holiday text */
#else
find_daytext(month, year, is_holiday)
	int month, year;
	int is_holiday;		/* TRUE: print holiday text */
#endif
{
	register int day;
	year_info *py;
	month_info *pm;
	register day_info *pd;
	int first;
	char *fcn = is_holiday ? "holidaytext" : "daytext";
	char hol = is_holiday ? '*' : ' ';

	/* if no text for this year and month, return */

	if ((py = find_year(year, FALSE)) == NULL ||
	    (pm = py->month[month-1]) == NULL)
		return;

	/* walk array of day text pointers and linked lists of text */

	for (day = 1; day <= LAST_NOTE_DAY; day++) {
		for (pd = pm->day[day-1], first = TRUE;
		     pd;
		     pd = pd->next) {
			if (pd->is_holiday != is_holiday)
				continue;
			/* Skip lines specifying an EPS image... */
			if (ci_strncmp(pd->text, "image:", 6) == 0) continue;
			if (first) {
				printf("%d [ \n", day >= FIRST_NOTE_DAY ?
					note_box(month, day, year) : day);
			}
			else {
				printf("\n");
				print_word(LINE_SEP);	/* separate lines */
				printf("\n");
			}
			this_day = day >= FIRST_NOTE_DAY ? 1 : day;
			RESET_DATE();		/* reset working date */
			if (debug_text) {
				if (day < FIRST_NOTE_DAY)
					fprintf(stderr, "%02d/%02d/%d%c ", month,
						day, year, hol);
				else
					fprintf(stderr, "%02d[%02d]%d  ", month,
						day - FIRST_NOTE_DAY + 1, year);
			}
			print_pstext(pd->text);
			if (debug_text)
				fprintf(stderr, "\n");
			first = FALSE;
		}
		if (! first) {		/* wrap up call (if one made) */
			printf("\n] %s\n", day >= FIRST_NOTE_DAY ? "notetext" :
								fcn);
		}
	}
}


/*
 * find_images - find and print the EPS images for the specified month/year
 */
void
#ifdef PROTOS
find_images(int month,
            int year)
#else
find_images(month, year)
	int month, year;
#endif
{
	register int day;
	year_info *py;
	month_info *pm;
	register day_info *pd;

	/* if no text for this year and month, return */

	if ((py = find_year(year, FALSE)) == NULL ||
	    (pm = py->month[month-1]) == NULL)
		return;

	/* walk array of day text pointers and linked lists of text */

	for (day = 1; day <= LAST_NOTE_DAY; day++) {
		for (pd = pm->day[day-1]; pd; pd = pd->next) {

			/* Determine if this is a 'daytext' or an 'image'... */
			if (ci_strncmp(pd->text, "image:", 6) == 0) {

				char temp_str[300];
				char *p;
				
				
				printf("%d [()] \n", day);
				
				strncpy(temp_str, pd->text, sizeof temp_str);
				p = strchr(pd->text, ' ');  /* Isolate the EPS image filename */
				strncpy(temp_str, p+1, sizeof temp_str);  /* Copy the remaining parameters */
				*p = '\0';
				printf("(%s) %s ", pd->text+6, temp_str);
				
				this_day = 1;
				RESET_DATE();		/* reset working date */
				
				printf("epsimage\n");
			}
		}
	}
}


/*
 * print_colors - print array specifying color of each date in current month
 * (formerly calculated on the fly in drawnums{} in pcalinit.ps)
 */
void
#ifdef PROTOS
print_colors(int month,
	      int year)
#else
print_colors(month, year)
	int month, year;
#endif
{
	register int day;
	year_info *py;
	month_info *pm;
	unsigned long holidays;
	int j, len;
	short color[32];

	len = LENGTH_OF(month, year);

	/* determine each date's color from its day of the week */
	for (day = 1, j = FIRST_OF(month, year);
	     day <= len;
	     day++, j = (j + 1) % 7) {
		color[day] = day_color[j];
	}

	pm = (py = find_year(year, FALSE)) ? py->month[month-1] : NULL;

	/* override weekday color for holidays */
	for (holidays = pm ? pm->holidays : 0, day = 1;
	     holidays;
	     holidays >>= 1, day++)
		if (holidays & 01)
			color[day] = holiday_color;

	printf("/date_color [ -1");		/* dummy value for element 0 */
	for (day = 1; day <= len; day++)
		printf("%s %d", day % 10 == 1 ? " " : "", color[day]);
	printf(" ] def\n");
}


/*
 * find_noteboxes - find and print the note box numbers for specified
 * month/year
 */
void
#ifdef PROTOS
find_noteboxes(int month,
	       int year)
#else
find_noteboxes(month, year)
	int month, year;
#endif
{
	register int day;
	year_info *py;
	month_info *pm;

	/* if no text for this year and month, print empty list and return */

	if ((py = find_year(year, FALSE)) == NULL ||
	    (pm = py->month[month-1]) == NULL) {
		printf("/noteboxes [ ] def\n");
		return;
	}

	printf("/noteboxes [");	/* start definition of list */

	/* walk array of note text pointers, converting days to box numbers */

	for (day = FIRST_NOTE_DAY; day <= LAST_NOTE_DAY; day++)
		if (pm->day[day-1])
			printf(" %d", note_box(month, day, year));
	printf(" ] def\n");

}


/*
 * print_dates - print the dates (in "calendar" format) for the specified
 * month and year
 */
void
#ifdef PROTOS
print_dates(int month,
	    int year)
#else
print_dates(month, year)
	int month, year;
#endif
{
	register int day;
	year_info *py;
	month_info *pm;
	register day_info *pd;
	unsigned long holidays;
	int has_holiday_text;
	static int save_year = 0;

	/* if no text for this year and month, return */

	if ((py = find_year(year, FALSE)) == NULL ||
	    (pm = py->month[month-1]) == NULL)
		return;

	/* print the year if it has changed */

	if (year != save_year)
		printf("%s %d\n", kw_year, save_year = year);

	/* walk array of day text pointers and linked lists of text */

	for (holidays = pm->holidays, day = 1;
	     day < FIRST_NOTE_DAY;
	     holidays >>= 1L, day++) {
		has_holiday_text = FALSE;
		for (pd = pm->day[day-1]; pd; pd = pd->next) {
			/* Bypass entries that specify an EPS image... */
			if (ci_strncmp(pd->text, "image:", 6) == 0) continue;
			if (date_style == USA_DATES)
				printf("%02d/%02d", month, day);
			else
				printf("%02d/%02d", day, month);
#ifdef KEEP_ASTERISKS
			printf(pd->is_holiday ? "*\t" : "\t");
#else
			printf("\t");
#endif
			this_day = day;
			RESET_DATE();	/* reset working date */
			print_text(pd->text);
			printf("\n");
			has_holiday_text |= pd->is_holiday;
		}
#ifdef KEEP_ASTERISKS
		/* was date flagged as holiday w/o associated text? */
		if ((holidays & 01) && !has_holiday_text) {
			if (date_style == USA_DATES)
				printf("%02d/%02d*\n", month, day);
			else
				printf("%02d/%02d*\n", day, month);
		}
#endif
	}
}



/*
 * print_moon_info - print the information necessary to draw moons.  If
 * printing moons on all days, print the phase for each day; if printing
 * only quarter moons, tweak the phase to an exact quarter (so the icon
 * is printed correctly) and generate a list of the quarter-moon dates
 */
void
#ifdef PROTOS
print_moon_info(int month,
	        int year)
#else
print_moon_info(month, year)
	int month, year;
#endif
{
	int n, ndays, day, quarter;
	char *p;
	unsigned long qdays;
	double phase;
	static char *q[4] = {"NM", "1Q", "FM", "3Q"};

	if (draw_moons == NO_MOONS)
		return;

	/* print the phase of the moon for each day of the month */

	printf("/moon_phases [\t\t%% from ");
	if ((p = find_moonfile(year)) != NULL)
		printf("%s", p);
	else {
		printf("algorithm");
		if (atof(time_zone) != 0.0)
			printf(" (UTC offset = %s)", time_zone);
	}
	printf("\n\t");

	for (n = 0, qdays = 0L, day = 1, ndays = LENGTH_OF(month, year);
	     day <= ndays;
	     day++) {
		phase = find_phase(month, day, year, &quarter);
		if (DEBUG(DEBUG_MOON))
			fprintf(stderr, "%02d/%02d/%d: %.3f %s\n", month, day,
				year, phase, quarter != MOON_OTHER ?
				q[quarter] : "");
		/* adjust phase to exact quarter if printing only quarters */
		if (draw_moons == SOME_MOONS && quarter != MOON_OTHER)
			phase = 0.25 * quarter;
		if (draw_moons == ALL_MOONS || quarter != MOON_OTHER)
			printf("%.3f%s", PRT_TWEAK(phase), ++n % 10 == 0 ?
			    "\n\t" : " ");
		if (quarter != MOON_OTHER)
			qdays |= 1L << (day - 1);
	}
	printf("] def\n");

	/* if drawing only quarter moons, print days when they occur */

	if (draw_moons == SOME_MOONS) {
		printf("/quarter_moons [ ");
		for (day = 1; qdays; day++, qdays >>= 1)
			if (qdays & 01)
				printf("%d ", day);
		printf("] def\n");
	}
}


/*
 * print_julian_info - print the information necessary to print Julian dates
 */
void
#ifdef PROTOS
print_julian_info(int month,
		  int year)
#else
print_julian_info(month, year)
	int month, year;
#endif
{

	if (julian_dates != NO_JULIANS)
		printf("/jdstart %d def\n", DAY_OF_YEAR(month, 1, year));
	if (julian_dates == ALL_JULIANS)
		printf("/yearlen %d def\n", YEAR_LEN(year));
}


/*
 * print_month - generate calendar for specified month/year
 */
void
#ifdef PROTOS
print_month(int month,
	    int year)
#else
print_month(month, year)
	int month, year;
#endif
{
	static int nmonths = 0;
#ifdef EPS
	static int page = 0;
#endif
	int startbox;

	/* start of new physical page? */
	if (!do_whole_year || nmonths % 12 == 0)
		PS_STARTPAGE();

	printf("/year %d def\n", year);		/* set up year and month */
	printf("/month %d def\n", month);

	/* move starting box to second row if conflict with small calendars */
	startbox = START_BOX(month, year);
	if (!do_whole_year &&
	    (prev_cal_box[small_cal_pos] == startbox ||
	     next_cal_box[small_cal_pos] == startbox) )
		startbox += 7;
	printf("/startbox %d def\n", startbox);
	printf("/ndays %d def\n", LENGTH_OF(month, year));

	find_noteboxes(month, year);		/* make list of note boxes */
	print_colors(month, year);		/* make list of date colors */

	/* are we printing 12 months per page or only one? */

	if (do_whole_year) {
		/* reset foot strings at start of each page */
		if (nmonths % 12 == 0) {
			def_footstring(lfoot, "Lfootstring");
			def_footstring(cfoot, "Cfootstring");
			def_footstring(rfoot, "Rfootstring");
			def_footstring(notes_hdr, "notesheading");
		}

		printf("/posn %d def\n", nmonths % 12);	/* location on page */
		printf("printmonth\n");
	}
	else {
		/* reset foot strings each month (may change) */
		def_footstring(lfoot, "Lfootstring");
		def_footstring(cfoot, "Cfootstring");
		def_footstring(rfoot, "Rfootstring");
		def_footstring(notes_hdr, "notesheading");

		/* generate information necessary for small calendars */

		if (small_cal_pos != SC_NONE) {
			int m, y;

			printf("/p_year %d def\n", y = PREV_YEAR(month, year));
			printf("/p_month %d def\n", m = PREV_MONTH(month, year));
			printf("/p_startbox %d def\n", START_BOX(m, y));
			printf("/p_ndays %d def\n", LENGTH_OF(m, y));

			printf("/n_year %d def\n", y = NEXT_YEAR(month, year));
			printf("/n_month %d def\n", m = NEXT_MONTH(month, year));
			printf("/n_startbox %d def\n", START_BOX(m, y));
			printf("/n_ndays %d def\n", LENGTH_OF(m, y));
		}

		print_julian_info(month, year);		/* Julian date info */
		print_moon_info(month, year);		/* moon info */

		printf("printmonth\n");
		find_daytext(month, year, TRUE);	/* holiday text */
		find_daytext(month, year, FALSE);	/* day and note text */
		find_images(month, year);		/* EPS images */
	}

	/* end of physical page? */
	if (!do_whole_year || ++nmonths % 12 == 0)
		PS_ENDPAGE();
}


/*
 * print_html - print character string in HTML format, converting all
 * non-printing characters to "&#NNN;" notation
 */
void
#ifdef PROTOS
print_html(char *p)
#else
print_html(p)
	char *p;
#endif
{
	char c, buf[STRSIZ];

	while ((c = *p) != '\0') {
		if (c == '%' && p[1] != '\0') {
			p = expand_fmt(buf, p + 1);
			PUTSTR_CLEAN(isalnum, buf, stdout);
		}
		else if (c < ' ' || c > '\176')
			printf("&#%03d;", *p++ & CHAR_MSK);
		else
			printf("%c", *p++);
	}
}
