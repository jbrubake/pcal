/*
 *  moonphas.c - encapsulates routines used by Pcal for moon phase calculation
 *
 *  The following suite of routines are to calculate the phase of the moon
 *  for a given month, day, year.  They compute the phase of the moon for
 *  noon (UT) on the day requested, the start of the Julian day.
 *
 *  Contents:
 *
 *		calc_phase
 *		find_moonfile
 *		find_phase
 *		read_moonfile
 *
 *  Revision history:
 *
 *	4.8.0	B.Marr	2004-11-15	Remove Ctl-L (page eject) characters 
 *					from source file.  Remove spaces
 *					embedded within tab fields.
 * 
 *	4.7	AWR	02/24/1998	rename "template" (C++ reserved word)
 *					as "templt" (cf. make_moonfile())
 *
 *			12/21/1997	clean up gcc warnings in -Wall mode
 *
 *			07/27/1997	delete obsolete FPR and PRT macros
 *
 *	4.5	AWR	11/06/1993	accept "opt -[AE]" in the moon file
 *
 *			06/25/1993	revise for pre-ANSI math libraries
 *					without fmod()
 *
 *			04/28/1993	restructure function definitions so
 *					function name appears in first column
 *					(to facilitate searching for definition
 *					by name)
 *
 *			04/22/1992	eliminated some unused variables and
 *					calculations
 *
 *	4.4	AWR	01/20/1992	use alternate timezone spec (-z)
 *
 *			12/16/1991	Revise find_moonfile() for efficiency
 *
 *	4.3	AWR	12/05/1991	Search for moon file in Pcal's path
 *					as well as in calendar file's path
 *
 *			10/25/1991	Many changes for support of moon
 *					phase wildcards and -Z flag
 *
 *	4.11	AWR	08/23/1991	Revise is_quarter() to eliminate
 *					occasional missing or duplicate
 *					quarter-moons in "-m" mode; add
 *					gen_phases()
 *
 *	4.1	AWR	08/02/1991	Fix bug in julday() where result is
 *					calculated incorrectly if floor() has
 *					no prototype
 *
 *      4.01    RLD     03/19/91        Upgraded calc_phase() to accurately
 *                                      calculate the lunar phase for any
 *                                      day without needing to resort to
 *                                      a moon file.
 *
 *	4.0	AWR	03/07/1991	Add find_moonfile()
 *
 *			01/15/1991	Author: translated PostScript
 *					routines to C and added moon
 *					file routines
 */

/*
 * Standard headers:
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
 * Pcal-specific definitions:
 */

#include "pcaldefs.h"
#include "pcalglob.h"
#include "pcallang.h"

/*
 * Macros:
 */

/*  Astronomical constants. */

#define epoch	    2444238.5	   /* 1980 January 0.0 */

/*  Constants defining the Sun's apparent orbit. */

#define elonge	    278.833540	   /* ecliptic longitude of the Sun
				        at epoch 1980.0 */
#define elongp	    282.596403	   /* ecliptic longitude of the Sun at
				        perigee */
#define eccent      0.016718       /* eccentricity of Earth's orbit */

/*  Elements of the Moon's orbit, epoch 1980.0. */

#define mmlong      64.975464      /* moon's mean lonigitude at the epoch */
#define mmlongp     349.383063	   /* mean longitude of the perigee at the
                                        epoch */
#define mlnode	    151.950429	   /* mean longitude of the node at the
				        epoch */
#define synmonth    29.53058868    /* synodic month (new Moon to new Moon) */

#undef PI
#define PI 3.14159265358979323846  /* assume not near black hole nor in
				        Tennessee ;) */

/*  Handy mathematical functions. */

