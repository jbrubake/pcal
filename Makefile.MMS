# A VMS MMS description file to build the pcal program.
# This has been tested with VAX OpenVMS v5.5-2 & VAX C v3.2-044
# and with Alpha OpenVMS v6.2 & DEC C v5.2-3.
#
# Written by: Rick Dyson (richard-dyson@uiowa.edu) for pcal v4.5
#              6-OCT-1994 
#             27-DEC-1996 updated for v4.6
#
# To use this 'makefile' on a  VAX, type
#           MMS /Macro = (VAX=1)
#
# To use this 'makefile' on an Alpha, just type
#           MMS 

DEFS = /Define = ("TIMEZONE=""6 [Iowa City]""", EPS)

.ifdef VAX
CFLAGS	= $(CFLAGS) $(DEFS) /NoList /Optimize /NoDebug
OPTS	= ,VAXCRTL.OPT /Option
.else
CFLAGS	= $(CFLAGS) $(DEFS) /NoList /Optimize /NoDebug /Standard = VAXC
OPTS	= 
.endif

OBJECTS = exprpars.obj moonphas.obj pcalutil.obj pcalpapr.obj readfile.obj writefil.obj
OBJLIST = exprpars,moonphas,pcalutil,pcalpapr,readfile,writefil

.last
	@- Set Message /Facility /Severity /Identification /Text

pcal :	pcal.exe pcal.hlb
	@ !
	@ Write Sys$Output "pcal is now ready for use.  Use @setup to define variables"

help :	pcal.hlb
	@ !

pcal.exe :	pcal.obj $(OBJECTS) pcalinit.h
	Link pcal,$(OBJLIST)$(OPTS)

pcalinit.exe :	pcalinit.obj
	Link $(LDFLAGS) pcalinit$(OPTS)

pcalinit.h :	pcalinit.exe pcalinit.ps
	MCR Sys$Disk:[]pcalinit pcalinit.ps pcalinit.h header

pcal.hlb :	pcal.hlp
exprpars.obj :	exprpars.c pcaldefs.h
moonphas.obj :	moonphas.c pcaldefs.h pcalglob.h
pcal.obj :	pcal.c pcaldefs.h pcalglob.h pcallang.h protos.h
pcalutil.obj :	pcalutil.c pcaldefs.h pcalglob.h pcallang.h protos.h
pcalpapr.obj :	pcalpapr.c pcalpapr.h pcaldefs.h pcalglob.h 
readfile.obj :	readfile.c pcaldefs.h pcalglob.h pcallang.h protos.h
writefil.obj :	writefil.c pcaldefs.h pcalglob.h pcallang.h pcalinit.h protos.h
pcalinit :	pcalinit.exe

clean :
	@- Set Message /NoFacility /NoSeverity /NoIdentification /NoText
	@- Set Protection = Owner:RWED *.*;-1,*.obj,pcalinit.h,pcalinit.exe
	@- Purge /NoLog /NoConfirm
	@- Delete /NoLog /NoConfirm *.obj;*,pcalinit.h;,pcalinit.exe;

clobber : clean
	@- Purge /NoLog /NoConfirm
	@- Delete /NoLog /NoConfirm pcal.exe;,pcal.hlb;

fresh :	clobber pcal
	@- continue
