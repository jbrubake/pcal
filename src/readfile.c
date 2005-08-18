/*
 * readfile.c - Pcal routines concerned with reading and parsing the datefile
 *
 * Contents:
 *
 *		cleanup
 *		clear_syms
 *		date_type
 *		delete_day_info
 *		do_define
 *		do_ifdef
 *		do_ifndef
 *		do_include
 *		do_undef
 *		enter_day_info
 *		enter_note
 *		find_easter
 *		find_odox_easter
 *		find_odox_marcus
 *		find_odox_stgeorge
 *		find_sym_name
 *		find_sym_val
 *		find_year
 *		get_keywd
 *		get_month
 *		get_ordinal
 *		get_phase
 *		get_predef_event
 *		get_prep
 *		get_token
 *		get_weekday
 *		is_anyday
 *		is_fquarter
 *		is_fullmoon
 *		is_holiday
 *		is_lquarter
 *		is_newmoon
 *		is_weekday
 *		is_workday
 *		not_holiday
 *		not_weekday
 *		not_workday
 *		odox_easter_from_april1
 *		parse
 *		parse_date
 *		parse_ord
 *		parse_rel
 *		process_date
 *		read_datefile
 *
 * Revision history:
 *
 *	4.9.0
 *		B.Marr		2005-08-02
 *		
 *		Incorporate patch by Bill Bogstad <bogstad at pobox dot com>
 *		to provide ability to delete specific events, thereby allowing
 *		one to exclude one or more events that were inserted as a
 *		group of events, by using the new 'delete' keyword.
 *		
 *		B.Marr		2005-01-23
 *		
 *		Fix bug introduced in v4.8.0 whereby a plural specification of
 *		the day-of-week (e.g. 'all Fridays in Oct') was not being
 *		recognized.
 *
 *	4.8.0
 *		B.Marr		2004-12-15
 *		
 *		Prevent potential buffer overflow attack caused by malicious
 *		calendar input file by restructuring code to avoid a
 *		'strcpy()' call in 'get_predef_event()'.  This security hole
 *		was detected by Danny Lungstrom and reported by
 *		D. J. Bernstein.
 *		
 *		B.Marr		2004-12-05
 *		
 *		Fix misleading references to "holiday" to instead refer to
 *		"predefined event" (i.e. not all pre-defined events are
 *		'holidays').  Create and support concept of 'input' language
 *		versus 'output' language.  Remove spaces embedded within tab
 *		fields.
 *		
 *		B.Marr		2004-11-19
 *		
 *		Provide an enhanced capability to process not just simple
 *		symbol names but associated symbolic values too, based on a
 *		patch from (unknown).  Provide support for "Friday the 13th"
 *		events, based on a patch from Don Laursen (donrl at users dot
 *		sourceforge dot net).  Detect more than just 3 characters (if
 *		available in the event specification) when attempting to match
 *		a generic token to either a day-of-week name or a month name
 *		to avoid problem of misdetecting month names as day-of-week
 *		names (for languages which have such possibilities, like
 *		French) and the problem of misdetecting month names and/or
 *		day-of-week names for inactive languages.  Remove Ctl-L (page
 *		eject) characters from source file.
 *
 *	4.7	AWR	12/15/1998	expand %y into every applicable year
 *					in "year all" mode
 *
 *			02/24/1998	add prototypes (using PROTO macro)
 *					to function pointer declarations
 *
 *			12/21/1997	clean up gcc warnings in -Wall mode
 *
 *			07/27/1997	delete obsolete FPR and PRT macros;
 *					minor revisions for -H (HTML) flag
 *
 *	4.6	AWR	05/14/1997	replace obsolete CENTURY macro with
 *					call to century() (cf. pcalutil.c)
 *
 *			09/13/1996	support "nearest_before" and
 *					"nearest_after" keywords
 *
 *			12/05/1995	fix "undef" bug (modify get_token()
 *					to return token struct index, not type
 *					type code; cf. read_datefile())
 *
 *			12/02/1995	avoid generating font shift sequences
 *					in -c mode
 *
 *			11/29/1995	explicitly initialize calloc'd pointers
 *					to NULL
 *
 *			11/10/1995	support -T flag to select default
 *					font style (Bold/Italic/Roman)
 *
 *			10/31/1995	add preprocessor debug statements to
 *					do_define(), do_undef()
 *
 *			09/21/1995	added enter_date(), enter_note(),
 *					related support for "year all"
 *
 *		AH	02/03/1995	added orthodox easter related holiday
 *					calculations
 *
 *	4.5	AWR	11/08/1993	accept European dates of form "dd. mm."
 *					and "dd. mon"
 *
 *			04/28/1993	restructure function definitions so
 *					function name appears in first column
 *					(to facilitate searching for definition
 *					by name)
 *
 *			03/05/1993	Propagate null text to output
 *
 *			02/11/1992	Support predefined holidays (strings
 *					and dispatch functions - cf. pcallang.h)
 *
 *	4.4	AWR	01/20/1992	Use predominant color ("logical black")
 *					in is_weekday()
 *
 *			12/16/1991	Avoid invalid array access in
 *					read_datefile()
 *
 *	4.3	AWR	10/25/1991	Support moon phase wildcards and
 *					-Z flag (debug information)
 *
 *	4.2	AWR	10/03/1991	Support "note/<n>" (user-selected
 *					notes box) as per Geoff Kuenning
 *
 *			09/30/1991	Support "elif" in datefile
 *
 *	4.11	AWR	08/20/1991	Support "nearest" keyword as per
 *					Andy Fyfe
 *
 *	4.0	AWR	02/19/1991	Support negative ordinals
 *
 *			02/06/1991	Support expressions in "if{n}def"
 *
 *			02/04/1991	Support "even" and "odd" ordinals
 *					and ordinals > 5th; support "year"
 *
 *			01/15/1991	Extracted from pcal.c
 *
 */

/*
 * Standard headers:
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Pcal-specific definitions:
 */

#include "pcaldefs.h"
#include "pcalglob.h"
#include "pcallang.h"

/*
 * Macros:
 */

/* status codes returned by parse(), enter_day_info() */
#define PARSE_OK	0	/* successful date parse */
#define PARSE_INVDATE	1	/* nonexistent date */
#define PARSE_INVLINE	2	/* syntax error */
#define PARSE_NOMATCH	3	/* no match for wildcard */

/* codes for states in read_datefile() */
#define PROCESSING	0	/* currently processing datefile lines */
#define AWAITING_TRUE	1	/* awaiting first TRUE branch in "if{n}def" */
#define SKIP_TO_ENDIF	2	/* finished processing first TRUE branch */

#define BLANK_TEXT	" "	/* substitute for null input text */

/* append date to list; terminate list */
#define ADD_DATE(_m, _d, _y)	do { \
				if (DEBUG(DEBUG_DATES)) \
					fprintf(stderr, "matched %d/%d/%d\n", \
						_m, _d, _y); \
				pdate->mm = _m, pdate->dd = _d, pdate++->yy = _y; \
				} while (0)

#define TERM_DATES		pdate->mm = pdate->dd = pdate->yy = 0

/*
 * Globals:
 */

static DATE dates[MAX_DATES+1];		/* array of date structures */
static char *pp_sym[MAX_PP_SYMS];	/* preprocessor defined symbols */
static char *pp_val[MAX_PP_SYMS];	/* preprocessor defined symbols' values */
static int curr_year_reset = FALSE;
static int delete_entry = FALSE;


/*
 * read_datefile - read and parse date file, handling preprocessor lines
 *
 * This is the main routine of this module.  It calls getline() to read each
 * non-null line (stripped of leading blanks and trailing comments), loadwords()
 * to "tokenize" it, and get_token() to classify it as a preprocessor directive
 * or "other".  A switch statement takes the appropriate action for each token
 * type; "other" lines are further classified by parse() (q.v.) which calls
 * parse_date() (q.v.) to parse date entries and enter them in the data
 * structure (as described in pcaldefs.h).
 *
 */
void
#ifdef PROTOS
read_datefile(FILE *fp,		/* file pointer (assumed open) */
	      char *filename)	/* file name (for error messages) */
#else
read_datefile(fp, filename)
	FILE *fp;		/* file pointer (assumed open) */
	char *filename;		/* file name (for error messages) */
