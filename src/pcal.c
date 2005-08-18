static char  VERSION_STRING[]	= "@(#)pcal v4.9.0 - generate PostScript calendars";
/*
 * pcal.c - generate PostScript file to print calendar for any month and year
 *
 * 
 * The original PostScript code to generate the calendars was written by
 * Patrick Wood (Copyright (c) 1987 by Patrick Wood of Pipeline Associates,
 * Inc.), and authorized for modification and redistribution.  The calendar
 * file inclusion code was originally written in "bs(1)" by Bill Vogel of AT&T.
 * Patrick's original PostScript was modified and enhanced several times by
 * King Ables, Tim Tessin, Joe Wood, Jeff Mogul, Mark Hanson, and others whose
 * names have regrettably been lost.  This C version was originally created by
 * Ken Keirnan of Pacific Bell; additional enhancements by Joseph P. Larson,
 * Ed Hand, Andrew Rogers, Mark Kantrowitz, Joe Brownlee, Andy Fyfe, Steve
 * Grandi, and Geoff Kuenning.  The moon routines were originally written by
 * Jef Poskanzer and Craig Leres, and were incorporated into Pcal by Richard
 * Dyson.
 * 
 * Note: Beginning with version 4.8.0, the 'man' page for 'pcal' is the most
 * up-to-date source of authors and contributors to 'pcal'.
 *
 *
 * Contents:
 *
 *		alt_fopen
 *		change_color
 *		check_numargs
 *		color_msg
 *		gen_lang_sym
 *		get_args
 *		get_flag
 *		init_misc
 *		main
 *		set_color
 *		set_debug_flag
 *		usage
 *
 * Revision history:
 *
 *	4.9.0
 *		B.Marr		2005-08-08
 *		
 *		Eliminate the hack to support Esperanto via a custom,
 *		dedicated character encoding.  Esperanto is now handled
 *		generically by the 'Latin3' (ISO 8859-3) character encoding.
 *		
 *		B.Marr		2005-01-24
 *		
 *		Update version number in version string.
 *		
 *		Add support for several new character mappings (PostScript
 *		encoding vectors) in order to support a wider variety of
 *		languages.  Rename enumerations for existing encodings to be
 *		more consistent (and easily searchable).
 * 
 *	4.8.0
 *		B.Marr		2004-12-04
 *		
 *		Support new paper sizes.  Support specification of paper size
 *		via run-time option (command-line, etc).  Define new
 *		pre-processor symbols for paper size and page orientation.
 *		Remove spaces embedded within tab fields.  Create and support
 *		concept of 'input' language versus 'output' language.
 *		
 *		B.Marr		2004-11-13
 *		
 *		Update version number in version string. Use new KOI8U mapping
 *		for newly-added Ukrainian language support.  Remove Ctl-L
 *		(page eject) characters from source file.
 *
 *	4.7.1	SF	01/06/2003	added q-flag for 1-column output
 *
 *	4.7	AWR	01/25/2000	look for calendar file in directory
 *					where Pcal executable lives only if
 *					SEARCH_PCAL_DIR (cf. pcaldefs.h) is
 *					non-zero; this avoids conflicts if
 *					a program named 'calendar' has also
 *					been installed there
 *
 *					revised default year calculation to
 *					fix Y2K-related problems reported
 *					under some flavors of Un*x
 *
 *			12/15/1998	postpone calculation of final_month
 *					and final_year until adjusted value
 *					of nmonths is known
 *
 *			06/19/1998	allow numeric (0-6) argument to -F
 *					(first day) flag
 *
 *			03/08/1998	treat scaling/translation factors
 *					and copy count as numeric values
 *					instead of strings (cf. pcaldefs.h)
 *
 *			12/21/1997	clean up gcc warnings in -Wall mode
 *
 *			07/27/1997	revise for -H (generate HTML output)
 *					support (cf. writefil.c); delete
 *					obsolete FPR and PRT macros
 *
 *	4.6	AWR	05/14/1997	replace obsolete CENTURY macro with
 *					call to century() (cf. pcalutil.c)
 *
 *			12/11/1995	predefine version symbol "vN_M" for
 *					pcal release vN.M (cf. init_misc())
 *
 *			11/10/1995	support -T flag to select default
 *					font style (Bold/Italic/Roman)
 *
 *			10/31/1995	define "lang_XX" when language is set
 *					to XX (-a XX)
 *
 *			10/03/1995	use globals for initial and final
 *					month and year
 *
 *			09/21/1995	support "year all" syntax (calculate
 *					and save last year printed - cf.
 *					check_numargs())
 *
 *			05/09/1995	support troff-style escape sequences
 *					in text (cf. pcalutil.c, pcalinit.ps)
 *					to set font style to bold/Italic/Roman
 *
 *	4.5	AWR	11/01/1994	update top-line comments to credit
 *					additional early Pcal hackers
 *
 *			11/16/1993	support red:green:blue shading value
 *					syntax (cf. writefil.c, pcalutil.ps)
 *
 *			11/03/1993	widen flag/argument field in usage()
 *
 *			10/01/1993	use define_font() and define_shading()
 *					(cf. pcalutil.c; latter replaces old
 *					gen_shading()) for font and shading
 *					redefinition
 *
 *			09/09/1993	predefine alternate character set
 *					mapping name (cf. writefil.c,
 *					fontmaps.ps, pcaldefs.h)
 *
 *			04/28/1993	restructure function definitions so
 *					function name appears in first column
 *					(to facilitate searching for definition
 *					by name)
 *
 *			02/05/1993	support -# flag (generate multiple
 *					copies of each page)
 *
 *			02/11/1992	Add support for predefined holidays
 *					(cf. pcallang.h, readfile.c)
 *
 *	4.4	AWR	02/10/1992	Pipe "help" message through filter
 *					defined by environment variable
 *					PAGER_ENV (cf. pcaldefs.h)
 *
 *			01/20/1992	Add -z flag (extension of change
 *					suggested by Steve Grandi)
 *
 *			01/15/1992	Add "holiday" to -b, -g; expand -b,
 *					-g functionality to -G, -O; allow
 *					range of weekday names in all
 *
 *			01/13/1992	Support alternate date and title font
 *					sizes (single-month calendars only)
 *
 *			01/05/1992	Support "{<ordinal>} <day_name> <prep>
 *					<date_spec>" (cf. readfile.c)
 *
 *	4.3	AWR	12/06/1991	Attempted to simplify some of the
 *					mysteries surrounding command-line
 *					parsing; moved some processing from
 *					get_args() to new check_numargs()
 *
 *			12/05/1991	Search for moon file in directory
 *					where Pcal lives (cf. moonphas.c)
 *
 *			12/03/1991	Add -s flag to override default values
 *					for date/fill box shading
 *
 *			11/22/1991	Use cvt_escape() (new; cf. pcalutil.c)
 *					to convert escape sequences in command
 *					line strings
 *
 *			11/18/1991	Improve documentation; add init_misc()
 *					as catch-all for various initializations
 *
 *			10/25/1991	Support moon phases as wildcards
 *
 *			10/17/1991	Add -Z flag to generate debugging
 *					information; add a pre-pass through
 *					command line flags to detect -ZO
 *					prior to parsing PCAL_OPTS
 *
 *			10/15/1991	Revise logic of date file search
 *
 *	4.2	AWR	10/08/1991	Add -k and -K flags to control
 *					positioning of small calendars
 *
 *			10/03/1991	Add "note{/<n>}" to select box for
 *					note text (as per Geoff Kuenning)
 *
 *					Add -S flag to suppress generation
 *					of the small calendars
 *
 *			10/02/1991	Add -N flag to specify alternate
 *					heading for notes box
 *
 *					Allow user to specify alternate notes
 *					font size (-n <name>/<size>)
 *
 *			10/01/1991	Add -u flag to generate version info
 *					and parameter usage message
 *
 *			09/30/1991	Support "if" and "elif" in date file
 *
 *			09/19/1991	Add -c flag to generate input file for
 *					Un*x "calendar" utility
 *
 *	4.11	AWR	08/20/1991	Add support for "nearest" keyword
 *					(as per Andy Fyfe)
 *
 *					define "whole_year" when -w set
 *
 *			08/21/1991	Support %u, %w, %D, %M format specs
 *					and optional number following %[+-]
 *					(cf. writefil.c)
 *
 *	4.1	AWR	08/16/1991	Add -G flag to print "gray" dates as
 *					outlined, gray-filled characters
 *
 *					Fix potential bug in julday() (cf.
 *					moonphas.c)
 *
 *	4.02	AWR	07/02/1991	Add -v flag to print version info only;
 *					call find_executable() to get true
 *					program pathname (cf. pcalutil.c);
 *					add format specifiers to text strings
 *					(cf. writefil.c)
 *
 *	4.01	AWR	03/19/1991	Incorporate revised moonphas.c (q.v.)
 *
 *	4.0	AWR	02/24/1991	Add alt_fopen() to search for file
 *					in alternate path; use to look for
 *					date file in same directory as
 *					Pcal executable (as per Floyd Miller)
 *
 *					Support negative ordinals (cf.
 *					readfile.c, pcalutil.c)
 *
 *					Support expressions in preprocessor
 *					"if{n}def" lines (cf. exprpars.c)
 *
 *					Support "even", "odd" ordinals (cf.
 *					readfile.c) and ordinals > 5th
 *
 *					Support -B (leave unused boxes blank)
 *					flag
 *
 *					Separated into moonphas.c, pcal.c,
 *					pcalutil.c, readfile.c, and writefil.c;
 *					added support for moon phase file
 *
 *					Support -w (whole year) flag; fix
 *					various bugs and nonportable constructs
 *
 *
 *	Parameters:
 *
 *		pcal [opts]		generate calendar for current month/year
 *					(current year if -w flag specified)
 *
 *		pcal [opts] yy		generate calendar for entire year yy
 *
 *		pcal [opts] mm yy	generate calendar for month mm
 *					(Jan = 1), year yy (CCyy if yy < 100,
 *					where CC is the current century);
 *					generate 12 consecutive months starting
 *					with mm/yy if -w specified)
 *
 *		pcal [opts] mm yy n	as above, for n consecutive months (n
 *					rounded to next multiple of 12 if -w
 *					specified)
 *
 *	Output:
 *
 *		PostScript file to print calendars for all selected months.
 *
 *	Options:
 *
 *		-I		initialize all parameters to program defaults
 *
 *		-b <DAY>	print specified weekday in black
 *		-g <DAY>	print specified weekday in gray
 *		-O <DAY>	print specified weekday as unfilled outlines
 *		-G <DAY>	print specified weekday as filled outlines
 *				(default: print Saturdays and Sundays in gray)
 *
 *		-s {<DATE>}{/<FILL>}
 *				specify alternate shading values for dates
 *				and fill boxes
 *				(default: dates = 0.8, fill boxes = 0.9)
 *
 *		-d <FONT>{/<SIZE>}
 *				specify alternate font/size for dates
 *				(default: Times-Bold/25)
 *
 *		-n <FONT>{/<SIZE>}
 *				specify alternate font/size for notes in boxes
 *				(default: Helvetica-Narrow/6)
 *
 *		-t <FONT>{/<SIZE>}
 *				specify alternate font for text headings (and
 *				font size for month/year title)
 *				(default: Times-Bold/48)
 *
 *		-D <SYM>	define preprocessor symbol
 *		-U <SYM>	un-define preprocessor symbol
 *
 *		-e		generate empty calendar (ignore date file)
 *
 *		-f <FILE>	specify alternate date file (default:
 *				~/.calendar on Un*x, SYS$LOGIN:CALENDAR.DAT
 *				on VMS, s:calendar.dat on Amiga; if
 *				environment variable [logical name on VMS]
 *				PCAL_DIR exists, looks there instead; if
 *				not found in either place, looks in same
 *				directory as Pcal executable)
 *
 *		-o <FILE>	specify alternate output file (default:
 *				stdout on Un*x, CALENDAR.PS on VMS, 
 *				RAM:calendar.ps on Amiga)
 *
 *		-L <STRING>	specify left foot string   (default: "")
 *		-C <STRING>	specify center foot string (default: "")
 *		-R <STRING>	specify right foot string  (default: "")
 *
 *		-N <STRING>	specify notes box header (default: "Notes")
 *
 *		-l		generate landscape-mode calendars
 *		-p		generate portrait-mode calendars
 *				(default: landscape-mode)
 *
 *		-h		(command line only) write version information
 *				and full help message to stdout
 *		-u		(command line only) write version information
 *				and parameter usage message to stdout
 *		-v		(command line only) write version information
 *				alone to stdout
 *
 *		-m		draw a small moon icon on the days of the
 *				full, new, and half moons.
 *		-M		draw a small moon icon every day.  
 *				(default: no moons)
 *
 *		-F <DAY>	select alternate day to be displayed as the 
 *				first day of the week (default: Sunday)
 *
 *		-A		dates are in American format (e.g., 10/15/90,
 *				Oct 15) (default)
 *		-E		dates are in European format (e.g., 15.10.90,
 *				15 Oct)
 *
 *		-x <XSCALE>	These two options can be used to change
 *		-y <YSCALE>	the size of the calendar.
 *
 *		-X <XTRANS>	These two options can be used to relocate
 *		-Y <YTRANS>	the position of the calendar on the page.
 *
 *		-j		print Julian dates (day of year)
 *		-J		print Julian dates and days remaining
 *				(default: neither)
 *
 *		-w		print whole year (12 months) per page
 *
 *		-c		generate input for Un*x calendar(1) utility
 *
 *		-z <VALUE>	specify alternate time zone for moon phase
 *				calculation algorithm
 *
 *		-B		leave unused calendar boxes blank
 *
 *
 *	There are many ways to specify these options in addition to using the
 *	command line; this facilitates customization to the user's needs.
 *
 *	If the environment variable (global symbol on VMS) PCAL_OPTS is
 *	present, its value will be parsed as if it were a command line.
 *	Any options specified will override the program defaults.
 *
 *	All options but -[cefhuvDU] may be specified in the date file by
 *	including one or more lines of the form "opt <options>".  Any such
 *	options override any previous values set either as program defaults,
 *	via PCAL_OPTS, or in previous "opt" lines.
 *
 *	Options explicitly specified on the command line in turn override all
 *	of the above.
 *
 *	Any flag which normally takes an argument may also be specified without
 *	an argument; this resets the corresponding option to its default.  -D
 *	alone un-defines all symbols; -U alone has no effect.
 *
 *	Parameters and flags may be mixed on the command line.  In some cases
 *	(e.g., when a parameter follows a flag without its optional argument)
 *	this may lead to ambiguity; the dummy flag '-' (or '--') may be used
 *	to separate them, i.e. "pcal -t - 9 90".
 *
 *
 *	Date file syntax:
 *
 *	1) Basic Syntax:
 *
 *	The following rules describe the syntax of date file entries:
 *
 *	  year <year>|all|*
 *
 *	  opt <options>
 *
 *	  note{/<number>} <month_spec> <text>
 *	  note{/<number>} <month> <text>
 *
 *	  if -A flag (American date formats) specified:
 *	    <month_name> <day>{*} {<text>}
 *	    <month><sep><day>{<sep><year>}{*} {<text>}
 *
 *	  if -E flag (European date formats) specified:
 *	    <day>{<sep>} <month_name>{*} {<text>}
 *	    <day>{<sep>} <month>{<sep>}{*} {<text>}
 *	    <day><sep><month>{<sep><year>}{*} {<text>}
 *
 *	  <ordinal> <day_name> in <month_spec>{*} {<text>}
 *	  {<ordinal>} <day_name> <prep> <date_spec>
 *
 *	  <holiday>
 *
 *	where
 *
 *	  {x}          means x is optional
 *
 *	  <date_spec> := any of the above date specs (not year, note, or opt)
 *	  <month_name> := first 3+ characters of name of month, or "all"
 *	  <month_spec> := <month_name>, or "year"
 *	  <day_name> := first 3+ characters of name of weekday, "day",
 *			"weekday", "workday", "holiday", "nonweekday",
 *			"nonworkday", "nonholiday", "new_moon",
 *			"first_quarter", "full_moon", or "last_quarter"
 *	  <ordinal> := ordinal number ("1st", "2nd", etc.), "first" .. "fifth",
 *			"last", "even", "odd", or "all"
 *	  <prep> := "before", "preceding", "after", "following", "nearest",
 *			"nearest_before", "nearest_after", "on_or_before", or
 *			"on_or_after"
 *	  <holiday> := "Christmas", "Thanksgiving", etc.
 *
 *	  <sep> := one or more non-numeric, non-space, non-'*' characters
 *	  <month>, <day>, <year> are the numeric forms
 *
 *	  <options> := any command-line option except -[cefhuvDU]
 *
 *	Comments start with '#' (unless escaped by '\') and run through
 *	end-of-line.
 *
 *	Holidays may be flagged by specifying '*' as the last character of the
 *	date field(s), e.g. "10/12* Columbus Day", "July 4* Independence
 *	Day", etc.  Any dates flagged as holidays will be printed in gray, and
 *	any associated text will appear adjacent to the date.
 *
 *	Note that the numeric date formats (mm/dd{/yy}, dd.mm{.yy}) support an
 *	optional year, which will become the subsequent default year.  The
 *	alphabetic date formats (month dd, dd month) do not support a year
 *	field; the "year yy" command is provided to reset the default year.
 *
 *	European dates may also be specified as "dd. mm." and "dd. month" if
 *	desired.
 *
 *	"year all" and "year *" specify that subsequent date specifications
 *	are to be wildcarded against every applicable year; this is in effect
 *	until the default year is explicitly reset as specified above.
 *
 *	"Floating" days may be specified in the date file as "first Mon in
 *	Sep", "last Mon in May", "4th Thu in Nov", etc.; any word may be used
 *	in place of "in".  "Relative floating" days (e.g. "Fri after 4th Thu
 *	in Nov") are also accepted; they may span month/year bounds.  Pcal
 *	also accepts date specs such as "all Friday{s} in October", "last
 *	Thursday in all", etc., and produces the expected results; "each" and
 *	"every" are accepted as synonyms for "all".  Negative ordinals are
 *	allowed; "-2nd" means "next to last".
 *
 *	"Floating" days may also be specified relative to a fixed date:
 *	"fourth Sunday before 12/25", etc; note, however, that only positive
 *	ordinals are meaningful in this context.
 *
 *	The words "day", "weekday", "workday", and "holiday" may be used as
 *	wildcards: "day" matches any day, "weekday" matches any day normally
 *	printed in black, "workday" matches any day normally printed in black
 *	and not explicitly flagged as a holiday, and "holiday" matches any day
 *	explicitly flagged as a holiday.  "Nonweekday", "nonworkday", and
 *	"nonholiday" are also supported and have the obvious meanings.  Moon
 *	phases may also appear as wildcards; "nm" is accepted as a synonym for
 *	"new_moon", "1q" and "fq" for "first_quarter", "fm" for "full_moon",
 *	and "3q", "lq", and "third_quarter" for "last_quarter".
 *
 *	"Odd" and "even" do not refer to the actual date; instead, "odd" means
 *	"alternate, starting with the first"; "even" means "alternate,
 *	starting with the second".  Thus, "odd Fridays in March" refers to the
 *	first, third, and (if present) fifth Fridays in March - not to those
 *	Fridays falling on odd dates.
 *
 *	"All" refers to each individual month; "year" refers to the year as an
 *	entity.  Thus "odd Fridays in all" refers to the first/third/fifth
 *	Friday of each month, while "odd Fridays in year" refers to the first
 *	Friday of January and every other Friday thereafter.
 *
 *	"Nearest", "nearest_before", and "nearest_after" refer to the nearest
 *	weekday or wildcard day with respect to the specified date.
 *	"Nearest_before" and "nearest_after" allow the user to specify how
 *	pcal is to disambiguate between two dates that are equally near:
 *	e.g., "nonweekday nearest_before [Wed.] 9/25/96" refers to Sunday,
 *	9/22 while "nonweekday nearest_after 9/25/96" refers to Saturday,
 *	9/28.  (Note that "nearest_before" and "nearest_after" are equivalent
 *	to "nearest" when no such ambiguity exists: e.g., "nonweekday
 *	nearest_before [Thu.] 9/26/96" refers to Saturday, 9/28.)
 *
 *	Additional notes may be propagated to an empty calendar box by the
 *	inclusion of one or more lines of the form "note{/<number>} <month>
 *	<text>", where <month> may be numeric or alphabetic; "note{/<number>}
 *	all <text>" propagates <text> to each month in the current year.
 *	<number> is an optional positive or negative number specifying the
 *	empty box where the associated text is to be placed: if positive,
 *	Pcal counts forward from the first empty box; if negative, Pcal counts
 *	backward from the last empty box.  Thus, "note/1 ..."  places the
 *	associated text in the first empty box, and "note/-3 ..."  in the
 *	third-to-last; the default is -1 (last empty box).  (Note that if the
 *	-S option is used, it must be specified either on the command line or
 *	prior to any "note" lines in the date file.)
 *
 *	`troff'-style escape sequences \fB, \fI, \fP, and \fR may be used to
 *	set the font style to Bold, Italic, the previous font style, or Roman
 *	respectively.  For those more familiar with HTML, <B>, <I>, and </B>
 *	or </I> may be used instead to set the font style to Bold, Italic, or
 *	the previous font style respectively.  The font style is reset to
 *	Roman after each line break.
 *
 *
 *	2) Format specifiers:
 *
 *	Pcal also allows format specifiers in the text (and foot strings - cf.
 *	the -L, -C, -R, and -N options); each will be replaced by its
 *	equivalent string as outlined in the table below.  (Most of these are
 *	derived from the strftime() function; %[lnouwMD0+-] are Pcal-specific.)
 *
 *		%a : abbreviated weekday
 *		%A : full weekday
 *		%b : abbreviated month name
 *		%B : full month name
 *		%d : day of month (1-31)
 *		%j : day of year (1-366)
 *		%l : days left in year (0-365)
 *		%m : month (1-12)
 *		%u : week number (1-54)
 *		%U : week number (0-53)
 *		%w : week number (1-54)
 *		%W : week number (0-53)
 *		%y : year w/o century (00-99)
 *		%Y : year w/century
 *		%% : '%' character
 *
 *		%o : print number as ordinal
 *		%0 : print number with leading zeroes
 *		%+ : use following month or year
 *		%- : use previous month or year
 *		%{+N}[DWMY] : adjust date by +N days/weeks/months/years
 *		%{-N}[DWMY] : adjust date by -N days/weeks/months/years
 *
 *	%u considers the week containing 1/1 as week 1 and the following
 *	logical Sunday (the first day of the week as printed; cf. the -F
 *	flag) as the start of week 2; %U considers the first logical Sunday as
 *	the first day of week 1.  %w and %W behave like %u and %U
 *	respectively, but use the first logical Monday instead.  (Note that %w
 *	has a different meaning to strftime().)
 *
 *	%o prints a number as an ordinal, with the appropriate suffix ("st",
 *	"nd", "rd", or "th" in English) appended; for example, "%od" prints
 *	the day of the month as "1st", "2nd", "3rd", etc.
 *
 *	Unlike strftime(), Pcal's default is to print numbers (except %y)
 *	without leading zeroes.  If leading zeroes are desired, the '0' prefix
 *	may be used; for example, "%0j" prints the day of year as 001-365.
 *
 *	%+ and %- direct Pcal to substitute the following/previous month/year
 *	in the following [bBmyY] specifier; for example, "%+B" prints the name
 *	of the next month.
 *
 *	%{[+-]N}[DWMY] do not print anything, but instead adjust the working
 *	date by +-N days (D), weeks (W), months (M), or years (Y); subsequent
 *	format specifiers use the adjusted date instead of the current date.
 *	For example, "%+1M %B %Y" adjusts the date forward by one month and
 *	then prints the resulting month and year ("January 1991" in December,
 *	1990); "%-2W %b %d" adjusts the date backward by two weeks and prints
 *	the resulting month and day ("Jul 26" on August 9).
 *
 *	Such date adjustments are normally cumulative; for example, "%+1Y%-1D"
 *	adjusts the date forward by one year and then backward by one day.  If
 *	%D or %M is specified alone (or if N is zero), Pcal restores the
 *	original date.  (Note that %M has a different meaning to strftime().)
 *
 *	The "Notes" box uses the first of the current month as the default
 *	date.  All foot strings use the first of the current month in single-
 *	month mode and the first of the starting month in whole-year mode.
 *
 *
 *	3) Preprocessing:
 *
 *	Simple cpp-like functionality is provided.  The date file may include
 *	the following commands, which work like their cpp counterparts:
 *
 *		define <sym>
 *		undef <sym>
 *
 *		if{{n}def} <expr>
 *		   ...
 *		{ elif <expr>
 *		   ... }*
 *		{ else
 *		   ... }
 *		endif
 *
 *		include{?} <file>
 *
 *	Note that these do not start with '#', which is reserved as a comment
 *	character.
 *
 *	<sym> is a symbol name consisting of a letter followed by zero or more
 *	letters, digits, or underscores ('_').  Symbol names are always
 *	treated in a case-insensitive manner.
 *
 *	<expr> is an expression consisting of symbol names joined by the
 *	logical operators (in order of precedence, high to low) '!'  (unary
 *	negate), '&' (and), '^' (exclusive or), and '|' (inclusive or).  '&&'
 *	and '||' are accepted as synonyms for '&' and '|' respectively; the
 *	order of evaluation may be altered by the use of parentheses.  A
 *	symbol whose name is currently defined evaluates to TRUE; one whose
 *	name is not currently defined evaluates to FALSE.  Thus "ifdef A | B |
 *	C" is TRUE if any of the symbols A, B, and C is currently defined, and
 *	"ifdef A & B & C" is TRUE if all of them are.
 *
 *	"ifndef A | B | C" is equivalent to "ifdef !(A | B | C)" (or, using
 *	DeMorgan's Law, "ifdef !A & !B & !C") - in other words, TRUE if none
 *	of the symbols A, B, and C is currently defined.
 *
 *	"if" is accepted as a synonym for "ifdef".
 *
 *	"elif A" is TRUE if A is defined.  Multiple "elif" clauses may appear;
 *	at most one "if{{n}def}", "elif", or "else" clause in a given block
 *	will be processed.
 *
 *	"define" alone deletes all the current definitions; "if{def}" alone is
 *	always false; "ifndef" alone is always true.
 *
 *	The file name in the "include" directive may optionally be surrounded
 *	by "" or <>; in any case, path names are taken to be relative to the
 *	location of the file containing the "include" directive.  If the
 *	string "%y" appears in the file name, it is replaced by the last two
 *	digits of the current year or is expanded to all applicable years if
 *	"year all" is in effect.  An alternate form, "include?", directs pcal
 *	to silently continue if the specified file cannot be opened. 
 *
 *
 *	4) Miscellaneous:
 *
 *	The "-w" flag defines the symbol "whole_year", which may be tested in
 *	the same manner as any user-defined symbol.  Similarly, the "-a XX"
 *	flag defines the symbol "lang_XX", where XX is the two-character
 *	abbreviation for the selected language.
 *
 *	Pcal also supports several predefined holidays; type "pcal -h" to
 *	generate a list of these.
 */

