# Copyright 2008-2012 .
#
# File    : deneme.mk   $Revision: 0.1 $
#
# Abstract:
#	Real-Time Workshop template makefile for building a UNIX-based
#	stand-alone generic real-time version of Simulink model using
#	generated C code.
#
# 	This makefile attempts to conform to the guidelines specified in the
# 	IEEE Std 1003.2-1992 (POSIX) standard. It is designed to be used
#       with GNU Make which is located in matlabroot/rtw/bin.
#
# 	Note that this template is automatically customized by the Real-Time
#	Workshop build procedure to create "<model>.mk"
#
#       The following defines can be used to modify the behavior of the
#	build:
#	  OPT_OPTS       - Optimization options. Default is -O. To enable
#                          debugging specify as OPT_OPTS=-g.
#                          Because of optimization problems in IBM_RS,
#                          default is no-optimization.
#	  CPP_OPTS       - C++ compiler options.
#	  OPTS           - User specific compile options.
#	  USER_SRCS      - Additional user sources, such as files needed by
#			   S-functions.
#	  USER_INCLUDES  - Additional include paths
#			   (i.e. USER_INCLUDES="-Iwhere-ever -Iwhere-ever2")
#
#       This template makefile is designed to be used with a system target
#       file that contains 'rtwgensettings.BuildDirSuffix' see grt.tlc

#------------------------ Macros read by make_rtw ------------------------------
#
# The following macros are read by the Real-Time Workshop build procedure:
#
#  MAKECMD         - This is the command used to invoke the make utility
#  HOST            - What platform this template makefile is targeted for
#                    (i.e. PC or UNIX)
#  BUILD           - Invoke make from the Real-Time Workshop build procedure
#                    (yes/no)?
#  SYS_TARGET_FILE - Name of system target file.

MAKECMD         = /usr/local/MATLAB/R2014a/bin/glnxa64/gmake
HOST            = UNIX
BUILD           = yes
SYS_TARGET_FILE = zenom.tlc
COMPILER_TOOL_CHAIN = g++


#---------------------- Tokens expanded by make_rtw ----------------------------
#
# The following tokens, when wrapped with "|>" and "<|" are expanded by the
# Real-Time Workshop build procedure.
#
#  MODEL_NAME          - Name of the Simulink block diagram
#  MODEL_MODULES       - 
#  MAKEFILE_NAME       - Name of makefile created from template makefile <model>.mk
#  MATLAB_ROOT         - Path to were MATLAB is installed.
#  S_FUNCTIONS         - List of S-functions.
#  S_FUNCTIONS_LIB     - List of S-functions libraries to link.
#  SOLVER              - Solver source file name
#  NUMST               - Number of sample times
#  TID01EQ             - yes (1) or no (0): Are sampling rates of continuous task
#                        (tid=0) and 1st discrete task equal.
#  NCSTATES            - Number of continuous states
#  COMPUTER            - Computer type. See the MATLAB computer command.
#  BUILDARGS           - Options passed in at the command line.
#  MULTITASKING        - yes (1) or no (0): Is solver mode multitasking
#  EXT_MODE            - yes (1) or no (0): Build for external mode
#  TMW_EXTMODE_TESTING - yes (1) or no (0): Build ext_test.c for external mode
#                        testing.
#  EXTMODE_TRANSPORT   - Index of transport mechanism (e.g. tcpip, serial) for extmode
#  EXTMODE_STATIC      - yes (1) or no (0): Use static instead of dynamic mem alloc.
#  EXTMODE_STATIC_SIZE - Size of static memory allocation buffer.
#  GEN_MDLREF_SFCN     - (1/0): are we generating model reference wrapper s-function
#  TGT_FCN_LIB         - Target Funtion library to use
#  MODELREFS           - List of referenced models