#endif
{
	static int file_level = 0;
	int if_level = 0;
	int line = 0;
	int extra = 0;
	int pptype, ntokens, save_year, expr, (*pfcn) PROTO((char *)) = NULL;
	char *ptok = NULL;
	char **pword;
	char msg[STRSIZ], incpath[STRSIZ], save_font;

	/* stack for processing nested "if{n}defs" - required for "elif" */
	struct {
		int state;	/* PROCESSING, AWAITING_TRUE, SKIP_TO_ENDIF */
		int else_ok;	/* is "elif" or "else" legal at this point? */
	} if_state[MAX_IF_NESTING+1];

	if (fp == NULL)				/* whoops, no date file */
		return;

	/* note that there is no functional limit on file nesting; this is
	 * mostly to catch infinite loops (e.g., a includes b, b includes a)
	 */
	if (++file_level > MAX_FILE_NESTING) {
		ERR(E_FILE_NESTING);
		exit(EXIT_FAILURE);
	}

	save_year = curr_year;			/* save current year... */
	save_font = fontstyle[0];		/* ... and font style	*/

	if_state[0].state = PROCESSING;		/* set up initial state */
	if_state[0].else_ok = FALSE;

	/* read lines until EOF */

	while (getline(fp, lbuf, &line)) {
		char suffix;

		if (DEBUG(DEBUG_PP)) {
			fprintf(stderr, "%s (%d)", filename, line);
			if (if_state[if_level].state == PROCESSING)
				fprintf(stderr, ": '%s'", lbuf);
			fprintf(stderr, "\n");
		}

		ntokens = loadwords(words, lbuf); /* split line into tokens */
		pword = words;			/* point to the first */

		/* get token type and pointers to function and name */

		suffix = words[0][strlen(words[0])-1];
		if ((pptype = get_token(*pword++)) != PP_OTHER) {
			pfcn = pp_info[pptype].pfcn;
			ptok = pp_info[pptype].name;
			pptype = pp_info[pptype].code;	/* use type code */
		}

		/* Perform symbol value substitution, but bypass it for any
		 * 'undef' preprocessor commands, otherwise there's no way to
		 * 'undef' a symbol which also had a value defined with it.
		 */
		if (pptype != PP_UNDEF) {
			char **ap, *nap;
			for (ap=words; *ap; ap++) {
				if ((nap = find_sym_val(*ap)))
					*ap=nap;
			}
		}

		switch (pptype) {

		case PP_DEFINE:
			if (if_state[if_level].state == PROCESSING)
				(void) (*pfcn)(*pword);
			if (ntokens == 3)
				do_define_sym_val(*pword,pword[1]);
			extra = ntokens > 3;
			break;

		case PP_UNDEF:
			if (if_state[if_level].state == PROCESSING) 
				(void) (*pfcn)(*pword);
			extra = ntokens > 2;
			break;

		case PP_ELIF:
			if (!if_state[if_level].else_ok) {
				ERR(E_ELIF_ERR);
				break;
			}

			/* if a true expression has just been processed, disable
			 * processing and skip to endif; if no true expression
			 * has been found yet and the current expression is
			 * true, enable processing
			 */
			switch (if_state[if_level].state) {
			case PROCESSING:
				if_state[if_level].state = SKIP_TO_ENDIF;
				break;
			case AWAITING_TRUE:
				copy_text(lbuf, pword);	/* reconstruct string */
				if ((expr = (*pfcn)(lbuf)) == EXPR_ERR) {
					ERR(E_EXPR_SYNTAX);
					expr = FALSE;
				}
				if (expr)
					if_state[if_level].state = PROCESSING;
				break;
			}

			extra = FALSE;
			break;

		case PP_ELSE:
			if (!if_state[if_level].else_ok) {
				ERR(E_ELSE_ERR);
				break;
			}

			/* if a true condition has just been processed, disable
			 * processing and skip to endif; if no true condition
			 * has been found yet, enable processing
			 */
			switch (if_state[if_level].state) {
			case PROCESSING:
				if_state[if_level].state = SKIP_TO_ENDIF;
				break;
			case AWAITING_TRUE:
				if_state[if_level].state = PROCESSING;
				break;
			}

			/* subsequent "elif" or "else" forbidden */
			if_state[if_level].else_ok = FALSE;
			extra = ntokens > 1;
			break;

		case PP_ENDIF:
			if (if_level < 1) {
				ERR(E_END_ERR);
				break;
			}
			if_level--;
			extra = ntokens > 1;
			break;

		case PP_IFDEF:
		case PP_IFNDEF:
			/* "if{n}def"s nested too deeply? */
			if (++if_level > MAX_IF_NESTING) {
				ERR(E_IF_NESTING);
				exit(EXIT_FAILURE);
				break;
			}

			/* if processing enabled at outer level, evaluate
			 * expression and enable/disable processing for
			 * following clause; if not, skip to matching endif
			 */
			if (if_state[if_level-1].state == PROCESSING) {
				copy_text(lbuf, pword);	/* reconstruct string */
				if ((expr = (*pfcn)(lbuf)) == EXPR_ERR) {
					ERR(E_EXPR_SYNTAX);
					expr = FALSE;
				}
				if_state[if_level].state = expr ? PROCESSING :
								  AWAITING_TRUE;
			} else
				if_state[if_level].state = SKIP_TO_ENDIF;

			if_state[if_level].else_ok = TRUE;
			extra = FALSE;
			break;

		case PP_INCLUDE:
			if (if_state[if_level].state == PROCESSING)
				do_include(mk_path(incpath, filename), *pword,
					   suffix == '?');
			extra = ntokens > 2;
			break;

		case PP_OTHER:	/* none of the above - parse as date */
			if (if_state[if_level].state == PROCESSING) {
				switch (parse(words, filename)) {
				case PARSE_INVDATE:
					ERR(E_INV_DATE);
					break;

				case PARSE_INVLINE:
					ERR(E_INV_LINE);
					break;

				case PARSE_NOMATCH:
					ERR(E_NO_MATCH);
					break;
				}
			}
			extra = FALSE;
			break;

		} /* end switch */

		if (extra) {		/* extraneous data? */
			sprintf(msg, E_GARBAGE, ptok);
			ERR(msg);
		}

	} /* end while */

	if (if_level > 0)
		fprintf(stderr, E_UNT_IFDEF, progname, filename);

	file_level--;
	curr_year = save_year;		/* restore saved year and font style */
	fontstyle[0] = save_font;
}


/*
 * Routines to free allocated data (symbol table and data structure) 
 */


/*
 * clear_syms - clear and deallocate the symbol table
 */
void
#ifdef PROTOS
clear_syms(void)
#else
clear_syms()
#endif
{
	int i;

	for (i = 0; i < MAX_PP_SYMS; i++) {
		if (pp_sym[i]) {
			free(pp_sym[i]);
			pp_sym[i] = NULL;
		}
		if (pp_val[i]) {
			free(pp_val[i]);
			pp_val[i] = NULL;
		}
	}

}


/*
 * cleanup - free all allocated data
 */
void
#ifdef PROTOS
cleanup(void)
#else
cleanup()
#endif
{
	int i, j;
	year_info *py, *pny;
	month_info *pm;
	day_info *pd, *pnd;

	for (py = head; py; py = pny) {		/* main data structure */
		pny = py->next;
		for (i = 0; i < 12; i++) {
			if ((pm = py->month[i]) == NULL)
				continue;
			for (j = 0; j < LAST_NOTE_DAY; j++)
				for (pd = pm->day[j]; pd; pd = pnd) {
					pnd = pd->next;
					free(pd->text);
					free(pd);
				}
			free(pm);
		}
	free(py);
	}

	clear_syms();				/* symbol table */

}


/*
 * Preprocessor token and symbol table routines
 */


/*
 * find_sym_name - look up symbol name; return symbol table index if found, PP_SYM_UNDEF
 * if not found
 */
int
#ifdef PROTOS
find_sym_name(char *sym)
#else
find_sym_name(sym)
	char *sym;
#endif
{
	int i;

	if (!sym)
		return PP_SYM_UNDEF;

	for (i = 0; i < MAX_PP_SYMS; i++)
		if (pp_sym[i] && ci_strcmp(pp_sym[i], sym) == 0)
			return i;

	return PP_SYM_UNDEF;
}


/*
 * find_sym_val - look up symbol's value; return symbol value table index if
 * found, PP_SYM_UNDEF if not found
 */
char *
#ifdef PROTOS
find_sym_val(char *sym)
#else
find_sym_val(sym)
     char *sym;
#endif
{
	int j;

	return (j=find_sym_name(sym))==PP_SYM_UNDEF ? NULL : pp_val[j];
}


