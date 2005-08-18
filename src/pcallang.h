/*
 * pcallang.h - language-dependent strings (month and day names, option file 
 * keywords, preprocessor tokens, prepositions, etc.):
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
 *		Fix long-standing error in Spanish 'Saturday' string (the
 *		first 'a' needed an acute accent).
 *		
 *		Replace certain uses of 'u'+circumflex with the proper
 *		'u'+macron in certain Latvian and Lithuanian month names.
 *		
 *		B.Marr		2005-08-02
 *		
 *		Support new 'delete' keyword, from Bill Bogstad's 'event
 *		deletion' patch.
 *		
 *		Support new language: Catalan (thanks to Carles Sadurní
 *		Anguita <blat at puntnet.org>).
 *		
 *		To be consistent, convert all high-order characters in the
 *		language string specifications to simple octal format, to
 *		match the octal format used in the PostScript encoding vector
 *		specifications of 'pcalinit.ps'.  This makes it easier to edit
 *		this ('pcallang.h') file in any editor.  It also makes
 *		checking of the language strings against the character
 *		encodings simpler.
 *		
 *		B.Marr		2005-01-04
 *		
 *		Support new character encodings (KOI8-R and several 'ISO
 *		8859-*' encodings).  Use new KOI8-R character mapping to
 *		properly support Russian langauge.  Use new Latin-4 character
 *		mapping to properly support Lithuanian and Latvian langauges.
 *		
 *		Support new languages: Czech (thanks to Peter Cernoch
 *		<pcernoch at volny dot cz>) and Hungarian (thanks to Ferenc
 *		Kruzslicz <kruzslic at ktk.pte.hu>)
 *
 *	4.8.0
 *		B.Marr		2004-12-12
 *		
 *		Add Russian, Latvian, and Lithuanian language support all
 *		based on patches from Neeme Praks (nemecec at users dot
 *		sourceforge dot net).
 *		
 *		B.Marr		2004-12-04
 *		
 *		Fix misleading references to "holiday" to instead refer to
 *		"predefined event" (i.e. not all pre-defined events are
 *		'holidays').  Create and support concept of 'input' language
 *		versus 'output' language.  Support new paper sizes.  Support
 *		specification of paper size via run-time option (command-line,
 *		etc).  Remove spaces embedded within tab fields.  Clarify help
 *		text output ('pcal -h').
 *		
 *		B.Marr		2004-11-11
 *		
 *		Add Swedish, Ukrainian, Portuguese, and Estonian languages
 *		based respectively on patches from Joel Fredrikson (joel at it
 *		dot uu dot se), Volodymyr M. Lisivka (lvm at mystery dot lviv
 *		dot net), Pedro Zorzenon Neto (pzn at terra dot com dot br),
 *		and Neeme Praks (nemecec at users dot sourceforge dot net).
 *		Provide support for "Friday the 13th" events, based on a patch
 *		from Don Laursen (donrl at users dot sourceforge dot net).
 *		Merge content from (now-obsolete) files 'months.h' and
 *		'days.h' into this file.
 *
 *	4.7.1	SF	01/06/2003	added F_1COLUMN and Finnish language
 *
 *	4.7	AWR	01/26/2000	move F_USAGE, F_VERSION, and F_HELP
 *					flags from first command line pass
 *					(P_CMD1) to preliminary command line
 *					pass (P_CMD0) in order to parse these
 *					flags (and exit) prior to parsing any
 *					numeric arguments
 *
 *			03/08/1998	introduce static strings for conversion
 *					of numeric defaults (cf. pcaldefs.h,
 *					usage() in pcal.c)
 *
 *			12/21/1997	add braces around subarray definitions
 *
 *					support -H flag to generate calendar
 *					as HTML table
 *
 *	4.6	AWR	09/13/1996	support "nearest_before" and
 *					"nearest_after" keywords
 *
 *		AWR	11/10/1995	support -T flag to select default
 *					font style (Bold/Italic/Roman)
 *
 *		AWR	05/17/1995	added Esperanto support
 *
 *		AWR	05/09/1995	add support for bold/Italic/Roman
 *					font styles using troff-style \f[BIR]
 *					escape sequences
 *
 *		AH	02/03/1995	added more languages for headers
 *					added orthodox special dates
 *
 *	4.5	AWR	12/21/1993	removed references to obsolete
 *					COLOR_MIN, COLOR_MAX (cf. pcaldefs.h)
 *
 *			11/09/1993	parameterize maximum length of entries
 *					in days[] (DAYSIZE)
 *
 *			09/30/1993	revise flag descriptions for [-dnt]
 *					to reflect independent specification
 *					of font name and point size
 *
 *			07/09/1993	revise PostScript comment block
 *
 *		AWR	02/05/1993	support -# flag (specify multiple
 *					copies of each output page)
 *
 *		AWR	02/11/1992	scrap moon file message
 *
 *					add support for predefined holidays
 *
 *	4.4	AWR	01/20/1992	support -z flag (specify time zone)
 *
 *		AWR	01/13/1992	support optional font size on -d, -t
 *					move color names here; change
 *					semantics, messages, etc. for -[bgOG]
 *
 *	4.3	AWR	12/03/1991	support -s flag (override default
 *					shading of dates and fill boxes)
 *
 *		AWR	10/17/1991	support -Z flag to generate debugging
 *					information
 *
 *	4.2	AWR	10/08/1991	support -[kK] flags (select position
 *					of small calendars
 *
 *		AWR	10/03/1991	support "note/<n>" (new message only)
 *
 *					support -S flag (no small calendars)
 *
 *		AWR	10/02/1991	support -N flag (user-specified notes
 *					heading)
 *
 *					change message for -n to reflect
 *					optional notes font size
 *
 *		AWR	09/30/1991	support "if" (synonym for "ifdef") and
 *					"elif" in date file
 *
 *		AWR	09/19/1991	support -c flag (generate input file
 *					suitable for use with Un*x "calendar"
 *					utility and Pcal)
 *
 *					support "elif" in "if{n}def" block
 *
 *	4.11	AWR	08/20/1991	support "nearest" keyword as per
 *					Andy Fyfe
 *
 *					changed MIN_ORD_LEN from 3 to 4 (to
 *					distinguish "even" from "every")
 *
 *					add ABBR_DAY_LEN and ABBR_MONTH_LEN
 *					(cf. writefil.c) for length of
 *					abbreviated day/month names
 *
 *					document %u and %w formats and number
 *					following %[+-] (cf. writefil.c)
 *
 *					add DEF_WHOLE_YEAR (predefined when
 *					-w flag specified - cf. pcal.c)
 *
 *	4.1	AWR	08/16/1991	support -G flag (outlined gray)
 *
 *	4.02	AWR	07/02/1991	support -v flag (version number);
 *					add ordinal_suffix(); add format
 *					specifiers to help message
 *
 *	4.0	AWR	03/01/1991	expand parameter message to explain
 *					parameter meaning when -w specified
 *
 *		AWR	02/19/1991	revise ordinal definitions for
 *					support of negative ordinals
 *
 *		AWR	02/06/1991	add text describing expression syntax
 *
 *		AWR	02/04/1991	support "odd" and "even" ordinals
 *
 *		AWR	01/28/1991	support -B (blank fill squares) flag
 *					and -O (outline "gray" dates) flag
 *
 *		AWR	01/16/1991	added moon file support (tokens, help
 *					file text, error messages); define
 *					note block heading here
 *
 *		AWR	01/07/1991	added support for -w (whole year) flag
 *
 */

#define ALL	"all"		/* command-line or date file keywords */
#define HOLIDAY	"holiday"

/* names of colors used in printing dates (cf. flag_msg[] below) */

#define W_BLACK		"black"
#define W_GRAY		"gray"
#define W_OUTLINE	"outline"
#define W_OUTLINE_GRAY	"outline-gray"

#define W_ROMAN		"Roman"		/* for -T usage message */

#define DAYSIZE 20	/* large enough for longest string in days_ml[] below */
#define LANGSIZE 20	/* large enough for longest string in lang_id[] below */

#define NUM_LANGUAGES		18	/* *** Change this if adding languages *** */
#define LANG_ENGLISH		0
#define LANG_GREEK		1
#define LANG_ITALIAN		2
#define LANG_FRENCH		3
#define LANG_GERMAN		4
#define LANG_ESPERANTO		5
#define LANG_SPANISH		6
#define LANG_FINNISH		7
#define LANG_SWEDISH		8
#define LANG_PORTUGUESE		9
#define LANG_UKRAINIAN		10
#define LANG_ESTONIAN		11
#define LANG_RUSSIAN 		12
#define LANG_LATVIAN		13
#define LANG_LITHUANIAN		14
#define LANG_CZECH		15
#define LANG_HUNGARIAN		16
#define LANG_CATALAN		17

#ifndef LANG_DEFAULT
#define LANG_DEFAULT	LANG_ENGLISH
#endif

#ifdef MAIN_MODULE