MODEL                = deneme
MODULES              = deneme_capi.c deneme_data.c rtGetInf.c rtGetNaN.c rt_logging.c rt_logging_mmi.c rt_nonfinite.c rtw_modelmap_utils.c 
MODEL                = deneme
MAKEFILE             = deneme.mk
MATLAB_ROOT          = /usr/local/MATLAB/R2014a
ALT_MATLAB_ROOT      = /usr/local/MATLAB/R2014a
MATLAB_BIN           = /usr/local/MATLAB/R2014a/bin
MASTER_ANCHOR_DIR    = 
START_DIR            = /home/robotlab/ahmet/matlab
S_FUNCTIONS          = 
S_FUNCTIONS_LIB      = 
SOLVER               = 
NUMST                = 2
TID01EQ              = 1
NCSTATES             = 1
COMPUTER             = GLNXA64
BUILDARGS            =  MAT_FILE=1
MULTITASKING         = 0
EXT_MODE             = 0
TMW_EXTMODE_TESTING  = 0
EXTMODE_TRANSPORT    = 0
EXTMODE_STATIC       = 0
EXTMODE_STATIC_SIZE  = 1000000
MEXEXT               = mexa64
TGT_FCN_LIB          = None
MODELREFS            = 
SHARED_SRC           = 
SHARED_SRC_DIR       = 
SHARED_BIN_DIR       = 
SHARED_LIB           = 
TARGET_LANG_EXT      = c
OPTIMIZATION_FLAGS   = 
ADDITIONAL_LDFLAGS   = 

### Xenomai directory, xeno-config and library directory ###########

SKIN              = xeno
XENO_ROOT         = /usr/xenomai
XENO_CONFIG       = $(XENO_ROOT)/bin/xeno-config
XENO_LIB_DIR      = $(shell $(XENO_CONFIG) --libdir)

XENO_LDFLAGS   = $(shell $(XENO_CONFIG) --skin=native --ldflags) -lrtdm
XENO_CFLAGS    = $(shell $(XENO_CONFIG) --skin=native --cflags)
 
#--------------------------- Model and reference models -----------------------
MODELLIB                  = denemelib.a
MODELREF_LINK_LIBS        = 
MODELREF_INC_PATH         = 
RELATIVE_PATH_TO_ANCHOR   = ..
# NONE: standalone, SIM: modelref sim, RTW: modelref rtw
MODELREF_TARGET_TYPE       = NONE

#-- In the case when directory name contains space ---
ifneq ($(MATLAB_ROOT),$(ALT_MATLAB_ROOT))
MATLAB_ROOT := $(ALT_MATLAB_ROOT)/usr/local/MATLAB/Work/xenomaiTargetTest1_xenomai/html/xenomaiTargetTest1_codegen_rpt.html
endif

#--------------------------- Tool Specifications -------------------------------

include $(MATLAB_ROOT)/rtw/c/zenom/zenom/xenotools.mk
#------------------------------ Include Path -----------------------------------

MATLAB_INCLUDES = \
	-I$(MATLAB_ROOT)/simulink/include \
	-I$(MATLAB_ROOT)/extern/include \
	-I$(MATLAB_ROOT)/rtw/c/src \
	-I$(MATLAB_ROOT)/rtw/c/src/ext_mode/common
   

# Additional file include paths
ADD_INCLUDES = \
	-I$(START_DIR)/deneme_zenom \
	-I$(START_DIR) \


SHARED_INCLUDES =
ifneq ($(SHARED_SRC_DIR),)
SHARED_INCLUDES = -I$(SHARED_SRC_DIR) 
endif

LINUX = /usr/src/linux
XENO_INCLUDES= -I$(LINUX)/include -I$(TARGET_ROOT)/include -I$(XENO_ROOT)/include -I$(MATLAB_ROOT)/rtw/c/zenom/include  -I$(MATLAB_ROOT)/rtw/c/zenom/rtw/ext_mode -I /usr/local/rtnet/include 

# Zenom
ZENOM_INCLUDES   = -I/usr/include/zenom
ZENOM_LFLAGS     = -L/usr/lib64
ZENOM_LIBS       = -lznm-controlbase


INCLUDES = -I. -I$(RELATIVE_PATH_TO_ANCHOR) $(MATLAB_INCLUDES) $(ADD_INCLUDES) $(USER_INCLUDES) \
	$(INSTRUMENT_INCLUDES)  $(MODELREF_INC_PATH) $(SHARED_INCLUDES) $(XENO_INCLUDES) $(ZENOM_INCLUDES)