/*
 * do_ifdef - return TRUE if 'expr' is true; FALSE if not; EXPR_ERR if invalid
 */
int
#ifdef PROTOS
do_ifdef(char *expr)
#else
do_ifdef(expr)
	char *expr;
#endif
{
	return parse_expr(expr);
}


/*
 * do_ifndef - return FALSE if 'expr' is true; TRUE if not; EXPR_ERR if invalid
 */
int
#ifdef PROTOS
do_ifndef(char *expr)
#else
do_ifndef(expr)
	char *expr;
#endif
{
	int val;

	return (val = parse_expr(expr)) == EXPR_ERR ? EXPR_ERR : ! val;
}


/*
 * do_define - enter 'sym' into symbol table; if 'sym' NULL, clear symbol table.
 * Always returns 0 (for compatibility with other dispatch functions).
 */
int
#ifdef PROTOS
do_define(char *sym)
#else
do_define(sym)
	char *sym;
#endif
{
	int i;

	if (! sym) {		/* null argument - clear all definitions */
		clear_syms();
		return 0;
	}

	if (DEBUG(DEBUG_PP))
		fprintf(stderr, "defining %s\n", sym);

	if (do_ifdef(sym))	/* already defined? */
		return 0;

	for (i = 0; i < MAX_PP_SYMS; i++)	/* find room for it */
		if (! pp_sym[i]) {
			strcpy(pp_sym[i] = alloc(strlen(sym)+1), sym);
			return 0;
		}

	fprintf(stderr, E_SYMFULL, progname, sym);
	return 0;
}


 /*
 * do_define_sym_val - enter symbol value 'val' into symbol table for symbol
 * 'sym'.  If 'sym' is not found, return 'PP_SYM_UNDEF'.
 */
int
#ifdef PROTOS
do_define_sym_val(char *sym,char *val)
#else
do_define_sym_val(sym,val)
	char *sym,*val;
#endif
{
	int i;

	i = find_sym_name(sym);
	if (i == PP_SYM_UNDEF)
		return PP_SYM_UNDEF;
	
	if (pp_val[i]) {
		free(pp_val[i]);
		pp_val[i]=NULL;
	}
	
	if (val) 
		strcpy(pp_val[i] = alloc(strlen(val)+1), val); 
	else 
		pp_val[i]=NULL;

	return 0;
}


/*
 * do_undef - undefine 'sym' and free its space; no error if not defined.
 * Always return 0 (for compatibility with other dispatch fcns).
 */
int
#ifdef PROTOS
do_undef(char *sym)
#else
do_undef(sym)
	char *sym;
#endif
{
	int i;

	if (! sym) 
		return 0;

	if (DEBUG(DEBUG_PP))
		fprintf(stderr, "undefining %s\n", sym);

	if ((i = find_sym_name(sym)) != PP_SYM_UNDEF) {
		if (pp_val[i]) {
			free(pp_val[i]);
			pp_val[i] = NULL;
		}
		free(pp_sym[i]);
		pp_sym[i] = NULL;
	}

	return 0;
}


/*
 * do_include - include specified file (optionally in "" or <>); always
 * returns 0 (for compatibility with related functions returning int)
 */
int
#ifdef PROTOS
do_include(char *path,
	   char *name,
	   int noerr)
#else
do_include(path, name, noerr)
	char *path;		/* path to file */
	char *name;		/* file name */
	int  noerr;		/* ignore nonexistent file */
#endif
{
	FILE *fp;
	char *p, incfile[STRSIZ], tmpnam[STRSIZ], sv_tmpnam[STRSIZ];
	int yy, yyfirst, yylast, sv_curr_year;

	if (! name)		/* whoops, no date file */
		return 0;

	/* copy name, stripping "" or <> */
	strcpy(tmpnam, name + (*name == '"' || *name == '<'));
	if ((p = P_LASTCHAR(tmpnam)) && (*p == '"' || *p == '>'))
		*p = '\0';

	/* save year and unmunged file name */
	sv_curr_year = curr_year;
	strcpy(sv_tmpnam, tmpnam);

	/* set up range of years to include: '%y', if present, is normally
	 * expanded to only the current year - except in "year all" mode where
	 * it is expanded to every applicable year
	 */
	yyfirst = yylast = curr_year;
	if (curr_year == ALL_YEARS) {
		yyfirst = yylast = init_year;
		if ((p = strchr(tmpnam, '%')) != NULL && *(p+1) == 'y')
			yylast = final_year;
	}

	/*
	 * loop over all years to be included
	 */
	for (yy = yyfirst; yy <= yylast; yy++) {
		strcpy(tmpnam, sv_tmpnam);	/* restore unmunged name */

		/* replace each instance of %y with last two digits of year */
		while ((p = strchr(tmpnam, '%')) != NULL && *(p+1) == 'y') {
			*p++ = (yy / 10) % 10 + '0';
			*p = yy % 10 + '0';
		}

		if ((fp = fopen(mk_filespec(incfile, path, tmpnam), "r")) == NULL) {
			if (noerr)	/* silently ignore in include? mode */
				continue;
			fprintf(stderr, E_FOPEN_ERR, progname, incfile);
			exit(EXIT_FAILURE);
		}

		curr_year = yy;			/* avoid infinite recursion */
		read_datefile(fp, incfile);	/* recursive call */
		fclose(fp);
	}

	curr_year = sv_curr_year;	/* restore original value */

	return 0;
}



/*
 * Dispatch functions for wildcard matching
 */


/*
 * is_anyday - dummy function which always returns TRUE
 */
int
#ifdef PROTOS
is_anyday(int mm GCC_UNUSED,
	  int dd GCC_UNUSED,
	  int yy GCC_UNUSED)
#else
is_anyday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return TRUE;
}


/*
 * is_weekday - determine whether or not mm/dd/yy is a weekday (i.e., the
 * day of the week normally prints in "logical black", the most prevalent
 * color)
 */
int
#ifdef PROTOS
is_weekday(int mm,
	   int dd,
	   int yy)
#else
is_weekday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return day_color[calc_weekday(mm, dd, yy)] == weekday_color;
}


/*
 * is_workday - determine whether or not mm/dd/yy is a workday (i.e., the
 * day of the week normally prints in black and the date is not a holiday)
 */
int
#ifdef PROTOS
is_workday(int mm,
	   int dd,
	   int yy)
#else
is_workday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return is_weekday(mm, dd, yy) && ! is_holiday(mm, dd, yy);
}


/*
 * is_holiday - determine whether or not mm/dd/yy is a holiday
 */
int
#ifdef PROTOS
is_holiday(int mm,
	   int dd,
	   int yy)
#else
is_holiday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	year_info *py;
	month_info *pm;

	pm = (py = find_year(yy, FALSE)) ? py->month[mm-1] : NULL;
	return pm ? (pm->holidays & (1L << (dd-1))) != 0 : FALSE;
}


/*
 * not_XXXXX - converses of is_XXXXX above
 */
int
#ifdef PROTOS
not_weekday(int mm,
	    int dd,
	    int yy)
#else
not_weekday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return !is_weekday(mm, dd, yy);
}


int
#ifdef PROTOS
not_workday(int mm,
	    int dd,
	    int yy)
#else
not_workday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return !is_workday(mm, dd, yy);
}


int
#ifdef PROTOS
not_holiday(int mm,
	    int dd,
	    int yy)
#else
not_holiday(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	return !is_holiday(mm, dd, yy);
}


/*
 * is_newmoon - determine whether or not mm/dd/yy is the date of a new moon
 */
int
#ifdef PROTOS
is_newmoon(int mm,
	   int dd,
	   int yy)
#else
is_newmoon(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	int quarter;

	(void) find_phase(mm, dd, yy, &quarter);
	return quarter == MOON_NM;
}


/*
 * is_firstq - determine whether or not mm/dd/yy is the date of a first quarter
 */
int
#ifdef PROTOS
is_firstq(int mm,
	  int dd,
	  int yy)
#else
is_firstq(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	int quarter;

	(void) find_phase(mm, dd, yy, &quarter);
	return quarter == MOON_1Q;
}


/*
 * is_fullmoon - determine whether or not mm/dd/yy is the date of a full moon
 */
int
#ifdef PROTOS
is_fullmoon(int mm,
	    int dd,
	    int yy)
#else
is_fullmoon(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	int quarter;

	(void) find_phase(mm, dd, yy, &quarter);
	return quarter == MOON_FM;
}