/*
 * Standard headers:
 */

#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

/*
 * Pcal-specific definitions:
 */

#define MAIN_MODULE	1
#include "pcaldefs.h"
#include "pcalglob.h"
#include "pcallang.h"
#include "pcalpapr.h"

/*
 * Globals:
 */

static int nargs = 0;				/* count of non-flag args  */
static int numargs[MAXARGS];			/* non-flag (numeric) args */
static int map_default = TRUE;			/* use default mapping	   */
static int oflag = FALSE;			/* -o flag specified	   */


/*
 * Main program - parse and validate command-line arguments, open files,
 * generate PostScript boilerplate and code to generate calendars.
 *
 * Program structure:
 *
 * For maximum user flexibility, Pcal gives the user several different ways
 * to set program flags and/or override earlier choices.  This necessitates
 * that main() call get_args() (directly or indirectly) several times:
 *
 *	a) to parse the command line, looking only for -Z flags (which turn on
 *	   debugging information) and numeric parameters
 *
 *	b) to parse environment variable (global symbol on VMS) PCAL_OPTS, if
 *	   defined
 *
 *	c) to parse the command line a second time, looking for options
 *	   related to finding/interpreting the date file: -[cefhuvDU]
 *
 *	d) main() calls read_datefile() to read and parse the date file; it
 *	   in turn calls get_args() once per "opt" line in the date file
 *
 *	e) to parse the command line one final time, allowing the user to
 *	   override any flags other than those listed in c) above
 *
 * The rest of it is straightforward: main() attempts to open the output file
 * (if any), and, if successful, calls write_psfile() to generate the
 * PostScript output (or write_calfile() to generate the "calendar" input).
 * Some minor housekeeping and we're done.
 *
 */
