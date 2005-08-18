/*
 * pcaldefs.h - definitions for Pcal program
 *
 * Revision history:
 *
 *	4.9.0
 *		B.Marr		2005-08-02
 *		
 *		Add enumeration for new 'delete' keyword, from Bill Bogstad's
 *		'event deletion' patch.
 *		
 *		B.Marr		2005-01-04
 *		
 *		Provide 12 new character encodings (KOI8-R and several 'ISO
 *		8859-*' encodings) to correspond to new PostScript encoding
 *		vectors in order to support a wider diversity of languages.
 *
 *	4.8.0
 *		B.Marr		2004-12-05
 *		
 *		Fix misleading references to "holiday" to instead refer to
 *		"predefined event" (i.e. not all pre-defined events are
 *		'holidays').  Create and support concept of 'input' language
 *		versus 'output' language.  Support specification of paper size
 *		via run-time option (command-line, etc).  Remove spaces
 *		embedded within tab fields. Perform various other cosmetic
 *		and/or comment cleanups.
 *		
 *		B.Marr		2004-11-13
 *		
 *		Use new KOI8U mapping for newly-added Ukrainian language
 *		support.  Provide support for "Friday the 13th" events, based
 *		on a patch from Don Laursen (donrl at users dot sourceforge
 *		dot net).
 * 
 *	4.7	AWR	01/25/2000	add SEARCH_PCAL_DIR to control whether
 *					or not Pcal looks for the calendar
 *					file in the same directory where Pcal
 *					itself lives (not desirable if an
 *					executable named 'calendar' has been
 *					installed there)
 *
 *			04/19/1999	add DIVIDE_BLANK_SPACE (cf. writefil.c)
 *					to control style of blank date boxes
 *					beginning and end of HTML calendars
 *
 *			06/19/1998	add support for generating full-year
 *					HTML calendars
 *
 *			03/08/1998	add alternate scaling and translation
 *					factors for A4 size paper (-DA4PAPER);
 *					express all such factors as numbers
 *					instead of character strings (cf.
 *					pcal.c, pcalglob.h, pcallang.h,
 *					writefil.c); reposition Y origin as
 *					top of calendar boxes
 *
 *			12/21/1997	add OS/2 definitions (as suggested by
 *					Alexander Mai)
 *
 *			07/27/1997	add support for -H flag (HTML-specific
 *					macro definitions); delete obsolete FPR
 *					and PRT macros
 *
 *	4.6	AWR	05/14/1997	replace CENTURY with TM_YEAR (cf.
 *					<time.h>)
 *
 *			09/13/1996	support "nearest_before" and
 *					"nearest_after" keywords
 *
 *			04/30/1996	define NOBODY pseudo-account (Un*x
 *					only) to suppress various inapplicable
 *					comments when pcal is run anonymously
 *					from a CGI script (cf. writefil.c)
 *
 *			04/22/1996	specify alternate (taller) date box
 *					height, used when printing single-month
 *					portrait-mode calendars
 *
 *			11/29/1995	add ARRAYSIZE() macro (cf. readfile.c)
 *
 *			11/10/1995	add safe TOUPPER() macro
 *
 *	4.5	AWR	08/24/1994	Define CHAR_MSK (instead of hard-coded
 *					0377) to avoid negative result when
 *					promoting char to int (see comments)
 *
 *			12/21/1993	Removed obsolete COLOR_MIN, COLOR_MAX
 *					definitions
 *
 *			11/08/1993	Add IS_EURDATE() macro to support
 *					"nn." style of date specifications
 *
 *			09/09/1993	Support Roman8 and ISO Latin1
 *					character mappings (cf. fontmaps.ps,
 *					writefil.c)
 *
 *					Redefine MIN_YR from 1900 to 1753
 *
 *			03/20/1993	Allow several site-specific program
 *					defaults to be set in Makefile
 *
 *			03/05/1993	add MAPFONTS for optional mapping to
 *					8-bit fonts (for European languages)
 *
 *			12/29/1992	add PS_RELEASE to define PostScript
 *					version (for Encapsulated PostScript)
 *
 *			04/07/1992	Revise PUTCHAR macro: add <ctype.h>
 *					function/macro name and file pointer
 *					as arguments
 *
 *			02/11/1992	Many new declarations to help support
 *					predefined holidays
 *
 *	4.4	AWR	02/10/1992	Add PAGER_ENV (environment variable
 *					used in piping "help" message output
 *					[Un*x only])
 *
 *			01/20/1992	Added support for -z flag and revised
 *					versions of -[bgGO]
 *
 *			01/13/1992	Add default font size to DATEFONT and
 *					TITLEFONT; move initialization of all
 *					font sizes here (from pcalinit.ps);
 *					enlarge medium calendar font sizes
 *
 *	4.3	AWR	12/05/1991	Revise moonfile name templates for
 *					consistency with current standards
 *					for substituting year in strings
 *
 *			12/03/1991	Add support for -s flag
 *
 *			10/17/1991	Add support for -Z flag and subflags;
 *					removed obsolete PUTSTR macro
 *
 *	4.2	AWR	10/08/1991	Add support for -[kK] flags; renamed
 *					START_DAY as START_BOX for clarity
 *
 *			10/03/1991	Add various definitions relating to
 *					"note/<n>"
 *
 *			09/30/1991	Add MAX_IF_NESTING (related to "elif";
 *					cf. read_datefile() in readfile.c)
 *
 *	4.11	AWR	08/20/1991	Add "nearest" keyword (as per Andy
 *					Fyfe)
 *
 *	4.1	AWR	08/16/1991	Add support for -G flag
 *
 *	4.0	AWR	03/01/1991	Add STDLIB macro for systems which
 *					support <stdlib.h>
 *
 *			02/22/1991	add definitions for MS-DOS support (as
 *					per Floyd Miller)
 *
 *			02/19/1991	revise ORD_XXX to support negative
 *					ordinals
 *
 *					add definitions for Amiga support (as
 *					per Bill Hogsett)
 *
 *			02/06/1991	additional defs for expression
 *					processing
 *
 *			02/04/1991	support "year" as additional month
 *					name; use negative numbers for
 *					special ordinal codes (reserve
 *					positive numbers for ordinals)
 *
 *			01/28/1991	miscellaneous new constants/macros
 *
 *			01/07/1991	add FEB_29_OK (cf. enter_day_info())
 *
 */