#----------------------------- External mode -----------------------------------
# Uncomment -DVERBOSE to have information printed to stdout
# To add a new transport layer, see the comments in
#   <matlabroot>/toolbox/simulink/simulink/extmode_transports.m
ifeq ($(EXT_MODE),1)
  EXT_CC_OPTS = -DEXT_MODE -D$(COMPUTER) #-DVERBOSE
  EXT_LIB     =
  EXT_SRC     =
  LINT_EXT_COMMON_SRC =
  LINT_EXT_TCPIP_SRC  =
  ifeq ($(EXTMODE_TRANSPORT),0) #UDP/IP-rtNET
    EXT_SRC = ext_svr.c updown.c ext_work.c rtiostream_interface.c \
	rtiostream_tcpip.c
    LINT_EXT_COMMON_SRC = ext_svr.c updown.c ext_work.c \
	rtiostream_interface.c
    LINT_EXT_TCPIP_SRC  = rtiostream_tcpip.c
  endif
  ifeq ($(EXTMODE_TRANSPORT),1) #
    err:
	@echo
	@echo "### ERROR: Not Available"
	@echo
  endif
  ifeq ($(TMW_EXTMODE_TESTING),1)
    EXT_SRC     += ext_test.c
    EXT_CC_OPTS += -DTMW_EXTMODE_TESTING
  endif
  ifeq ($(EXTMODE_STATIC),1)
    EXT_SRC             += mem_mgr.c
    LINT_EXT_COMMON_SRC += mem_mgr.c
    EXT_CC_OPTS         += -DEXTMODE_STATIC -DEXTMODE_STATIC_SIZE=$(EXTMODE_STATIC_SIZE)
  endif
  ifeq ($(COMPUTER),SOL64)
    EXT_LIB = -lsocket -lnsl
  endif
endif

#----------------------------- Real-Time Model ---------------------------------
RTM_CC_OPTS = -DUSE_RTMODEL

#-------------------------------- C Flags --------------------------------------

# Optimization Options
ifndef OPT_OPTS
OPT_OPTS = $(DEFAULT_OPT_OPTS)
endif

# General User Options
OPTS =

# Compiler options, etc:
ifneq ($(OPTIMIZATION_FLAGS),)
CC_OPTS = $(OPTS) $(EXT_CC_OPTS) $(RTM_CC_OPTS) $(OPTIMIZATION_FLAGS)
else
CC_OPTS = $(OPT_OPTS) $(OPTS) $(EXT_CC_OPTS) $(RTM_CC_OPTS)
endif


CPP_REQ_DEFINES = -DMODEL=$(MODEL) -DRT -DNUMST=$(NUMST) -DDQ8=$(MATLAB_ROOT)/rtw/c/zenom/drivers/q8driver.ko \
                  -DTID01EQ=$(TID01EQ) -DNCSTATES=$(NCSTATES) -DUNIX -DXENOMAI\
                  -DMT=$(MULTITASKING) -DHAVESTDIO 


CFLAGS   = $(ANSI_OPTS) $(CC_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES)
CPPFLAGS = $(CPP_ANSI_OPTS) $(CPP_OPTS) $(CC_OPTS) $(CPP_REQ_DEFINES) $(INCLUDES)
#-------------------------- Additional Libraries ------------------------------

SYSLIBS = $(EXT_LIB) 

LIBS =
 
LIBS += $(S_FUNCTIONS_LIB) $(INSTRUMENT_LIBS)

#----------------------------- Source Files ------------------------------------

USER_SRCS =

USER_OBJS       = $(addsuffix .o, $(basename $(USER_SRCS)))
LOCAL_USER_OBJS = $(notdir $(USER_OBJS))


SRCS = $(MODULES) $(S_FUNCTIONS) 

ifeq ($(MODELREF_TARGET_TYPE), NONE)
    PRODUCT            = $(RELATIVE_PATH_TO_ANCHOR)/$(MODEL)
    BIN_SETTING        = $(XENO_CFLAGS) $(LDFLAGS) $(ADDITIONAL_LDFLAGS) $(XENO_LDFLAGS) $(ZENOM_LFLAGS) $(XENO_LIBS) $(SYSLIBS)  $(ZENOM_LIBS) -lm 
    BUILD_PRODUCT_TYPE = "executable"
    SRCS               += $(MODEL).$(TARGET_LANG_EXT) main.cpp rt_sim.c $(EXT_SRC) $(SOLVER) Q8Lib_utils.c
else
    # Model reference rtw target
    PRODUCT            = $(MODELLIB)
    BUILD_PRODUCT_TYPE = "library"