int
#ifdef PROTOS
main(int argc GCC_UNUSED,
     char **argv)
#else
main(argc, argv)
	int argc;
	char **argv;
#endif
{
	FILE *dfp = NULL;		/* date file pointer */
	char *p, *pathlist[10];
	char tmp[STRSIZ];
	int n;

	/* get version from VERSION_STRING (for PostScript comments and
	 * version symbol predefined in init_misc())
	 */
	strcpy(tmp, VERSION_STRING + 4);
	p = strchr(tmp, ' ') + 1;	/* skip program name */
	*strchr(p, ' ') = '\0';		/* terminate after version */
	strcpy(version, p);

	init_misc();			/* handle initialization warts */

	/* extract root program name and program path - note that some
	 * systems supply the full pathname and others just the root
	 */

	strcpy(progname, **argv ? *argv : "pcal");

	if ((p = strrchr(progname, END_PATH)) != NULL)
		strcpy(progname, ++p);
#ifndef UN_X
	if ((p = strchr(progname, '.')) != NULL)
		*p = '\0';		/* strip suffix if non-Un*x */
#endif

	mk_path(progpath, find_executable(*argv));

	/*
	 * Get the arguments from a) the command line (pre-pass to pick up
	 * debug and usage flags), b) the environment variable PCAL_OPTS,
	 * c) the first command line pass, d) "opt" lines in the date file,
	 * and e) a final command line pass, in that order
	 */

	/* Make a preliminary pass to look for the debug flags (to ensure
	 * that -ZO will print any flags set in PCAL_OPTS) and the usage
	 * flags (-h, -u, and -v).  If none of the latter is specified,
	 * this pass will also validate any numeric command-line arguments.
	 */
	if (!get_args(argv, P_CMD0, NULL, TRUE)) {
		usage(stderr, FALSE);		/* invalid flag or param */
		exit(EXIT_FAILURE);
	}

	/* parse environment variable PCAL_OPTS as a command line */

	if ((p = getenv(PCAL_OPTS)) != NULL) {
		strcpy(lbuf, "pcal ");		/* dummy program name */
		strcat(lbuf, p);
		(void) loadwords(words, lbuf);	/* split string into words */
		if (! get_args(words, P_ENV, PCAL_OPTS, FALSE)) {
			usage(stderr, FALSE);
			exit(EXIT_FAILURE);
		}
	}

	/* parse command-line arguments once to find name of date file, etc. */

	(void) get_args(argv, P_CMD1, NULL, FALSE);

	/* if in whole-year mode, round number of months up to full year and
	 * set default starting month to January of current year
	 */
	if (do_whole_year) {
		nmonths = ((nmonths + 11) / 12) * 12;
		if (nargs == 0)
			init_month = JAN;

		/* also disable some meaningless flags */
		draw_moons = NO_MOONS;
		julian_dates = NO_JULIANS;
	}

	/* recalculate final month and year (latter needed for "year all") */
	final_month = (init_month + (nmonths - 1) - 1) % 12 + 1,
	final_year = init_year + ((nmonths - 1 + init_month - 1) / 12);


	/* Attempt to open the date file as specified by the [-e | -f] flags */

	switch (datefile_type) {
	case NO_DATEFILE:
		dfp = NULL;
		break;

	case USER_DATEFILE:
		/* Attempt to open user-specified calendar file: search
		 * first in the current directory, then in PCAL_DIR (if
		 * defined), and finally in the directory where the Pcal
		 * executable lives (if SEARCH_PCAL_DIR != 0).  It is a
		 * fatal error if the user-specified date file cannot be
		 * found.
		 */
		n = 0;
		pathlist[n++] = "";
		if ((p = trnlog(PCAL_DIR)) != NULL)
			pathlist[n++] = p;
#if (SEARCH_PCAL_DIR != 0)
		pathlist[n++] = progpath;
#endif
		pathlist[n] = NULL;
		
		strcpy(tmp, datefile);	/* save original name for error msg */

		if ((dfp = alt_fopen(datefile, tmp, pathlist, "r")) == NULL) {
			fprintf(stderr, E_FOPEN_ERR, progname, tmp);
			exit(EXIT_FAILURE);
		}
		break;

	case SYS_DATEFILE:
		/* Attempt to open system-specified calendar file: search
		 * first in PCAL_DIR, then in HOME_DIR (current directory
		 * if neither is defined) and finally in the directory where
		 * the Pcal executable lives (if SEARCH_PCAL_DIR != 0).  It
		 * is not an error if the system-specified date file cannot
		 * be opened; Pcal will simply generate an empty calendar.
		 */
		n = 0;
		if ((p = trnlog(PCAL_DIR)) != NULL)
			pathlist[n++] = p;
		if ((p = trnlog(HOME_DIR)) != NULL)
			pathlist[n++] = p;
		if (n == 0)
			pathlist[n++] = "";
#if (SEARCH_PCAL_DIR != 0)
		pathlist[n++] = progpath;
#endif
		pathlist[n] = NULL;
		
		dfp = alt_fopen(datefile, DATEFILE, pathlist, "r");

		/* if the date file has not been found and ALT_DATEFILE is
		 * defined, search same paths for ALT_DATEFILE before
		 * giving up
		 */
#ifdef ALT_DATEFILE
		if (!dfp)
			dfp = alt_fopen(datefile, ALT_DATEFILE, pathlist, "r");
#endif
		break;
	}

	/* read the date file (if any) and build internal data structure */

	if (dfp) {
		curr_year = init_year;
		read_datefile(dfp, datefile);
		fclose(dfp);
	} else
		datefile[0] = '\0';		/* for PostScript comment */

	/* reparse command line - flags there supersede those in date file */

	(void) get_args(argv, P_CMD2, NULL, FALSE);

	/* if the user has not explicitly selected a mapping, choose one
	 * based on the chosen language
	 */
	if (map_default == TRUE)
		mapfonts = lang_mapping[output_language];

	/* fprintf(stderr, "Font mapping is %d\n", mapfonts); */

	/* select an appropriate color for holidays if not set explicitly */
	if (holiday_color == HOLIDAY_DEFAULT)
		holiday_color = select_color();

	/* done with the arguments and flags - try to open the output file */

	/* use default output files (cf. pcaldefs.h) if -o flag not used */
	if (!oflag && output_type == OUTPUT_PS)
		strcpy(outfile, PS_OUTFILE);
	if (!oflag && output_type == OUTPUT_HTML)
		strcpy(outfile, HTML_OUTFILE);

	/* reopen stdout as alternate file if one was specified */
	if (*outfile && freopen(outfile, "w", stdout) == (FILE *) NULL) {
		fprintf(stderr, E_FOPEN_ERR, progname, outfile);
		exit(EXIT_FAILURE);
	}

	/* Set up the initial page-related values based on the default paper
	 * size.
	 */
	recalc_paper_parameters(paper_size);

	/* generate the "calendar", HTML, or PostScript code (cf. writefil.c) */

	if (output_type == OUTPUT_CAL)
		write_calfile();
	else if (output_type == OUTPUT_HTML)
		write_htmlfile();
	else
		write_psfile();
	
	cleanup();		/* free allocated data */

	/* if output was written to a non-obvious location, tell user where */

	if (!oflag && *outfile)
		fprintf(stderr, I_OUT_NAME, progname, outfile);

	exit(EXIT_SUCCESS);
}


