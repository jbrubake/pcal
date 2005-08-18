/*
 * protos.h - ANSI-style function prototypes for Pcal sources
 *
 * Revision history:
 *
 *	4.9.0
 *		B.Marr		2005-08-02
 *		
 *		Add prototype for new function 'delete_day_info()' and rename
 *		'enter_date()' to 'process_date()', all based on a patch from
 *		Bill Bogstad.
 *
 *	4.8.0
 *		B.Marr		2004-12-03
 *		
 *		Add prototype for new function 'recalc_paper_parameters()'.
 *		
 *		B.Marr		2004-11-13
 *		
 *		Add prototypes for new functions 'do_define_sym_val()',
 *		'find_sym_val()', 'find_fri13th()', and
 *		'recalc_paper_parameters()'.  Rename 'find_sym()' to
 *		'find_sym_name()' to avoid search collisions with new
 *		function.

 *	4.7	AWR	06/19/1998	add multiple_months_html() and
 *					single_month_html() (cf. writefil.c)
 *
 *			02/24/1998	add prototypes for exit() and free()
 *					if not already included via <stdlib.h>
 *
 *			07/27/1997	add print_html(), write_htmlfile() (cf.
 *					writefil.c) and set_fontstyle() (cf.
 *					pcalutil.c)
 *
 *	4.6	AWR	05/14/1997	add century() (cf. pcalutil.c)
 *
 *			10/31/1995	add gen_lang_sym() (cf. pcal.c)
 *
 *			10/03/1995	revise write_psfile, write_calfile()
 *					(cf. writefil.c)
 *
 *			09/21/1995	add enter_date(), enter_date() (cf.
 *					readfile.c)
 *
 *	4.5	AWR	11/24/1993	replace find_holidays() with
 *					print_colors() (cf. writefil.c)
 *
 *			11/16/1993	added new routines define_font(),
 *					define_shading() (latter replaces
 *					old gen_shading()), and set_rgb()
 *
 *			11/16/1992	eliminated "noprotos.h" by means of
 *					PROTO() macro (q.v.)
 *
 *	4.4	AWR	01/15/1992	added even more routines
 *
 *	4.3	AWR	12/06/1991	added more routines
 *
 *	4.2	AWR	10/03/1991	added various routines newly
 *					incorporated into writefil.c
 *					and pcalutil.c
 *
 *	4.0	AWR	03/01/1991	use <stdlib.h> where possible
 *
 *			02/19/1991	adapted from Bill Hogsett's source
 *
 */

/* macro to translate parenthesized argument list into ANSI- or K&R-style
 * function declaration, depending on definition of PROTOS - obviates need
 * for separate "noprotos.h" (now gone) containing K&R declarations
 */
#ifdef PROTOS
#define PROTO(args)	args		/* ANSI */
#else
#define PROTO(args)	()		/* K&R  */
#endif

/*
 * Prototypes for functions defined in exprpars.c:
 */
int parse_expr PROTO((char *pbuf));


/*
 * Prototypes for functions defined in moonphas.c:
 */
double	calc_phase PROTO((int month, int day, int year));
double	find_phase PROTO((int month, int day, int year, int *pquarter));
char	*find_moonfile PROTO((int year));
int	read_moonfile PROTO((int year));


/*
 * Prototypes for functions defined in pcal.c:
 */
FILE	*alt_fopen PROTO((char *fullpath, char *name, char *pathlist[],
			  char *access));
int	check_numargs PROTO((void));
char	*color_msg PROTO((void));
char	*gen_lang_sym PROTO((int));
int	get_args PROTO((char **argv, int curr_pass, char *where,
			int get_numargs));
FLAG_USAGE *get_flag PROTO((char flag));
void	init_misc PROTO((void));
int	main PROTO((int argc, char **argv));
void	set_color PROTO((char *day, int col));
void	set_debug_flag PROTO((char *flag));
void	usage PROTO((FILE *fp, int fullmsg));


/*
 * Prototypes for functions defined in pcalutil.c:
 */
char	*alloc PROTO((int size));
int	calc_day PROTO((int ord, int wkd, int mm));
int	calc_weekday PROTO((int mm, int dd, int yy));
int	calc_year_day PROTO((int ord, int wkd, DATE *pdate));
int	century PROTO((void));
int	ci_strcmp PROTO((register char *s1, register char *s2));
int	ci_strncmp PROTO((register char *s1, register char *s2, int n));
void	copy_text PROTO((char *pbuf, char **ptext));
void	cvt_escape PROTO((char *obuf, char *ibuf));
void	define_font PROTO((char *orig_font, char *new_font, char *dflt_font));
void	define_shading PROTO((char *orig_shading, char *new_shading,
			      char *dflt_shading));
