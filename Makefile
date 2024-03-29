#
# Makefile for 'pcal' under Unix/Linux, DOS+DJGPP, and DOS/Windows+Cygwin
# 
# Use these commands:
# 
#    Unix/Linux: make
# 
#    DOS+DJGPP: make OS=DJGPP
# 
# 
# v4.11.0: Bill Marr
#    
#    Provide support for a user-specified destination directory ('$DESTDIR') 
#    on the installation step.
#    
#       For example: make DESTDIR=$HOME/test install
#    
#    Remove all remaining references (environment variables, etc) to the
#    unneeded 'pcalinit' items, which were accidentally overlooked in the 
#    last release.
#    
# v4.10.0: Bill Marr
#    
#    Accommodate the fact that the use of 'pcalinit.{c,ps,h}' is no longer
#    needed.  PostScript output is now directly generated by C code, without
#    the use of a PostScript template which was converted to a C header file.
#    
#    Eliminate other now-unused files ('pcalglob.h', 'pcalpapr.{c,h}') and add
#    new files ('encvec.c', 'pcallang.c').
#    
#    Drop support for obsolete platforms (Amiga, VMS, OS/2).
#    
#    Rename 'EPS' definition to 'EPS_DSC' (in order to more clearly show its
#    purpose and to avoid confusion with the embedded EPS image capability for
#    monthly calendars).  Enhance comments about its use.
#    
#    Expand the explanation and provide more examples of the use of the 
#    timezone setting.
#    
#    Remove the now-unneeded 'VERSION = x.y' declaration.
# 
# v4.9.1: Bill Marr
#    
#    Fix syntax which causes error when using 'make install' command.
#    
# v4.9.0: Bill Marr
#    
#    Add comments for new 'MAPFONTS' options to control PostScript character
#    encodings to support multiple languages.
#    
#    Remove obsolete comment about directory usage.  Fix minor comment typo.
#    
#    Remove reference to now-obsolete 'Esperanto' character encoding.
#    
#    Enable the '-DSEARCH_PCAL_DIR=0' line by default now, so that the 'pcal'
#    configuration file (a.k.a. the 'date' file, often named 'calendar') will
#    _not_ be looked for in the same directory as the 'pcal' executable file.
#    This prevents erroneous use of a common Unix executable named 'calendar'
#    as a 'pcal' configuration file.  This change was done to minimize the
#    deltas applied by the Debian distribution.
#    
#    Bypass some processing of the documentation if the directory specified
#    by 'MANDIR' does not exist.  This prevents an error 
#    during the 'make install' step when building in certain environments 
#    (e.g. DOS+DJGPP).
# 
# v4.8.0: Bill Marr
#    
#    Enhance comments; reverse sense of 'SEARCH_PCAL_DIR' flag so that
#    enabling the commented-out line has the desired effect; remove unused
#    'make' targets ('compress' and 'uuencode') and now-obsolete list of files
#    for 'TARSRC'; remove refs to obsolete source files; remove now-unneeded
#    specification of paper size; provide for new directory locations for
#    source code, object code, and executable code; merge OS/2 'Makefile.os2'
#    file into this file; add support for DOS+DJGPP building based on a patch
#    from Thiago F.G. Albuquerque; provide a new flag 'BUILD_ENV_xxx' so that
#    we can distinquish the build environment among Unix, OS/2, and DOS+DJGPP
#    at compile time; fixed up the out-of-date target dependency lists
#         
# v4.7: Andrew Rogers
# 
#       add HTML-specific command-line definitions (cf. pcaldefs.h)
#       delete moon96; add moon98
#       add Makefile.os2 to TARSRC definition
#       parameterize executable names; define pcalinit-specific compiler
#       name (to facilitate cross-builds)
#
# v4.6: Andrew Rogers
# 
#       "make compress" creates compressed tar file; "make uuencode" creates
#       uuencode-d version thereof
#
# v4.5: Andrew Rogers
# 
#       "make clean" leaves pcal intact but removes other by-products;
#       "make clobber" blows everything away;
#       "make fresh" rebuilds from scratch
#

# -----------------------------------------------------------------------------
# 
# Depending on whether we're compiling for Unix/Linux or DOS+DJGPP, use
# appropriate values for the OS name, the 'build environment' flag, the names
# of the executable files, the compiler(s), and the 'PACK' value.
# 
# The 'PACK' value is used for packing the 'man' page.  Note that setting
# 'PACK' to ":" will cause no packing to be done; otherwise, choose
# "compress", "pack", or "gzip" as your system requires.
# 
ifeq ($(OS),DJGPP)   # DOS+DJGPP
	OS_NAME = "DOS+DJGPP"
	D_BUILD_ENV	= -DBUILD_ENV_DJGPP
	PCAL		= pcal.exe
	CC		= gcc
	PACK =		:
else   # Unix
	OS_NAME = "Unix"
	D_BUILD_ENV	= -DBUILD_ENV_UNIX
	PCAL		= pcal
	CC		= /usr/bin/gcc
	PACK		= gzip
	PREFIX          = /usr/local
endif

# 
# Define various directories for the following items:
# 
#    - source code
#    - object code (and compile-time, auto-generated C header files)
#    - installed 'pcal' executable
#    - documentation
#    - 'man' pages
# 
SRCDIR	= src
OBJDIR	= obj
EXECDIR	= exec
DOCDIR	= doc

# 
# Compiling for DOS+DJGPP requires different directories for the installed
# executable and the 'man' pages.  Unix uses the values shown below.
# 
ifeq ($(OS),DJGPP)   # DOS+DJGPP
	BINDIR = $(DJDIR)/bin
	MANDIR = $(DJDIR)/man/man1
else   # Unix
	BINDIR = $(PREFIX)/bin
	MANDIR = $(PREFIX)/share/man/man1
endif

OBJECTS = $(OBJDIR)/pcal.o \
		$(OBJDIR)/encvec.o $(OBJDIR)/exprpars.o \
		$(OBJDIR)/moonphas.o $(OBJDIR)/pcalutil.o \
		$(OBJDIR)/pcallang.o \
		$(OBJDIR)/readfile.o $(OBJDIR)/writefil.o

# ------------------------------------------------------------------
# 
# Site-specific defaults which may be overridden here (cf. pcallang.h);
# uncomment the examples below and/or change them to your liking
# 

# 
# Allow the ability to specify an alternate 8-bit character mapping by
# defining 'MAPFONTS' to one of the following:
#    
#    ENC_LATIN_1   (ISO 8859-1)
#    ENC_LATIN_2   (ISO 8859-2)
#    ENC_LATIN_3   (ISO 8859-3)
#    ENC_LATIN_4   (ISO 8859-4)
#    ENC_CYRILLIC  (ISO 8859-5)
#    ENC_ARABIC    (ISO 8859-6) (*** unsupported ***)
#    ENC_GREEK     (ISO 8859-7)
#    ENC_HEBREW    (ISO 8859-8)  (*** unsupported ***)
#    ENC_LATIN_5   (ISO 8859-9)
#    ENC_LATIN_6   (ISO 8859-10)
#    ENC_THAI      (ISO 8859-11)
#    ENC_LATIN_7   (ISO 8859-13)
#    ENC_LATIN_8   (ISO 8859-14)
#    ENC_LATIN_9   (ISO 8859-15)
#    ENC_LATIN_10  (ISO 8859-16)  (*** unsupported ***)
#    ENC_KOI8_R    (Russian)
#    ENC_KOI8_U    (Ukrainian)
#    ENC_ROMAN8    (Roman8)
#    
# This is equivalent to the use of the '-r' option.
# 
# Note that this is not normally needed, since 'pcal' will automatically
# assign the proper character encoding (if any) depending on the language
# selected.
# 
# D_MAPFONTS = -DMAPFONTS=ENC_KOI8_R

# redefine title, date, and notes font/pointsize ('-t', '-d', '-n')
# D_TITLEFONT = '-DTITLEFONT="Helvetica-Bold/48"'
# D_DATEFONT  = '-DDATEFONT="Helvetica-Bold/28"'
# D_NOTESFONT = '-DNOTESFONT="Helvetica/8"'

# redefine shading (dates/fillboxes) ('-s')
# D_SHADING = '-DSHADING="0.6/0.99"'

# specify American (USA_DATES) or European (EUR_DATES) date parsing ('-A', '-E')
# D_DATE_STYLE = -DDATE_STYLE=USA_DATES

# specify first (leftmost) weekday on calendar ('-F')
# D_FIRST_DAY  = -DFIRST_DAY=MON

# 
# Specify the timezone ('-z') for determining the correct day when
# calculating the moon phases:
#    
#    "0"    = UTC/GMT (default)
#    "5"    = New York EST
#    "-3"   = Moscow
#    "-5.5" = India
# 
# D_TIMEZONE = '-DTIMEZONE="5 [New York EST]"'
# D_TIMEZONE = '-DTIMEZONE="4 [New York EDT]"'
# D_TIMEZONE = '-DTIMEZONE="8 [Los Angeles PST]"'
# D_TIMEZONE = '-DTIMEZONE="7 [Los Angeles PDT]"'
# D_TIMEZONE = '-DTIMEZONE="-3 [Moscow]"'
# D_TIMEZONE = '-DTIMEZONE="-5.5 [India]"'

# 
# This flag controls the generation of EPS-like PostScript Document
# Structuring Conventions (DSC) so that the output contains enhanced comments
# ('%%Page:', etc.) that are necessary for programs like 'psnup' to do useful
# things with the generated calendars.  These comments also allow PostScript
# previewer applications to page up to the previous page.
# 
D_EPS_DSC = -DEPS_DSC