/*
 * init_misc - various initializations not easily handled in .h files
 */
void
#ifdef PROTOS
init_misc(void)
#else
init_misc()
#endif
{
	char tmp[STRSIZ], *p;

	INIT_COLORS;		/* copy default_color to day_color */
	strcpy(notes_hdr, default_notes_hdr);	/* initialize notes_hdr */

	/* define preprocessor symbol "lang_XX" for default language XX */
	do_define(gen_lang_sym(LANG_DEFAULT));

	/* Define 'PAPERSIZE_XXX' (where XXX is the name of the selected paper
	 * size).
	 */
	sprintf(tmp, "PAPERSIZE_%s", paper_info[paper_size].name);
	do_define(tmp);

	/* Define 'ORIENTATION_XXX' (where XXX is the name of the current
	 * paper orientation -- 'portrait' or 'landscape').
	 */
	sprintf(tmp, "ORIENTATION_%s", (rotate == PORTRAIT) ? "PORTRAIT" : "LANDSCAPE");
	do_define(tmp);

	/* define preprocessor symbol vN_N for Pcal version vN.N */
	strcpy(tmp, version);
	for (p = tmp; *p; p++)
		if (*p == '.')
			*p = '_';
	do_define(tmp);
}


/*
 * set_color - set one or all weekdays (or all holidays) to print in selected
 * color; also accept range such as mon-fri or sat-sun
 */