endif

ifneq ($(findstring .cpp,$(suffix $(SRCS), $(USER_SRCS))),)
  LD = $(CPP)
endif

OBJS      = $(addsuffix .o, $(basename $(SRCS))) $(USER_OBJS)
LINK_OBJS = $(addsuffix .o, $(basename $(SRCS))) $(LOCAL_USER_OBJS)

SHARED_SRC := $(wildcard $(SHARED_SRC))
SHARED_OBJS = $(addsuffix .o, $(basename $(SHARED_SRC)))

#------------- Test Compile using gcc -Wall to look for warnings ---------------
#
# DO_GCC_TEST=1 runs gcc with compiler warning flags on all the source files
# used in this build. This includes the generated code, and any user source
# files needed for the build and placed in this directoryinclude $(MATLAB_ROOT)/rtw/c/tools/unixtools.mk.
#
# WARN_ON_GLNX=1 runs the linux compiler with warnings flags. On hand-written
# code we use the max amount of flags availabe while on the generated code, a
# few less.
#
# See rtw/c/tools/unixtools.mk for the definition of GCC_WARN_OPTS
GCC_TEST_CMD  := echo
GCC_TEST_OUT  := > /dev/null
ifeq ($(DO_GCC_TEST), 1)
  GCC_TEST := gcc -c -o /dev/null $(GCC_WARN_OPTS_MAX) $(CPP_REQ_DEFINES) \
                                     $(INCLUDES)
  GCC_TEST_CMD := echo; echo "\#\#\# GCC_TEST $(GCC_TEST) $<"; $(GCC_TEST)
  GCC_TEST_OUT := ; echo
endif

#----------------------------- Lint (sol2 only) --------------------------------

LINT_SRCS = $(MATLAB_ROOT)/rtw/c/grt/grt_main.c \
	    $(MATLAB_ROOT)/rtw/c/src/rt_sim.c
ifneq ($(SOLVER), )
  LINT_SRCS += $(MATLAB_ROOT)/rtw/c/src/$(SOLVER)
endif
ifneq ($(LINT_EXT_COMMON_SRC), )
  LINT_SRCS += $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/$(LINT_EXT_COMMON_SRC)
endif
ifneq ($(LINT_EXT_TCPIP_SRC), )
  LINT_SRCS += $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/$(LINT_EXT_TCPIP_SRC)
endif
LINT_SRCS += $(MODEL).$(TARGET_LANG_EXT) $(MODULES) $(USER_SRCS) $(S_FUNCTIONS)
LINTOPTSFILE = $(MODEL).lintopts

LINT_ERROFF1 = E_NAME_DEF_NOT_USED2,E_NAME_DECL_NOT_USED_DEF2
LINT_ERROFF2 = $(LINT_ERROFF1),E_FUNC_ARG_UNUSED
LINT_ERROFF  = $(LINT_ERROFF2),E_INDISTING_FROM_TRUNC2,E_NAME_USED_NOT_DEF2

#--------------------------------- Rules ---------------------------------------
#$(PRODUCT).znm :
#	@touch $(PRODUCT).znm

ifeq ($(MODELREF_TARGET_TYPE),NONE)
$(PRODUCT) : $(OBJS) $(SHARED_LIB) $(LIBS) $(MODELREF_LINK_LIBS)
	$(LD) -o $(PRODUCT) $(LINK_OBJS) $(BIN_SETTING) $(MODELREF_LINK_LIBS) $(SHARED_LIB) $(LIBS) 
	@echo "### Created $(BUILD_PRODUCT_TYPE): $@"$(XENO_CFLAGS)
else
$(PRODUCT) : $(OBJS) $(SHARED_LIB)
	@rm -f $(MODELLIB)
	ar ruvs $(MODELLIB) $(LINK_OBJS)
	@echo "### Created $(MODELLIB)"
	@echo "### Created $(BUILD_PRODUCT_TYPE): $@"
endif

ifneq ($(SHARED_SRC_DIR),)
$(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.c
	cd $(SHARED_BIN_DIR); $(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) $(notdir $?)

$(SHARED_BIN_DIR)/%.o : $(SHARED_SRC_DIR)/%.cpp
	cd $(SHARED_BIN_DIR); $(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG_MAX) $(notdir $?)
endif

%.o : %.cpp
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) $(GCC_WALL_FLAG) "$<"