/*
 * is_lastq - determine whether or not mm/dd/yy is the date of a last quarter
 */
int
#ifdef PROTOS
is_lastq(int mm,
	 int dd,
	 int yy)
#else
is_lastq(mm, dd, yy)
	int mm;
	int dd;
	int yy;
#endif
{
	int quarter;

	(void) find_phase(mm, dd, yy, &quarter);
	return quarter == MOON_3Q;
}



/*
 * Routines to find predefined holidays too complicated to express as Pcal
 * date strings.  All add the matching date(s) (yes, holidays which span
 * multiple days are allowed) to the dates[] array (pointed to by pdate)
 * and return the number of matching dates.
 */ 


/*
 * find_easter - find Easter of current year; add to date array (adapted
 * by parties unknown from Knuth's _The Art of Computer Programming_, v. 1)
 */

#define METONIC 19			/* length of metonic cycle */

int
#ifdef PROTOS
find_easter(DATE *pdate)		/* pointer into date array */
#else
find_easter(pdate)
	DATE *pdate;			/* pointer into date array */
#endif
{
	/* Easter is defined as the Sunday after the full moon on or
	 * after March 21.  You could express that as "1st Sun after
	 * 1st full_moon ooa 21st day in March" instead of using this
	 * routine, but depending on your timezone your full moon might
	 * not fall on the same day as the "official" one.
	 */

	DATE *sv_pdate = pdate;
	register int epact, fm;
	int golden, century, nleap;

	golden = curr_year % METONIC + 1;
	century = curr_year / 100 + 1;
	nleap = 3 * century / 4 - 12;

	/* correct for moon's orbit */
	epact = (11 * golden + 20 + (8 * century + 5) / 25 - 5 - nleap) % 30;
	if (epact < 0)
	        epact += 30;
	if ((epact == 25 && golden > 11) || epact == 24)
	        ++epact;

	/* find full moon on or after 3/21 */
	fm = 44 - epact;
	if (fm < 21)
	        fm += 30;

	/* find date of following Sunday */
	fm += 7 - ((int)(5L * curr_year / 4) - nleap - 10 + fm) % 7;

	/* add Easter to date array (adjust if in April) */
	ADD_DATE(fm <= 31 ? MAR : APR, fm <= 31 ? fm : fm - 31, curr_year);

	/* return number of dates added (always 1 in this routine) */
	return pdate - sv_pdate;
}

#ifndef NO_ORTHODOX

/*
 * orthodox Easter related holiday calculations (ah@doc.ic.ac.uk)
 */

/*
 * days easter is from April 1st of given year
 */ 
int
#ifdef PROTOS
odox_easter_from_april1(int year)
#else
odox_easter_from_april1(year)
	int year;
#endif
{
	int a, b, leap;

	a = ( ( ((year-1) % METONIC) + 1 ) * METONIC + 15 ) % 30;
	leap = IS_LEAP(year) ? 1 : 0;
	b = (calc_weekday(1,1,year) + 90 + leap + a + 2) % 7;
	b = 7 - b + 3 + a;
	/* In 1923, 13 days  were added to the new greek calendar */
	if (year <= 1923)
		b -= 13;
	return (b);
}

#define odox_add(offs) do { \
	int mon; \
	mon = APR; \
	if (offs < 1) { \
		mon = MAR; \
		offs = 31 + offs; \
	} else if (offs > 30) { \
		mon = MAY; \
		offs -= 30; \
	} \
	ADD_DATE(mon, offs, curr_year); \
} while (0);

/* 
 * find the date for Orthodox Easter
 * (almost blindly copied from postscript src in gpscal)
 */
int
#ifdef PROTOS
find_odox_easter(DATE *pdate)	/* pointer into date array */
#else
find_odox_easter(pdate)
	DATE *pdate;		/* pointer into date array */
#endif
{
	DATE *sv_pdate = pdate;
	int offs;

	/* easter is days from 1st April for easter sun */
	offs = odox_easter_from_april1(curr_year);
	odox_add(offs);

	/* return number of dates added (always 1 in this routine) */
	return pdate - sv_pdate;
}

int
#ifdef PROTOS
find_odox_stgeorge(DATE *pdate)	/* pointer into date array */
#else
find_odox_stgeorge(pdate)
	DATE *pdate;		/* pointer into date array */
#endif
{
	DATE *sv_pdate = pdate;
	int offs;

	/* offs is days from 1st April for easter sun */
	offs = odox_easter_from_april1(curr_year);
	if (offs >= 23) {	/* it's before easter; will move to easter Mon */
		offs++;
		odox_add(offs);
	} else
		ADD_DATE(APR, 23, curr_year);
	return pdate - sv_pdate;
}

int
#ifdef PROTOS
find_odox_marcus(DATE *pdate)	/* pointer into date array */
#else
find_odox_marcus(pdate)
	DATE *pdate;		/* pointer into date array */
#endif
{
	DATE *sv_pdate = pdate;
	int offs;

	/* offs is days from 1st April for easter sun */
	offs = odox_easter_from_april1(curr_year);
	if (offs >= 25) {	/* it's before easter; will move to easter Tue */
		offs += 2;
		odox_add(offs);
	} else
		ADD_DATE(APR, 25, curr_year);
	return pdate - sv_pdate;
}

#endif /* !NO_ORTHODOX */

#ifdef FRIDAY_13th
	/* Allow user to specify "F13" in calendar file for 'Friday the 13th'
	 * events
	 */
int
#ifdef PROTOS
find_fri13th(DATE *pdate)       /* pointer into date array */
#else /* PROTOS */
find_fri13th(pdate)
	DATE *pdate;		/* pointer into date array */
#endif /* PROTOS */
{
	DATE *sv_pdate = pdate;
	int drlmonth;

	for (drlmonth = JAN; drlmonth <= DEC; drlmonth++) 
		if (calc_weekday(drlmonth, 13, curr_year) == 5)
			ADD_DATE(drlmonth, 13, curr_year);

	return pdate - sv_pdate;

}
#endif /* FRIDAY_13th */

/*
 * Keyword classification routines
 */

/*
 * get_month - convert alpha (or, optionally, numeric) string to month; return 
 * 1..12 if valid, NOT_MONTH if not, ALL_MONTHS if "all", ENTIRE_YEAR if "year"
 */
int
#ifdef PROTOS
get_month(char *cp,		/* string to convert */
	  int numeric_ok,	/* accept numeric string ? */
	  int year_ok)		/* accept "year"? */
#else
get_month(cp, numeric_ok, year_ok)
	char *cp;		/* string to convert */
	int numeric_ok;		/* accept numeric string ? */
	int year_ok;		/* accept "year"? */
#endif
{
	int mm;

	if (! cp)
		return NOT_MONTH;

	if (get_keywd(cp) == DT_ALL)
		return ALL_MONTHS;

	if (year_ok && get_keywd(cp) == DT_YEAR)
		return ENTIRE_YEAR;

	if (numeric_ok && isdigit(*cp))
		mm = atoi(cp);
	else
		/* accept month names in the active 'input' language only */
		for (mm = JAN; mm <= DEC; mm++) {
			/* Need to shorten length of string compare by 1
			 * character when the last character is '*' so that
			 * event like '2nd Sunday in May*' don't report 'no
			 * match for wildcard'.
			 */
			if ((ci_strncmp(cp, months_ml[input_language][mm-1], 
				strlen(cp) - ((*(cp + strlen(cp) - 1) == '*') ? 1 : 0)) == 0))
				break;
		}

	return mm >= JAN && mm <= DEC ? mm : NOT_MONTH;
}


/*
 * get_weekday - look up string in weekday list; return 0-6 if valid,
 * NOT_WEEKDAY if not.  If wild_ok flag is set, accept "day", "weekday",
 * "workday", "holiday", or moon phase and return appropriate value.
 */
int
#ifdef PROTOS
get_weekday(char *cp,
	    int wild_ok)
#else
get_weekday(cp, wild_ok)
	char *cp;
	int wild_ok;