/*
 * International section for month names and full/abbreviated day-of-week
 * names.  Support for (almost) any language is possible.
 *
 * Note: The order of the languages used here must match the order defined by
 * the 'LANG_xxx' enumerations (defined above)!
 * 
 * Greek given in the ELOT-928 encodings (ISO 8859-7)
 * 
 */
char *months_ml[NUM_LANGUAGES][12] = {

   /* English */
   { "January", "February", "March", "April", "May", "June", 
     "July", "August", "September", "October", "November", "December" },

   /* Greek */
   { "\311\341\355\357\365\334\361\351\357\362", 
     "\326\345\342\361\357\365\334\361\351\357\362", 
     "\314\334\361\364\351\357\362", 
     "\301\360\361\337\353\351\357\362", 
     "\314\334\351\357\362", 
     "\311\357\375\355\351\357\362", 
     "\311\357\375\353\351\357\362", 
     "\301\375\343\357\365\363\364\357\362", 
     "\323\345\360\364\335\354\342\361\351\357\362", 
     "\317\352\364\376\342\361\351\357\362", 
     "\315\357\335\354\342\361\351\357\362", 
     "\304\345\352\335\354\342\361\351\357\362" },

   /* Italian */
   { "Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", 
     "Luglio", "Agosto", "Settembre", "Ottobre", "Novembre", "Dicembre" },

   /* French */
   { "Janvier", "F\351vrier", "Mars", "Avril", "Mai", "Juin", 
     "Juillet", "Ao\373t", "Septembre", "Octobre", "Novembre", "D\351cembre" },

   /* German */
   { "Januar", "Februar", "M\344rz", "April", "Mai", "Juni", 
     "Juli", "August", "September", "Oktober", "November", "Dezember" },

   /* Esperanto */
   { "Januaro", "Februaro", "Marto", "Aprilo", "Majo", "Junio", 
     "Julio", "A\375gusto", "Septembro", "Oktobro", "Novembro", "Decembro" },

   /* Spanish */
   { "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", 
     "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" },

   /* Finnish */
   { "Tammikuu", "Helmikuu", "Maaliskuu", "Huhtikuu", "Toukokuu", "Kes\344kuu",
     "Hein\344kuu", "Elokuu", "Syyskuu", "Lokakuu", "Marraskuu", "Joulukuu" },

   /* Swedish */
   { "Januari", "Februari", "Mars", "April", "Maj", "Juni", 
     "Juli", "Augusti", "September", "Oktober", "November", "December" },

   /* Portuguese */
   { "Janeiro", "Fevereiro", "Mar\347o", "Abril", "Maio", "Junho", 
     "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro" },

   /* Ukrainian */
   { "\323\246\336\305\316\330",
     "\314\300\324\311\312", 
     "\302\305\322\305\332\305\316\330",
     "\313\327\246\324\305\316\330", 
     "\324\322\301\327\305\316\330",
     "\336\305\322\327\305\316\330", 
     "\314\311\320\305\316\330",
     "\323\305\322\320\305\316\330", 
     "\327\305\322\305\323\305\316\330",
     "\326\317\327\324\305\316\330", 
     "\314\311\323\324\317\320\301\304",
     "\307\322\325\304\305\316\330" },

   /* Estonian */
   { "Jaanuar", "Veebruar", "Märts", "Aprill", "Mai", "Juuni",
     "Juuli", "August", "September", "Oktoober", "November", "Detsember" },

   /* Russian */
   { "\321\316\327\301\322\330",
     "\306\305\327\322\301\314\330", 
     "\315\301\322\324",
     "\301\320\322\305\314\330", 
     "\315\301\312",
     "\311\300\316\330", 
     "\311\300\314\330",
     "\301\327\307\325\323\324", 
     "\323\305\316\324\321\302\322\330",
     "\317\313\324\321\302\322\330", 
     "\316\317\321\302\322\330",
     "\304\305\313\301\302\322\330" },

   /* Latvian */
   { "janv\340ris", "febru\340ris", "marts", "apr\357lis",
     "maijs", "j\376nijs", "j\376lijs", "augusts", 
     "septembris", "oktobris", "novembris", "decembris" },

   /* Lithuanian */
   { "Sausis", "Vasaris", "Kovas", "Balandis",
     "Gegu\276\354", "Bir\276elis", "Liepa", "Rugpj\376tis",
     "Rugs\354jis", "Spalis", "Lapkritis", "Gruodis" },

   /* Czech */
   { "Leden", "\332nor", "B\370ezen", "Duben",
     "Kv\354ten", "\310erven", "\310ervenec", "Srpen",
     "Z\341\370\355", "\330\355jen", "Listopad", "Prosinec" },

   /* Hungarian */
   { "Janu\341r", "Febru\341r", "M\341rcius", "\301prilis",
     "M\341jus", "J\372nius", "J\372lius", "Augusztus",
     "Szeptember", "Okt\363ber", "November", "December" },

   /* Catalan */
   { "Gener", "Febrer", "Mar\347", "Abril", "Maig", "Juny", 
     "Juliol", "Agost", "Setembre", "Octubre", "Novembre", "Desembre" },

};

/* 
 * Day-of-week full names... 
 * 
 * Must be a 3-D array so address within may be used as an initializer.
 *
 */