#define sgn(x) (((x) < 0) ? -1 : ((x) > 0 ? 1 : 0))	  /* extract sign */
#ifndef abs
#define abs(x) ((x) < 0 ? (-(x)) : (x))			  /* absolute val */
#endif
#define fixangle(a) ((a) - 360.0 * (floor((a) / 360.0)))  /* fix angle	  */
#define torad(d) ((d) * (PI / 180.0))			  /* deg->rad	  */
#define todeg(d) ((d) * (180.0 / PI))			  /* rad->deg	  */
#define FNITG(x) (sgn (x) * floor (abs (x)))

/* fmod() may be absent from pre-ANSI math libraries */

#ifdef __STDC__
#define FMOD(x, y)	fmod(x, y)
#else
#define FMOD(x, y)	((x) - ((long) ((x)/(y)) * (y)))
#endif

/* Misc. stuff, to disappear someday along with moon file routines */

#define HOUR		12		/* hour of day when phase calculated */
#define PERIOD		synmonth	/* for backward compatibility */

/* interpolate phase for day "d" from moon_info array elements "n1" and "n2" */
#define CALC_PHASE(d, n1, n2) \
	moon_info[n1].phase + ((d) - moon_info[n1].doy) * \
	((moon_info[n2].phase - moon_info[n1].phase) / \
	 (moon_info[n2].doy - moon_info[n1].doy))

/* generate error message, close file, and quit */
#define ERR_EXIT(msg)	\
	do { ERR(msg); fclose(fp); return FALSE; } while (0)

/* day and phase sequence error conditions - cf. read_moonfile() */
#define DAY_TOO_SOON	(nrec > 1 && doy < prevdoy + 6)
#define DAY_TOO_LATE	(doy > prevdoy + 9)
#define WRONG_PHASE	(nrec > 1 && ph != (prevph + 1) % 4)


/*
 * Globals:
 */

typedef struct {
	int	doy;	 /* day of year (1..366) */
	int	quarter; /* quarter (MOON_NM, MOON_1Q, etc.) */
	double	phase;	 /* moon phase (cycles since new moon prior to 1/1) */
} MOON_INFO;

static MOON_INFO moon_info[60];		/* quarter moons for year + dummies */


/*
 *  Routines to accurately calculate the phase of the moon
 *
 *  Originally adapted from "moontool.c" by John Walker, Release 2.0.
 *
 *      This routine (calc_phase) and its support routines were adapted from
 *      phase.c (v 1.2 88/08/26 22:29:42 jef) in the program "xphoon"        
 *      (v 1.9 88/08/26 22:29:47 jef) by Jef Poskanzer and Craig Leres.
 *      The necessary notice follows...
 *
 *      Copyright (C) 1988 by Jef Poskanzer and Craig Leres.
 *
 *      Permission to use, copy, modify, and distribute this software and its
 *      documentation for any purpose and without fee is hereby granted,
 *      provided that the above copyright notice appear in all copies and that
 *      both that copyright notice and this permission notice appear in
 *      supporting documentation.  This software is provided "as is" without
 *      express or implied warranty.
 *
 *      These were added to "pcal" by RLD on 19-MAR-1991
 */


/*
 *  julday -- calculate the julian date from input month, day, year
 *      N.B. - The Julian date is computed for noon UT.
 *
 *      Adopted from Peter Duffett-Smith's book `Astronomy With Your
 *      Personal Computer' by Rick Dyson 18-MAR-1991
 */
static double
#ifdef PROTOS
julday(int month,
       int day,
       int year)
#else
julday(month, day, year)
	int month, day, year;
#endif
{
        int mn1, yr1;
        double a, b, c, d, djd;

        mn1 = month;
        yr1 = year;
        if ( yr1 < 0 ) yr1 = yr1 + 1;
        if ( month < 3 )
            {
	        mn1 = month + 12;
	        yr1 = yr1 - 1;
            }
        if (( year < 1582 ) ||
            ( year == 1582  && month < 10 ) ||
                ( year == 1582  && month == 10 && day < 15.0 ))
                    b = 0;
                else
                {
                    a = floor (yr1 / 100.0);
                    b = 2 - a + floor (a / 4);
                }
        if ( yr1 >= 0 )
            c = floor (365.25 * yr1) - 694025;
        else
            c = FNITG ((365.25 * yr1) - 0.75) - 694025;
        d = floor (30.6001 * (mn1 + 1));
        djd = b + c + d + day + 2415020.0;
        return djd;
}