#endif
{
	int w;

	if (!cp)
		return NOT_WEEKDAY;

	if (wild_ok) {		/* try wildcards first */
		for (w = WILD_FIRST_WKD; w <= WILD_LAST_WKD; w++)
			if (ci_strncmp(cp, days[w], strlen(days[w])) == 0)
				return w;
		if ((w = get_phase(cp)) != MOON_OTHER)
			return w + WILD_FIRST_MOON;
	}

	/* accept day names in the active 'input' language only */
	for (w = SUN; w <= SAT; w++) {
		int compare_count;
		/* To allow for proper detection of the day-of-week name in
		 * constructs from the configuration file like 'all Fridays in Oct'
		 * (i.e. with the plural form of the day-of-week name), we need to
		 * compare the fewest number of characters of the 2 strings.
		 */
		compare_count = 
			(strlen(cp) < strlen(days_ml[input_language][w])) ? 
			strlen(cp) : strlen(days_ml[input_language][w]);
		
		if (ci_strncmp(cp, days_ml[input_language][w], compare_count) == 0)
			return w;
	}
	
	return NOT_WEEKDAY;
}


/*
 * get_keywd - look up string in misc. keyword list; return keyword code
 * if valid, DT_OTHER if not
 */
int
#ifdef PROTOS
get_keywd(char *cp)
#else
get_keywd(cp)
	char *cp;
#endif
{
	KWD *k;

	if (!cp)
		return DT_OTHER;

	for (k = keywds;
	     k->name && ci_strncmp(cp, k->name, strlen(k->name));
	     k++)
		;

	return k->code;
}


/*
 * get_ordinal - look up string in ordinal list; return ordinal code (and
 * fill in ordinal value) if valid, return ORD_OTHER if not
 */
int
#ifdef PROTOS
get_ordinal(char *cp,
	    int *pval)
#else
get_ordinal(cp, pval)
	char *cp;
	int *pval;
#endif
{
	KWD_O *o;
	int val;
	char **psuf;

	if (!cp)
		return ORD_OTHER;

	if (isdigit(*cp) || *cp == '-') {		/* numeric? */
		if ((val = atoi(cp)) == 0)
			return ORD_OTHER;

		if (*cp == '-')				/* skip over number */
			cp++;
		cp += strspn(cp, DIGITS);

		for (psuf = ord_suffix; *psuf; psuf++)	/* find suffix */
			if (ci_strcmp(cp, *psuf) == 0) {
				*pval = val;
				return val < 0 ? ORD_NEGNUM : ORD_POSNUM;
			}

		return ORD_OTHER;
	}

	/* look for word in ordinals list */

	for (o = ordinals; o->name && ci_strncmp(cp, o->name, MIN_ORD_LEN); o++)
		;

	*pval = o->value;
	return o->code;
}


/*
 * get_phase - convert moon phase string to appropriate value
 */
int
#ifdef PROTOS
get_phase(char *cp)
#else
get_phase(cp)
	char *cp;
#endif
{
	KWD *p;

	if (!cp)
		return MOON_OTHER;

	for (p = phases; p->name && ci_strcmp(cp, p->name); p++)
		;

	return p->code;
}


/*
 * get_prep - look up string in preposition list; return preposition code if 
 * valid, PR_OTHER if not
 */
int
#ifdef PROTOS
get_prep(char *cp)
#else
get_prep(cp)
	char *cp;
#endif
{
	KWD *p;

	if (!cp)
		return PR_OTHER;

	for (p = preps; p->name && ci_strncmp(cp, p->name, MIN_PREP_LEN); p++)
		;

	return p->code;
}


/*
 * get_token - look up 'token' in list of preprocessor tokens; return its
 * index (PP_OTHER if not found)
 */
int
#ifdef PROTOS
get_token(char *token)
#else
get_token(token)
	char *token;
#endif
{
	KWD_F *p;

	for (p = pp_info; p->name; p++)
             if (ci_strncmp(token, p->name, MIN_PPTOK_LEN) == 0)
		return p - pp_info;

	return PP_OTHER;
}


/*
 * get_predef_event - look up string in predefined-event list; return its index if
 * found, NOT_PREDEF_EVENT if not
 */
int
#ifdef PROTOS
get_predef_event(char *cp)
#else
get_predef_event(cp)
	char *cp;
#endif
{
	KWD_H *p;

	for (p = predef_events; p->name; p++)
		if (ci_strncmp(cp, p->name, strlen(p->name)) == 0)
			return p - predef_events;

	return NOT_PREDEF_EVENT;
}


/*
 * date_type - examine token and return date type code; if DT_MONTH, DT_ORDINAL,
 * or DT_WEEKDAY, fill in appropriate code (and value if DT_ORDINAL)
 */
int
#ifdef PROTOS
date_type(char *cp,	/* pointer to start of token  */
	  int *pn,	/* token type code (returned) */
	  int *pv)	/* ordinal value (returned)   */
#else
date_type(cp, pn, pv)
	char *cp;	/* pointer to start of token  */
	int *pn;	/* token type code (returned) */
	int *pv;	/* ordinal value (returned)   */
#endif
{
	int n, v;

	/* look for weekdays first, to catch wildcards "1q", "3q", etc. */
	if ((n = get_weekday(cp, TRUE)) != NOT_WEEKDAY)	/* weekday name? */
		return (*pn = n, DT_WEEKDAY);

	if ((n = get_predef_event(cp)) != NOT_PREDEF_EVENT)	/* pre-defined event? */
		return (*pn = n, DT_PREDEF_EVENT);

	if ((n = get_ordinal(cp, &v)) != ORD_OTHER)	/* ordinal? */
		return (*pn = n, *pv = v, DT_ORDINAL);

	if (isdigit(*cp))				/* other digit? */
		return (IS_NUMERIC(cp) ||
		        (date_style == EUR_DATES && IS_EURDATE(cp))) ?
			DT_EURDATE : DT_DATE;

	/* "all" can be either a keyword or a month wildcard - look for
	   the former usage first */

	if ((n = get_keywd(cp)) != DT_OTHER)
		return n;

	if ((n = get_month(cp, FALSE, FALSE)) != NOT_MONTH)  /* month name? */
		return (*pn = n, DT_MONTH);

	return DT_OTHER;		/* unrecognized keyword - give up */

}


/*
 * Routines for entering data in the data structure (described in pcaldefs.h)
 */


/*
 * find_year - find record in year list; optionally create if not present 
 */
year_info *
#ifdef PROTOS
find_year(int year,
	  int insert)			/* insert if missing */
#else
find_year(year, insert)
	int year;
	int insert;			/* insert if missing */
#endif
{
	year_info *pyear, *plast, *p;

	for (plast = NULL, pyear = head;		/* search linked list */
	     pyear && pyear->year < year;
	     plast = pyear, pyear = pyear->next)
		;

	if (pyear && pyear->year == year)		/* found - return it */
		return pyear;

	if (insert) {		/* not found - insert it if requested */
		int i;
		p = (year_info *) alloc((int) sizeof(year_info));	/* create new record */
		p->year = year;
		for (i = 0; i < (int)(ARRAYSIZE(p->month)); i++)
			p->month[i] = NULL;
		p->next = pyear;				/* link it in */
		return *(plast ? &plast->next : &head) = p;
	}
	else
		return NULL;
}


/*
 * enter_day_info - enter text for specified day; avoid entering duplicates.
 * Returns PARSE_INVDATE if date invalid, PARSE_OK if OK; if symbol FEB_29_OK
 * is non-zero (cf. pcaldefs.h), will silently ignore Feb 29 of common year.
 */
int
#ifdef PROTOS
enter_day_info(int m,
	       int d,
	       int y,
	       int text_type,
	       char **pword)
#else
enter_day_info(m, d, y, text_type, pword)
	int m, d, y;
	int text_type;
	char **pword;
