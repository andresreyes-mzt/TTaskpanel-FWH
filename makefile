# --------------------------------------------------------------
#
# --- ENGLISH ---
#
# Modify the path of your C & Harbour compiler here.
#
# if you want to build the lin with borland and xharbour  only
# comment the other paths using # and let without comment
# BCC_PATH y XHB_BCC_PATH
#
#
# --- SPANISH ---
#
# Modifica las rutas de acceso a tu compilador c y harbour aqui
#
# si quieres generar la libreria solo para borland c y xharbour
# comenta las demas rutas de acceso agregando # al principio y
# deja sin comentar BCC_PATH y XHB_BCC_PATH
#

# --------------------------------------------------------------
# Example :
#
#     FWH_PATH       = \etc\fwh\10.6
#
#     BCC_PATH       = \etc\bcc\55
#     #HB_BCC_PATH    = \etc\hb\bcc
#     XHB_BCC_PATH   = \etc\xhb\bcc
#
#     #POCC_PATH      = \etc\pellesc
#     #HB_POCC_PATH   = \etc\hb\pocc
#     #XHB_POCC_PATH  = \etc\xhb\pocc
#
# --------------------------------------------------------------

FWH_PATH       = \etc\fwh\10.6

BCC_PATH       = \etc\bcc\7
#HB_BCC_PATH    = \etc\hb\bcc
XHB_BCC_PATH   = \etc\xhb\1.2.3\bcc

#POCC_PATH      = \etc\pellesc
#HB_POCC_PATH   = \etc\hb\pocc
#XHB_POCC_PATH  = \etc\xhb\pocc

# ------------------
#  Codigo Fuente
# ------------------


PRG_FILES = \
	taskpanel \
	h_ctaskpanel \
	h_ctaskgroup \
	h_ctaskitem \
	h_tthememanager \
	h_ttaskpanel \
	h_ttaskgroup \
	h_ttaskitem \
	h_texplorerbar

C_FILES = \
	c_themeapi \
	c_themeaux \
	c_themegdi \
	c_tthememanager \
	c_dibbmp \
	c_drawhtml \
	r_themeclassic \
	r_themexphomestead \
	r_themexpmetallic \
	r_themexpnormalcolor \
	c_outputdebug

# ------------------
#  Test Exe Files
# ------------------
#
# Solo comenta los ejemplos que no deseas generar el ejecutable
#

TEST=yes
TESTCONTROL=yes
TESTCUSTOM=yes
TESTDIALOG=yes
TESTDIALOG2=yes
TESTEXPLORERBAR=yes
TESTMDI=yes
TESTMDI2=yes
TESTRIBBON=yes


#No mover esta linea
include rules.mak