/*
 *  kepler - solve the equation of Kepler
 */
static double
#ifdef PROTOS
kepler(double m,
       double ecc)
#else
kepler(m, ecc)
	double m, ecc;
#endif
{
	double e, delta;
#define EPSILON 1E-6

	e = m = torad(m);
	do {
	   delta = e - ecc * sin(e) - m;
	   e -= delta / (1 - ecc * cos(e));
	} while (abs(delta) > EPSILON);
	return e;
}


/*
 *  calc_phase - calculate phase of moon as a fraction:
 *
 *	The argument is the time for which the phase is requested,
 *	expressed as the month, day and year.  It returns the phase of
 *	the moon (0.0 -> 0.99) with the ordering as New Moon, First Quarter,
 *      Full Moon, and Last Quarter.
 *
 *	Converted from the subroutine phase.c used by "xphoon.c" (see
 *      above disclaimer) into calc_phase() for use in "moonphas.c"
 *	by Rick Dyson 18-MAR-1991
 */
double
#ifdef PROTOS
calc_phase(int month,
	   int inday,
	   int year)
#else
calc_phase(month, inday, year)
	int month, inday, year;
#endif
{
	double Day, N, M, Ec, Lambdasun, ml, MM, Ev, Ae, A3, MmP,
	       mEc, A4, lP, V, lPP, MoonAge, pdate, phase;
	static int first = TRUE;

	/* get offset from UTC first time through; normalize to +- 1/2 day */
	if (first) {
		utc_offset = FMOD(atof(time_zone), 24.0) / 24.0;
		if (utc_offset < 0.0)
			utc_offset++;
		if (utc_offset > 0.5)
			utc_offset -= 0.5;
		first = FALSE;
		}

	/*  need to convert month, day, year into a Julian pdate */
	pdate = julday (month, inday, year) + utc_offset;

        /*  Calculation of the Sun's position. */

	Day = pdate - epoch;			/* date within epoch */
	N = fixangle((360 / 365.2422) * Day);	/* mean anomaly of the Sun */
	M = fixangle(N + elonge - elongp);      /* convert from perigee
					         co-ordinates to epoch 1980.0 */
	Ec = kepler(M, eccent);			/* solve equation of Kepler */
	Ec = sqrt((1 + eccent) / (1 - eccent)) * tan(Ec / 2);
	Ec = 2 * todeg(atan(Ec));		/* true anomaly */
        Lambdasun = fixangle(Ec + elongp);	/* Sun's geocentric ecliptic
					         longitude */

        /*  Calculation of the Moon's position. */

        /*  Moon's mean longitude. */
	ml = fixangle(13.1763966 * Day + mmlong);

        /*  Moon's mean anomaly. */
	MM = fixangle(ml - 0.1114041 * Day - mmlongp);

	/*  Evection. */
	Ev = 1.2739 * sin(torad(2 * (ml - Lambdasun) - MM));

	/*  Annual equation. */
	Ae = 0.1858 * sin(torad(M));

	/*  Correction term. */
	A3 = 0.37 * sin(torad(M));

	/*  Corrected anomaly. */
	MmP = MM + Ev - Ae - A3;

	/*  Correction for the equation of the centre. */
	mEc = 6.2886 * sin(torad(MmP));

	/*  Another correction term. */
	A4 = 0.214 * sin(torad(2 * MmP));

	/*  Corrected longitude. */
	lP = ml + Ev + mEc - Ae + A4;

	/*  Variation. */
	V = 0.6583 * sin(torad(2 * (lP - Lambdasun)));

	/*  True longitude. */
	lPP = lP + V;

	/*  Calculation of the phase of the Moon. */

	/*  Age of the Moon in degrees. */
	MoonAge = lPP - Lambdasun;

	if ((phase = fixangle(MoonAge) / 360.0) < 0.0)
		phase += 1.0;
        return (phase);
}