/*
 * System dependencies:
 */

#ifdef VMS		/* VMS oddities isolated here */

#include <ssdef.h>	/* required for trnlog() */
#include <descrip.h>

#define HOME_DIR	"SYS$LOGIN"
#define DATEFILE	"calendar.dat"
#define MOONFILE	"moon%y.dat"	/* '%y' replaced with year */
#define DEFAULT_PS_OUTFILE	"calendar.ps"
#define DEFAULT_HTML_OUTFILE	"calendar.html"
#define START_PATH	'['
#define END_PATH	']'

#define EXIT_SUCCESS	1		/* VMS exit() parameters */
#define EXIT_FAILURE	3

#else
#ifdef AMIGA		/* more oddities for Amiga */

#include <string.h>

#define PROTOS				/* compiler accepts prototypes */
#define STDLIB				/* system has <stdlib.h> */
#define HOME_DIR	"RAM:"
#define DATEFILE	"s:calendar.dat"
#define MOONFILE	"s:.moon%y"	/* '%y' replaced with year */
#define DEFAULT_PS_OUTFILE	"RAM:calendar.ps"
#define DEFAULT_HTML_OUTFILE	"RAM:calendar.html"
#define START_PATH	'/'
#define END_PATH	'/'

#else
#ifdef DOS		/* even more oddities for MS-DOS */

#define DATEFILE	"pcal.dat"
#define MOONFILE	"moon%y.dat"	/* '%y' replaced with year */
#define START_PATH	'\\'
#define END_PATH	'\\'
#define HOME_DIR	"HOME"
#define ALT_DATEFILE	"calendar"	/* for backward compatibility */

#else			/* more for OS/2 - note Un*x similarities */
#ifdef __EMX__
#include <pwd.h>			/* password info (cf. writefil.c) */
#include <unistd.h>
#define HOME_DIR	"HOME"
#define DATEFILE	".calendar"
#define ALT_DATEFILE	"calendar"	/* for backward compatibility */
#define MOONFILE	".moon%y"	/* '%y' replaced with year */
#define ALT_MOONFILE	"moon%y"	/* analogous to ALT_DATEFILE */
#define START_PATH	'/'
#define END_PATH	'/'

#define PAGER_ENV	"PAGER"		/* points to help message pager */
#define PAGER_DEFAULT	"more"		/* default pager (NULL = none) */
#define NOBODY		"nobody"	/* anonymous account (cf. writefil.c) */

#else			/* not VMS, Amiga, MS-DOS, or OS/2 - assume Un*x */
#include <pwd.h>			/* password info (cf. writefil.c) */
#include <unistd.h>

#define UN_X				/* to distinguish Un*x from others */

#define HOME_DIR	"HOME"
#define DATEFILE	".calendar"
#define ALT_DATEFILE	"calendar"	/* for backward compatibility */
#define MOONFILE	".moon%y"	/* '%y' replaced with year */
#define ALT_MOONFILE	"moon%y"	/* analogous to ALT_DATEFILE */
#define START_PATH	'/'
#define END_PATH	'/'

#define PAGER_ENV	"PAGER"		/* points to help message pager */
#define PAGER_DEFAULT	"more"		/* default pager (NULL = none) */
#define NOBODY		"nobody"	/* anonymous account (cf. writefil.c) */

#endif
#endif
#endif
#endif

/* 
 * Define a 'convenience' macro used to explicitly declare arguments as
 * 'unused' where intentionally-unused function parameters are declared.  This
 * prevents warnings by the 'gcc' GNU C compiler that would otherwise appear
 * because of using the '-W' compile option.
 *
 * The Unix and DOS+DJGPP build environments support this ability.  We assume
 * that all other build environments do not support it, so this macro is
 * defined as a null value in such cases.
 */ 
#if defined (BUILD_ENV_UNIX) || defined (BUILD_ENV_DJGPP)
#define GCC_UNUSED  __attribute__ ((unused))
#else
#define GCC_UNUSED
#endif


/* define PS_OUTFILE and HTML_OUTFILE as DEFAULT_*_OUTFILE if defined;
 * otherwise define as "" (stdout)
*/

#ifdef DEFAULT_PS_OUTFILE
#define PS_OUTFILE	DEFAULT_PS_OUTFILE
#else
#define PS_OUTFILE	""
#endif