void
#ifdef PROTOS
set_color(char *day,		/* weekday name (or "all" or "holiday") */
	  int col)		/* select black/gray/outline/outline-gray */
#else
set_color(day, col)
	char *day;		/* weekday name (or "all" or "holiday") */
	int  col;		/* select black/gray/outline/outline-gray */
#endif
{
	int i, j, max, count[NUM_COLORS];
	char tmp[STRSIZ], *p;

	if (ci_strncmp(day, ALL, strlen(ALL)) == 0) {	/* set all days */
		for (i = 0; i < 7; i++)
			day_color[i] = col;
	}

	else if (ci_strncmp(day, HOLIDAY, MIN_DAY_LEN) == 0) /* set holidays */
		holiday_color = col;

	else {						/* set range of days */
		strcpy(tmp, day);
		if ((p = strchr(tmp, '-')) != NULL)
			*p++ = '\0';

		/* parse single day or range of days - return if error */
		if ((i = get_weekday(tmp, FALSE)) == NOT_WEEKDAY ||
		    (j = p ? get_weekday(p, FALSE) : i) == NOT_WEEKDAY)
			return;

		/* set day colors, wrapping around end of week */
		for (j = (i > j) ? j + 7 : j; i <= j; i++)
			day_color[i % 7] = col;
	}

	/* reset weekday_color to most prevalent color */

	for (i = 0; i < NUM_COLORS; i++)		/* clear counts */
		count[i] = 0;

	for (i = SUN; i <= SAT; i++)			/* count colors */
		count[(int)day_color[i]]++;

	for (i = max = 0; i < NUM_COLORS; i++)		/* get most prevalent */
		if (count[i] > max)
			max = count[weekday_color = i];

}


/*
 * change_color - reset existing non-black colors to selected color
 * (for backward-compatibility with pre-v4.4 -G and -O flags)
 */
void
#ifdef PROTOS
change_color(int col)
#else
change_color(col)
	int  col;
#endif
{
	int i;

	for (i = SUN; i <= SAT; i++)
		if (day_color[i] != BLACK)
			day_color[i] = col;

	if (holiday_color != BLACK && holiday_color != HOLIDAY_DEFAULT)
		holiday_color = col;

}


/*
 * get_flag() - look up flag in flag_tbl; return pointer to its entry
 * (NULL if not found)
 */
FLAG_USAGE *
#ifdef PROTOS
get_flag(char flag)
#else
get_flag(flag)
	char flag;
#endif
{
	FLAG_USAGE *pflag;

	for (pflag = flag_tbl; pflag->flag; pflag++)
		if (flag == pflag->flag)
			return pflag;

	return flag ? NULL : pflag;		/* '\0' is a valid flag */
}


/*
 * set_debug_flag - look up "flag" in debug flag table and set selected
 * bits in debug flag word (clear word if "flag" is NULL)
 */
void
#ifdef PROTOS
set_debug_flag(char *flag)
#else
set_debug_flag(flag)
	char *flag;
#endif
{
	char f;
	DEBUG_INFO *pd;

	if (!flag) {			/* clear all if NULL */
		debug_flags = 0;
		return;
	}

	/* loop through all characters in "flag", setting corresponding
	 * bits in debug_flags
	 */
	while ((f = *flag++) != '\0')
		for (pd = debug_info; pd->flag; pd++)
			if (pd->flag == f)
				debug_flags |= pd->value;

}

/*
 * get_args - walk the argument list, parsing all arguments but processing only
 * those specified (in flag_tbl[]) to be processed this pass; return TRUE if
 * OK, FALSE if invalid flag found.
 */
int
#ifdef PROTOS
get_args(char **argv,		/* argument list */
	 int  curr_pass,	/* current pass (P_xxx) */
	 char *where,		/* for error messages */
	 int  get_numargs)	/* parse and save numeric arguments? */
#else
get_args(argv, curr_pass, where, get_numargs)
	char **argv;		/* argument list */
	int  curr_pass;		/* current pass (P_xxx) */
	char *where;		/* for error messages */
	int  get_numargs;	/* parse and save numeric arguments? */