#endif
{
	static year_info *pyear;
	static int prev_year = 0;
	month_info *pmonth;
	day_info *pday, *plast;
	int is_holiday = text_type == HOLIDAY_TEXT;
	char text[LINSIZ], *tface;

	if (! is_valid(m, d >= FIRST_NOTE_DAY && text_type == NOTE_TEXT ? 1 : d, y))
		return (m == FEB && d == 29 && FEB_29_OK) ? PARSE_OK : PARSE_INVDATE;

	if (y != prev_year)		/* avoid unnecessary year lookup */
		pyear = find_year(y, 1);

	--m, --d;			/* adjust for use as subscripts */

	if ((pmonth = pyear->month[m]) == NULL) {	/* find/create month record */
		int i;
		pyear->month[m] = pmonth = (month_info *) alloc((int) sizeof(month_info));
		for (i = 0; i < (int)(ARRAYSIZE(pmonth->day)); i++)
			pmonth->day[i] = NULL;
	}

	if (is_holiday)
		pmonth->holidays |= (1L << d);

	/* insert text for day at end of list (preserving the order of entry
	 * for multiple lines on same day); eliminate those differing only
	 * in spacing and capitalization from existing entries
	 */

	copy_text(text, pword);	/* consolidate text from lbuf into text */

	if (DEBUG(DEBUG_DATES)) {
		char *p;
		fprintf(stderr, "%02d/%02d/%d%c '", m+1, d+1, y,
			is_holiday ? '*' : ' ');
		for (p = text; *p; p++)
			fprintf(stderr, isprint(*p) ? "%c" : "\\%03o",
				*p & CHAR_MSK);
		fprintf(stderr, "'\n");
	}

#if KEEP_NULL_LINES		/* preserve blank text lines in output */
	if (*text == '\0' && pmonth->day[d])
		strcpy(text, BLANK_TEXT);
#endif

	/* check that non-null text is unique */

	if (*text) {
		for (plast = NULL, pday = pmonth->day[d];
		     pday;
		     plast = pday, pday = pday->next)
			if (ci_strcmp(pday->text, text) == 0
#if KEEP_NULL_LINES
			    && strcmp(text, BLANK_TEXT) != 0
#endif
				) {
				pday->is_holiday |= is_holiday;
				return PARSE_OK;
				}

		/* unique - add to end of list */

		pday = (day_info *) alloc(sizeof(day_info));
		pday->is_holiday = is_holiday;

		if (fontstyle[0] == ROMAN || output_type != OUTPUT_PS) {
			/* copy text intact (no font shift) */
			strcpy(pday->text = (char *) alloc(strlen(text)+1), text);
		} else {
			/* prepend font shift sequence to text */
			tface = fontstyle[0] == BOLD   ? BOLD_FONT :
				fontstyle[0] == ITALIC ? ITALIC_FONT :
							 "";
			pday->text = (char *) alloc(strlen(tface) + strlen(text) + 2);
			strcpy(pday->text, tface);
			if (*tface)
				strcat(pday->text, " ");
			strcat(pday->text, text);
		}

		pday->next = NULL;
		*(plast ? &plast->next : &pmonth->day[d]) = pday;
	}

	return PARSE_OK;
}


/*
 * delete_day_info - delete text for specified day.
 * Returns PARSE_INVDATE if date invalid, PARSE_OK if OK; if symbol FEB_29_OK
 * is non-zero (cf. pcaldefs.h), will silently ignore Feb 29 of common year.
 * It will silently ignore attempts to remove non-existent entries.  It also
 * recalculates is_holiday.
 */
int
#ifdef PROTOS
delete_day_info(int m,
		int d,
		int y,
		int text_type,
		char **pword)
#else
delete_day_info(m, d, y, text_type, pword)
		int m, d, y;
		int text_type;
		char **pword;
#endif
{
	static year_info *pyear;
	static int prev_year = 0;
	month_info *pmonth;
	day_info *pday, *plast, *pdel = NULL, *pldel = NULL;
	int is_holiday = FALSE;
	int found = FALSE;
	char text[LINSIZ];
	
	if (! is_valid(m, d >= FIRST_NOTE_DAY && text_type == NOTE_TEXT ? 1 : d, y))
		return (m == FEB && d == 29 && FEB_29_OK) ? PARSE_OK : PARSE_INVDATE;
	
	if (y != prev_year)		/* avoid unnecessary year lookup */
		pyear = find_year(y, 1);
	
	--m, --d;			/* adjust for use as subscripts */
	
	if ((pmonth = pyear->month[m]) == NULL) {	/* ignore delete if no entries exist */
		return PARSE_OK;
	}
	if (pmonth->day[d] == NULL)
		return PARSE_OK;
	
	/* delete text for day from list ignoring differences in only
	 * spacing and capitalization in the existing entry.
	 */
	
	copy_text(text, pword); /* consolidate text from lbuf into text */
	
	if (DEBUG(DEBUG_DATES)) {
		char *p;
		fprintf(stderr, "%02d/%02d/%d%c '", m+1, d+1, y,
			is_holiday ? '*' : ' ');
		for (p = text; *p; p++)
			fprintf(stderr, isprint(*p) ? "%c" : "\\%03o",
				*p & CHAR_MSK);
		fprintf(stderr, "'\n");
	}

#if KEEP_NULL_LINES	      /* preserve blank text lines in output */
	if (*text == '\0' && pmonth->day[d])
		strcpy(text, BLANK_TEXT);
#endif

	/* check if non-null and find entry to delete */
  
	if (*text) {
		for (plast = NULL, pday = pmonth->day[d];
		     pday;
		     plast = pday, pday = pday->next) {
			if (ci_strcmp(pday->text, text) == 0) {
				found = TRUE;
				pdel = pday;
				pldel = plast;
			} else {
				is_holiday |= pday->is_holiday;
			}
		}
  
		if (found) {
			if (pldel) {
				pldel->next = pdel->next;
			} else {
				pmonth->day[d] = pdel->next;
			}
			free(pdel);
  
			if (is_holiday)
				pmonth->holidays |= (1L << d);
			else
				pmonth->holidays &= ~(1L << d);
		}
	}
  
	return PARSE_OK;
}


/*
 * enter_note - wrapper around enter_day_info for entering note text
 */
int
#ifdef PROTOS
enter_note(int mm,
	   char **pword,
	   int n)
#else
enter_note(mm, pword, n)
	int mm;
	char **pword;
	int n;
#endif
{
	int valid;

	/* expand "note all" into all twelve months */
	if (mm == ALL_MONTHS || mm == ENTIRE_YEAR) {
		valid = FALSE;	/* is at least one note box valid? */
		for (mm = JAN; mm <= DEC; mm++)
			valid |= enter_day_info(mm,
					note_day(mm, n, curr_year),
					curr_year, NOTE_TEXT,
					pword+1) == PARSE_OK;
		return valid ? PARSE_OK : PARSE_NOMATCH;
	}

	return enter_day_info(mm, note_day(mm, n, curr_year),
			curr_year, NOTE_TEXT, pword+1);
}

/*
 * process_date - wrapper around enter_day_info/delete_day_info for
 * entering/deleting date text
 */
int
#ifdef PROTOS
process_date(char **pword,
	   int *ptext_type,
	   char ***pptext)
#else
process_date(pword, ptext_type, pptext)
	char **pword;
	int *ptext_type;
	char ***pptext;
#endif
{
	int rtn, match;
	DATE *pd;

	/* parse date spec and enter information for each match */
	if ((rtn = parse_date(pword, ptext_type, pptext)) == PARSE_OK) {
		match = FALSE;
		for (pd = dates; pd->mm; pd++)

			if (delete_entry)
				match |= delete_day_info(pd->mm, pd->dd, pd->yy,
					*ptext_type, *pptext) == PARSE_OK;
			else
				match |= enter_day_info(pd->mm, pd->dd, pd->yy,
					*ptext_type, *pptext) == PARSE_OK;

		rtn = match ? PARSE_OK : PARSE_NOMATCH;
	}
	return rtn;
}

/*
 * Date parsing routines:
 */


/*
 * parse_ord - parse an ordinal date spec (e.g. "first Monday in September",
 * "every Sunday in October", "last workday in all"); return PARSE_OK if line
 * syntax valid, PARSE_INVLINE if not.  Write all matching dates (if any) to
 * global array dates[]; terminate date list with null entry.
 */
int
#ifdef PROTOS
parse_ord(int ord,		/* valid ordinal code - from get_ordinal() */
	  int val,		/* ordinal value - also from get_ordinal() */
	  char **pword)		/* pointer to word after ordinal */
#else
parse_ord(ord, val, pword)
	int ord;		/* valid ordinal code - from get_ordinal() */
	int val;		/* ordinal value - also from get_ordinal() */
	char **pword;		/* pointer to word after ordinal */