char days_ml[NUM_LANGUAGES][7][DAYSIZE] = {

   /* English */
   { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", 
     "Saturday" },

   /* Greek */
   { "\312\365\361\351\341\352\336", "\304\345\365\364\335\361\341",
     "\324\361\337\364\347", "\324\345\364\334\361\364\347",
     "\320\335\354\360\364\347", "\320\341\361\341\363\352\345\365\336",
     "\323\334\342\342\341\364\357" },
   
   /* Italian */
   { "Domenica", "Luned\354", "Marted\354", "Mercoled\354", "Gioved\354", 
     "Venerd\354", "Sabato" },
   
   /* French */
   { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" },
   
   /* German */
   { "Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", 
     "Samstag" },
   
   /* Esperanto */
   { "Diman\346o", "Lundo", "Mardo", "Merkredo", "\254a\375do", "Vendredo", 
     "Sabato" },
   
   /* Spanish */
   { "Domingo", "Lunes", "Martes", "Mi\351rcoles", "Jueves", "Viernes", 
     "S\341bado" },
   
   /* Finnish */
   { "Sunnuntai", "Maanantai", "Tiistai", "Keskiviikko", "Torstai", 
     "Perjantai", "Lauantai" },
   
   /* Swedish */
   { "S\366ndag", "M\345ndag", "Tisdag", "Onsdag", "Torsdag", "Fredag", 
     "L\366rdag" },
   
   /* Portuguese */
   { "Domingo", "Segunda", "Ter\347a", "Quarta", "Quinta", "Sexta", 
     "S\341bado" },
   
   /* Ukrainian */
   { "\316\305\304\246\314\321", "\320\317\316\305\304\246\314\317\313", 
     "\327\246\327\324\317\322\317\313", "\323\305\322\305\304\301", 
     "\336\305\324\327\305\322", "\320\047\321\324\316\311\303\321", 
     "\323\325\302\317\324\301" },
   
   /* Estonian */
   { "P\374hap\344ev", "Esmasp\344ev", "Teisip\344ev", "Kolmap\344ev", 
     "Neljap\344ev", "Reede", "Laup\344ev" },

   /* Russian */
   { "\327\317\323\313\322\305\323\305\316\330\305", 
     "\320\317\316\305\304\305\314\330\316\311\313", 
     "\327\324\317\322\316\311\313", "\323\322\305\304\301", 
     "\336\305\324\327\305\322\307", "\320\321\324\316\311\303\301", 
     "\323\325\302\302\317\324\301" }, 

   /* Latvian */
   { "Sv\272tdiena", "Pirmdiena", "Otrdiena", "Tre\271diena",
     "Ceturtdiena", "Piektdiena", "Sestdiena" },

   /* Lithuanian */
   { "Sekmadienis", "Pirmadienis", "Antradienis", "Tre\350iadienis",
     "Ketvirtadienis", "Penktadienis", "\251e\271tadienis" },

   /* Czech */
   { "Ned\354le", "Pond\354l\355", "\332ter\375", "St\370eda", 
     "\310tvrtek", "P\341tek", "Sobota" },

   /* Hungarian */
   { "Vas\341rnap", "H\351tf\365", "Kedd", "Szerda",
     "Cs\374t\366rt\366k", "P\351ntek", "Szombat" },

   /* Catalan */
   { "Diumenge", "Dilluns", "Dimarts", "Dimecres", 
     "Dijous", "Divendres", "Dissabte" },

};

/* 
 * Day-of-week abbreviated names... 
 * 
 */
char days_ml_short[NUM_LANGUAGES][7][DAYSIZE] = {
   /* English */
   { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" },
   
   /* Greek */
   { "\312\365\361", "\304\345\365", "\324\361\337", "\324\345\364", 
     "\320\335\354", "\320\341\361", "\323\334\342" },
   
   /* Italian */
   { "Dom", "Lun", "Mar", "Mer", "Gio", "Ven", "Sab" },
   
   /* French */
   { "Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam" },
   
   /* German */
   { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" },
   
   /* Esperanto */
   { "Dim", "Lun", "Mar", "Mer", "\254a\375", "Ven", "Sab" },
   
   /* Spanish */
   { "Dom", "Lun", "Mar", "Mi\351", "Jue", "Vie", "S\341b" },
   
   /* Finnish */
   { "Su", "Ma", "Ti", "Ke", "To", "Pe", "La" },
   
   /* Swedish */
   { "S\366n", "M\345n", "Tis", "Ons", "Tor", "Fre", "L\366r" },
   
   /* Portuguese */
   { "Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "S\341b" },
   
   /* Ukrainian */
   { "\316\304", "\320\316", "\327\324", "\323\322", 
     "\336\324", "\320\324", "\323\302" },

   /* Estonian */
   { "P", "E", "T", "K", "N", "R", "L" },

   /* Russian */
   { "\367\323", "\360\316", "\367\324", "\363\322", 
     "\376\324", "\360\324", "\363\302" }, 

   /* Latvian */
   { "Sv", "Pm", "Ot", "Tr", "Ce", "Pt", "Se" },

   /* Lithuanian */
   { "S", "Pr", "A", "T", "K", "P", "\251" },

   /* Czech */
   { "Ne", "Po", "\332t", "St", "\310t", "P\341", "So" },

   /* Hungarian */
   { "V", "H", "K", "Sze", "Cs", "P", "Szo" },

   /* Catalan */
   { "Dg", "Dl", "Dm", "Dc", "Dj", "Dv", "Ds" },

};


/*
 * list of languages supported (-a flag) in same order as LANG_* pseudo-
 * enumeration above (must be a 2-D array so address within may be used
 * as an initializer); the full name (in brackets) is used only when
 * printing the default in the usage message (see MIN_LANG_LEN below)
 * 
 * The codes used here should match the 2-letter version of the ISO 639
 * (language code) standard.  Do not use the ISO 3166 (country code) standard
 * since those values are often different from the language code values
 * (e.g. Sweden country code = 'se', Swedish language code = 'sv').
 * 
 */
char lang_id[NUM_LANGUAGES][LANGSIZE] = {
	"en [English]",
	"el [Greek]",
	"it [Italian]",
	"fr [French]",
	"de [German]",
	"eo [Esperanto]",
	"es [Spanish]",
	"fi [Finnish]",
	"sv [Swedish]",
	"pt [Portuguese]",
	"uk [Ukrainian]",
	"et [Estonian]",
	"ru [Russian]",
	"lv [Latvian]",
	"lt [Lithuanian]",
	"cs [Czech]",
	"hu [Hungarian]",
	"ca [Catalan]",
};

/*
 * Associate a default character mapping with each of the supported languages.
 */
int lang_mapping[] = {
	ENC_NONE,	/* English */
	ENC_GREEK,	/* Greek */
	ENC_LATIN_1,	/* Italian */
	ENC_LATIN_1,	/* French */
	ENC_LATIN_1,	/* German */
	ENC_LATIN_3,	/* Esperanto */
	ENC_LATIN_1,	/* Spanish */
	ENC_LATIN_1,	/* Finnish */
	ENC_LATIN_1,	/* Swedish */
	ENC_LATIN_1,	/* Portuguese */
	ENC_KOI8_U,	/* Ukrainian */
	ENC_LATIN_1,	/* Estonian */
	ENC_KOI8_R,	/* Russian */
	ENC_LATIN_4,	/* Latvian */
	ENC_LATIN_4,	/* Lithuanian */
	ENC_LATIN_2,	/* Czech */
	ENC_LATIN_2,	/* Hungarian */
	ENC_LATIN_1,	/* Catalan */
};

/* Define the default 'input' language (for interpretation of dates on events
 * in the configuration file) and the default 'output' language (for the month
 * names and day-of-week names on the output).
 */
int input_language = LANG_DEFAULT;
int output_language = LANG_DEFAULT;

/* must be in same order as color definitions in pcaldefs.h; also see
 * "prtday{}" in pcalinit.ps
 */

char *color_names[] = { W_BLACK, W_GRAY, W_OUTLINE, W_OUTLINE_GRAY };

/* Must be a 2-D array so address within may be used as an initializer;
 * wildcard names must be in same order as symbolic names in pcaldefs.h
 * Note that the weekday names were superseded by days_ml[][] throughout
 * as of v4.6; only the wildcards are still used
 */
char *days[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
        "Saturday",		/* weekday names */
	"day", "weekday", "workday", HOLIDAY, "nonweekday", "nonworkday",
	"nonholiday"		/* wildcards */
	};

/* preprocessor tokens: token name, token code, dispatch routine; note that
 * substring "if" must follow "ifdef" and "ifndef" for proper matching
 */

KWD_F pp_info[] = {
	{ "define",	PP_DEFINE,	do_define },
	{ "elif",	PP_ELIF,	do_ifdef },
	{ "else",	PP_ELSE,	NULL },
	{ "endif",	PP_ENDIF,	NULL },
	{ "ifdef",	PP_IFDEF,	do_ifdef },
	{ "ifndef",	PP_IFNDEF,	do_ifndef },
	{ "if",		PP_IFDEF,	do_ifdef },	/* "ifdef" synonym */
	{ "include",	PP_INCLUDE,	NULL },		/* do_include */
	{ "undef",	PP_UNDEF,	do_undef },
	{ NULL,		PP_OTHER,	NULL }		/* must be last */
};

/* ordinal numbers - e.g. "first Monday in September": ordinal name,
 * ordinal code, ordinal value; note that "all" is parsed as a keyword
 * and (depending on context) may be subsequently treated as an ordinal
 */

KWD_O ordinals[] = {
	{ "first",	ORD_POSNUM,	FIRST },
	{ "second",	ORD_POSNUM,	SECOND },
	{ "third",	ORD_POSNUM,	THIRD },
	{ "fourth",	ORD_POSNUM,	FOURTH },
	{ "fifth",	ORD_POSNUM,	FIFTH },
	{ "last",	ORD_NEGNUM,	LAST },
	{ "odd",	ORD_ODD,	0 },
	{ "even",	ORD_EVEN,	0 },
	{ NULL,		ORD_OTHER,	0 }		/* must be last */
};

/* predefined events (often, but not always, holiday events): event name,
 * definition, and dispatch function.  The definition and dispatch function
 * are mutually exclusive; those events which can easily be redefined as an
 * equivalent Pcal date are translated in-line by parse_date() using the
 * definition string, while the "hard" ones - e.g., Easter - each use a
 * dedicated dispatch function instead.  (The syntax for Christmas is awkward,
 * but unlike the obvious "12/25" it is independent of American/European date
 * parsing conventions.)
 */

KWD_H predef_events[] = {
	{ "Christmas",	  "25th day of December",   NULL },
	{ "Thanksgiving", "Fourth Thu in November", NULL },
	{ "Easter",	  NULL,			    find_easter },
	{ "Good_Friday",  "Friday before Easter",   NULL },
#ifdef FRIDAY_13th
	/* Allow user to specify "F13" as set of events */
	{ "F13",	  NULL,                     find_fri13th },
#endif /* FRIDAY_13th by DRL */
#ifndef NO_ORTHODOX
	/* Orthodox Easter related */
	{ "GEaster",	  NULL,			    find_odox_easter },
	{ "Gstgeorge",	  NULL,			    find_odox_stgeorge },
	{ "Gmarcus",	  NULL,			    find_odox_marcus },
#endif /* !NO_ORTHODOX  */
	{ NULL,		  NULL,			    NULL }  /* must be last */
};

/* allowable suffixes for ordinal numbers - these must be in order 0, 1, 2...
 * according to the rules of the target language; cf. ordinal_suffix() below
 */

char *ord_suffix[] = { "th", "st", "nd", "rd", NULL };

/* prepositions - e.g., "Friday after fourth Thursday in November" */

KWD preps[] = {
	{ "before",		PR_BEFORE },
	{ "preceding",		PR_BEFORE },
	{ "on_or_before",	PR_ON_BEFORE },
	{ "oob",		PR_ON_BEFORE },
	{ "after",		PR_AFTER },
	{ "following",		PR_AFTER },
	{ "on_or_after",	PR_ON_AFTER },
	{ "ooa",		PR_ON_AFTER },
	{ "nearest",		PR_NEAREST },
	{ "nearest_before",	PR_NEAREST_BEFORE },
	{ "nearest_after",	PR_NEAREST_AFTER },
	{ NULL,			PR_OTHER }		/* must be last */
};

/* other keywords */

KWD keywds[] = {
	{ ALL,			DT_ALL },
	{ "each",		DT_ALL },
	{ "every",		DT_ALL },
	{ "note",		DT_NOTE },
	{ "opt",		DT_OPT },
	{ "input-language",	DT_INPUT_LANGUAGE },
	{ "year",		DT_YEAR },
	{ "delete",		DT_DELETE },
	{ NULL,			DT_OTHER }	/* must be last */
};

/* moon phases (for moon file) */

KWD phases[] = {
	{ "new_moon",		MOON_NM },	/* new moon */
	{ "nm",			MOON_NM },
	{ "first_quarter",	MOON_1Q },	/* first quarter */	
	{ "1Q",			MOON_1Q },
	{ "FQ",			MOON_1Q },
	{ "full_moon",		MOON_FM },	/* full moon */
	{ "FM",			MOON_FM },
	{ "last_quarter",	MOON_3Q },	/* last (third) quarter */
	{ "LQ",			MOON_3Q },
	{ "third_quarter",	MOON_3Q },
	{ "3Q",			MOON_3Q },
	{ NULL,			MOON_OTHER }	/* must be last */
};

/* default notes box header */

char default_notes_hdr[] = "Notes";

/* default font style (must be Roman) */

char fontstyle[] = W_ROMAN;

#else
extern char *color_names[];
extern char *months_ml[NUM_LANGUAGES][12];
extern char *days[];
extern char days_ml[NUM_LANGUAGES][7][DAYSIZE];
extern char days_ml_short[NUM_LANGUAGES][7][DAYSIZE];
extern char *esp_accent;
extern char lang_id[NUM_LANGUAGES][LANGSIZE];
extern int input_language;
extern int output_language;
extern KWD_F pp_info[];
// extern KWD_H predef_events[];
extern KWD_H predef_events[];
extern KWD preps[];
extern KWD_O ordinals[];
extern char *ord_suffix[];
extern KWD keywds[];
extern KWD phases[];
extern char default_notes_hdr[];
extern char fontstyle[];
#endif

/* minimum size of abbreviations - adjust as appropriate for target language */

#define MIN_DAY_LEN	3	/* distinguish "Thursday" from "third" */
#define ABBR_DAY_LEN	3	/* length of abbreviated day names */
#define ABBR_MONTH_LEN	3	/* length of abbreviated month names */
#define MIN_PPTOK_LEN	3	/* minimum length of a pre-processor token */
#define MIN_PREP_LEN	9	/* distinguish "nearest", "nearest_before",
				   "nearest_after" */
#define MIN_ORD_LEN	4	/* distinguish "every" from "even" */
#define MIN_LANG_LEN	2	/* effective size of language names */


/*
 * Symbolic names for command-line flags.  These may be changed
 * as desired in order to be meaningful in languages other than
 * English.
 */

#define F_INITIALIZE	'I'		/* re-initialize program defaults */
#define	F_BLACK_DAY	'b'		/* print day in black */
#define F_GRAY_DAY	'g'		/* print day in gray */
#define F_OUTLINE	'O'		/* draw "gray" dates as outlines */
#define F_OUTLINE_GRAY	'G'		/* outline and fill "gray" dates */

#define F_DAY_FONT	'd'		/* select alternate day font */
#define F_NOTES_FONT	'n'		/* select alternate notes font */
#define F_TITLE_FONT	't'		/* select alternate title font */

#define F_REMAP_FONT	'r'		/* remap font for 8-bit characters */

#define F_EMPTY_CAL	'e'		/* print empty calendar */
#define F_DATE_FILE	'f'		/* select alternate date file */
#define F_OUT_FILE	'o'		/* select alternate output file */

#define F_LANDSCAPE	'l'		/* landscape mode */
#define F_PORTRAIT	'p'		/* portrait mode */

#define F_PAPERSIZE	'P'		/* paper size */

#define F_HELP		'h'		/* generate full help message */
#define F_USAGE		'u'		/* generate parameter usage message */
#define F_VERSION	'v'		/* generate version ID */

#define F_MOON_4	'm'		/* print new/quarter/full moons */
#define F_MOON_ALL	'M'		/* print all moons */

#define F_DEFINE	'D'		/* define preprocessor symbol */
#define F_UNDEF		'U'		/* undefine preprocessor symbol */

#define F_L_FOOT	'L'		/* define left foot string */
#define F_C_FOOT	'C'		/* define center foot string */
#define F_R_FOOT	'R'		/* define right foot string */

#define F_NOTES_HDR	'N'		/* define heading for notes box */

#define F_FIRST_DAY	'F'		/* define alternate starting day */

#define F_USA_DATES	'A'		/* parse American date format */
#define F_EUR_DATES	'E'		/* parse European date format */

#define F_X_TRANS	'X'		/* X-axis transformation */
#define F_Y_TRANS	'Y'		/* Y-axis transformation */
#define F_X_SCALE	'x'		/* X-axis scale factor */
#define F_Y_SCALE	'y'		/* Y-axis scale factor */

#define F_JULIAN	'j'		/* print Julian day (day of year) */
#define F_JULIAN_ALL	'J'		/* print Julian day and days left */

#define F_WHOLE_YEAR	'w'		/* print whole year per page */
					/* (cf. W_WYFLAG below) */

#define F_BLANK_BOXES	'B'		/* don't fill unused boxes */

#define F_NUM_PAGES	'#'		/* print multiple copies of each page */

#define F_SC_NONE	'S'		/* suppress small calendars */
#define F_SC_FIRST	'k'		/* prev/next in first two boxes */
#define F_SC_SPLIT	'K'		/* split between first and last boxes */

#define F_SHADING	's'		/* define date/fill box shading */

#define F_CALENDAR	'c'		/* generate "calendar" utility input */

#define F_HTML		'H'		/* generate calendar in HTML */
#define F_1COLUMN	'q'		/* print one column per month (HTML) */

#define F_TIMEZONE	'z'		/* specify time zone for moon phase */

#define F_SETLANG	'a'		/* set output lang for months/days */

#define F_TYPEFACE	'T'		/* set fontstyle (Bold/Roman/Italic) */

/* special "hidden" flag (and subflags) for debug info generation */

#define F_DEBUG		'Z'		/* generate debugging information */

#define D_DATES		'D'		/* debug dates as read */
#define D_FILE_PATHS	'F'		/* debug date file paths */
#define D_MOON		'M'		/* debug moon phases */
#define D_TEXT		'T'		/* debug dates/text as written */
#define D_OPT		'O'		/* debug option flags */
#define D_PREPROCESSOR	'P'		/* debug "preprocessor" operation */

/*
 * Flag usage information - not strictly language-dependent, but here anyway
 * (N.B.: all flags must be represented by an entry in this table!)
 *
 * Flags may appear in any of three places: in environment variable
 * PCAL_OPTS, on the command line, or in "opt" lines in the date file.
 * The command line is actually parsed twice: once before reading the date
 * file to get the flags needed in processing it (-[bcefgklpwADEKSU]), and
 * and again after reading the date file to give the user one last chance
 * to override any of the other flags set earlier.  (Note, however, that
 * the only way to turn off -J|-j [Julian dates], -M|-m [moons], -w [whole
 * year], or -G|-O [outline "gray" dates] once selected is to use -I to
 * reinitialize all program defaults.)
 *
 * The table below supplies the following information about each flag:
 *
 *	- Its name (cf. symbolic definitions above)
 *
 *	- Whether or not it can take an (optional) argument
 *
 *	- Which passes parse it - in order, they are: P_CMD0 ("pre-pass" of
 *	  command line to find debugging flags), P_ENV (environment variable),
 *	  P_CMD1 (first command line pass), P_OPT ("opt" lines in date file), 
 *	  and P_CMD2 (second command line pass).
 *
 *	  The basic idea here is that some flags need to be in effect prior
 *	  to reading the date file (e.g., -E, -A) while the user should
 *	  be able to override other flags set in the date file (e.g., -d,
 *	  -n, -t) on the command line if desired.  Two flags - F_OUTLINE
 *	  and F_OUTLINE_GRAY - are processed in both command-line passes, for
 *	  backward compatibility with earlier versions of Pcal (cf. pcal.c).
 */

#ifdef MAIN_MODULE

FLAG_USAGE flag_tbl[] = {

/*	flag name	arg?		     passes where parsed	*/

	{ F_INITIALIZE,	FALSE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },

	{ F_BLACK_DAY,	TRUE,	         P_ENV | P_CMD1 | P_OPT          },
	{ F_GRAY_DAY,	TRUE,	         P_ENV | P_CMD1 | P_OPT          },
	{ F_OUTLINE,	TRUE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },
	{ F_OUTLINE_GRAY, TRUE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },

	{ F_DAY_FONT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_NOTES_FONT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_TITLE_FONT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_REMAP_FONT,	TRUE,		 P_ENV | P_CMD1 | P_OPT          },

	{ F_PAPERSIZE,	TRUE,		 P_ENV | P_CMD1 | P_OPT          },

	{ F_EMPTY_CAL,	FALSE,	         P_ENV | P_CMD1                  },
	{ F_DATE_FILE,	TRUE,	         P_ENV | P_CMD1                  },
	{ F_OUT_FILE,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_LANDSCAPE,	FALSE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },
	{ F_PORTRAIT,	FALSE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },

	{ F_HELP,	FALSE,	P_CMD0                                   },
	{ F_USAGE,	FALSE,	P_CMD0                                   },
	{ F_VERSION,	FALSE,	P_CMD0                                   },

	{ F_MOON_4,	FALSE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_MOON_ALL,	FALSE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_DEFINE,	TRUE,	         P_ENV | P_CMD1                  },
	{ F_UNDEF,	TRUE,	         P_ENV | P_CMD1                  },

	{ F_L_FOOT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_C_FOOT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_R_FOOT,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_NOTES_HDR,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_FIRST_DAY,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_USA_DATES,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },
	{ F_EUR_DATES,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },

	{ F_X_TRANS,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_Y_TRANS,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_X_SCALE,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_Y_SCALE,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_JULIAN,	FALSE,	         P_ENV          | P_OPT | P_CMD2 },
	{ F_JULIAN_ALL,	FALSE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_WHOLE_YEAR,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },

	{ F_BLANK_BOXES, FALSE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_NUM_PAGES,	TRUE,	         P_ENV          | P_OPT | P_CMD2 },

	{ F_SC_NONE,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },
	{ F_SC_FIRST,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },
	{ F_SC_SPLIT,	FALSE,	         P_ENV | P_CMD1 | P_OPT          },

	{ F_SHADING,	TRUE,            P_ENV          | P_OPT | P_CMD2 },

	{ F_CALENDAR,	FALSE,	         P_ENV | P_CMD1                  },
	{ F_HTML,	FALSE,	         P_ENV | P_CMD1                  },
	{ F_1COLUMN,	FALSE,	         P_ENV | P_CMD1                  },

	{ F_TIMEZONE,	TRUE,            P_ENV          | P_OPT | P_CMD2 },

	{ F_SETLANG,	TRUE,		 P_ENV | P_CMD1 | P_OPT          },

	{ F_TYPEFACE,	TRUE,		 P_ENV | P_CMD1 | P_OPT          },

	{ F_DEBUG,	TRUE,	P_CMD0 | P_ENV          | P_OPT          },

	{ '-',		FALSE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 },
	{ '\0',		FALSE,	         P_ENV | P_CMD1 | P_OPT | P_CMD2 }	/* must be last */
};

/* subflags for debug info flag F_DEBUG */

DEBUG_INFO debug_info[] = {
	{ D_DATES,		DEBUG_DATES },
	{ D_FILE_PATHS,		DEBUG_PATHS },
	{ D_MOON,		DEBUG_MOON },
	{ D_OPT,		DEBUG_OPTS },
	{ D_PREPROCESSOR,	DEBUG_PP },
	{ D_TEXT,		DEBUG_TEXT },
	{ '\0',			0 }		/* must be last */
};

#else
extern FLAG_USAGE flag_tbl[];
extern DEBUG_INFO debug_info[];
#endif

/*
 * Words used in usage() message - translate as necessary
 */

#define W_DEFAULT	"default"		/* translate as required */
#define W_USAGE		"Usage"

#define W_FONT_SIZE	"{<FONT>}{/<n>}"	/* names of metavariables */
#define W_DAY		"<DAY>"
#define W_DAY2		"<DAY>{-<DAY>}"
#define W_STRING	"<STRING>"
#define W_FILE		"<FILE>"
#define W_SYMBOL	"<SYMBOL>"
#define W_VALUE		"<VALUE>"
#define W_LANG		"<LANG>"
#define W_PAPERSIZE	"<PAPERSIZE>"
#define W_TYPEFACE	"B|I|R"
#define W_MAPPING	"<MAPPING>"
#define W_N		"<n>"
#define W_SHADING	"{<d>}{/<f>}"

#define W_MM		"MM"			/* abbrev. for month, year */
#define W_YY		"YY"

#define W_WYFLAG	"-w"			/* must conform to F_WHOLE_YEAR */

/* special flag_msg[] entries for end of option group, etc. */

#define END_GROUP	'\n', NULL, NULL, NULL		/* end of option group */
#define END_LIST	'\0', NULL, NULL, NULL		/* end of list */
#define GROUP_DEFAULT	' ', NULL, " "			/* group default */

/*
 * Message strings to be printed by usage() - translate as necessary
 */
#ifdef MAIN_MODULE

/* to be filled in by usage() */
static char Xtval[VALSIZ], Ytval[VALSIZ], Xsval[VALSIZ], Ysval[VALSIZ],
	    Ncopy[VALSIZ];

FLAG_MSG flag_msg[] = {

/*	flag name	metasyntax	description						default */

	{ F_INITIALIZE,	NULL,		"initialize all parameters to program defaults",	NULL },
	{ END_GROUP },

	{ F_BLACK_DAY,	W_DAY2,		"print weekday(s) in black",				NULL },
	{ F_GRAY_DAY,	W_DAY2,		"print weekday(s) in gray",				NULL },
	{ F_OUTLINE,	W_DAY2,		"print weekday(s) as outline",				NULL },
	{ F_OUTLINE_GRAY, W_DAY2,		"print weekday(s) as filled outline",		NULL },
	{ END_GROUP },

	{ F_SHADING,	W_SHADING,	"specify date/fill box shading (or r:g:b values)",	NULL },
	{ ' ',		NULL,		" ",							SHADING },
	{ END_GROUP },

	{ F_DAY_FONT,	W_FONT_SIZE,	"specify alternate date font and optional size <n>",	NULL },
	{ ' ',		NULL,		" ",							DATEFONT },
	{ F_NOTES_FONT,	W_FONT_SIZE,	"specify alternate notes font and optional size <n>",	NULL },
	{ ' ',		NULL,		" ",							NOTESFONT },
	{ F_TITLE_FONT,	W_FONT_SIZE,	"specify alternate title font and optional size <n>",	NULL },
	{ ' ',		NULL,		" ",							TITLEFONT },
	{ END_GROUP },

	{ F_REMAP_FONT,	W_MAPPING,	"remap 8-bit characters (Latin1, KOI8-U, Roman8, etc)", NULL },
#if MAPFONTS == ENC_LATIN_1
	{ GROUP_DEFAULT,									MAPPING_LATIN_1 },
#elif MAPFONTS == ENC_LATIN_2
	{ GROUP_DEFAULT,									MAPPING_LATIN_2 },
#elif MAPFONTS == ENC_LATIN_3
	{ GROUP_DEFAULT,									MAPPING_LATIN_3 },
#elif MAPFONTS == ENC_LATIN_4
	{ GROUP_DEFAULT,									MAPPING_LATIN_4 },
#elif MAPFONTS == ENC_CYRILLIC
	{ GROUP_DEFAULT,									MAPPING_CYRILLIC },
/* #elif MAPFONTS == ENC_ARABIC */  /* currently unsupported */
/* 	{ GROUP_DEFAULT,									MAPPING_ARABIC }, */
#elif MAPFONTS == ENC_GREEK
	{ GROUP_DEFAULT,									MAPPING_GREEK },
/* #elif MAPFONTS == ENC_HEBREW */  /* currently unsupported */
/* 	{ GROUP_DEFAULT,									MAPPING_HEBREW }, */
#elif MAPFONTS == ENC_LATIN_5
	{ GROUP_DEFAULT,									MAPPING_LATIN_5 },
#elif MAPFONTS == ENC_LATIN_6
	{ GROUP_DEFAULT,									MAPPING_LATIN_6 },
#elif MAPFONTS == ENC_THAI
	{ GROUP_DEFAULT,									MAPPING_THAI },
#elif MAPFONTS == ENC_LATIN_7
	{ GROUP_DEFAULT,									MAPPING_LATIN_7 },
#elif MAPFONTS == ENC_LATIN_8
	{ GROUP_DEFAULT,									MAPPING_LATIN_8 },
#elif MAPFONTS == ENC_LATIN_9
	{ GROUP_DEFAULT,									MAPPING_LATIN_9 },
/* #elif MAPFONTS == ENC_LATIN_10 */  /* currently unsupported */
/* 	{ GROUP_DEFAULT,									MAPPING_LATIN_10 }, */
#elif MAPFONTS == ENC_KOI8_R
	{ GROUP_DEFAULT,									MAPPING_KOI8_R },
#elif MAPFONTS == ENC_KOI8_U
	{ GROUP_DEFAULT,									MAPPING_KOI8_U },
#elif MAPFONTS == ENC_ROMAN8
	{ GROUP_DEFAULT,									MAPPING_ROMAN8 },
#else
	{ GROUP_DEFAULT,									"language-specific mapping" },
#endif
	{ END_GROUP },

	{ F_EMPTY_CAL,	NULL,		"generate empty calendar (ignore date file)",		NULL },
	{ END_GROUP },

	{ F_DATE_FILE,	W_FILE,		"specify alternate date file",				DATEFILE },
	{ END_GROUP },

#ifdef DEFAULT_OUTFILE
	{ F_OUT_FILE,	W_FILE,		"specify alternate output file",			DEFAULT_OUTFILE },
#else
	{ F_OUT_FILE,	W_FILE,		"specify alternate output file",			"stdout" },
#endif
	{ END_GROUP },

	{ F_LANDSCAPE,	NULL,		"generate landscape-style calendar",			NULL },
	{ F_PORTRAIT,	NULL,		"generate portrait-style calendar",			NULL },
#if ROTATE == LANDSCAPE
	{ GROUP_DEFAULT,									"landscape" },
#else	
	{ GROUP_DEFAULT,									"portrait" },
#endif
	{ END_GROUP },

	{ F_PAPERSIZE,	W_PAPERSIZE,	"specify paper size (letter, legal, a4, tabloid)",	NULL },
	{ END_GROUP },

	{ F_HELP,	NULL,		"print this help message",				NULL },
	{ F_USAGE,	NULL,		"print parameter usage message",			NULL },
	{ F_VERSION,	NULL,		"print version information",				NULL },
	{ END_GROUP },

	{ F_MOON_4,	NULL,		"draw a \"moon\" icon at full/new/quarter moons",	NULL },
	{ F_MOON_ALL,	NULL,		"draw a \"moon\" icon every day",			NULL },
#if DRAW_MOONS == NO_MOONS
	{ GROUP_DEFAULT,									"no moons" },
#else
#if DRAW_MOONS == SOME_MOONS
	{ GROUP_DEFAULT,									"full/new/quarter moons" },
#else
	{ GROUP_DEFAULT,									"every day" },
#endif
#endif
	{ END_GROUP },

	{ F_DEFINE,	W_SYMBOL,	"define preprocessor symbol",				NULL },
	{ F_UNDEF,	W_SYMBOL,	"undefine preprocessor symbol",				NULL },
	{ END_GROUP },

	{ F_L_FOOT,	W_STRING,	"specify left foot string",				LFOOT },
	{ F_C_FOOT,	W_STRING,	"specify center foot string",				CFOOT },
	{ F_R_FOOT,	W_STRING,	"specify right foot string",				RFOOT },
	{ END_GROUP },

	{ F_NOTES_HDR,	W_STRING,	"specify header for notes box",				default_notes_hdr },
	{ END_GROUP },

	{ F_FIRST_DAY,	W_DAY,		"specify starting day of week",				days_ml[LANG_DEFAULT][FIRST_DAY] },
	{ END_GROUP },

	{ F_USA_DATES,	NULL,		"parse American dates (\"mm/dd{/yy}\" and \"month dd\")", NULL },
	{ F_EUR_DATES,	NULL,		"parse European dates (\"dd/mm{/yy}\" and \"dd month\")", NULL },
#if DATE_STYLE == USA_DATES
	{ GROUP_DEFAULT,									"American" },
#else
	{ GROUP_DEFAULT,									"European" },
#endif
	{ END_GROUP },

	{ F_X_TRANS,	W_VALUE,	"specify x-axis translation",				Xtval },
	{ F_Y_TRANS,	W_VALUE,	"specify y-axis translation",				Ytval },
	{ F_X_SCALE,	W_VALUE,	"specify x-axis scale factor",				Xsval },
	{ F_Y_SCALE,	W_VALUE,	"specify y-axis scale factor",				Ysval },
	{ END_GROUP },

	{ F_JULIAN,	NULL,		"print Julian day (day of year)",			NULL },
	{ F_JULIAN_ALL,	NULL,		"print Julian day and days left in year",		NULL },
#if JULIAN_DATES == NO_JULIANS
	{ GROUP_DEFAULT,									"neither" },
#else
#if JULIAN_DATES == SOME_JULIANS
	{ GROUP_DEFAULT,									"Julian day" },
#else
	{ GROUP_DEFAULT,									"both" },
#endif
#endif
	{ END_GROUP },

#if DO_WHOLE_YEAR == FALSE
	{ F_WHOLE_YEAR,	NULL,		"print whole year (12 consecutive months) per page",	NULL },
#else
	{ F_WHOLE_YEAR,	NULL,		"print individual months one per page",			NULL },
#endif
	{ END_GROUP },

#if BLANK_BOXES == FALSE
	{ F_BLANK_BOXES, NULL,		"leave unused boxes blank",				NULL },
#else
	{ F_BLANK_BOXES, NULL,		"shade unused boxes",					NULL },
#endif
	{ END_GROUP },

	{ F_NUM_PAGES,	W_N,		"print <n> copies of each output page",			Ncopy },
	{ END_GROUP },

	{ F_SC_NONE,	NULL,		"suppress generation of small calendars",		NULL },
	{ F_SC_FIRST,	NULL,		"print small calendars in first two boxes",		NULL },
	{ F_SC_SPLIT,	NULL,		"print previous month in first box, next in last",	NULL },
#if SMALL_CAL_POS == SC_LAST
	{ GROUP_DEFAULT,									"last two boxes" },
#else
#if SMALL_CAL_POS == SC_FIRST
	{ GROUP_DEFAULT,									"first two boxes" },
#else
#if SMALL_CAL_POS == SC_SPLIT
	{ GROUP_DEFAULT,									"first/last boxes" },
#else
	{ GROUP_DEFAULT,									"suppress small calendars" },
#endif
#endif
#endif
	{ END_GROUP },

	{ F_CALENDAR,	NULL,		"generate input for Un*x calendar(1) utility",		NULL },
	{ F_HTML,	NULL,		"generate calendar as HTML table",			NULL },
	{ END_GROUP },

	{ F_1COLUMN,	NULL,		"print one column per month (HTML)",			NULL },
	{ END_GROUP },

	{ F_TIMEZONE,	W_VALUE,	"specify time zone in hours west of GMT/UTC",		NULL },
	{ GROUP_DEFAULT,									TIMEZONE },
	{ END_GROUP },

	{ F_SETLANG,	W_LANG,		"specify language for day/month names",			NULL },
	{ GROUP_DEFAULT,									lang_id[LANG_DEFAULT] },
	{ END_GROUP },

	{ F_TYPEFACE,	W_TYPEFACE,	"specify font style (Bold | Italic | Roman)",		NULL },
	{ GROUP_DEFAULT,									W_ROMAN },
	{ END_GROUP },

	{ END_LIST }			/* must be last */
};

#else
extern FLAG_MSG flag_msg[];
#endif

/* Numeric parameter descriptions and text */

#ifdef MAIN_MODULE

#ifdef __STDC__
PARAM_MSG param_msg[] = {
	{ W_YY,				"generate calendar for year " W_YY " (19" W_YY " if " W_YY " < 100)" },
	{ W_MM " " W_YY,		"generate calendar for month " W_MM " (Jan = 1), year " W_YY },
	{ W_MM " " W_YY " N",		"generate calendars for N months, starting at " W_MM "/" W_YY },
	{ "(" W_DEFAULT ")",		"generate calendar for current month and/or year" },
	{ "",				"" },
	{ "if " W_WYFLAG " specified:",	"" },
	{ "",				"" },
	{ W_YY,				"generate calendar for year " W_YY " (19" W_YY " if " W_YY " < 100)" },
	{ W_MM " " W_YY,		"generate calendars for 12 months, starting at " W_MM "/" W_YY },
	{ W_MM " " W_YY " N",		"generate calendars for N months, starting at " W_MM "/" W_YY },
	{ "",				"  (N rounded up to next multiple of 12)" },
	{ "(" W_DEFAULT ")",		"generate calendar for current year" },
	{ NULL,				NULL }		/* must be last */
};
#else
PARAM_MSG param_msg[] = {
	{ "YY",			"generate calendar for year YY (19YY if YY < 100)" },
	{ "MM YY",		"generate calendar for month MM (Jan = 1), year YY" },
	{ "MM YY N",		"generate calendars for N months, starting at MM/YY" },
	{ "(default)",		"generate calendar for current month and year" },
	{ "",			"" },
	{ "if -w specified:",	"" },
	{ "",			"" },
	{ "YY",			"generate calendar for year YY (19YY if YY < 100)" },
	{ "MM YY",		"generate calendar for 12 months, starting at MM/YY" },
	{ "MM YY N",		"generate calendars for N months, starting at MM/YY" },
	{ "",			"  (N rounded up to next multiple of 12)" },
	{ "(default)",		"generate calendar for current year" },
	{ NULL,			NULL }		/* must be last */
};
#endif
#else
extern PARAM_MSG param_msg[];
#endif

#define PARAM_MSGS	3	/* number of above to print in command-line syntax message */

#ifdef MAIN_MODULE

/* Date file syntax message - lines are copied intact */

char *date_msg[] = {
	"",
	"Date file syntax:",
	"",
	"1) Basic Syntax:",
	"",
	"The following rules describe the syntax of date file entries:",
	"",
	"  year <year>|all|*",
	"",
	"  opt <options>",
	"",
	"  note{/<number>} <month_spec> <text>",
	"  note{/<number>} <month> <text>",
	"",
	"  if -A flag (American date formats) specified:",
	"    <month_name> <day>{*} {<text>}",
	"    <month><sep><day>{<sep><year>}{*} {<text>}",
	"",
	"  if -E flag (European date formats) specified:",
	"    <day>{<sep>} <month_name>{*} {<text>}",
	"    <day>{<sep>} <month>{<sep>}{*} {<text>}",
	"    <day><sep><month>{<sep><year>}{*} {<text>}",
	"",
	"  <ordinal> <day_name> in <month_spec>{*} {<text>}",
	"  {<ordinal>} <day_name> <prep> <date_spec>",
	"",
	"  <holiday>",
	"",
	"where",
	"",
	"  {x}          means x is optional",
	"",
	"  <date_spec> := any of the above date specs (not year, note, or opt)",
	"  <month_name> := first 3+ characters of name of month, or \"all\"",
	"  <month_spec> := <month_name>, or \"year\"",
	"  <day_name> := first 3+ characters of name of weekday, \"day\",",
	"\t\t\"weekday\", \"workday\", \"holiday\", \"nonweekday\",",
	"\t\t\"nonworkday\", \"nonholiday\", \"new_moon\",",
	"\t\t\"first_quarter\", \"full_moon\", or \"last_quarter\"",
	"  <ordinal> := ordinal number (\"1st\", \"2nd\", etc.), \"first\" .. \"fifth\",",
	"\t\t\"last\", \"even\", \"odd\", or \"all\"",
	"  <prep> := \"before\", \"preceding\", \"after\", \"following\", \"nearest\",",
	"\t\t\"nearest_before\", \"nearest_after\", \"on_or_before\", or",
	"\t\t\"on_or_after\"",
	"  <holiday> := \"Christmas\", \"Thanksgiving\", etc.",
	"",
	"  <sep> := one or more non-numeric, non-space, non-'*' characters",
	"  <month>, <day>, <year> are the numeric forms",
	"",
	"  <options> := any command-line option except -[cefhuvDU]",
	"",
	"Comments start with '#' (unless escaped by '\\') and run through",
	"end-of-line.",
	"",
	"Holidays may be flagged by specifying '*' as the last character of the",
	"date field(s), e.g. \"10/12* Columbus Day\", \"July 4* Independence",
	"Day\", etc.  Any dates flagged as holidays will cause the date",
	"numerics to be printed in the color specified by the '-s' option",
	"(default = gray) and will cause the associated text (on monthly-format",
	"calendars) to be placed adjacent to the numeric date in the day box",
	"rather than below the numeric date (as is done for all non-holiday",
	"events).",
	"",
	"Note that the numeric date formats (mm/dd{/yy}, dd.mm{.yy}) support an",
	"optional year, which will become the subsequent default year.  The",
	"alphabetic date formats (month dd, dd month) do not support a year",
	"field; the \"year yy\" command is provided to reset the default year.",
	"",
	"European dates may also be specified as \"dd. mm.\" and \"dd. month\" if",
	"desired.",
	"",
	"\"year all\" and \"year *\" specify that subsequent date specifications",
	"are to be wildcarded against every applicable year; this is in effect",
	"until the default year is explicitly reset as specified above.",
	"",
	"\"Floating\" days may be specified in the date file as \"first Mon in",
	"Sep\", \"last Mon in May\", \"4th Thu in Nov\", etc.; any word may be used",
	"in place of \"in\".  \"Relative floating\" days (e.g. \"Fri after 4th Thu",
	"in Nov\") are also accepted; they may span month/year bounds.  Pcal",
	"also accepts date specs such as \"all Friday{s} in October\", \"last",
	"Thursday in all\", etc., and produces the expected results; \"each\" and",
	"\"every\" are accepted as synonyms for \"all\".  Negative ordinals are",
	"allowed; \"-2nd\" means \"next to last\".",
	"",
	"\"Floating\" days may also be specified relative to a fixed date:",
	"\"fourth Sunday before 12/25\", etc; note, however, that only positive",
	"ordinals are meaningful in this context.",
	"",
	"The words \"day\", \"weekday\", \"workday\", and \"holiday\" may be used as",
	"wildcards: \"day\" matches any day, \"weekday\" matches any day normally",
	"printed in black, \"workday\" matches any day normally printed in black",
	"and not explicitly flagged as a holiday, and \"holiday\" matches any day",
	"explicitly flagged as a holiday.  \"Nonweekday\", \"nonworkday\", and",
	"\"nonholiday\" are also supported and have the obvious meanings.  Moon",
	"phases may also appear as wildcards; \"nm\" is accepted as a synonym for",
	"\"new_moon\", \"1q\" and \"fq\" for \"first_quarter\", \"fm\" for \"full_moon\",",
	"and \"3q\", \"lq\", and \"third_quarter\" for \"last_quarter\".",
	"",
	"\"Odd\" and \"even\" do not refer to the actual date; instead, \"odd\" means",
	"\"alternate, starting with the first\"; \"even\" means \"alternate,",
	"starting with the second\".  Thus, \"odd Fridays in March\" refers to the",
	"first, third, and (if present) fifth Fridays in March - not to those",
	"Fridays falling on odd dates.",
	"",
	"\"All\" refers to each individual month; \"year\" refers to the year as an",
	"entity.  Thus \"odd Fridays in all\" refers to the first/third/fifth",
	"Friday of each month, while \"odd Fridays in year\" refers to the first",
	"Friday of January and every other Friday thereafter.",
	"",
	"\"Nearest\", \"nearest_before\", and \"nearest_after\" refer to the nearest",
	"weekday or wildcard day with respect to the specified date.",
	"\"Nearest_before\" and \"nearest_after\" allow the user to specify how",
	"pcal is to disambiguate between two dates that are equally near:",
	"e.g., \"nonweekday nearest_before [Wed.] 9/25/96\" refers to Sunday,",
	"9/22 while \"nonweekday nearest_after 9/25/96\" refers to Saturday,",
	"9/28.  (Note that \"nearest_before\" and \"nearest_after\" are equivalent",
	"to \"nearest\" when no such ambiguity exists: e.g., \"nonweekday",
	"nearest_before [Thu.] 9/26/96\" refers to Saturday, 9/28.)",
	"",
	"Additional notes may be propagated to an empty calendar box by the",
	"inclusion of one or more lines of the form \"note{/<number>} <month>",
	"<text>\", where <month> may be numeric or alphabetic; \"note{/<number>}",
	"all <text>\" propagates <text> to each month in the current year.",
	"<number> is an optional positive or negative number specifying the",
	"empty box where the associated text is to be placed: if positive,",
	"Pcal counts forward from the first empty box; if negative, Pcal counts",
	"backward from the last empty box.  Thus, \"note/1 ...\"  places the",
	"associated text in the first empty box, and \"note/-3 ...\"  in the",
	"third-to-last; the default is -1 (last empty box).  (Note that if the",
	"-S option is used, it must be specified either on the command line or",
	"prior to any \"note\" lines in the date file.)",
	"",
	"`troff'-style escape sequences \\fB, \\fI, \\fP, and \\fR may be used to",
	"set the font style to Bold, Italic, the previous font style, or Roman",
	"respectively.  For those more familiar with HTML, <B>, <I>, and </B>",
	"or </I> may be used instead to set the font style to Bold, Italic, or",
	"the previous font style respectively.  The font style is reset to",
	"Roman after each line break.",
	"",
	"",
	"2) Format specifiers:",
	"",
	"Pcal also allows format specifiers in the text (and foot strings - cf.",
	"the -L, -C, -R, and -N options); each will be replaced by its",
	"equivalent string as outlined in the table below.  (Most of these are",
	"derived from the strftime() function; %[lnouwMD0+-] are Pcal-specific.)",
	"",
	"\t%a : abbreviated weekday",
	"\t%A : full weekday",
	"\t%b : abbreviated month name",
	"\t%B : full month name",
	"\t%d : day of month (1-31)",
	"\t%j : day of year (1-366)",
	"\t%l : days left in year (0-365)",
	"\t%m : month (1-12)",
	"\t%u : week number (1-54)",
	"\t%U : week number (0-53)",
	"\t%w : week number (1-54)",
	"\t%W : week number (0-53)",
	"\t%y : year w/o century (00-99)",
	"\t%Y : year w/century",
	"\t%% : '%' character",
	"",
	"\t%o : print number as ordinal",
	"\t%0 : print number with leading zeroes",
	"\t%+ : use following month or year",
	"\t%- : use previous month or year",
	"\t%{+N}[DWMY] : adjust date by +N days/weeks/months/years",
	"\t%{-N}[DWMY] : adjust date by -N days/weeks/months/years",
	"",
	"%u considers the week containing 1/1 as week 1 and the following",
	"logical Sunday (the first day of the week as printed; cf. the -F",
	"flag) as the start of week 2; %U considers the first logical Sunday as",
	"the first day of week 1.  %w and %W behave like %u and %U",
	"respectively, but use the first logical Monday instead.  (Note that %w",
	"has a different meaning to strftime().)",
	"",
	"%o prints a number as an ordinal, with the appropriate suffix (\"st\",",
	"\"nd\", \"rd\", or \"th\" in English) appended; for example, \"%od\" prints",
	"the day of the month as \"1st\", \"2nd\", \"3rd\", etc.",
	"",
	"Unlike strftime(), Pcal's default is to print numbers (except %y)",
	"without leading zeroes.  If leading zeroes are desired, the '0' prefix",
	"may be used; for example, \"%0j\" prints the day of year as 001-365.",
	"",
	"%+ and %- direct Pcal to substitute the following/previous month/year",
	"in the following [bBmyY] specifier; for example, \"%+B\" prints the name",
	"of the next month.",
	"",
	"%{[+-]N}[DWMY] do not print anything, but instead adjust the working",
	"date by +-N days (D), weeks (W), months (M), or years (Y); subsequent",
	"format specifiers use the adjusted date instead of the current date.",
	"For example, \"%+1M %B %Y\" adjusts the date forward by one month and",
	"then prints the resulting month and year (\"January 1991\" in December,",
	"1990); \"%-2W %b %d\" adjusts the date backward by two weeks and prints",
	"the resulting month and day (\"Jul 26\" on August 9).",
	"",
	"Such date adjustments are normally cumulative; for example, \"%+1Y%-1D\"",
	"adjusts the date forward by one year and then backward by one day.  If",
	"%D or %M is specified alone (or if N is zero), Pcal restores the",
	"original date.  (Note that %M has a different meaning to strftime().)",
	"",
	"The \"Notes\" box uses the first of the current month as the default",
	"date.  All foot strings use the first of the current month in single-",
	"month mode and the first of the starting month in whole-year mode.",
	"",
	"",
	"3) Preprocessing:",
	"",
	"Simple cpp-like functionality is provided.  The date file may include",
	"the following commands, which work like their cpp counterparts:",
	"",
	"\tdefine <sym>",
	"\tundef <sym>",
	"",
	"\tif{{n}def} <expr>",
	"\t   ...",
	"\t{ elif <expr>",
	"\t   ... }*",
	"\t{ else",
	"\t   ... }",
	"\tendif",
	"",
	"\tinclude{?} <file>",
	"",
	"Note that these do not start with '#', which is reserved as a comment",
	"character.",
	"",
	"<sym> is a symbol name consisting of a letter followed by zero or more",
	"letters, digits, or underscores ('_').  Symbol names are always",
	"treated in a case-insensitive manner.",
	"",
	"<expr> is an expression consisting of symbol names joined by the",
	"logical operators (in order of precedence, high to low) '!'  (unary",
	"negate), '&' (and), '^' (exclusive or), and '|' (inclusive or).  '&&'",
	"and '||' are accepted as synonyms for '&' and '|' respectively; the",
	"order of evaluation may be altered by the use of parentheses.  A",
	"symbol whose name is currently defined evaluates to TRUE; one whose",
	"name is not currently defined evaluates to FALSE.  Thus \"ifdef A | B |",
	"C\" is TRUE if any of the symbols A, B, and C is currently defined, and",
	"\"ifdef A & B & C\" is TRUE if all of them are.",
	"",
	"\"ifndef A | B | C\" is equivalent to \"ifdef !(A | B | C)\" (or, using",
	"DeMorgan's Law, \"ifdef !A & !B & !C\") - in other words, TRUE if none",
	"of the symbols A, B, and C is currently defined.",
	"",
	"\"if\" is accepted as a synonym for \"ifdef\".",
	"",
	"\"elif A\" is TRUE if A is defined.  Multiple \"elif\" clauses may appear;",
	"at most one \"if{{n}def}\", \"elif\", or \"else\" clause in a given block",
	"will be processed.",
	"",
	"\"define\" alone deletes all the current definitions; \"if{def}\" alone is",
	"always false; \"ifndef\" alone is always true.",
	"",
	"The file name in the \"include\" directive may optionally be surrounded",
	"by \"\" or <>; in any case, path names are taken to be relative to the",
	"location of the file containing the \"include\" directive.  If the",
	"string \"%y\" appears in the file name, it is replaced by the last two",
	"digits of the current year or is expanded to all applicable years if",
	"\"year all\" is in effect.  An alternate form, \"include?\", directs pcal",
	"to silently continue if the specified file cannot be opened. ",
	"",
	"",
	"4) Miscellaneous:",
	"",
	"The \"-w\" flag defines the symbol \"whole_year\", which may be tested in",
	"the same manner as any user-defined symbol.  Similarly, the \"-a XX\"",
	"flag defines the symbol \"lang_XX\", where XX is the two-character",
	"abbreviation for the selected language.",
	NULL
	};
#else
extern char *date_msg[];
#endif

/* format strings for color_msg() - translate as necessary */
#define COLOR_MSG_1	"all days in %s"
#define COLOR_MSG_2	"in %s; others in %s"

/* format string for short usage() message */
#define USAGE_MSG	"\"%s -%c\" prints full description of flags, parameters, and file formats\n"

/* format strings for comment in PostScript output file */
#define CREATED_MSG	"Generated by"
#define TITLE_MSG	"calendar for"
#define DATEFILE_MSG	" (from %s)"

/* font style special characters: \f[BIR] => " .[bir] " (cf. pcalutil.c) */
#define BOLD		'B'		/* in calendar file (\fB[BIR]) */
#define ITALIC		'I'
#define ROMAN		'R'
#define PREVFONT	'P'

#define BOLD_FONT	".b"		/* in PostScript output */
#define ITALIC_FONT	".i"
#define ROMAN_FONT	".r"

#define LINE_SEP	".p"		/* text line separator */

/* strings used in error messages */
#define ENV_VAR		"environment variable "
#define DATE_FILE	"date file "

/* strings for list of predefined holidays (cf. usage()) */
#define PREDEF_MSG_1	\
	"Pcal also supports the following predefined events:"
#define PREDEF_MSG_2	\
	"and will automatically calculate the appropriate date for each.\n" \
	"\t('F13' refers to 'Friday the 13th' events.)"

/* Error and information messages - translate as necessary */

/* program error messages */
#define	E_ALLOC_ERR	"%s: calloc() failed - out of memory\n"
#define	E_FOPEN_ERR	"%s: can't open file %s\n"
#define	E_ILL_LINE	"%s: %s in file %s, line %d\n"
#define	E_ILL_MONTH	"%s: month %d not in range %d .. %d\n"
#define	E_ILL_OPT	"%s: unrecognized flag %s"
#define E_ILL_OPT2	" (%s\"%s\")"
#define	E_ILL_YEAR	"%s: year %d not in range %d .. %d\n"
#define	E_SYMFULL	"%s: symbol table full - can't define %s\n"
#define	E_UNT_IFDEF	"%s: unterminated if{n}def..{else..}endif in file %s\n"
#define E_FLAG_IGNORED	"%s: -%c flag ignored (%s\"%s\")\n"
#define	E_ILL_PAPERSIZE	"%s: unrecognized paper size '%s'\n"

/* preprocessor error strings */
#define E_ELSE_ERR	"unmatched \"else\""
#define E_ELIF_ERR	"unmatched \"elif\""
#define E_END_ERR	"unmatched \"endif\""
#define E_GARBAGE	"extraneous data on \"%s\" line"
#define E_INV_DATE	"invalid date"
#define E_NO_MATCH	"no match for wildcard"
#define E_INV_LINE	"unrecognized line"
#define E_FILE_NESTING	"maximum file nesting level exceeded"
#define E_IF_NESTING	"maximum \"if{n}def\" nesting level exceeded"
#define E_EXPR_SYNTAX	"syntax error in expression"

/* moon file error strings */
#define E_DATE_SEQ	"date or phase out of sequence"
#define E_PREM_EOF	"premature EOF"

/* information message (VMS, Amiga only) */
#define	I_OUT_NAME	"%s: output is in file %s\n"

/* predefined macro names */
#define DEF_WHOLE_YEAR	"whole_year"	/* defined when -w set */
#define DEF_HTML	"html"		/* defined when -H set */
#define DEF_LANG	"lang_"		/* lang_XX defined when -aXX set */

/*
 * Yes, some source code in a header file.  This is a language-dependent
 * routine used by writefil.c to translate the %o (ordinal suffix) format
 * specifier into the appropriate string for printing, and seemed to belong
 * here with the other language dependencies.
 */
#ifdef WRITEFIL			/* include only in writefil.c */
#ifdef PROTOS
static char *ordinal_suffix(int num)
#else
static char *ordinal_suffix(num)
	int num;		/* ordinal number - assumed positive */
#endif
{
	static char buf[10];
	int tens, units;

	/* Select suffix according to rules for target language.  In English,
	 * the rules are as follows:
	 *
	 *	tens digit	units digit	suffix
	 *	  (any)		  0, 4..9	 "th"
	 *	    1		   (any)	 "th"
	 *	  != 1		     1		 "st"
	 *	  != 1		     2		 "nd"
	 *	  != 1		     3		 "rd"
	 */
	tens = (num / 10) % 10;
	units = num % 10;
	strcpy(buf, ord_suffix[(units > 3 || tens == 1) ? 0 : units]);

	return buf;
}
#endif
