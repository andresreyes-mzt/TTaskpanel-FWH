# -----------------------------------------------------
# rules.mak
#
# do not move anything here / no mover nada aqui
# -----------------------------------------------------


TARGETS :=

ifneq ($(BCC_PATH),)
	ifneq ($(HB_BCC_PATH),)
		TARGETS := $(TARGETS) bcch
	endif
	ifneq ($(XHB_BCC_PATH),)
		TARGETS := $(TARGETS) bcchx
	endif	
endif

ifneq ($(POCC_PATH),)
	ifneq ($(HB_POCC_PATH),)
		TARGETS := $(TARGETS) pocch
	endif
	ifneq ($(XHB_POCC_PATH),)
		TARGETS := $(TARGETS) pocchx
	endif	
endif

ifeq ($(C_COMPILER),bcc)
	C_PATH = $(BCC_PATH)
	ifeq ($(HB_COMPILER),harbour)
		HB_PATH = $(HB_BCC_PATH)
	else
		ifeq ($(HB_COMPILER),xharbour)
			HB_PATH = $(XHB_BCC_PATH)
		endif
	endif	
else
	ifeq ($(C_COMPILER),pocc)
		C_PATH = $(POCC_PATH)
		ifeq ($(HB_COMPILER),harbour)
			HB_PATH = $(HB_POCC_PATH)
		else
			ifeq ($(HB_COMPILER),xharbour)
				HB_PATH = $(XHB_POCC_PATH)
			endif
		endif	
	endif
endif