#endif
{
	int wkd, mon, mm, dd, len, doit, (*pfcn) PROTO((int, int, int));
	int val_first, val_last, val_incr, mon_first, mon_last;
	DATE *pdate, date;

	if ((wkd = get_weekday(*pword, TRUE)) == NOT_WEEKDAY ||	/* weekday */
	    *++pword == NULL ||					/* any word */
	    (mon = get_month(*++pword, FALSE, TRUE)) == NOT_MONTH) /* month */
		return PARSE_INVLINE;

	/* set up loop boundaries for month loop */
	mon_first = mon == ALL_MONTHS || mon == ENTIRE_YEAR ? JAN : mon;
	mon_last  = mon == ALL_MONTHS || mon == ENTIRE_YEAR ? DEC : mon;

	pdate = dates;			/* start of date array */

	/* special case of "all|odd|even <wildcard> in <month>|all|year" */

	if ((ord == ORD_ALL || ord == ORD_EVEN || ord == ORD_ODD) &&
	    IS_WILD(wkd)) {
		pfcn = pdatefcn[wkd - WILD_FIRST];
		doit = ord != ORD_EVEN;
		for (mm = mon_first; mm <= mon_last; mm++) {
			len = LENGTH_OF(mm, curr_year);
			if (mon != ENTIRE_YEAR)
				doit = ord != ORD_EVEN;
			for (dd = 1; dd <= len; dd++)
				if ((*pfcn)(mm, dd, curr_year)) {
					if (doit)
						ADD_DATE(mm, dd, curr_year);
					if (ord != ORD_ALL)
						doit = ! doit;
				}
		}
	}

	/* special case of "odd|even <weekday> in year" */

	else if ((ord == ORD_EVEN || ord == ORD_ODD) && mon == ENTIRE_YEAR) {
		date.mm = JAN;			/* starting date */
		date.dd = calc_day(ord == ORD_EVEN ? 2 : 1, wkd, JAN);
		date.yy = curr_year;
		do {				/* alternates throughout year */
			ADD_DATE(date.mm, date.dd, date.yy);
			date.dd += 14;
			normalize(&date);
		} while (date.yy == curr_year);
	}

	/* special case of "<ordinal>|last <weekday>|<wildcard> in year" */

	else if ((ord == ORD_NEGNUM || ord == ORD_POSNUM) &&
	         mon == ENTIRE_YEAR) {
		if (calc_year_day(val, wkd, &date))
			ADD_DATE(date.mm, date.dd, date.yy);
	}

	/* all other combinations of ordinal and day */

	else {
		/* set up loop boundaries for "wildcard" ordinals */

		val_first = ord == ORD_ALL || ord == ORD_ODD ? 1 :
			    ord == ORD_EVEN ? 2 : val;
		val_last  = ord == ORD_ALL || ord == ORD_ODD ? 5 :
			    ord == ORD_EVEN ? 4 : val;
		val_incr  = ord == ORD_ODD || ord == ORD_EVEN ? 2 : 1;

		for (mm = mon_first; mm <= mon_last; mm++)
			for (val = val_first; val <= val_last; val += val_incr)
				if ((dd = calc_day(val, wkd, mm)) != 0)
					ADD_DATE(mm, dd, curr_year);
	}

	TERM_DATES;		/* terminate array with null entry */
	return PARSE_OK;
}


/*
 * parse_rel - parse a relative date spec (e.g. "Friday after fourth Thursday
 * in November", "2nd Saturday after first Friday in all"; return PARSE_OK if
 * line syntax valid, PARSE_INVLINE if not.  Transform all dates that match
 * the base date to the appropriate day, month, and year.
 *
 * This calls parse_date() recursively in order to handle cases such as
 * "Friday after Tuesday before last day in all".
 */
int
#ifdef PROTOS
parse_rel(int val,		/* valid (positive) ordinal value */
	  int wkd,		/* valid weekday code - from get_weekday() */
	  char **pword,		/* pointer to word after weekday */
	  int *ptype,		/* return text type (holiday/non-holiday) */
	  char ***pptext)	/* return pointer to first word of text */
#else
parse_rel(val, wkd, pword, ptype, pptext)
	int val;		/* valid (positive) ordinal value */
	int wkd;		/* valid weekday code - from get_weekday() */
	char **pword;		/* pointer to word after weekday */
	int *ptype;		/* return text type (holiday/non-holiday) */
	char ***pptext;		/* return pointer to first word of text */
#endif
{
	int prep, n, rtn, base_wkd, incr = 1, (*pfcn) PROTO((int, int, int));
	DATE *pd;

	/* we have the weekday - now look for the preposition */
	if ((prep = get_prep(*pword++)) == PR_OTHER)
		return PARSE_INVLINE;

	/* get the base date */
	if ((rtn = parse_date(pword, ptype, pptext)) != PARSE_OK)
		return rtn;

	/* transform date array in place - note that the relative date may
	   not be in the same month or even year */

	if (IS_WILD(wkd)) {		/* wildcard for weekday name? */
		pfcn = pdatefcn[wkd - WILD_FIRST];

		for (pd = dates; pd->mm; pd++) {

			/* search for nearest matching date */

			switch (prep) {
			case PR_BEFORE:
				pd->dd -= 1;	/* start with previous date */
				normalize(pd);
				/* fall through */
			case PR_ON_BEFORE:
				incr = -1;	/* search backwards */
				break;

			case PR_AFTER:
				pd->dd += 1;	/* start with following date */
				normalize(pd);
				/* fall through */
			case PR_ON_AFTER:
				incr = 1;	/* search forward */
				break;

			case PR_NEAREST:
				/* If NEAREST_INCR (cf. pcaldefs.h) is 1,
				 * pcal will disambiguate "nearest" in favor
				 * of the later date; if -1, in favor of the
				 * earlier.  "incr" will take the values
				 * 1, -2, 3, -4, ... or -1, 2, -3, 4 ...
				 * respectively during the search loop below.
				 *
				 * ("nearest_before" and "nearest_after" were
				 * added to v4.6 to allow the user to specify
				 * disambiguation of two equally near dates.)
				 */
				incr = NEAREST_INCR;
				val = 1;	/* ordinals meaningless here */
				break;

			case PR_NEAREST_BEFORE:
				incr = -1;	/* start searching backward */
				val = 1;	/* ordinals meaningless here */
				break;

			case PR_NEAREST_AFTER:
				incr = 1;	/* start searching forward */
				val = 1;	/* ordinals meaningless here */
				break;
			}

			n = val;
			while (!((*pfcn)(pd->mm, pd->dd, pd->yy) && --n == 0)) {
				pd->dd += incr;
				normalize(pd);
				/* if searching for "nearest" date, invert sign
				 * and bump magnitude of 'incr' on each pass
				 */
				if (prep == PR_NEAREST ||
				    prep == PR_NEAREST_BEFORE ||
				    prep == PR_NEAREST_AFTER)
					incr -= (incr > 0) ? (2 * incr + 1) :
							     (2 * incr - 1);
			}
		}

	} else  {			/* explicit weekday name */
		for (pd = dates; pd->mm; pd++) {

			/* calculate nearest matching weekday - note that
			 * "nearest_before" and "nearest_after" are synonyms
			 * for "before" and "after"
			 */

			base_wkd = calc_weekday(pd->mm, pd->dd, pd->yy);
			switch (prep) {
			case PR_BEFORE:
			case PR_NEAREST_BEFORE:
			case PR_ON_BEFORE:
				if (prep != PR_ON_BEFORE || wkd != base_wkd)
					pd->dd -= 7 - (wkd - base_wkd + 7) % 7;
				pd->dd -= 7 * (val - 1);
				break;

			case PR_AFTER:
			case PR_NEAREST_AFTER:
			case PR_ON_AFTER:
				if (prep != PR_ON_AFTER || wkd != base_wkd)
					pd->dd += (wkd - base_wkd + 6) % 7 + 1;
				pd->dd += 7 * (val - 1);
				break;

			case PR_NEAREST:
				/* use closer of previous and next */
				val = wkd - base_wkd;
				pd->dd += (val > 3) ? (val - 7) :
					  (val < -3) ? (val + 7) :
					  val;
				break;

			default:
				return PARSE_INVLINE;
				break;
			}

			normalize(pd);	/* adjust for month/year crossing */
		}
	}

	return PARSE_OK;
}


/*
 * parse_date - parse a date specification in any of its myriad forms; upon
 * return, array dates[] will contain a list of all the dates that matched,
 * terminated by a null entry.  Also fill in the date type (holiday/non-
 * holiday) code and the pointer to the first word of text and set flag
 * 'curr_year_reset' if date spec (e.g., dd/mm/yy) explicitly reset year.
 */
int
#ifdef PROTOS
parse_date(char **pword,	/* first word to parse */
	   int *ptype,		/* return date type (holiday/non-holiday) */
	   char ***pptext)	/* return pointer to first word of text */
#else
parse_date(pword, ptype, pptext)
	char **pword;		/* first word to parse */
	int *ptype;		/* return date type (holiday/non-holiday) */
	char ***pptext;		/* return pointer to first word of text */