/*
 *  is_quarter - if current phase of moon coincides with quarter moon, return
 *  MOON_NM, MOON_1Q, etc.; otherwise return MOON_OTHER;
 *  
 */
static int
#ifdef PROTOS
is_quarter(double prev,
	   double curr,
	   double next)
#else
is_quarter(prev, curr, next)
	double prev, curr, next;
#endif
{
	int quarter;
	double phase, diff;

	if (curr < prev)	/* adjust phases for 1 -> 0 wraparound */
		curr++;
	if (next < prev)
		next++;

	/* if a quarter moon has occurred between "prev" and "next", return
	 * TRUE if "curr" is closer to it than "prev" or "next" is	
	 */
	for (quarter = 1; quarter <= 4; quarter++) 
		if (prev < (phase = quarter/4.0) && next > phase &&
		    (diff = abs(curr - phase)) < phase - prev &&
		    diff < next - phase)
			return quarter % 4;		/* MOON_NM == 0 */

	return MOON_OTHER;
}


/*
 * Routines to read moon file and calculate moon phase from data within
 */

 
/*
 * make_moonfile - create the base name for the moon file from the supplied
 * template and year; fill it in and return pointer to it
 */
static char *
#ifdef PROTOS
make_moonfile(char *filename,
	      char *templt,
	      int year)
#else
make_moonfile(filename, templt, year)
	char *filename;		/* base file name (output) */
	char *templt;		/* file name template */
	int year;		/* year to substitute for "%y" in template */
#endif
{
	char *p;

	/* copy the template to the file name; replace "%y" (if present)
	 * with last two digits of year (as per other year substitutions)
	 */
	strcpy(filename, templt);
	if ((p = strchr(filename, '%')) && p[1] == 'y') {
		*p++ = '0' + (year / 10) % 10;
		*p   = '0' + year % 10;
	}

	return filename;
}


/*
 * find_moonfile - look for moon file for specified year.  If it exists
 * and is readable, return its full path name; else return NULL.  (There
 * are admittedly ways to do this without attempting to open the file -
 * cf. find_executable() in pcalutil.c - but they may not be portable.)
 */
char *
#ifdef PROTOS
find_moonfile(int year)
#else
find_moonfile(year)
	int year;
#endif
{
	static char filename[STRSIZ];
	char path[STRSIZ], *pathlist[3], moonfile[STRSIZ];
	FILE *fp;
	static int sv_year = 0;
	static char *sv_path = NULL;

	/* if -z or -e specified, use algorithm even if moon file present */
	if (tz_flag || ! *datefile)
		return NULL;

	/* if year hasn't changed, then return the previous path */
	if (year == sv_year)
		return sv_path;

	/* create list of paths for alt_fopen() to search */
	pathlist[0] = mk_path(path, datefile);	/* datefile path */
	pathlist[1] = progpath;			/* program path */
	pathlist[2] = NULL;			/* terminate list */

	/* attempt to open the moon file */
	fp = alt_fopen(filename, make_moonfile(moonfile, MOONFILE, year),
		       pathlist, "r");

#ifdef ALT_MOONFILE
	if (!fp)			/* try again with alternate name */
		fp = alt_fopen(filename, make_moonfile(moonfile, ALT_MOONFILE,
			       year), pathlist, "r");
#endif

	/* save year and path for next time around */
	sv_year = year;
	sv_path = fp ? (fclose(fp), filename) : NULL;

	return sv_path;
}


/*
 * read_moonfile - looks for moon data file (in same directory as .calendar
 * or where pcal executable lives - cf. find_moonfile()); if found, reads
 * file, fills in moon_info[] and returns TRUE; if not found (or error
 * encountered), returns FALSE
 */