#ifdef DEFAULT_HTML_OUTFILE
#define HTML_OUTFILE	DEFAULT_HTML_OUTFILE
#else
#define HTML_OUTFILE	""
#endif

/* PROTOS may be defined independently of __STDC__ for compilers which
 * support function prototypes but are not fully ANSI-compliant
 */

#if defined(__STDC__) && ! defined(PROTOS)
#define PROTOS	
#endif

/* STDLIB may be defined independently of __STDC__ for systems which
 * support <stdlib.h> but are not fully ANSI-compliant
 */

#if defined(__STDC__) && ! defined(STDLIB)
#define STDLIB			/* cf. {no}protos.h */
#endif

#ifdef STDLIB			/* include <stdlib.h> if known to exist */
#include <stdlib.h>
#endif

/* EXIT_SUCCESS and EXIT_FAILURE should be defined in <stdlib.h>
 * (or above if non-standard); define here if necessary
 */

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE	1
#endif

/* definition of NULL (if needed) */
 
#ifndef NULL
#define NULL	0
#endif

/*
 * Environment variables (global symbol, logical name on VMS):
 */

#define PCAL_OPTS	"PCAL_OPTS"	/* command-line flags */
#define PCAL_DIR	"PCAL_DIR"	/* calendar file directory */
#define PATH_ENV_VAR	"PATH"		/* cf. find_executable() (pcalutil.c) */

/*
 * mask to avoid sign extension when promoting char to int (as in the PUTCHAR
 * macro below); some older compilers may sign-extend this mask anyway, in
 * which case you may want to try defining it as ((int) 0377) or even 255 :
 */
#define CHAR_MSK	0377

/*
 * Function-like macros:
 */

/* write character _c to file _fp; print as octal escape if _fc(_c) is FALSE */
#define PUTCHAR(_fc, _c, _fp) \
	fprintf((_fp), (_c) == ' ' || _fc((_c) & CHAR_MSK) ? "%c" : "\\%03o" , (_c) & CHAR_MSK)

#define PUTSTR(_fc, _s, _fp) \
	do { char *_p; for (_p = (_s); *_p; _p++) PUTCHAR(_fc, *_p, _fp); } while (0)

#define IS_LEAP(y)	((y) % 4 == 0 && ((y) % 100 != 0 || (y) % 400 == 0))
#define LENGTH_OF(m, y) (month_len[(m)-1] + ((m) == FEB && IS_LEAP(y)))
#define YEAR_LEN(y)	(IS_LEAP(y) ? 366 : 365)
#define DAY_OF_YEAR(m, d, y) ((month_off[(m)-1] + ((m) > FEB && IS_LEAP(y))) + d)
#define OFFSET_OF(m, y) ((month_off[(m)-1] + ((m) > FEB && IS_LEAP(y))) % 7)
#define FIRST_OF(m, y)	calc_weekday(m, 1, y)
#define START_BOX(m, y)	((FIRST_OF(m, y) - first_day_of_week + 7) % 7)

#define PREV_MONTH(m, y) ((m) == JAN ? DEC : (m) - 1)
#define PREV_YEAR(m, y)  ((m) == JAN ? (y) - 1 : (y))
#define NEXT_MONTH(m, y) ((m) == DEC ? JAN : (m) + 1)
#define NEXT_YEAR(m, y)  ((m) == DEC ? (y) + 1 : (y))

#define BUMP_MONTH_AND_YEAR(m, y) (((m) == DEC) ? (++(y), (m) = JAN) : (++(m)))

#define INIT_COLORS	do { \
	memcpy(day_color, default_color, sizeof(day_color)); \
	holiday_color = HOLIDAY_COLOR; \
	weekday_color = WEEKDAY_COLOR; \
	} while (0)

#define P_LASTCHAR(p)	((p) && *(p) ? (p) + strlen(p) - 1 : NULL)
#define LASTCHAR(p)	(p)[strlen(p) - 1]

#define IS_NUMERIC(p)	((p)[strspn((p), DIGITS)] == '\0')
#define IS_EURDATE(p)	((ispunct((p)[strspn((p), DIGITS)])) && \
			 ((p)[strspn((p), DIGITS)+1] == '\0'))
#define IS_WILD(w)	((w) >= WILD_FIRST && (w) <= WILD_LAST)

#define MAKE_DATE(dt, m, d, y) \
	do { (dt).mm = m; (dt).dd = d; (dt).yy = y; } while (0)

#define ERR(errmsg) \
	fprintf(stderr, E_ILL_LINE, progname, errmsg, filename, line);

#define DEBUG(f)	((debug_flags & f) != 0)

#define ARRAYSIZE(a)	(sizeof(a)/sizeof(a[0]))

#ifdef __STDC__
#define TOLOWER(c)	tolower(c)
#define TOUPPER(c)	toupper(c)
#else
#define TOLOWER(c)	(isupper(c) ? tolower(c) : (c))
#define TOUPPER(c)	(islower(c) ? toupper(c) : (c))
#endif

