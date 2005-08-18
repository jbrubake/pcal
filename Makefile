#
# Makefile for pcal under Un*x, OS/2, or DOS+DJGPP
# 
# Use these commands:
# 
#    Unix/Linux: make
# 
#    OS/2: make OS=OS2
# 
#    DOS+DJGPP: make OS=DJGPP
# 
# 
# v4.9.0: 
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
#    by 'MANDIR' and/or 'CATDIR' does not exist.  This prevents an error 
#    during the 'make install' step when building in certain environments 
#    (e.g. DOS+DJGPP).
# 
# v4.8.0: 
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
# v4.7: add HTML-specific command-line definitions (cf. pcaldefs.h)
#       delete moon96; add moon98
#       add Makefile.os2 to TARSRC definition
#       parameterize executable names; define pcalinit-specific compiler
#       name (to facilitate cross-builds)
#
# v4.6: "make compress" creates compressed tar file; "make uuencode" creates
#       uuencode-d version thereof
#
# v4.5: "make clean" leaves pcal intact but removes other by-products;
#       "make clobber" blows everything away;
#       "make fresh" rebuilds from scratch
#

VERSION  = 4.9.0

# 
# Depending on whether we're compiling for Unix, OS/2, or DOS/DJGPP, use
# appropriate values for the OS name, the 'build environment' flag, the names
# of the executable files, the compiler(s), and the 'pack' value.
# 
# The 'PCALINIT_CC' value should always specify the compiler native to the
# host on which we're building 'pcal'.  The 'CC' value should specify the same
# (native) compiler, unless the target machine requires a cross-compiler.
# 
# The 'PACK' value is used for packing the 'man' page.  Note that setting
# 'PACK' to ":" will cause no packing to be done; otherwise, choose
# "compress", "pack", or "gzip" as your system requires.
# 
ifeq ($(OS),OS2)   # OS/2
	OS_NAME = "OS/2"
	D_BUILD_ENV	= -DBUILD_ENV_OS2
	PCAL		= pcal.exe
	PCALINIT	= pcalinit.exe
	CC		= gcc
	PCALINIT_CC	= gcc
	PACK		= :
else
	ifeq ($(OS),DJGPP)   # DOS/DJGPP
		OS_NAME = "DOS/DJGPP"
		D_BUILD_ENV	= -DBUILD_ENV_DJGPP
		PCAL		= pcal.exe
		PCALINIT	= pcalinit.exe
		CC		= gcc
		PCALINIT_CC	= gcc
		PACK =		:
	else   # Unix
		OS_NAME = "Unix"
		D_BUILD_ENV	= -DBUILD_ENV_UNIX
		PCAL		= pcal
		PCALINIT	= pcalinit
		CC		= /usr/bin/gcc
		PCALINIT_CC	= /usr/bin/gcc
		PACK		= compress
		# PACK		= pack
		# PACK		= gzip
	endif
endif

# 
# Define various directories for the following items:
# 
#    - source code
#    - object code (and compile-time, auto-generated C header files)
#    - executable code
#    - documentation
#    - installed 'pcal' executable
#    - 'man' pages
#    - 'cat' pages
# 
SRCDIR	= src
OBJDIR	= obj
EXECDIR	= exec
DOCDIR	= doc
BINDIR	= /usr/local/bin
MANDIR	= /usr/man/man1
CATDIR	= /usr/man/cat1

# 
# Compiling for DOS/DJGPP requires different directories for the installed
# executable and the 'man'/'cat' pages.  Unix and OS/2 use the same (default)
# values shown above.
# 
ifeq ($(OS),DJGPP)
	BINDIR = $(DJDIR)/bin
	MANDIR = $(DJDIR)/man/man1
	CATDIR = $(DJDIR)/man/cat1
endif

OBJECTS = $(OBJDIR)/pcal.o $(OBJDIR)/exprpars.o $(OBJDIR)/moonphas.o \
		$(OBJDIR)/pcalutil.o $(OBJDIR)/readfile.o \
		$(OBJDIR)/writefil.o $(OBJDIR)/pcalpapr.o

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
# Note that this is not normally needed, since 'pcal' will automatically
# assign the proper character encoding (if any) depending on the language
# selected.
# 
# D_MAPFONTS = -DMAPFONTS=ENC_KOI8_R

# redefine title, date, and notes font/pointsize (-t, -d, -n)
# D_TITLEFONT = '-DTITLEFONT="Helvetica-Bold/48"'
# D_DATEFONT  = '-DDATEFONT="Helvetica-Bold/28"'
# D_NOTESFONT = '-DNOTESFONT="Helvetica/8"'

# redefine shading (dates/fillboxes)
# D_SHADING = '-DSHADING="0.6/0.99"'

# specify American (USA_DATES) or European (EUR_DATES) date parsing (-A, -E)
# D_DATE_STYLE = -DDATE_STYLE=USA_DATES

# specify first (leftmost) weekday on calendar (-F)
# D_FIRST_DAY  = -DFIRST_DAY=MON

# specify default time zone for moon phases: "0" = GMT; "5" = Boston (-z)
# D_TIMEZONE   = '-DTIMEZONE="5 [Boston]"'