int
#ifdef PROTOS
read_moonfile(int year)
#else
read_moonfile(year)
	int year;
#endif
{
	char *filename;
	int line, nrec, month, day, hh, mm, dummy, mf_date_style;
	int ph, prevph = MOON_OTHER, doy, prevdoy, n, quarter = 0;
	double phase;
	FILE *fp;
	static char *words[MAXWORD];	/* avoid conflicts with globals */
	static char lbuf[LINSIZ];

	/* get name of moon file and attempt to open it */

	if ((filename = find_moonfile(year)) == NULL ||
	    (fp = fopen(filename, "r")) == NULL) {
		if (DEBUG(DEBUG_MOON) || DEBUG(DEBUG_PATHS))
			fprintf(stderr, "No moon file for %d\n", year);
		return FALSE;
	}

	if (DEBUG(DEBUG_MOON) || DEBUG(DEBUG_PATHS))
		fprintf(stderr, "Using moon file %s\n", filename);

	/*
	 * Moon file entries are of the form <phase> <date> {<time>}; each
	 * is converted below to a moon_info[] record (note that only the
	 * initial phase of the moon is directly calculated from <phase>;
	 * it is subsequently used only for error checking).  Dummy entries
	 * in moon_info[] precede and follow the information read from the
	 * moon file; these are used for subsequent interpolation of dates
	 * before the first / after the last quarter of the year.
	 */

	prevdoy = 0;
	line = 0;
	mf_date_style = date_style;	/* unless overridden by "opt -[AE]" */

	for (nrec = 1; getline(fp, lbuf, &line); nrec++) {

		/* special check for "opt -[AE]" line */
		n = loadwords(words, lbuf);

		if (n == 2 && date_type(words[0], &dummy, &dummy) == DT_OPT) {
			if (words[1][0] == '-') {
				char c = words[1][1];
				if (c == F_USA_DATES || c == F_EUR_DATES) {
					mf_date_style = c == F_USA_DATES ?
							USA_DATES : EUR_DATES;
					nrec--;		/* skip this line */
					continue;
				}
			}
		}

		/* ensure line is recognizable */
		if (n < 2 || (ph = get_phase(words[0])) == MOON_OTHER)
			ERR_EXIT(E_INV_LINE);
	
		if (nrec == 1)			/* phase at initial quarter */
			quarter = ph == MOON_NM ? 4 : ph;

		/* extract the month and day fields (in appropriate order) */

		(void) split_date(words[1],
				  mf_date_style == USA_DATES ? &month : &day,
				  mf_date_style == USA_DATES ? &day : &month,
				  NULL);

		/* validate the date and phase */

		if (!is_valid(month, day, year))	/* date OK? */
			ERR_EXIT(E_INV_DATE);

		doy = DAY_OF_YEAR(month, day, year);	/* in sequence? */
		if (DAY_TOO_SOON || DAY_TOO_LATE || WRONG_PHASE)
			ERR_EXIT(E_DATE_SEQ);

		prevdoy = doy;			/* save for sequence check */
		prevph = ph;

		/* calculate moon phase, factoring in time (if present) */

		phase = 0.25 * quarter++;
		if (n > 2) {			/* extract hour and minute */
			(void) split_date(words[2], &hh, &mm, NULL);
			phase += (HOUR - (hh + (mm / 60.0))) / (24 * PERIOD); 
		}
		moon_info[nrec].doy = doy;	/* enter day and phase */
		moon_info[nrec].quarter = ph % 4;
		moon_info[nrec].phase = phase;

		if (DEBUG(DEBUG_MOON))
			fprintf(stderr, "moon_info[%2d]: %3d %d %6.3f\n", nrec,
				moon_info[nrec].doy, moon_info[nrec].quarter,
				moon_info[nrec].phase);
	}

	/* check to see that file is all there */

	doy = YEAR_LEN(year) + 1;	/* day after end of year */
	if (DAY_TOO_LATE)
		ERR_EXIT(E_PREM_EOF);

	/* extrapolate dummy entries from nearest lunar month */

	moon_info[nrec].doy = doy;	/* day after end of year */
	moon_info[nrec].quarter = MOON_OTHER;
	moon_info[nrec].phase = CALC_PHASE(doy, nrec-5, nrec-1);
	if (DEBUG(DEBUG_MOON))
		fprintf(stderr, "moon_info[%2d]: %3d %d %6.3f\n", nrec,
			moon_info[nrec].doy, moon_info[nrec].quarter,
			moon_info[nrec].phase);

	moon_info[0].doy = 0;		/* day before start of year */
	moon_info[0].quarter = MOON_OTHER;
	moon_info[0].phase = CALC_PHASE(0, 1, 5);
	if (DEBUG(DEBUG_MOON))
		fprintf(stderr, "moon_info[%2d]: %3d %d %6.3f\n", 0,
			moon_info[0].doy, moon_info[0].quarter,
			moon_info[0].phase);
	
	fclose(fp);
	return TRUE;
}


