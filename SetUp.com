$ Write Sys$Output "SETting UP Pcal..."
$ THIS_PATH = F$Element (0, "]", F$Environment ("PROCEDURE")) + "]"
$ Define Pcal_Dir 'THIS_PATH'
$ PCAL :== $ PCAL_Dir:PCAL.EXE
$!
$!  Put the help library into the next available help library slot
$!
$ LIB = "Hlp$Library"
$ X = F$Trnlnm (LIB, "Lnm$Process")
$ If X .eqs. "" Then GoTo INSERT
$ If X .eqs. "''THIS_PATH'PCAL.HLB" Then GoTo EXIT
$ BASE = LIB + "_"
$ N = 1
$NEXTLIB:
$   LIB := 'BASE''N'
$   X = F$Trnlnm (LIB, "Lnm$Process")
$   If X .eqs. "" Then GoTo INSERT
$   If X .eqs. "''THIS_PATH'PCAL.HLB" Then GoTo EXIT
$   N = N + 1
$   GoTo NEXTLIB
$INSERT:
$   Define 'LIB' 'THIS_PATH'PCAL.HLB
$EXIT:
$   Exit