#ifndef isodigit			/* rare */
#define isodigit(c)	((c) >= '0' && (c) <= '7')
#endif
#ifndef isxdigit			/* ANSI standard */
#define isxdigit(c) \
	(isdigit(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#endif

/* debug subflag codes (must be distinct) - cf. pcallang.h */

#define DEBUG_DATES	(1 << 1)
#define DEBUG_MOON	(1 << 2)
#define DEBUG_PATHS	(1 << 3)
#define DEBUG_OPTS	(1 << 4)
#define DEBUG_PP	(1 << 5)
#define DEBUG_TEXT	(1 << 6)

/* preprocessor token codes - cf. get_token(), pcallang.h */

#define PP_DEFINE	 0
#define PP_ELIF		 1
#define PP_ELSE		 2
#define PP_ENDIF	 3
#define PP_IFDEF	 4
#define PP_IFNDEF	 5
#define PP_INCLUDE	 6
#define PP_UNDEF	 7
#define PP_OTHER	-1	/* not pp token */

/* ordinal number codes - cf. get_ordinal(), pcallang.h */

#define ORD_NEGNUM	-1	/* negative ordinal (-2nd == next to last) */
#define ORD_POSNUM	 1	/* positive ordinal */
#define ORD_ODD		 2	/* special codes for "odd" and "even" */
#define ORD_EVEN	 3
#define ORD_ALL		 4	/* special code for "all" used as ordinal */
#define ORD_OTHER	 0	/* not ordinal token */

/* ordinal numbers themselves - added for predefined holiday routines */

#define FIRST		 1
#define SECOND		 2
#define THIRD		 3
#define FOURTH		 4
#define FIFTH		 5
#define LAST		-1

/* moon phase codes - cf. pcallang.h and moonphas.c; these must take the
 * values 0 (NM) .. 3 (3Q) since they are used in phase calculations
 */

#define MOON_NM		 0	/* new moon */
#define MOON_1Q		 1	/* first quarter */
#define MOON_FM		 2	/* full moon */
#define MOON_3Q		 3	/* last quarter */
#define MOON_OTHER	-1	/* unrecognizable */

/* date type codes - cf. date_type(), get_keywd(), and pcallang.h */

#define DT_ALL			 0	/* "all" keyword" */
#define DT_NOTE			 1	/* "note" keyword */
#define DT_OPT			 2	/* "opt" keyword */
#define DT_INPUT_LANGUAGE	 3	/* "input-language" keyword */
#define DT_YEAR			 4	/* "year" keyword */
#define DT_MONTH		 5	/* name of month */
#define DT_DATE			 6	/* date of form dd/mm{/yy} or mm/dd{/yy} */
#define DT_EURDATE		 7	/* European date of form dd <month> */
#define DT_ORDINAL		 8	/* ordinal (first, 1st, ... last) */
#define DT_WEEKDAY		 9	/* weekday name */
#define DT_PREDEF_EVENT		10	/* predefined event (often a holiday) */
#define DT_DELETE		11	/* delete date entry following on this line */
#define DT_OTHER		-1	/* unrecognizable first token */

/* preposition token codes - cf. get_prep(), pcallang.h */

#define PR_BEFORE	  0
#define PR_ON_BEFORE	  1
#define PR_AFTER	  2
#define PR_ON_AFTER	  3
#define PR_NEAREST	  4
#define PR_NEAREST_BEFORE 5
#define PR_NEAREST_AFTER  6
#define PR_OTHER	 -1	/* not a preposition */


/*
 * Miscellaneous other constants:
 */

#define COMMENT_CHAR	'#'	/* delimiter for datefile comments */

#define RGB_CHAR	':'	/* delimiter for red/green/blue values */

#ifndef TRUE
#define FALSE		0	/* pseudo-Booleans */
#define TRUE		1
#endif

#define MAX_FILE_NESTING 10	/* maximum nesting level for file inclusion */
#define MAX_IF_NESTING	20	/* maximum nesting level for if{n}def */

#define MAX_PP_SYMS	100	/* number of definable preprocessor symbols */
#define PP_SYM_UNDEF     -1	/* flag for undefined symbol */

#define EXPR_ERR	-1	/* expression parsing error */

#define MIN_YR		1753	/* significant years (calendar limits) */
#define MAX_YR		9999

#define ALL_YEARS	-1	/* wildcard for years */
#define TM_YEAR	1900		/* offset for tm_year field of struct tm  */
				/* (cf. <time.h>); may need to be changed */
				/* for non-Un*x implementations           */

#define SCREENWIDTH	78	/* command-line message in usage() */

#define FEB_29_OK	 1	/* if != 0, ignore 2/29 of common year */

#define KEEP_NULL_LINES	 1	/* if != 0, copy blank text lines to output */

#define NEAREST_INCR	 1	/* if 1, disambiguate "nearest" as later    */
				/* date; if -1, to earlier (cf. readfile.c) */

#ifndef SEARCH_PCAL_DIR
#define SEARCH_PCAL_DIR  1	/* if 1, look for the calendar file in the  */
				/* same directory where the pcal executable */
				/* lives; if 0, avoid this (e.g., in case a */
				/* program named 'calendar' has also been   */
				/* installed in this directory)             */
#endif

#define JAN		 1	/* months (for predefined holiday routines) */
#define FEB		 2
#define MAR		 3
#define APR		 4
#define MAY		 5
#define JUN		 6
#define JUL		 7
#define AUG		 8
#define SEP		 9
#define OCT		10
#define NOV		11
#define DEC		12
#define NOT_MONTH	-1	/* not valid month */

#define ALL_MONTHS	 0	/* select all months */
#define ENTIRE_YEAR	13	/* select entire year as one entity */

#define SUN		 0	/* weekdays - cf. wildcard codes below */
#define MON		 1
#define TUE		 2
#define WED		 3
#define THU		 4
#define FRI		 5
#define SAT		 6
#define NOT_WEEKDAY	-1	/* not valid weekday */

#define NOT_PREDEF_EVENT	-1	/* flag for end of 'pre-defined event' list */

/* wildcards - cf. days[] in pcallang.h and pdatefcn[] in pcalglob.h */

#define ANY_DAY		 7	/* special - matches any day */
#define ANY_WEEKDAY	 8	/* matches any weekday (including holidays) */
#define ANY_WORKDAY	 9	/* matches any weekday (excluding holidays) */
#define ANY_HOLIDAY	10	/* matches any holiday */
#define ANY_NONWEEKDAY	11	/* converses of above three */
#define ANY_NONWORKDAY	12
#define ANY_NONHOLIDAY	13
/* moon phase wildcards - must be in same order as MOON_* below */
#define ANY_NM		14	/* matches any new moon, etc. */
#define ANY_1Q		15
#define ANY_FM		16
#define ANY_3Q		17

#define WILD_FIRST	ANY_DAY
#define WILD_FIRST_WKD	ANY_DAY
#define WILD_LAST_WKD	ANY_NONHOLIDAY
#define WILD_FIRST_MOON ANY_NM 
#define WILD_LAST_MOON	ANY_3Q
#define WILD_LAST	ANY_3Q


#define DAY_TEXT	0	/* types of text in data structure */
#define HOLIDAY_TEXT	1
#define NOTE_TEXT	2

#define NOTE_DEFAULT	-1	/* default note box number (-1 = last box) */
#define FIRST_NOTE_BOX	 0	/* first box position for notes text */
#define LAST_NOTE_BOX	41	/* last position (w/o small calendars) */
#define NULL_NOTE_BOX	-1	/* negative number for nonexistent box */

/* number and range of dummy days for notes text */
#define NUM_NOTE_DAYS	(LAST_NOTE_BOX - 28 + 1)	
#define FIRST_NOTE_DAY	32
#define LAST_NOTE_DAY	(FIRST_NOTE_DAY + NUM_NOTE_DAYS - 1)

#define MAX_DATES	366	/* maximum "wildcard" dates */

#define MAXWORD		100	/* maximum words in date file line */
#define STRSIZ		200	/* size of misc. strings */
#define VALSIZ		12	/* size of numeric value strings */
#define LINSIZ		512	/* size of source line buffer */

#define MAXARGS		3	/* numeric command-line args */

#define WHITESPACE	" \t"	/* token delimiters in date file */
#define DIGITS		"0123456789"

/* passes where flags may be recognized (cf. get_args(), pcallang.h) */
#define P_CMD0	(1 << 1)	/* parsed in command line pre-pass (-Z only) */
#define P_ENV	(1 << 2)	/* parsed from environment variable */
#define P_CMD1	(1 << 3)	/* parsed in first command-line pass */
#define P_OPT	(1 << 4)	/* parsed on "opt" lines in date file */
#define P_CMD2	(1 << 5)	/* parsed in second command-line pass */

/*
 * Defaults for calendar layout:
 */

#ifdef EPS					/* generate EPS-like comments */
#define PS_RELEASE	"PS-Adobe-2.0"		/* for comments at top */
#else
#define PS_RELEASE	"PS-Adobe-1.0"		/* for comments at top */
#endif

/* If 'MAPFONTS' is defined, generate PostScript code to re-map text fonts
 * (cf. pcalinit.ps) according to the definitions of the specified 8-bit
 * character set.  Note that the overhead to do this slows PostScript down --
 * especially for small jobs -- so users in English-speaking countries might
 * prefer to leave 'MAPFONTS' undefined.
 *
 * Speakers of languages other than English may select any of the available
 * remappings by defining 'MAPFONTS' as described in the 'Makefile'.
 * 
 * Here's a list of the supported remappings...
 * 
 *    Character Encoding	A.K.A.		Regions/Languages
 *    ----------------------------------------------------------------
 *					
 *	ISO 8859-1		Latin1		Western Europe
 *					
 *						(e.g. Italian, French, German,
 *						Spanish, Finnish, Swedish,
 *						Portuguese, Estonian, Catalan,
 *						etc)
 *					
 *	ISO 8859-2		Latin2		Central European, Slavic
 *					
 *						(e.g. Czech, Hungarian)
 *					
 *	ISO 8859-3		Latin3		South European, Esperanto, 
 *						Galician, Maltest, Turkish
 *					
 *	ISO 8859-4		Latin4		North European, Old Baltic
 *					
 *						(e.g. Latvian, Lithuanian)
 *					
 *	ISO 8859-5		Cyrillic	
 *					
 *	ISO 8859-7		Greek		Modern Greek
 *					
 *	ISO 8859-9		Latin5		Turkish
 *					
 *	ISO 8859-10		Latin6		Nordic
 *					
 *	ISO 8859-11		Thai		
 *					
 *	ISO 8859-13		Latin7		Baltic Rim
 *					
 *	ISO 8859-14		Latin8		Celtic
 *					
 *	ISO 8859-15		Latin9/Latin0	Western Europe
 *						(Latin1 + 'Euro' symbol)
 *					
 *	KOI8-R					Russian
 *					
 *	KOI8-U					Ukrainian
 *					
 *	Roman8					
 *					
 */

/* 
 * Define enumerations for each of the available character encodings
 * (mappings).
 * 
 * The 'Arabic', 'Hebrew', and 'Latin-10' character encodings are not enabled
 * because we currently (Aug 2005) have no PostScript encoding vector (see
 * 'pcalinit.ps') for those character sets.
 * 
 * 'ISO 8859-12' is not included because that was a draft for 'Latin-7' that
 * was never implemented and was therefore skipped.
 * 
 */
#define ENC_NONE 		0	/* no re-mapping */
#define ENC_LATIN_1		1	/* ISO 8859-1 */
#define ENC_LATIN_2		2	/* ISO 8859-2 */
#define ENC_LATIN_3		3	/* ISO 8859-3 */
#define ENC_LATIN_4		4	/* ISO 8859-4 */
#define ENC_CYRILLIC		5	/* ISO 8859-5 */
/* #define ENC_ARABIC		6 */	/* ISO 8859-6 */
#define ENC_GREEK		7	/* ISO 8859-7 */
/* #define ENC_HEBREW		8 */	/* ISO 8859-8 */
#define ENC_LATIN_5		9	/* ISO 8859-9 */
#define ENC_LATIN_6		10	/* ISO 8859-10 */
#define ENC_THAI		11	/* ISO 8859-11 */
#define ENC_LATIN_7		12	/* ISO 8859-13 */
#define ENC_LATIN_8		13	/* ISO 8859-14 */
#define ENC_LATIN_9		14	/* ISO 8859-15 */
/* #define ENC_LATIN_10		15 */	/* ISO 8859-16 */
#define ENC_KOI8_R		16	/* KOI8-R */
#define ENC_KOI8_U		17	/* KOI8-U */
#define ENC_ROMAN8		18	/* Roman8 */

/* 
 * Define the strings which can be used as values for the '-r' (remap
 * character set) option.
 */
#define MAPPING_LATIN_1		"Latin1"
#define MAPPING_LATIN_2		"Latin2"
#define MAPPING_LATIN_3		"Latin3"
#define MAPPING_LATIN_4		"Latin4"
#define MAPPING_CYRILLIC	"Cyrillic"
/* #define MAPPING_ARABIC	"Arabic" */
#define MAPPING_GREEK		"Greek"
/* #define MAPPING_HEBREW	"Hebrew" */
#define MAPPING_LATIN_5		"Latin5"
#define MAPPING_LATIN_6		"Latin6"
#define MAPPING_THAI		"Thai"
#define MAPPING_LATIN_7		"Latin7"
#define MAPPING_LATIN_8		"Latin8"
#define MAPPING_LATIN_9		"Latin9"
/* #define MAPPING_LATIN_10	"Latin10" */
#define MAPPING_KOI8_R		"KOI8-R"
#define MAPPING_KOI8_U		"KOI8-U"
#define MAPPING_ROMAN8		"Roman8"

/* 
 * If 'MAPFONTS' is defined in the 'Makefile', make sure it's set to one of
 * the legal values or else invalidate it...
 */
#ifdef MAPFONTS
#if	( \
	(MAPFONTS != ENC_LATIN_1)		&& \
	(MAPFONTS != ENC_LATIN_2)		&& \
	(MAPFONTS != ENC_LATIN_3)		&& \
	(MAPFONTS != ENC_LATIN_4)		&& \
	(MAPFONTS != ENC_CYRILLIC)		&& \
	/* (MAPFONTS != ENC_ARABIC)		&& */ \
	(MAPFONTS != ENC_GREEK)		&& \
	/* (MAPFONTS != ENC_HEBREW)		&& */ \
	(MAPFONTS != ENC_LATIN_5)		&& \
	(MAPFONTS != ENC_LATIN_6)		&& \
	(MAPFONTS != ENC_THAI)		&& \
	(MAPFONTS != ENC_LATIN_7)		&& \
	(MAPFONTS != ENC_LATIN_8)		&& \
	(MAPFONTS != ENC_LATIN_9)		&& \
	/* (MAPFONTS != ENC_LATIN_10)	&& */ \
	(MAPFONTS != ENC_KOI8_R)		&& \
	(MAPFONTS != ENC_KOI8_U)		&& \
	(MAPFONTS != ENC_ROMAN8)		\
	)
#undef MAPFONTS	
#define MAPFONTS	ENC_NONE
#endif
#else
#define MAPFONTS	ENC_NONE
#endif

/* default font names and sizes (large calendars) */

#ifndef TITLEFONT
#define TITLEFONT	"Times-Bold/48"		/* month/year title */
#endif
#ifndef DATEFONT
#define DATEFONT	"Times-Bold/25"		/* dates */
#endif
#ifndef NOTESFONT
#define NOTESFONT	"Helvetica-Narrow/6"	/* notes in boxes */
#endif

/* define secondary fonts in terms of main fonts */
#define WEEKDAYFONT	"titlefont"		/* weekday names */
#define FOOTFONT	"titlefont"		/* footer strings */
#define HEADINGFONT	"titlefont"		/* notes box heading */

/* define MAP_DATEFONT as 1 if any secondary font (above) is defined as
 * "datefont" (cf. writefil.c); otherwise, define as 0
 */
#define MAP_DATEFONT	0			/* re-map date font? */

/* font sizes (small, medium, large calendars respectively) - moved here
 * from pcalinit.ps; enlarged (again) for medium (whole-year) calendars
 */
#define SMALL		0
#define MEDIUM		1
#define LARGE		2
#define CALSIZE		{ "small", "medium", "large" };

#define TITLEFONTSIZE	{ 60, 64, 48 }	/* month/year (cf. TITLEFONT above) */
#define DATEFONTSIZE	{ 60, 56, 25 }	/* dates (cf. DATEFONT above) */
#define WEEKDAYFONTSIZE	{  0, 30, 12 }	/* weekdays */
#define FOOTFONTSIZE	{  0, 14, 12 }	/* footer strings */
#define HEADINGFONTSIZE	12		/* same as large weekdays */

#define GRIDLINEWIDTH   {2.5,  2,  1 }	/* grid lines */
#define DATEMARGIN	{ 10,  8,  4 }  /* top/side date margin */

#ifndef SHADING
#define SHADING		"0.8/0.9"	/* default shading (dates/fill boxes) */
#endif

#define LFOOT		""              /* default foot strings */
#define CFOOT		""
#define RFOOT		""

#define LANDSCAPE	90		/* degrees to rotate */
#define PORTRAIT	 0
#define ROTATE		LANDSCAPE	/* default */

#define	NCOPY		1		/* copies of each output page */
#define MAXCOPY		100		/* just in case... */

#define USA_DATES	0		/* date styles */
#define EUR_DATES	1
#ifndef DATE_STYLE
#define DATE_STYLE	USA_DATES	/* default */
#endif

#ifndef FIRST_DAY
#define FIRST_DAY	SUN		/* first day of logical week */
#endif

/* values below must be in sync with color_names[] in pcallang.h and also
 * with prtday{} in pcalinit.ps
 */
#define BLACK		0		/* colors for dates */
#define GRAY		1
#define OUTLINE		2
#define OUTLINE_GRAY	3
#define NUM_COLORS	4		/* total number of colors */

#define HOLIDAY_DEFAULT	-1		/* use same color as weekends */
#define	HOLIDAY_COLOR	HOLIDAY_DEFAULT

/* default time zone for -z flag; you may optionally define this as
 * (say) "5 [Boston]" (note space) to print the city in the -h message
 */ 
#ifndef TIMEZONE
#define TIMEZONE	"0"		/* hours west of GMT/UTC */
#endif

/* default colors for weekdays (presumes black and at most one other color -
 * cf. color_msg() in pcal.c)
 */
#define DAY_COLOR	{ GRAY, BLACK, BLACK, BLACK, BLACK, BLACK, GRAY }
#define WEEKDAY_COLOR	BLACK		/* prevalent color in DAY_COLOR */

#define NO_DATEFILE	0		/* date file (if any) to use */
#define USER_DATEFILE	1
#define SYS_DATEFILE	2

/* position of small calendars */
#define SC_NONE		0		/* suppress small calendars */
#define SC_LAST		1		/* use last two boxes */
#define SC_FIRST	2		/* use first two boxes */
#define SC_SPLIT	3		/* split between first and last */
#define SMALL_CAL_POS	SC_LAST		/* default */

/* box numbers for small calendars - must conform to order defined above */
#define PREV_CAL_BOX	\
	{ NULL_NOTE_BOX, LAST_NOTE_BOX - 1, FIRST_NOTE_BOX, FIRST_NOTE_BOX }
#define NEXT_CAL_BOX	\
	{ NULL_NOTE_BOX, LAST_NOTE_BOX, FIRST_NOTE_BOX + 1, LAST_NOTE_BOX }

/* values below must be in sync with cond[] in writefil.c */
#define NO_MOONS	0		/* no moon icons */
#define ALL_MOONS	1		/* moon icon on every day */
#define SOME_MOONS	2		/* icons on full, half, new moons */
#define DRAW_MOONS	NO_MOONS	/* default */

#define NO_JULIANS	0		/* no Julian dates */
#define ALL_JULIANS	1		/* Julian date + days left */
#define SOME_JULIANS	2		/* just Julian dates */
#define JULIAN_DATES	NO_JULIANS	/* default: none */

#define DO_WHOLE_YEAR	FALSE		/* -w default */
#define BLANK_BOXES	FALSE		/* -B default */

#define OUTPUT_PS	0		/* output PostScript */
#define OUTPUT_CAL	1		/* -c: output Un*x calendar(1) input */
#define OUTPUT_HTML	2		/* -H: output HTML table */
#define OUTPUT_TYPE	OUTPUT_PS	/* default for above */

/* Enable support for 'Friday the 13th' events. */
#define FRIDAY_13th	1

/*
 * HTML definitions - may be changed according to local requirements either
 * here or on the C compiler command line (cf. Makefile)
 */

/* attributes for <body> tag (cf. body_attributes[] in writefil.c) */

#ifndef BGCOLOR
#define BGCOLOR		"ffffff"		/* background color */
#endif

#ifndef BACKGROUND
#define BACKGROUND	NULL			/* background pattern */
#endif

#ifndef TEXT
#define TEXT		NULL			/* text color */
#endif

#ifndef LINK
#define LINK		NULL			/* link color */
#endif

#ifndef ALINK
#define ALINK		NULL			/* active link color */
#endif

#ifndef VLINK
#define VLINK		NULL			/* viewed link color */
#endif

/* definitions for HTML table format */

#ifndef BORDER
#define BORDER		1			/* border width attribute */
#endif

#ifndef TEXTLINES
#define TEXTLINES	2			/* min. text lines per box */
#endif

/* HTML sequences preceding/following holiday dates and headings */

/* holiday dates are bold and red for HTML calendars */
#if !(defined(HOLIDAY_PRE) && defined(HOLIDAY_POST))
#define HOLIDAY_PRE	"<font color=#ff0000><b>"
#define HOLIDAY_POST	"</b></font>"
#endif

/* month/year heading (single-month mode) */
#if !(defined(MONTHYEAR_PRE) && defined(MONTHYEAR_POST))
#define MONTHYEAR_PRE	"<br><font size=+1>"
#define MONTHYEAR_POST	"</font><br>&nbsp;"
#endif

/* month/year heading (whole-year mode) */
#if !(defined(MONTHYEAR_W_PRE) && defined(MONTHYEAR_W_POST))
#define MONTHYEAR_W_PRE	 "<font size=+1>"
#define MONTHYEAR_W_POST "</font>"
#endif

/* page heading (-C flag) */
#if !(defined(HEADING_PRE) && defined(HEADING_POST))
#define HEADING_PRE	"<h3 align=center>"
#define HEADING_POST	"</h3><p>"
#endif

/* print blank space (vacant date boxes) at beginning/end of HTML calendars
 * as N single-column boxes (1) or one N-column box (0) according to your
 * preference
 */
#ifndef DIVIDE_BLANK_SPACE
#define DIVIDE_BLANK_SPACE	0
#endif

/*
 * Global typedef declarations for data structure (cf. readfile.c)
 */

/*
 * The internal data structure consists of a series of linked lists as 
 * pictured below (for clarity, only one instance of each node is shown):
 *
 *
 * head
 *  |
 *  |        year_info
 *  |    -----------------           month_info
 *   -->| year            |       -----------------            day_info
 *      | month_info*[0]  |----->| holidays        |       ----------------
 *      |   ...           |      | day_info*[0]    |----->| is_holiday     |
 *      | month_info*[11] |--->  |   ...           |      | text_string*   |--->
 *      | next year_info* |--    | day_info*[30]   |--->  | next day_info* |-- 
 *       -----------------   |   | day_info*[NOTE] |--->   ----------------   |
 *                           |   |   ...           |                          |
 *                           |    -----------------                           |
 *             ...        <--                                     ...      <--
 *
 * 
 * Each year_info node consists of the year, 12 pointers (Jan .. Dec) to
 * month_info nodes, and a pointer to the next year_info node in the chain.
 * (The year_info chain is maintained in ascending order by year.)
 *
 * Each month_info node consists of a bitmap of the holidays for that month, 
 * a pointer to the day_info chain for all 31 possible days of the month, and
 * N additional pointers to the day_info chains for the "Notes" pseudo-days. 
 * (A day_info chain is a linked list of all the text entries for a given
 * day, in the order in which they were encountered in the date file.)
 *
 * Each day_info node consists of a flag indicating whether the associated
 * text string is holiday or non-holiday text (any given day may have both),
 * a pointer to the text string itself, and a pointer to the next day_info 
 * node in the chain.
 *
 */

typedef struct d_i {
	int is_holiday;
	char *text;
	struct d_i *next;
	} day_info;

typedef struct m_i {
	unsigned long holidays;
	day_info *day[LAST_NOTE_DAY];	/* including extra days for notes */
	} month_info;

typedef struct y_i {
	int year;
	month_info *month[12];
	struct y_i *next;
	} year_info;

/*
 * Global typedef declaration for date structure (cf. parse_ord())
 */

typedef struct {
	int mm;
	int dd;
	int yy;
} DATE;

/*
 * Global typedef declarations for keyword descriptors (cf. pcallang.h)
 */

typedef struct {
	char *name;
	int   code;
	} KWD;

typedef struct {
	char *name;
	int   code;
	int   value;
	} KWD_O;		/* keyword plus ordinal value */

typedef struct {
	char *name;
	int   code;
#ifdef PROTOS
	int (*pfcn)(char *);
#else
	int (*pfcn)();
#endif
	} KWD_F;		/* keyword plus dispatch function */

typedef struct {
	char *name;
	char *def;		/* "def" and "pfcn" are mutually exclusive */
#ifdef PROTOS
	int (*pfcn)(DATE *);
#else
	int (*pfcn)();
#endif
	} KWD_H;		/* keyword, equivalent string, dispatch fcn */

/* 
 * Global typedef declaration for flag usage struct (cf. pcallang.h, get_args())
 */

typedef struct {
	char flag;		/* name of flag */
	char has_arg;		/* TRUE if flag takes (optional) arg */
	char passes;		/* "or" of P_xxx for relevant passes */
	} FLAG_USAGE;

/*
 * Global typedef declaration for debugging info struct (cf. pcallang.h)
 */

typedef struct {
	char flag;		/* name of flag */
	int  value;		/* value corresponding to flag (1 << n) */
	} DEBUG_INFO;

/*
 * Global typedef declarations for usage message (cf. pcallang.h, usage())
 */

typedef struct {
	char flag;		/* name of flag */
	char *meta;		/* metaname for argument (if any) */
	char *text;		/* associated text */
	char *def;		/* default value (if any) */
	} FLAG_MSG;

typedef struct {
	char *desc;		/* description of param */
	char *text;		/* associated text */
	} PARAM_MSG;

typedef char *DATE_MSG;		/* date file syntax message */