/*
 * gen_phases - fill array with moon phases for all days in month/year (plus
 * extra entries at beginning and end for last day of previous month and
 * first day of next month, respectively)
 */
static void
#ifdef PROTOS
gen_phases(double phase[],
	   int month,
	   int year)
#else
gen_phases(phase, month, year)
	double phase[];
	int month, year;
#endif
{
	int day, len;
	DATE date;

	/* start with moon phase for last day of previous month */
	MAKE_DATE(date, month, 0, year);
	normalize(&date);
	phase[0] = calc_phase(date.mm, date.dd, date.yy);

	/* add the moon phases for all days in the current month */
	for (day = 1, len = LENGTH_OF(month, year); day <= len; day++)
		phase[day] = calc_phase(month, day, year);

	/* append the moon phase for the first day of next month */
	MAKE_DATE(date, month, len + 1, year);
	normalize(&date);
	phase[len + 1] = calc_phase(date.mm, date.dd, date.yy);
}


/*
 * find_phase - look up phase of moon in moon phase file (if possible);
 * otherwise calculate it using calc_phase() above.  Sets *pquarter to
 * MOON_NM, MOON_1Q, etc. if quarter moon, MOON_OTHER if not
 */
double
#ifdef PROTOS
find_phase(int month,
	   int day,
	   int year,
	   int *pquarter)
#else
find_phase(month, day, year, pquarter)
	int month, day, year;
	int *pquarter;
#endif
{
	static int sv_year = 0, sv_month = 0;
	static int use_file;
	static double mphase[33];	/* 31 days + 2 dummies */
	int i, doy;
	double phase;

	if (year != sv_year) {		/* look for file for new year */
		use_file = read_moonfile(year);
	}

	if (! use_file) {		/* no file - calculate phase */
		/* new month?  fill mphase[] with moon phases */
		if (month != sv_month || year != sv_year) {
			gen_phases(mphase, month, year);
			sv_month = month;
			sv_year = year;
		}

		phase = mphase[day];
		*pquarter = is_quarter(mphase[day-1], phase, mphase[day+1]);
		return phase;
	}

	/* moon file found - use the data read by read_moonfile() */

	sv_year = year;				/* avoid re-reading same file */
	doy = DAY_OF_YEAR(month, day, year);
	for (i = 1; doy > moon_info[i].doy; i++)	/* find interval */
		;

	/* if day appears in table, return exact value; else interpolate */

	if (doy == moon_info[i].doy) {
		*pquarter = moon_info[i].quarter;
		phase = moon_info[i].phase;
	} else {
		*pquarter = MOON_OTHER;
		phase = CALC_PHASE(doy, i-1, i);
	}

	return phase - (int)phase;			/* 0.0 <= phase < 1.0 */
}