#endif
{
	char *parg, *opt, *p, *pass, tmpbuf[STRSIZ], c, sym[STRSIZ];
	FLAG_USAGE *pflag, *pf;
	int i, flag, sv_debug;
	int flags_ok = TRUE;		/* return value */
	FILE *fp = stdout;		/* for piping "help" message */

/*
 * If argument follows flag (immediately or as next parameter), return
 * pointer to it (and bump argv if necessary); else return NULL
 */
#define GETARG() (*(*argv + 2) ? *argv + 2 : \
		  (*(argv+1) && **(argv+1) != '-' ? *++argv : NULL))

	/* set up pass name to print in debug messages */

	pass = where ? where :
	       curr_pass == P_CMD0 ? "CMD0" :
	       curr_pass == P_ENV  ? "ENV"  :
	       curr_pass == P_CMD1 ? "CMD1" :
	       curr_pass == P_CMD2 ? "CMD2" :
				     "OPT"  ;

	/* Walk argument list, ignoring first element (program name) */

	while ((opt = *++argv) != NULL) {

		/* Assume that any non-flag argument is a numeric argument */
		if (*opt != '-') {
			if (get_numargs && nargs < MAXARGS) {
				if (! IS_NUMERIC(opt))
					goto bad_par;
				numargs[nargs++] = atoi(opt);
				if (DEBUG(DEBUG_OPTS))
					fprintf(stderr, "%s: %s\n", pass, opt);
			}
			continue;
		}

		/* Check that flag is legal */
		if (! (pflag = get_flag(flag = opt[1])) )
			goto bad_par;

		/* get optional argument even if flag not processed this pass */
		parg = pflag->has_arg ? GETARG() : NULL;

		/* skip to next flag if not to be processed this pass */
		if (! (pflag->passes & curr_pass)) {
			if (curr_pass == P_OPT)
				fprintf(stderr, E_FLAG_IGNORED, progname, flag,
					DATE_FILE, where);
			continue;
		}

		/* echo pass name and flag if debugging on */
		if (DEBUG(DEBUG_OPTS))
			fprintf(stderr, "%s: -%c%s%s\n", pass, flag,
				parg ? " " : "", parg ? parg : "");

		/* convert escape sequences in command-line parameters */
		if (parg && (curr_pass == P_CMD1 || curr_pass == P_CMD2)) {
			cvt_escape(tmpbuf, parg);
			parg = tmpbuf;
		}

		switch (flag) {

		case F_INITIALIZE:	/* reset all flags to defaults */

			/* set up a command line to reset all of the
			 * flags; call get_args() recursively to parse it
			 * (note that some of the flags must be reset
			 * explicitly, as no command-line flags exist to
			 * reset them)
			 */

			/* reset flags described above */
			julian_dates  = JULIAN_DATES;
			draw_moons    = DRAW_MOONS;
			do_whole_year = DO_WHOLE_YEAR;
			blank_boxes   = BLANK_BOXES;
			output_type   = OUTPUT_TYPE;
			small_cal_pos = SMALL_CAL_POS;

			/* select program default for landscape/portrait
			 * mode (must be done first because -[xXyY] depend
			 * on it) and US/European date styles
			 */
			sprintf(lbuf, "pcal -%c -%c",
#if (ROTATE == LANDSCAPE)
				F_LANDSCAPE,
#else
				F_PORTRAIT,
#endif
#if (DATE_STYLE == USA_DATES)
				F_USA_DATES);
#else
				F_EUR_DATES);
#endif
			p = lbuf + strlen(lbuf);

			/* all other flags take arguments and are reset
			 * by specifying the flag without an argument
			 */
			for (pf = flag_tbl; pf->flag; pf++)
				if ((pf->passes & curr_pass) && pf->has_arg) {
					sprintf(p, " -%c", pf->flag);
					p += strlen(p);
				}

			/* split new command line into words; parse it */
			(void) loadwords(words, lbuf);
			(void) get_args(words, curr_pass, NULL, FALSE);

			/* -G and -O without arguments change some day
			 * colors (for backward-compatibility with
			 * pre-v4.4 versions), so initialize the day 
			 * colors explicitly
			 */
			INIT_COLORS;

			/* also disable the time zone flag explicitly */
			tz_flag = FALSE;

			break;

		case F_BLACK_DAY:	/* print day in black or gray */
		case F_GRAY_DAY:
			if (parg)
				set_color(parg, flag == F_BLACK_DAY ?
						  BLACK : GRAY);
			else
				INIT_COLORS;	/* reset to defaults */
			break;

		case F_OUTLINE:		/* print day in outline or outline-gray */
		case F_OUTLINE_GRAY:
			/* ignore "-Gall" etc. on second command-line pass;
			 * this is to avoid changing the definition of
			 * "weekday", et. al. after the date file has already
			 * been read
			 */
			if (parg) {
				if (curr_pass != P_CMD2)
					set_color(parg, flag == F_OUTLINE ?
							OUTLINE :
							OUTLINE_GRAY);
			}
			else
				/* special hack for backward compatibility
				 * with v4.3 and earlier: -G or -O alone
				 * change all non-black days to the selected
				 * color
				 */
				change_color(flag == F_OUTLINE ?
						OUTLINE : OUTLINE_GRAY);

			break;

		case F_DAY_FONT:	/* specify alternate day font */
			define_font(datefont, parg, DATEFONT);
			break;

		case F_NOTES_FONT:	/* specify alternate notes font */
			define_font(notesfont, parg, NOTESFONT);
			break;

		case F_TITLE_FONT:	/* specify alternate title font */
			define_font(titlefont, parg, TITLEFONT);
			break;

		case F_REMAP_FONT:	/* specify 8-bit font mapping */
			if (parg) {
				if (ci_strncmp(parg, MAPPING_LATIN_1, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_1;
				else if (ci_strncmp(parg, MAPPING_LATIN_2, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_2;
				else if (ci_strncmp(parg, MAPPING_LATIN_3, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_3;
				else if (ci_strncmp(parg, MAPPING_LATIN_4, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_4;
				else if (ci_strncmp(parg, MAPPING_CYRILLIC, strlen(parg)) == 0)
					mapfonts = ENC_CYRILLIC;
				/* else if (ci_strncmp(parg, MAPPING_ARABIC, strlen(parg)) == 0) */
					/* mapfonts = ENC_ARABIC; */
				else if (ci_strncmp(parg, MAPPING_GREEK, strlen(parg)) == 0)
					mapfonts = ENC_GREEK;
				/* else if (ci_strncmp(parg, MAPPING_HEBREW, strlen(parg)) == 0) */
					/* mapfonts = ENC_HEBREW; */
				else if (ci_strncmp(parg, MAPPING_LATIN_5, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_5;
				else if (ci_strncmp(parg, MAPPING_LATIN_6, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_6;
				else if (ci_strncmp(parg, MAPPING_THAI, strlen(parg)) == 0)
					mapfonts = ENC_THAI;
				else if (ci_strncmp(parg, MAPPING_LATIN_7, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_7;
				else if (ci_strncmp(parg, MAPPING_LATIN_8, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_8;
				else if (ci_strncmp(parg, MAPPING_LATIN_9, strlen(parg)) == 0)
					mapfonts = ENC_LATIN_9;
				/* else if (ci_strncmp(parg, MAPPING_LATIN_10, strlen(parg)) == 0) */
					/* mapfonts = ENC_LATIN_10; */
				else if (ci_strncmp(parg, MAPPING_KOI8_R, strlen(parg)) == 0)
					mapfonts = ENC_KOI8_R;
				else if (ci_strncmp(parg, MAPPING_KOI8_U, strlen(parg)) == 0)
					mapfonts = ENC_KOI8_U;
				else if (ci_strncmp(parg, MAPPING_ROMAN8, strlen(parg)) == 0)
					mapfonts = ENC_ROMAN8;
				else
					mapfonts = ENC_NONE;
			} else /* Use the value defined in the 'Makefile' */
				mapfonts = MAPFONTS;

			map_default = !parg;
			break;

		case F_EMPTY_CAL:	/* generate empty calendar */
			datefile_type = NO_DATEFILE;
			strcpy(datefile, "");
			break;

		case F_DATE_FILE:	/* specify alternate date file */
			datefile_type = parg ? USER_DATEFILE : SYS_DATEFILE;
			strcpy(datefile, parg ? parg : "");
			break;

		case F_OUT_FILE:	/* specify alternate output file */
			oflag = TRUE;
			strcpy(outfile, parg ? parg : "");
			break;

		case F_LANDSCAPE:	/* generate landscape calendar */
			rotate = LANDSCAPE;
			recalc_paper_parameters(paper_size);

			/* Undefine any old paper orientation symbol and
			 *   define a new symbol to match the new paper
			 *   orientation.
			 */
			do_undef("ORIENTATION_PORTRAIT");
			do_define("ORIENTATION_LANDSCAPE");
			break;
 
		case F_PORTRAIT:	/* generate portrait calendar */
			rotate = PORTRAIT;
			recalc_paper_parameters(paper_size);

			/* Undefine any old paper orientation symbol and
			 *   define a new symbol to match the new paper
			 *   orientation.
			 */
			do_undef("ORIENTATION_LANDSCAPE");
			do_define("ORIENTATION_PORTRAIT");
			break;

		case F_PAPERSIZE:	/* specify paper size */
			/* Undefine 'papersize_XXX' (where XXX is the name of
			 * the previously-selected paper size).
			 */
			sprintf(sym, "papersize_%s", paper_info[paper_size].name);
			do_undef(sym);

			paper_size = PAPERSIZE_DEFAULT;
			if (parg) {
				int found = FALSE;
				for (i = 0; i < NUM_PAPER_SIZES; i++) {
					if (ci_strncmp(paper_info[i].name, parg, strlen(paper_info[i].name)) == 0) {
						paper_size = i;
						found = TRUE;
						break;
					}
				}
				if (!found) {
					fprintf(stderr, E_ILL_PAPERSIZE, progname, parg);
				}
			}
			recalc_paper_parameters(paper_size);

			/* Define 'papersize_XXX' (where XXX is the name of
			 * the newly-selected paper size).
			 */
			sprintf(sym, "papersize_%s", paper_info[paper_size].name);
			do_define(sym);

			break;

		case F_HELP:		/* request version info and/or help */
		case F_USAGE:
		case F_VERSION:
			/* PAGER_ENV (cf. pcaldefs.h) defines the name of an
			 * environment variable which, if set, points to the
			 * appropriate pager (e.g., "more", "less", "pg")
			 * for piping the "help" message (Un*x systems only)
			 */
#ifdef PAGER_ENV
			if (flag == F_HELP) {
				FILE *pfp;
				char *pager, *p;

				pager = (p = getenv(PAGER_ENV)) ? p :
					PAGER_DEFAULT;
				/* null pointer or string: no paging */
				if (pager && *pager &&
				    (pfp = popen(pager, "w")) != (FILE *)NULL)
					fp = pfp;
			}
#endif
			fprintf(fp, "%s\n", VERSION_STRING + 4);
			if (flag != F_VERSION)
				usage(fp, flag == F_HELP);
			fflush(fp);
#ifdef PAGER_ENV
			if (fp != stdout)
				pclose(fp);
#endif
			exit(EXIT_SUCCESS);
			break;

		case F_MOON_4:		/* draw four moons */
		case F_MOON_ALL:	/* draw a moon for each day */
			draw_moons = flag == F_MOON_ALL ? ALL_MOONS : SOME_MOONS;
			break;

		case F_DEFINE:		/* define preprocessor symbol */
			(void) do_define(parg);
			break;

		case F_UNDEF:		/* undef preprocessor symbol */
			(void) do_undef(parg);
			break;

		case F_L_FOOT:		/* specify alternate left foot */
			strcpy(lfoot, parg ? parg : LFOOT);
			break;

		case F_C_FOOT:		/* specify alternate center foot */
			strcpy(cfoot, parg ? parg : CFOOT);
			break;

		case F_R_FOOT:		/* specify alternate right foot */
			strcpy(rfoot, parg ? parg : RFOOT);
			break;

		case F_NOTES_HDR:	/* specify alternate notes header */
			strcpy(notes_hdr, parg ? parg : default_notes_hdr);
			break;

		case F_FIRST_DAY:	/* select starting day of week */
			if (parg) {
				if ((i = get_weekday(parg, FALSE)) != NOT_WEEKDAY)
					first_day_of_week = i;
				else if (parg[0] >= '0' && parg[0] <= '6')
					first_day_of_week = parg[0] - '0';
			}
			else
				first_day_of_week = FIRST_DAY;
			break;

		case F_USA_DATES:	/* select American date style */
		case F_EUR_DATES:	/* select European date style */
			date_style = flag == F_USA_DATES ? USA_DATES : EUR_DATES;
			break;

		case F_X_TRANS:		/* set x-axis translation factor */
			if (parg) xtval_user = atoi(parg);
			break;

		case F_Y_TRANS:		/* set y-axis translation factor */
			if (parg) ytval_user = atoi(parg);
			break;

		case F_X_SCALE:		/* set x-axis scaling factor */
                   if (parg) xsval_user = atof(parg);
			break;

		case F_Y_SCALE:		/* set y-axis scaling factor */
                   if (parg) ysval_user = atof(parg);
			break;

		case F_JULIAN:		/* display Julian dates */
		case F_JULIAN_ALL:	/* Julian date/days remaining */
			julian_dates = flag == F_JULIAN_ALL ? ALL_JULIANS :
							      SOME_JULIANS;
			break;

		case F_WHOLE_YEAR:	/* print whole year at once */
			do_whole_year = !(DO_WHOLE_YEAR);  /* toggle default */
			if (do_whole_year)
				(void) do_define(DEF_WHOLE_YEAR);
			recalc_paper_parameters(paper_size);
			break;

		case F_CALENDAR:	/* generate "calendar(1)" input */
			output_type = OUTPUT_CAL;
			break;

		case F_HTML:		/* generate HTML table */
			output_type = OUTPUT_HTML;
			do_define(DEF_HTML);
			break;

		case F_1COLUMN:		/* print one column per month (HTML) */
			one_column = 1;
			break;

		case F_BLANK_BOXES:	/* suppress shading unused boxes */
			blank_boxes = !(BLANK_BOXES);
			break;

		case F_NUM_PAGES:	/* print multiple copies of each page */
			ncopy = parg ? atoi(parg) : NCOPY;
			break;

		case F_SC_NONE:		/* suppress small calendars */
			small_cal_pos = SC_NONE;
			break;

		case F_SC_FIRST:	/* reposition of small calendars */
		case F_SC_SPLIT:
			small_cal_pos = flag == F_SC_FIRST ? SC_FIRST :
							     SC_SPLIT;
			break;

		case F_SHADING:		/* set date/fill shading levels */
			define_shading(shading, parg, SHADING);
			break;

		case F_TIMEZONE:	/* select alternate time zone */
			strcpy(time_zone, parg ? parg : TIMEZONE);
			tz_flag = TRUE;
			break;

		case F_SETLANG:		/* select output lang for month/days */
			/* undefine lang_XX (where XX is current language) */
			do_undef(gen_lang_sym(output_language));

			output_language = LANG_DEFAULT;
			if (parg) {
				for (i = 0; i < NUM_LANGUAGES; i++) {
					if (ci_strncmp(lang_id[i], parg,
						MIN_LANG_LEN) == 0 ) {
							output_language = i;
							break;
					}
				}
			}

			/* define lang_XX (where XX is new current language) */
			do_define(gen_lang_sym(output_language));
			break;

		case F_TYPEFACE:	/* select font style (Bold/Italic) */
			c = parg ? TOUPPER(parg[0]) : ROMAN;
			fontstyle[0] = (c == BOLD || c == ITALIC) ? c : ROMAN;
			fontstyle[1] = '\0';		/* just in case */
			break;

		case F_DEBUG:		/* turn on debugging (undocumented) */
			sv_debug = DEBUG(DEBUG_OPTS);
			set_debug_flag(parg);

			/* print -ZO flag if first time set */
			if (!sv_debug && DEBUG(DEBUG_OPTS))
				fprintf(stderr, "%s: -%c%s\n", pass, flag,
					parg ? parg : "");
			break;

		case '-' :		/* accept - and -- as dummy flags */
		case '\0':
			break;

		default:		/* missing case label if reached!!! */

bad_par:				/* unrecognized parameter */

			fprintf(stderr, E_ILL_OPT, progname, opt);
			if (where)
				fprintf(stderr, E_ILL_OPT2,
					curr_pass == P_ENV ? ENV_VAR :
					curr_pass == P_OPT ? DATE_FILE : "",
					where);
			fprintf(stderr, "\n");
			flags_ok = FALSE;
			break;
		}
        }

	/* if we read the numeric arguments, validate and interpret them */
	if (get_numargs)
		flags_ok &= check_numargs();

	return flags_ok;
}


/*
 * check_numargs - validate and interpret numeric command-line parameters;
 * return TRUE if all OK, print error message and return FALSE if not
 */
int
#ifdef PROTOS
check_numargs(void)
#else
check_numargs()
#endif
{
	/* Validate non-flag (numeric) parameters */

	struct tm *p_tm;	/* for getting current month/year */
	time_t tmp;
	int params_ok = TRUE;	/* return value */

	switch (nargs) {
	case 0:		/* no arguments - print current month and/or year */
		time(&tmp);
		p_tm = localtime(&tmp);
		init_month = p_tm->tm_mon + 1;
		/* revised to conform to standard Un*x behavior 01/25/00 */
/*		init_year = century() + (p_tm->tm_year % 100); */
		init_year = TM_YEAR + p_tm->tm_year;
		nmonths = 1;
		break;

	case 1:		/* one argument - print entire year */
		init_month = JAN;
		init_year = numargs[0];
		nmonths = 12;
		break;

	default:	/* two or three arguments - print one or more months */
		init_month = numargs[0];
		init_year = numargs[1];
		nmonths = nargs > 2 ? numargs[2] : 1;
		break;
	}

	if (nmonths < 1)		/* ensure at least one month */
		nmonths = 1;

	/* check range of month and year */

	if (init_month < JAN || init_month > DEC) {
		fprintf(stderr, E_ILL_MONTH, progname, init_month, JAN, DEC);
		params_ok = FALSE;
	}
	
	if (init_year >= 0 && init_year < 100)	/* treat nn as CCnn */
		init_year += century();
	
	if (init_year < MIN_YR || init_year > MAX_YR) {
		fprintf(stderr, E_ILL_YEAR, progname, init_year, MIN_YR, MAX_YR);
		params_ok = FALSE;
	}

	return params_ok;
}


/*
 * color_msg - return character string explaining default day colors;
 * assumes that defaults consist of black and at most one other color
 */
char *
#ifdef PROTOS
color_msg(void)
#else
color_msg()
#endif
{
	int i, ngray = 0, others, gray = GRAY;
	static char msg[80], *alt_color;

	for (i = SUN; i <= SAT; i++)	/* count "logical gray" weekdays */
		if (default_color[i] != BLACK) {
			gray = default_color[i];
			ngray++;
			}

	alt_color = color_names[gray];	/* map "logical gray" to its name */

	if (ngray == 0 || ngray == 7) {		/* all same color? */
		sprintf(msg, COLOR_MSG_1, ngray ? alt_color : W_BLACK);
		return msg;
	}

	others = ngray <= 3 ? BLACK : gray;	/* no - get predominant color */
	msg[0] = '\0';
	for (i = SUN; i <= SAT; i++)
		if (default_color[i] != others) {
			strncat(msg, days_ml[LANG_DEFAULT][i], MIN_DAY_LEN);
			strcat(msg, "/");
		}
	LASTCHAR(msg) = ' ';

	sprintf(msg + strlen(msg), COLOR_MSG_2,
		others == BLACK ? alt_color : W_BLACK,
                others == BLACK ? W_BLACK : alt_color);
	return msg;
}


/*
 * usage - print message explaining correct usage of the command-line
 * arguments and flags.  If "fullmsg" is true, print associated text
 */
void
#ifdef PROTOS
usage(FILE *fp,		/* destination of usage message */
      int fullmsg)	/* print complete message? */
#else
usage(fp, fullmsg)
	FILE *fp;	/* destination of usage message */
	int fullmsg;	/* print complete message? */
#endif
{
	FLAG_MSG *pflag;
	PARAM_MSG *ppar;
	DATE_MSG *pdate;
	KWD_H *phol;
	char buf[30], *p, flag, *meta;
	int nchars, first, i, indent, n;

	sprintf(buf, "%s: %s", W_USAGE, progname);
	nchars = indent = strlen(buf);
	first = TRUE;
	meta = p = NULL;
	fprintf(fp, "\n%s", buf);

	/* convert various values to strings for printing (cf. pcallang.h) */
	sprintf(Xtval, "%d", 0);
	sprintf(Ytval, "%d", 0);
	sprintf(Xsval, "%.3f", 1.0);
	sprintf(Ysval, "%.3f", 1.0);

	sprintf(Ncopy, "%d", NCOPY);

	/* loop to print command-line syntax message (by group of flags) */

	for (pflag = flag_msg; (flag = pflag->flag) != '\0'; pflag++) {
		if (flag == '\n') {		/* end of group? */
			if (p)
				*p = '\0';
			if (meta) {		/* append metavariable name */
				strcat(buf, " ");
				strcat(buf, meta);
			}
			strcat(buf, "]");
			n = strlen(buf);
			if (nchars + n > SCREENWIDTH) {	/* does it fit on line? */
				fprintf(fp, "\n");		/* no - start new one */
				for (nchars = 0; nchars < indent; nchars++)
					fprintf(fp, " ");
			}
			fprintf(fp, "%s", buf);
			nchars += n;
			first = TRUE;
		}
		else if (flag != ' ') {		/* accumulate flags for group */
			if (first) {
				sprintf(buf, " [");
				p = buf + strlen(buf);
			}
			else
				*p++ = '|';
			*p++ = '-';
			*p++ = flag;
			meta = pflag->meta;	/* save metavariable name */
			first = FALSE;
		}
	}

	/* loop to print selected numeric parameter descriptions */

	for (i = 0; i < PARAM_MSGS; i++) {
		sprintf(buf, " [%s]%s", param_msg[i].desc,
			i < PARAM_MSGS - 1 ? " |" : "");
		n = strlen(buf);
		if (nchars + n > SCREENWIDTH) {	/* does it fit on line? */
			fprintf(fp, "\n");		/* no - start new one */
			for (nchars = 0; nchars < indent; nchars++)
				fprintf(fp, " ");
		}
		fprintf(fp, "%s", buf);
		nchars += n;
	}

	fprintf(fp, "\n\n");
	if (! fullmsg) {
		fprintf(fp, USAGE_MSG, progname, F_HELP);
		return;
	}
	
	/* loop to print the full flag descriptions */

	for (pflag = flag_msg; (flag = pflag->flag) != '\0'; pflag++) {
		if (flag == '\n') {	/* newline?  print and quit */
			fprintf(fp, "\n");
			continue;
		}
		p = buf;		/* copy flag and metavariable to buffer */
		if (flag != ' ')
			*p++ = '-';
	/* special hack for VMS - surround upper-case flags in quotes */
#ifdef VMS
		if (isupper(flag)) {
			*p++ = '"';
			*p++ = flag;
			*p++ = '"';
		}
		else
			*p++ = flag;
#else
		*p++ = flag;
#endif
		*p = '\0';
		if (pflag->meta)
			sprintf(p, " %s", pflag->meta);
		fprintf(fp, "\t%-20.20s", buf);
		if (pflag->text)
			fprintf(fp, "%s", pflag->text);

		/* print default value if specified */
		if (pflag->def)
			fprintf(fp, " (%s: %s)", W_DEFAULT, pflag->def[0] ? pflag->def : "\"\"" );
		fprintf(fp, "\n");

		/* special case - print color messages after F_OUTLINE_GRAY */
		if (flag == F_OUTLINE_GRAY)
			fprintf(fp, "\t%20s  (%s: %s)\n", "", W_DEFAULT, color_msg());

	}
	
	/* now print the information about the numeric parameters */

	for (ppar = param_msg; ppar->desc; ppar++)
		fprintf(fp, "\t%-16.16s%s\n", ppar->desc, ppar->text);
	
	/* print the date file syntax message */

	fprintf(fp, "\n");
	for (pdate = date_msg; *pdate; pdate++)
		fprintf(fp, "\t%s\n", *pdate);

	/* print list of predefined holidays */
	fprintf(fp, "\n\t%s\n\n", PREDEF_MSG_1);
#define HOLIDAYS_PER_LINE 4
	for (phol = predef_events, n = 0;
	     phol->name;
	     phol++, n = (n + 1) % HOLIDAYS_PER_LINE)
		fprintf(fp, "%s%-15.15s %s", n == 0 ? "\t    " : "",
			phol->name,
			(n == HOLIDAYS_PER_LINE - 1) || !(phol[1].name) ?
			"\n" : "");
	fprintf(fp, "\n\t%s\n", PREDEF_MSG_2);
	fprintf(fp, "\n");

}


/*
 * alt_fopen - attempt to open a file in one of several paths in a
 * NULL-terminated path list.  If successful, return (opened) file pointer
 * and fill in full path name; if not, return NULL
 */
FILE *
#ifdef PROTOS
alt_fopen(char *fullpath,	/* full path name (output) */
	  char *name,		/* base name (or full path spec) */
	  char *pathlist[],	/* NULL-terminated path list */
	  char *access)		/* permission requested */
#else
alt_fopen(fullpath, name, pathlist, access)
	char *fullpath;		/* full path name (output) */
	char *name;		/* base name (or full path spec) */
	char *pathlist[];	/* NULL-terminated path list */
	char *access;		/* permission requested */
#endif
{
	char **path;
	FILE *fp;

	/* print complete list of paths if debugging path names */
	if (DEBUG(DEBUG_PATHS)) {
		fprintf(stderr, "Searching for %s in the following paths:\n",
			name);
		for (path = pathlist; *path; path++)
			fprintf(stderr, "  %s\n", **path ? *path : ".");
	}

	/* search path list for requested file */
	for (path = pathlist; *path; path++) {
		mk_filespec(fullpath, *path, name);
		if ((fp = fopen(fullpath, access)) != NULL) {
			if (DEBUG(DEBUG_PATHS))
				fprintf(stderr, "found %s\n", fullpath);
			return fp;
		}
	}

	fullpath[0] = '\0';		/* file not found */
	return NULL;
}


/*
 * gen_lang_sym - create a symbol name "lang_XX" corresponding to the language
 * code passed as an argument (e.g., LANG_ENGLISH => "lang_en")
 */
char *
#ifdef PROTOS
gen_lang_sym(int lang)		/* language code (LANG_ENGLISH, et. al.) */
#else
gen_lang_sym(lang)
	int lang;		/* language code (LANG_ENGLISH, et. al.) */
#endif
{
	static char buf[sizeof(DEF_LANG) + MIN_LANG_LEN + 1];
	char *p;
	int i;

	strcpy(buf, DEF_LANG);
	p = buf + strlen(buf);
	for (i = 0; i < MIN_LANG_LEN; i++)
		p[i] = lang_id[lang][i];
	p[i] = '\0';

	return buf;
}