# 
# This flag controls the generation of EPS-like PostScript so that the output
# contains enhanced comments ('%%Page:', etc.) that are necessary for programs
# like 'psnup' to do useful things with the generated calendars.
# 
D_EPS = -DEPS

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
	$(D_EPS) $(D_LANGUAGE) $(D_BGCOLOR) $(D_BACKGROUND) \
	$(D_TEXT) $(D_LINK) $(D_ALINK) $(D_VLINK) \
	$(D_HOLIDAY_PRE) $(D_HOLIDAY_POST) $(D_BLANK_STYLE) \
	$(D_SEARCH_PCAL_DIR) $(D_BUILD_ENV)

# 
# Depending on whether we're compiling for Unix, OS/2, or DOS/DJGPP, use
# appropriate values as flags for the C compiler.
# 
#    '-O2' enables a 2nd-level code optimization
#    '-Wall' enables many compile-time warning messages
#    '-W' enables some additional compile-time warning messages
#    '-DDOS' is used in DOS+DJGPP to get DOS-like settings
# 
ifeq ($(OS),OS2)   # OS/2
	CFLAGS = -O2 -Wall
else
	ifeq ($(OS),DJGPP)   # DOS/DJGPP
		CFLAGS = -DDOS
	else   # Unix
		CFLAGS = -O2 -Wall -W
	endif
endif

$(EXECDIR)/$(PCAL):	$(EXECDIR)/$(PCALINIT) $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECDIR)/$(PCAL) $(OBJECTS) -lm
	@ echo "Build of $(PCAL) for" $(OS_NAME) "completed."

$(OBJDIR)/exprpars.o:	$(SRCDIR)/exprpars.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/exprpars.c

$(OBJDIR)/moonphas.o:	$(SRCDIR)/moonphas.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/moonphas.c

$(OBJDIR)/pcal.o:	$(SRCDIR)/pcal.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/pcallang.h \
			$(SRCDIR)/pcalpapr.h $(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcal.c

$(OBJDIR)/pcalpapr.o:	$(SRCDIR)/pcalpapr.c $(SRCDIR)/pcalpapr.h \
			$(SRCDIR)/pcaldefs.h $(SRCDIR)/pcalglob.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcalpapr.c

$(OBJDIR)/pcalutil.o:	$(SRCDIR)/pcalutil.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/pcalutil.c

$(OBJDIR)/readfile.o:	$(SRCDIR)/readfile.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h
	$(CC) $(CFLAGS) $(COPTS) -o $@ -c $(SRCDIR)/readfile.c

# 
# Note: Unlike the other C-source-based object file targets above, this target
# for 'writefil.o' adds the '-I$(OBJDIR)' so that the 'pcalinit.h' file (which
# is generated automatically at compile time from the 'pcalinit.ps' source,
# using the 'pcalinit' executable) will be properly found when 'writefil.c'
# code does '#include pcalinit.h'.
# 
$(OBJDIR)/writefil.o:	$(SRCDIR)/writefil.c $(SRCDIR)/pcaldefs.h \
			$(SRCDIR)/pcalglob.h $(SRCDIR)/pcallang.h \
			$(SRCDIR)/protos.h $(OBJDIR)/pcalinit.h \
			$(SRCDIR)/pcalpapr.h 
	$(CC) $(CFLAGS) $(COPTS) -I$(OBJDIR) -o $@ -c $(SRCDIR)/writefil.c

# 
# This target builds the 'pcalinit' executable utility, which is used to
# convert the 'pcalinit.ps' file into a C header file named 'pcalinit.h'.
# 
$(EXECDIR)/$(PCALINIT): $(SRCDIR)/pcalinit.c
	$(PCALINIT_CC) $(CFLAGS) $(LDFLAGS) $(COPTS) \
		-o $(EXECDIR)/$(PCALINIT) $(SRCDIR)/pcalinit.c

# 
# This target runs the 'pcalinit' executable utility to convert the
# 'pcalinit.ps' file into a C header file named 'pcalinit.h'.
# 
$(OBJDIR)/pcalinit.h: $(EXECDIR)/$(PCALINIT) $(SRCDIR)/pcalinit.ps
	$(EXECDIR)/$(PCALINIT) $(SRCDIR)/pcalinit.ps $(OBJDIR)/pcalinit.h header

# 
# This target will delete everything except the 'pcal' executable.
# 
clean:
	rm -f $(OBJECTS) $(EXECDIR)/$(PCALINIT) $(OBJDIR)/pcalinit.h \
		$(DOCDIR)/pcal.cat $(DOCDIR)/pcal-help.ps \
		$(DOCDIR)/pcal-help.html $(DOCDIR)/pcal-help.txt

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
	nroff -man $(DOCDIR)/pcal.man > $(DOCDIR)/pcal.cat
	groff -man -Tps $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.ps
	groff -man -Thtml $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.html
	groff -man -Tascii $(DOCDIR)/pcal.man >$(DOCDIR)/pcal-help.txt

install:	$(EXECDIR)/$(PCAL) man
	cp $(EXECDIR)/$(PCAL) $(BINDIR)
	if [ -d $(MANDIR) ]; then \
		cp $(DOCDIR)/pcal.man $(MANDIR)/pcal.1
		$(PACK) $(MANDIR)/pcal.1
	fi
	if [ -d $(CATDIR) ]; then \
		cp $(DOCDIR)/pcal.cat $(CATDIR)/pcal.1; \
		$(PACK) $(CATDIR)/pcal.1;               \
	fi
