$! A DCL command procedure created by MMS from Makefile.MMS
$! and hand edited for VAX vs. Alpha
$!
$ If F$GetSYI("Arch_Name") .eqs. "Alpha"
$   Then
$       Alpha = "/Standard = VAXC"
$       Options = ""
$   Else
$       Alpha = ""
$       Options = ",VAXCRTL.OPT /Option"
$ EndIf
$
$ CC 'Alpha' /Object=PCAL.OBJ     /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug PCAL.C
$ CC 'Alpha' /Object=EXPRPARS.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug EXPRPARS.C
$ CC 'Alpha' /Object=MOONPHAS.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug MOONPHAS.C
$ CC 'Alpha' /Object=PCALUTIL.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug PCALUTIL.C
$ CC 'Alpha' /Object=READFILE.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug READFILE.C
$ CC 'Alpha' /Object=PCALINIT.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug PCALINIT.C
$ CC 'Alpha' /Object=PCALPAPR.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug PCALPAPR.C
$
$ Link  pcalinit'Options'
$ MCR Sys$Disk:[]pcalinit pcalinit.ps pcalinit.h header
$
$ CC 'Alpha' /Object=WRITEFIL.OBJ /Define = ("TIMEZONE=""6 [Iowa City]""", EPS) /NoList /Optimize /NoDebug WRITEFIL.C
$ Link pcal,exprpars,moonphas,pcalutil,pcalpapr,readfile,writefil'Options'
$
$ If "''F$Search("PCAL.HLB")'" .EQS. "" Then Library /Create /Help PCAL.HLB
$ Library /Replace PCAL.HLB PCAL.HLP
$
$ Exit