#endif
{
	int mm, dd, yy;
	int token, n, v, ord, val, wkd, rtn;
	DATE *pdate;
	char *cp;

	pdate = dates;
	curr_year_reset = FALSE;	/* set below if date is dd/mm/yy */

	switch (token = date_type(*pword, &n, &v)) {

	case DT_MONTH:		/* <month> dd */
		if (date_style != USA_DATES)
			return PARSE_INVLINE;

		if ((cp = *++pword) == NULL)
			return PARSE_INVLINE;

		ADD_DATE(n, atoi(cp), curr_year);
		TERM_DATES;

		break;

	case DT_DATE:		/* mm/dd{/yy} | dd/mm{/yy} */
		n = split_date(*pword,
			       date_style == USA_DATES ? &mm : &dd,
			       date_style == USA_DATES ? &dd : &mm,
			       &yy);

		if (n > 2) {			/* year present? */
			if (yy < 100)
				yy += century();
			curr_year = yy;		/* reset current year */
			curr_year_reset = TRUE;
		}

		ADD_DATE(mm, dd, curr_year);
		TERM_DATES;

		break;

	case DT_EURDATE:	/* dd [ <month> | "all" ] */
		if (date_style != EUR_DATES)
			return PARSE_INVLINE;

		dd = atoi(*pword);

		if (get_keywd(*++pword) == DT_ALL) {
			for (mm = JAN; mm <= DEC; mm++)		/* wildcard */
				ADD_DATE(mm, dd, curr_year);
		}
		else {						/* one month */
			if ((mm = get_month(*pword, TRUE, FALSE)) == NOT_MONTH)
				return PARSE_INVLINE;

			ADD_DATE(mm, dd, curr_year);
		}

		TERM_DATES;
		break;

	case DT_ALL:		/* "all" <weekday> "in" [ <month> | "all" ] */
				/* or "all" <day>" */
		if ((cp = *(pword+1)) && (*(cp += strspn(cp, DIGITS)) == '\0' ||
		    *cp == '*')) {
			dd = atoi(*++pword);		/* "all" <day> */
			for (mm = JAN; mm <= DEC; mm++)
				ADD_DATE(mm, dd, curr_year);
			TERM_DATES;
			break;		/* leave switch */
		}

		n = ORD_ALL;	/* "all" <weekday> ... */
		v = 0;
	     /* fall through */

	case DT_ORDINAL:	/* <ordinal> <weekday> in [ <month> | "all" ] */
			     /* or <ordinal> <weekday> <prep> <date> */
		ord = n;
		val = v;
		/* disambiguate above cases based on preposition */
		if (ord == ORD_POSNUM && pword[1] &&
		    (get_prep(pword[2]) != PR_OTHER)) {
			if ((wkd = get_weekday(pword[1], TRUE)) == NOT_WEEKDAY)
				return PARSE_INVLINE;
			return parse_rel(val, wkd, pword += 2, ptype, pptext);
		}
		if ((rtn = parse_ord(ord, val, pword + 1)) != PARSE_OK)
			return rtn;

		pword += 3;		/* last word of date */
		break;

	case DT_WEEKDAY:	/* <weekday> <prep> <date> */
		wkd = n;
		/* parse_rel() calls parse_date() recursively */
		return parse_rel(1, wkd, ++pword, ptype, pptext);
		break;

	case DT_PREDEF_EVENT:	/* predefined event */
		/*
		 * predefined events will either have a redefinition string
		 * or a dispatch function - never both (cf. pcallang.h)
		 */
		if (predef_events[n].pfcn == NULL) {
			char redef[STRSIZ], *rwords[20], **pdum;
			int rtn, idum;

			/* tokenize local copy of redefinition string */
			strcpy(redef, predef_events[n].def);
			(void) loadwords(rwords, redef);

			/* call parse_date() recursively to parse the
			 * redefinition - if OK, drop through to fill in
			 * real ptype and pptext from original string (this
			 * works basically because the holiday is always
			 * the last token before the text)
			 */
			if ((rtn = parse_date(rwords, &idum, &pdum)) != PARSE_OK)
				return rtn;
		} else {
			/* predefined event has a dispatch function - use it */
			pdate += (*predef_events[n].pfcn)(pdate);
			TERM_DATES;
		}
		break;

	default:
		return PARSE_INVLINE;
		break;
	}

	/* at this point, pword points to the last component of the date;
	 * fill in type code and pointer to following word (start of text)
	 */
	*ptype = LASTCHAR(*pword) == '*' ? HOLIDAY_TEXT : DAY_TEXT;
	*pptext = ++pword;

	return PARSE_OK;
}


/*
 * parse - parse non-preprocessor lines in date file
 *
 * This routine parses "year", "opt", "note", and date entries in the date
 * file.  It calls parse_date() to parse date entries (and enter the date(s)
 * matched in global array "dates"), and then calls enter_day_info() to
 * enter each date found (and its associated text) in the date file.
 *
 * N.B.: "inc" and other cpp-like lines are handled in read_datefile().
 *
 */
int
#ifdef PROTOS
parse(char **pword,		/* pointer to first word to parse */
      char *filename)		/* name of file (for error messages) */
#else
parse(pword, filename)
	char **pword;		/* pointer to first word to parse */
	char *filename;		/* name of file (for error messages) */
#endif
{
	register char *cp;
	char **ptext;
	int mm, yy;
	int text_type, n, v, match;
	int token;

	/*
	 * Get first field and call date_type() to decode it
         */
	cp = *pword;

	switch (token = date_type(cp, &n, &v)) {

	case DT_YEAR:
		if ((cp = *++pword) != NULL && (yy = atoi(cp)) > 0) {
			if (yy < 100)
				yy += century();
			curr_year = yy;
			return PARSE_OK;
		}
		if (strcmp(cp, ALL) == 0 || strcmp(cp, "*") == 0) {
			curr_year = ALL_YEARS;
			return PARSE_OK;
		}
		return PARSE_INVLINE;	/* year missing or non-numeric */
		break;

	case DT_OPT:
		if (!get_args(pword, P_OPT, filename, FALSE)) {
			usage(stderr, FALSE);
			exit(EXIT_FAILURE);
		}
		return PARSE_OK;
		break;

	case DT_INPUT_LANGUAGE:
		pword++;  /* point to 2-letter 'language code' string */
		if (*pword) {
			int i;
			for (i = 0; i < NUM_LANGUAGES; i++) {
				if (ci_strncmp(lang_id[i], *pword, MIN_LANG_LEN) == 0) {
					input_language = i;
					return PARSE_OK;
				}
			}
		}
		return PARSE_INVLINE;
		break;

	case DT_NOTE:
		/* look for optional "/<n>" following keyword */
		n = (cp = strrchr(cp, '/')) ? atoi(++cp) : 0;

		if ((mm = get_month(*++pword, TRUE, TRUE)) == NOT_MONTH)
			return PARSE_INVLINE;

		/* if "year all" in effect, wildcard all applicable years */
		if (curr_year == ALL_YEARS) {
			match = FALSE;
			for (curr_year = init_year;
			     curr_year <= final_year;
			     curr_year++)
				match |= enter_note(mm, pword, n) == PARSE_OK;
			curr_year = ALL_YEARS;	/* reset to wildcard */
			return match ? PARSE_OK : PARSE_NOMATCH;
		} else
			return enter_note(mm, pword, n);
		break;

	case DT_OTHER:		/* unrecognized token */
		return PARSE_INVLINE;
		break;

	case DT_DELETE:         /* fall through to actually delete entry */
		pword++;
		delete_entry = TRUE;


	/* assume anything else is a date */

	default:
		/* If the current year is a wildcard ("all" or "*" - see
		 * above), enter the date for each year covered at least
		 * partially by the calendar.  Note that a "mm/dd/yy" date
		 * spec explicitly resets curr_year; parse_date() sets
		 * the 'curr_year_reset' flag when this happens so that
		 * we can quit immediately.
		 */
		if (curr_year == ALL_YEARS) {
			match = FALSE;
			/* loop over each applicable year */
			for (curr_year = init_year;
			     curr_year <= final_year;
			     curr_year++) {
				match |= process_date(pword, &text_type, &ptext) ==
						PARSE_OK;
				if (curr_year_reset) /* quit if year reset */
					return match ? PARSE_OK : PARSE_NOMATCH;
			}

			/* restore year to wildcard for next time */
			curr_year = ALL_YEARS;
			delete_entry = FALSE;
			return match ? PARSE_OK : PARSE_NOMATCH;
		}

		match = process_date(pword, &text_type, &ptext);
		delete_entry = FALSE;
		return match;

		break;
	}
}