%.o : %.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG) "$<"

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG) "$<"

%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) "$<"

%.o : $(MATLAB_ROOT)/rtw/c/zenom/%.c
	#@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"

%.o : $(MATLAB_ROOT)/rtw/c/zenom/rtw/%.c
	$(CC) -c $(CFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"

%.o : $(MATLAB_ROOT)/rtw/c/zenom/rtw/ext_mode/%.c
	$(CC) -c $(CFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"

%.o : $(MATLAB_ROOT)/rtw/c/zenom/rtw/c/%.c
	$(CC) -c $(CFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"

%.o : $(MATLAB_ROOT)/rtw/c/zenom/src/%.c
	$(CC) -c $(CFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)   "$<"	

%.o : $(MATLAB_ROOT)/rtw/c/zenom/src/%.cpp
	$(CPP) -c $(CPPFLAGS) $(XENO_CFLAGS) $(GCC_WALL_FLAG_MAX)   "$<"
	
%.o : #$(TARGET_ROOT)/xenomai/%.c
       # $(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"	

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/common/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"

%.o : $(MATLAB_ROOT)/rtw/c/src/rtiostream/rtiostreamtcpip/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX)  "$<"

%.o : $(MATLAB_ROOT)/rtw/c/src/ext_mode/custom/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"

%.o : $(MATLAB_ROOT)/rtw/c/src/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"

%.o : $(MATLAB_ROOT)/simulink/src/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"



%.o : $(MATLAB_ROOT)/rtw/c/src/%.cpp
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) "$<"
%.o : $(MATLAB_ROOT)/simulink/src/%.cpp
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) "$<"


%.o : $(MATLAB_ROOT)/simulink/src/%.c
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CC) -c $(CFLAGS) $(GCC_WALL_FLAG_MAX) "$<"

%.o : $(MATLAB_ROOT)/simulink/src/%.cpp
	@$(GCC_TEST_CMD) "$<" $(GCC_TEST_OUT)
	$(CPP) -c $(CPPFLAGS) "$<"

#------------------------------- Libraries -------------------------------------





#----------------------------- Dependencies ------------------------------------

$(OBJS) : $(MAKEFILE) rtw_proj.tmw

$(SHARED_LIB) : $(SHARED_OBJS)
	@echo "### Creating $@ "
	ar ruvs $@ $(SHARED_OBJS)
	@echo "### $@ Created  "


#--------- Miscellaneous rules to purge, clean and lint (sol2 only) ------------

purge : clean
	@echo "### Deleting the generated source code for $(MODEL)"
	@\rm -f $(MODEL).c $(MODEL).h $(MODEL)_types.h $(MODEL)_data.c \
	        $(MODEL)_private.h $(MODEL).rtw $(MODULES) rtw_proj.tmw $(MAKEFILE)

clean :
	@echo "### Deleting the objects and $(PRODUCT)"
	@\rm -f $(LINK_OBJS) $(PRODUCT)

lint  : rtwlib.ln
	@lint -errchk -errhdr=%user -errtags=yes -F -L. -lrtwlib -x -Xc \
	      -erroff=$(LINT_ERROFF) \
	      -D_POSIX_C_SOURCE $(CFLAGS) $(LINT_SRCS)
	@\rm -f $(LINTOPTSFILE)
	@echo
	@echo "### Created lint output only, no executable"
	@echo

rtwlib.ln : $(MAKEFILE) rtw_proj.tmw
	@echo
	@echo "### Linting ..."
	@echo
	@\rm -f llib-lrtwlib.ln $(LINTOPTSFILE)
	@echo "-dirout=. -errchk -errhdr=%user " >> $(LINTOPTSFILE)
	@echo "-errtags -F -ortwlib -x -Xc " >> $(LINTOPTSFILE)
	@echo "-erroff=$(LINT_ERROFF) " >> $(LINTOPTSFILE)
	@echo "-D_POSIX_C_SOURCE $(CFLAGS) " >> $(LINTOPTSFILE)
	@for file in $(MATLAB_ROOT)/rtw/c/libsrc/*.c; do \
	  echo "$$file " >> $(LINTOPTSFILE); \
	done
	lint -flagsrc=$(LINTOPTSFILE)

# EOF: xenomai.tmf