char	*find_executable PROTO((char *prog));
int	getline PROTO((FILE *fp, char *buf, int *pline));
int	is_valid PROTO((register int m, register int d, register int y));
int	loadwords PROTO((char **words, char *buf));
char	*mk_filespec PROTO((char *filespec, char *path, char *name));
char	*mk_path PROTO((char *path, char *filespec));
void	normalize PROTO((DATE *pd));
int	note_box PROTO((int mm, int dd, int yy));
int	note_day PROTO((int mm, int n, int yy));
char	*set_fontstyle PROTO((char *p, char *esc));
int	split_date PROTO((char *pstr, int *pn1, int *pn2, int *pn3));
char	*trnlog PROTO((char *logname));


/*
 * Prototypes for functions defined in pcalpapr.c:
 */
void recalc_paper_parameters PROTO((int paper_size_idx));

/*
 * Prototypes for functions defined in readfile.c:
 */
void	cleanup PROTO((void));
void	clear_syms PROTO((void));
int	date_type PROTO((char *cp, int *pn, int *pv));
int	do_define PROTO((char *sym));
int	do_define_sym_val PROTO((char *sym,char *val));
int	do_ifdef PROTO((char *expr));
int	do_ifndef PROTO((char *expr));
int	do_include PROTO((char *path, char *name, int noerr));
int	do_undef PROTO((char *sym));
int	enter_day_info PROTO((int m, int d, int y, int text_type,
			      char **pword));
int	delete_day_info PROTO((int m, int d, int y, int text_type, 
			       char **pword));
int	process_date PROTO((char **pword, int *ptext_type, char ***pptext));
int	enter_note PROTO((int mm, char **pword, int n));
int	find_easter PROTO((DATE *pdate));
#ifdef FRIDAY_13th
int	find_fri13th PROTO((DATE *pdate));
#endif
#ifndef NO_ORTHODOX
int	odox_easter_from_april1 PROTO((int year));
int	find_odox_easter PROTO((DATE *pdate));
int	find_odox_stgeorge PROTO((DATE *pdate));
int	find_odox_marcus PROTO((DATE *pdate));
#endif
int	find_sym_name PROTO((char *sym));
char	*find_sym_val PROTO((char *sym));
year_info *find_year PROTO((int year, int insert));
int	get_keywd PROTO((char *cp));
int	get_month PROTO((char *cp, int numeric_ok, int year_ok));
int	get_ordinal PROTO((char *cp, int *pval));
int	get_phase PROTO((char *cp));
int	get_prep PROTO((char *cp));
int	get_token PROTO((char *token));
int	get_weekday PROTO((char *cp, int wild_ok));
int	is_anyday PROTO((int mm, int dd, int yy));
int	is_firstq PROTO((int mm, int dd, int yy));
int	is_fullmoon PROTO((int mm, int dd, int yy));
int	is_holiday PROTO((int mm, int dd, int yy));
int	is_lastq PROTO((int mm, int dd, int yy));
int	is_newmoon PROTO((int mm, int dd, int yy));
int	is_weekday PROTO((int mm, int dd, int yy));
int	is_workday PROTO((int mm, int dd, int yy));
int	not_holiday PROTO((int mm, int dd, int yy));
int	not_weekday PROTO((int mm, int dd, int yy));
int	not_workday PROTO((int mm, int dd, int yy));
int	parse PROTO((char **pword, char *filename));
int	parse_date PROTO((char **pword, int *ptype, char ***pptext));
int	parse_ord PROTO((int ord, int val, char **pword));
int	parse_rel PROTO((int val, int wkd, char **pword, int *ptype,
			 char ***pptext));
void	read_datefile PROTO((FILE *fp, char *filename));


/*
 * Prototypes for functions defined in writefil.c:
 */
void	def_footstring PROTO((char *p, char *str));
char	*expand_fmt PROTO((char *buf, char *p));
void	find_daytext PROTO((int month, int year, int is_holiday));
void	find_noteboxes PROTO((int month, int year));
void	multiple_months_html PROTO((int first_month, int first_year,
				    int ncols));
void	print_colors PROTO((int month, int year));
void	print_html PROTO((char *s));
void	print_dates PROTO((int month, int year));
void	print_julian_info PROTO((int month, int year));
void	print_month PROTO((int month, int year));
void	print_moon_info PROTO((int month, int year));
void	print_pstext PROTO((char *p));
void	print_text PROTO((char *p));
char	*print_word PROTO((char *p));
int	select_color PROTO((void));
char	*set_rgb PROTO((char *s));
void	single_month_html PROTO((int this_month, int this_year));
void	single_month_one_column_html PROTO((int this_month, int this_year));
void	write_calfile PROTO((void));
void	write_htmlfile PROTO((void));
void	write_psfile PROTO((void));


/*
 * Prototypes for miscellaneous library routines (if not already included
 * via <stdlib.h> - cf. pcaldefs.h):
 */
#ifndef STDLIB
extern double	atof PROTO((char *));
extern int	atoi PROTO((char *));
extern char	*calloc PROTO((unsigned int, unsigned int));
extern int	exit PROTO((int));
extern int	free PROTO((void *));
extern char	*getenv PROTO((char *));
#endif