# specify default language
# D_LANGUAGE = -DLANG_DEFAULT=LANG_ENGLISH

# customize HTML output
# D_BGCOLOR = '-DBGCOLOR="ffffff"'
# D_BACKGROUND = -DBACKGROUND=NULL
# D_TEXT = -DTEXT=NULL
# D_LINK = -DLINK=NULL
# D_ALINK = -DALINK=NULL
# D_VLINK = -DVLINK=NULL
# D_HOLIDAY_PRE = '-DHOLIDAY_PRE="<font color=\"ff0000\"><b>"'
# D_HOLIDAY_POST = '-DHOLIDAY_POST="</b></font>"'
# D_BLANK_STYLE = '-DDIVIDE_BLANK_SPACE=0'

# 
# This flag controls whether 'pcal' will search for the calendar
# control/options file ('calendar') in the directory where the 'pcal'
# executable code resides.  Disabling this flag (by enabling the line below)
# prevents the search, thereby avoiding a potential conflict with any program
# named 'calendar' which has also been installed there.  Note that the
# 'pcaldefs.h' source code file enables this search by default, unless it has
# been overridden here.
# 
D_SEARCH_PCAL_DIR = '-DSEARCH_PCAL_DIR=0'

# ------------------------------------------------------------------

COPTS = $(D_MAPFONTS) $(D_TITLEFONT) $(D_DATEFONT) $(D_NOTESFONT) \
	$(D_SHADING) $(D_DATE_STYLE) $(D_FIRST_DAY) $(D_TIMEZONE) \
	$(D_EPS_DSC) $(D_LANGUAGE) $(D_BGCOLOR) $(D_BACKGROUND) \
	$(D_TEXT) $(D_LINK) $(D_ALINK) $(D_VLINK) \
	$(D_HOLIDAY_PRE) $(D_HOLIDAY_POST) $(D_BLANK_STYLE) \
	$(D_SEARCH_PCAL_DIR) $(D_BUILD_ENV)

# 
# Depending on whether we're compiling for Unix/Linux or DOS+DJGPP, use
# appropriate values as flags for the C compiler.
# 
#    '-O2' enables a 2nd-level code optimization
#    '-Wall' enables many compile-time warning messages
#    '-W' enables some additional compile-time warning messages
# 
ifeq ($(OS),DJGPP)   # DOS+DJGPP
	CFLAGS = -Wall -W
else   # Unix
	CFLAGS = -O2 -Wall -W
endif

$(EXECDIR)/$(PCAL):	$(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECDIR)/$(PCAL) $(OBJECTS) -lm
	@ echo Build of $(PCAL) for $(OS_NAME) completed.

$(OBJDIR)/encvec.o:	$(SRCDIR)/encvec.c
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/encvec.c

$(OBJDIR)/exprpars.o:	$(SRCDIR)/exprpars.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/exprpars.c

$(OBJDIR)/moonphas.o:	$(SRCDIR)/moonphas.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/moonphas.c

$(OBJDIR)/pcal.o:	$(SRCDIR)/pcal.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcal.c

$(OBJDIR)/pcallang.o:	$(SRCDIR)/pcallang.c $(SRCDIR)/pcallang.h \
			$(SRCDIR)/pcaldefs.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcallang.c

$(OBJDIR)/pcalutil.o:	$(SRCDIR)/pcalutil.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcalutil.c

$(OBJDIR)/readfile.o:	$(SRCDIR)/readfile.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/readfile.c

$(OBJDIR)/writefil.o:	$(SRCDIR)/writefil.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h 
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/writefil.c

# 
# This target will delete everything except the 'pcal' executable.
# 
clean:
	rm -f $(OBJECTS) \
		$(DOCDIR)/pcal-help.ps \
		$(DOCDIR)/pcal-help.html \
		$(DOCDIR)/pcal-help.txt

# 
# This target will delete everything, including the 'pcal' executable.
# 
clobber: clean
	rm -f $(EXECDIR)/$(PCAL)

# 
# This target will delete everything and rebuild 'pcal' from scratch.
# 
fresh:	clobber $(PCAL)

man:	$(DOCDIR)/pcal.man
	groff -man -Tps $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.ps
	groff -man -Thtml $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.html
	groff -man -Tascii $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.txt

install:	$(EXECDIR)/$(PCAL) man
	mkdir -p $(DESTDIR)/$(BINDIR)
	mkdir -p $(DESTDIR)/$(MANDIR)
	cp $(EXECDIR)/$(PCAL) $(DESTDIR)/$(BINDIR)
	cp $(DOCDIR)/pcal.man $(DESTDIR)/$(MANDIR)/pcal.1
	$(PACK) $(DESTDIR)/$(MANDIR)/pcal.1