ifeq ($(wildcard ./source/*.c),)
	C_FILES =
endif

# ----------------------------------------------------
# ejemplos
# ----------------------------------------------------

ifneq ($(C_COMPILER),)
	ifneq ($(HB_COMPILER),)
		ifeq ($(TEST),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) test
		endif
		ifeq ($(TESTCONTROL),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testcontrol
		endif
		ifeq ($(TESTCUSTOM),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testcustom
		endif
		ifeq ($(TESTDIALOG),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testdialog
		endif
		ifeq ($(TESTDIALOG2),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testdialog2
		endif		
		ifeq ($(TESTEXPLORERBAR),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testexplorerbar
		endif
		ifeq ($(TESTMDI),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testmdi
		endif
		ifeq ($(TESTMDI2),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testmdi2
		endif
		ifeq ($(TESTRIBBON),yes)
			SAMPLES_TARGETS := $(SAMPLES_TARGETS) testribbon
		endif
		
	endif
endif


# ----------------------------------------------------
# Rutas de acceso
# ----------------------------------------------------

C_BIN       = $(C_PATH)\bin
C_INCLUDE   = $(C_PATH)\include
C_LIB       = $(C_PATH)\lib

HB_BIN      = $(HB_PATH)\bin
HB_INCLUDE  = $(HB_PATH)\include
HB_LIB      = $(HB_PATH)\lib

FWH_INCLUDE = $(FWH_PATH)\include
FWH_LIB     = $(FWH_PATH)\lib

LIB_NAME    = ttaskpanel.lib
LIB_DIR     = lib\$(HB_COMPILER)\$(C_COMPILER)
LIB_FILE    = $(LIB_DIR)\$(LIB_NAME)

OBJ_DIR     = obj\$(HB_COMPILER)\$(C_COMPILER)
LIB_DIR     = lib\$(HB_COMPILER)\$(C_COMPILER)
BIN_DIR     = bin\$(HB_COMPILER)\$(C_COMPILER)

OBJ_FILES   = $(addprefix $(OBJ_DIR)\,$(addsuffix .obj,$(PRG_FILES))) $(addprefix $(OBJ_DIR)\,$(addsuffix .obj,$(C_FILES)))

# ----------------------------------------------------
# Librerias de Harbour
# ----------------------------------------------------

ifeq ($(HB_COMPILER),xharbour)

HB_LIBS = \
	common \
	vm \
	rtl \
	debug \
	pcrepos \
	gtgui \
	lang \
	nulsys \
	rdd \
	macro \
	pp \
	hsx \
	hbsix \
	ct \
	tip \
	zlib
	
endif

ifeq ($(HB_COMPILER),harbour)

HB_LIBS = \
	hbcommon \
	hbvm \
	hbrtl \
	hbdebug \
	hbpcre \
	gtgui \
	hblang \
	hbnulrdd \
	hbrdd \
	hbmacro \
	hbpp \
	hbhsx \
	hbsix \
	hbwin \
	hbct \
	hbtip \
	hbzlib

endif

ifeq ($(C_COMPILER),bcc)

STD_OBJS = \
	c0w32

STD_LIBS = \
	import32 \
	cw32 \
	msimg32

endif

ifeq ($(C_COMPILER),bcc)
	ifeq ($(HB_COMPILER),xharbour)
		FWH_LIBS = fivehx fivehc
	endif
	ifeq ($(HB_COMPILER),harbour)
		FWH_LIBS = fiveh fivehc
	endif
endif

STD_OBJS := $(addsuffix .obj,$(STD_OBJS))
LIBS     := $(LIB_FILE) $(addsuffix .lib, $(FWH_LIBS) $(HB_LIBS) $(STD_LIBS) )

# ----------------------------------------------------
# Opciones de compilador Harbour
# ----------------------------------------------------

HB_FLAGS = -q0 -w1 -es2 -gc0 -n -o$@ -i"include" -i"$(FWH_INCLUDE)" -i"$(HB_INCLUDE)"

define HB_CMD
	@echo Compiling $<
	@echo $(HB_BIN)\harbour.exe $< $(HB_FLAGS) >> make.log
	@$(HB_BIN)\harbour.exe $< $(HB_FLAGS) >> make.log
	@echo. >> make.log
endef

# ----------------------------------------------------
# Opciones de compilador C
# ----------------------------------------------------

define C_CMD
	@echo Compiling $<
	@echo $(CC_CMD) >>make.log
	@$(CC_CMD) >>make.log
	@echo. >>make.log
endef

define LIB_CMD
	@echo Building $@
	@echo $(AR_CMD) >> make.log
	@$(AR_CMD) >> make.log
	@echo. >> make.log
endef

define RC_CMD
	@echo Compiling $<
	@echo  $(RCC_CMD) >> make.log
	@$(RCC_CMD) >> make.log
	@echo. >> make.log	
endef

define LINK_CMD
	@echo Building $@
	@echo  $(LD_CMD) >> make.log
	@$(LD_CMD) >> make.log
	@echo. >> make.log	
	@$(UPX_CMD)
endef

ifeq ($(C_COMPILER),bcc)
	CC       = bcc32.exe
	AR       = tlib.exe	
	RC       = brc32.exe
	LD       = ilink32.exe
	
	CC_FLAGS = -w -v- -a8 -c -O2 -tW -5 -6 -X  -D__WIN__32 -D__HARBOUR__ -o$@ -I"INCLUDE" -I"$(HB_INCLUDE)" -I"$(C_INCLUDE)"
    AR_FLAGS = /P512
	
	CC_CMD   = $(C_BIN)\$(CC) $(CC_FLAGS) "$<"
	AR_CMD   = $(C_BIN)\$(AR) "$@" $(AR_FLAGS) $(addprefix +-",$(addsuffix ",$(filter %.obj,$^))),
	
	RC_FLAGS = -x -r -i"$(C_INCLUDE)" -fo"$@"
	RCC_CMD  = $(C_BIN)\$(RC) $(RC_FLAGS) "$<"
	
	LD_FLAGS = -Gn -aa -Tpe -s -x -L"$(C_LIB)" -L"$(C_LIB)\psdk" -L"$(LIB_DIR)" -L"$(HB_LIB)" -L"$(FWH_LIB)"
	LD_CMD   = $(C_BIN)\$(LD) $(LD_FLAGS) $(STD_OBJS) $(addprefix ",$(addsuffix ",$(filter %.obj,$^))),"$@",, $(addprefix ",$(addsuffix ",$(LIBS))),,$(addprefix ",$(addsuffix ",$(filter %.res,$^)))
endif

ifeq ($(C_COMPILER),pocc)
	CC       = pocc.exe
	AR       = polib.exe	
	
	CC_FLAGS = /Ze /Zx /Tx86-coff /Go /I "include" /I "$(C_INCLUDE)" /I "$(C_INCLUDE)\win" /I "$(HB_INCLUDE)" /D"NEED_DUMMY_RETURN" /D"__WIN32__" /D"WIN32" /D"NULL=0"
	AR_FLAGS = /P512
	
	CC_CMD   = $(C_BIN)\$(CC) $(CC_FLAGS)  /Fo "$@" "$<"
	AR_CMD   = $(C_BIN)\$(AR) /out:"$@" $(addprefix ",$(addsuffix ",$^))
	RC_CMD=
	LINK_CMD=
	SAMPLES_TARGETS=
endif

# ----------------------------------------------------
# UPX
# ----------------------------------------------------

ifeq ($(COMPRESS),yes)
	UPX_CMD = upx $@
endif

# -----------------------------------------------------
# General Rules
# -----------------------------------------------------

.PHONY: all clean lib sample samples dir initlog initlog2 initlog3 bcch bcchx pocch pocchx pack 


all: initlog $(TARGETS)

ifeq ($(C_COMPILER),)

clean : $(addsuffix clean,$(TARGETS))

else

clean:
	@del /f /q $(OBJ_DIR)\*.*
	@del /f /q $(BIN_DIR)\*.exe
	@del /f /q $(BIN_DIR)\*.tds
	@del /f /q $(LIB_DIR)\*.*

endif

ifeq ($(SAMPLES_TARGETS),)

samples:

else

samples: dir lib
	@for %%c IN ($(SAMPLES_TARGETS)) Do $(MAKE) $(MKFLAGS) -f makefile sample SAMPLE=%%c C_COMPILER=$(C_COMPILER) HB_COMPILER=$(HB_COMPILER) COMPRESS=$(COMPRESS)
	
endif

initlog:
	@echo. > make.log

initlog2:
	@echo # ----------------------------------------------------------------------- >> make.log
	@echo # Building $(LIB_FILE)                                                    >> make.log
	@echo #                                                                         >> make.log
	@echo # C_COMPILER    :$(C_COMPILER) at $(C_PATH)                               >> make.log
	@echo # HB_COMPILER   :$(HB_COMPILER) at $(HB_PATH)                             >> make.log
	@echo # ----------------------------------------------------------------------- >> make.log
	@echo # -----------------------------------------------------------------------
	@echo # Building $(LIB_FILE)
	@echo #
	@echo # C_COMPILER    :$(C_COMPILER) at $(C_PATH)
	@echo # HB_COMPILER   :$(HB_COMPILER) at $(HB_PATH)
	@echo # -----------------------------------------------------------------------

initlog3:
	@echo # ----------------------------------------------------------------------- >> make.log
	@echo # Building $(FILE_EXE)                                                    >> make.log
	@echo #                                                                         >> make.log
	@echo # C_COMPILER    :$(C_COMPILER) at $(C_PATH)                               >> make.log
	@echo # HB_COMPILER   :$(HB_COMPILER) at $(HB_PATH)                             >> make.log
	@echo # ----------------------------------------------------------------------- >> make.log
	@echo # -----------------------------------------------------------------------
	@echo # Building $(FILE_EXE)
	@echo #
	@echo # C_COMPILER    :$(C_COMPILER) at $(C_PATH)
	@echo # HB_COMPILER   :$(HB_COMPILER) at $(HB_PATH)
	@echo # -----------------------------------------------------------------------


bcch:
	@$(MAKE) $(MKFLAGS) -f makefile lib samples C_COMPILER=bcc HB_COMPILER=harbour COMPRESS=$(COMPRESS)

bcchx:
	@$(MAKE) $(MKFLAGS) -f makefile lib samples C_COMPILER=bcc HB_COMPILER=xharbour COMPRESS=$(COMPRESS)

pocch:
	@$(MAKE) $(MKFLAGS) -f makefile lib samples C_COMPILER=pocc HB_COMPILER=harbour COMPRESS=$(COMPRESS)
	
pocchx:
	@$(MAKE) $(MKFLAGS) -f makefile lib samples C_COMPILER=pocc HB_COMPILER=xharbour COMPRESS=$(COMPRESS)

bcchclean:
	@$(MAKE) $(MKFLAGS) -f makefile clean C_COMPILER=bcc HB_COMPILER=harbour

bcchxclean:
	@$(MAKE) $(MKFLAGS) -f makefile clean C_COMPILER=bcc HB_COMPILER=xharbour

pocchclean:
	@$(MAKE) $(MKFLAGS) -f makefile clean C_COMPILER=pocc HB_COMPILER=harbour
	
pocchxclean:
	@$(MAKE) $(MKFLAGS) -f makefile clean C_COMPILER=pocc HB_COMPILER=xharbour

# -----------------------------------------------------
# pack
# -----------------------------------------------------

pack:
	@$(MAKE) -B -f makefile all COMPRESS=yes
	@7z u -tzip -r0 "TTaskPanel for FWH v1.0-Win32.zip" -ir!tests\*.prg -ir!tests\*.rc -ir!lib\*.lib -ir!include\*.ch -ir!manual\*.pdf -ir!source\*.prg  -ir!resource\*.bmp -ir!resource\*.ico -ir!resource\*.manifest -ir!.\makefile -ir!rules.mak -ir!win-make.exe -ir!changelog.txt -ir!howto.txt -ir!license.txt
	@7z u -tzip -r0 "TTaskPanel for FWH v1.0-Win32-Samples-Bin.zip" -ir!bin\xharbour\bcc\*.exe -ir!bin\xharbour\bcc\*.dll
	
# -----------------------------------------------------
# dir
# -----------------------------------------------------

dir: $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

$(OBJ_DIR) :
	@if not exist $@ md $@

$(LIB_DIR) :
	@if not exist $@ md $@

$(BIN_DIR) :
	@if not exist $@ md $@

# -----------------------------------------------------
# lib
# -----------------------------------------------------

ifneq ($(SAMPLE),)

lib:

else

lib: initlog2 dir $(LIB_FILE)

$(LIB_FILE) : $(OBJ_FILES)
	$(LIB_CMD)

endif	
# -----------------------------------------------------
# Obj Files
# -----------------------------------------------------

SRC_DIR = source

# ---------------------
# taskpanel
# ---------------------

FILE    = taskpanel

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_tthememanager
# ---------------------

FILE    = h_tthememanager

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ctaskpanel
# ---------------------

FILE    = h_ctaskpanel

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ctaskgroup
# ---------------------

FILE    = h_ctaskgroup

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ctaskitem
# ---------------------

FILE    = h_ctaskitem

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ttaskpanel
# ---------------------

FILE    = h_ttaskpanel

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ttaskgroup
# ---------------------

FILE    = h_ttaskgroup

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_ttaskitem
# ---------------------

FILE    = h_ttaskitem

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# h_texplorerbar
# ---------------------

FILE    = h_texplorerbar

$(OBJ_DIR)\$(FILE).obj 	:$(OBJ_DIR)\$(FILE).c
	$(C_CMD)

$(OBJ_DIR)\$(FILE).c 	:$(SRC_DIR)\$(FILE).prg
	$(HB_CMD)

# ---------------------
# c_themeapi
# ---------------------

FILE    = c_themeapi

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_themeaux
# ---------------------

FILE    = c_themeaux

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_themegdi
# ---------------------

FILE    = c_themegdi

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_tthememanager
# ---------------------

FILE    = c_tthememanager

c_TThemeManager.c : $(OBJ_DIR)\$(FILE).obj

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_dibbmp
# ---------------------

FILE    = c_dibbmp

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_drawhtml
# ---------------------

FILE    = c_drawhtml

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# r_themeclassic
# ---------------------

FILE    = r_themeclassic

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# r_themexphomestead
# ---------------------

FILE    = r_themexphomestead

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# r_themexpmetallic
# ---------------------

FILE    = r_themexpmetallic

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# r_themexpnormalcolor
# ---------------------

FILE    = r_themexpnormalcolor

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)

# ---------------------
# c_outputdebug
# ---------------------

FILE    = c_outputdebug

$(OBJ_DIR)\$(FILE).obj 	:$(SRC_DIR)\$(FILE).c
	$(C_CMD)


# -----------------------------------------------------
# samples
# -----------------------------------------------------

ifneq ($(SAMPLE),)

FILE_EXE = $(BIN_DIR)\$(SAMPLE).exe
FILE_OBJ = $(addprefix $(OBJ_DIR)\,$(addsuffix .obj,$(SAMPLE)))
FILE_C   = $(addprefix $(OBJ_DIR)\,$(addsuffix .c,$(SAMPLE)))
FILE_PRG = $(addprefix tests\$(SAMPLE)\,$(addsuffix .prg,$(SAMPLE)))
FILE_RES = $(addprefix $(OBJ_DIR)\,$(addsuffix .res,$(SAMPLE)))
FILE_RC  = $(addprefix tests\$(SAMPLE)\,$(addsuffix .rc,$(SAMPLE)))

sample		: initlog3 $(FILE_EXE)

$(FILE_EXE)	: $(FILE_OBJ) $(FILE_RES) $(LIB_FILE)
	$(LINK_CMD)
	
$(FILE_OBJ) : $(FILE_C)
	$(C_CMD)
	
$(FILE_C)   : $(FILE_PRG)
	$(HB_CMD)	
	
$(FILE_RES) : $(FILE_RC)
	$(RC_CMD)

endif
