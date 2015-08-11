# Copyright 2008-2012  .
#
# File    : xenotools.mk   $Revision: 0.95 $
# Abstract:
#	Setup xenomai realtime Linux patch tools for GNU make

ARCH := $(shell echo "$(COMPUTER)" | tr '[A-Z]' '[a-z]')
OS:=$(shell uname)

#
# Modify the following macros to reflect the tools you wish to use for
# compiling and linking your code.
#

DEFAULT_OPT_OPTS = -O0
ANSI_OPTS        =
CPP_ANSI_OPTS    =
CPP              = c++
LD               = $(CC)
SYSLIBS          =
LDFLAGS          =
SHRLIBLDFLAGS    =

# Override based on platform if needed

GCC_WALL_FLAG     :=
GCC_WALL_FLAG_MAX :=
ifeq ($(COMPUTER),GLNX86)
  CC  = gcc
  CPP = g++
  DEFAULT_OPT_OPTS = -O0 -ffloat-store -fPIC -m32
  LDFLAGS = -m32
  SHRLIBLDFLAGS = -shared -m32 -Wl,--no-undefined -Wl,--version-script,
  # Allow ISO-C functions like fmin to be called
  ifeq ($(TGT_FCN_LIB),ISO_C)
    ANSI_OPTS = -std=-pedantic
  else
    ifeq ($(TGT_FCN_LIB),GNU)
      ANSI_OPTS = -std= 
    else
      ifneq ($(NON_ANSI_TRIG_FCN), 1)
         ANSI_OPTS =
      endif
    endif
  endif
  # These definitions are used by targets that have the WARN_ON_GLNX option
  GCC_WARN_OPTS     := -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes \
		     -Wnested-externs -Wpointer-arith -Wcast-align
#-Wall -W -Wwrite-strings -Winline -Wstrict-prototypes \
#		     -Wnested-externs -Wpointer-arith -Wcast-align
  GCC_WARN_OPTS_MAX := $(GCC_WARN_OPTS) -Wcast-qual -Wshadow

  ifeq ($(WARN_ON_GLNX), 1)
    GCC_WALL_FLAG     := $(GCC_WARN_OPTS)
    GCC_WALL_FLAG_MAX := $(GCC_WARN_OPTS_MAX)
  endif
endif

ifeq ($(COMPUTER),GLNXA64)
  CC  = gcc
  CPP = g++
  DEFAULT_OPT_OPTS = -O0 -fPIC -Wno-unused
  SHRLIBLDFLAGS = -shared -Wl,--no-undefined -Wl,--version-script,
  # Allow ISO-C functions like fmin to be called
  ifeq ($(TGT_FCN_LIB),ISO_C)
    ANSI_OPTS = -std=
  else
    ifeq ($(TGT_FCN_LIB),GNU)
      ANSI_OPTS = -std=
    else
      ifneq ($(NON_ANSI_TRIG_FCN), 1)
         ANSI_OPTS =
      endif
    endif
  endif
  # These definitions are used by targets that have the WARN_ON_GLNX option
  GCC_WARN_OPTS     := -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes \
		     -Wnested-externs -Wpointer-arith -Wcast-align
#-Wall -Wwrite-strings -Winline -Wstrict-prototypes \
		    # -Wnested-externs -Wpointer-arith -Wcast-align
  GCC_WARN_OPTS_MAX := $(GCC_WARN_OPTS) -Wcast-qual -Wshadow

  ifeq ($(WARN_ON_GLNX), 1)
    GCC_WALL_FLAG     := $(GCC_WARN_OPTS)
    GCC_WALL_FLAG_MAX := $(GCC_WARN_OPTS_MAX)
  endif
endif

ifeq ($(OS), Darwin)
  CC  = gcc
  CPP = g++
  DEFAULT_OPT_OPTS = -O0 -ffloat-store
  
  # Determine architecture
  ifeq ($(COMPUTER), MACI64)
    ARCHS = x86_64
    LD_NAMESPACE = -twolevel_namespace
    LD_UNDEFS = -undefined error
    SDKROOT = /Developer/SDKs/MacOSX10.5.sdk
    MACOSX_DEPLOYMENT_TARGET = 10.5
  else
    ARCHS = $(shell uname -p)
    LD_NAMESPACE = -flat_namespace
    LD_UNDEFS = -undefined suppress
    SDKROOT = /Developer/SDKs/MacOSX10.4u.sdk
    MACOSX_DEPLOYMENT_TARGET = 10.4
  endif
  
  ANSI_OPTS = \
    -arch $(ARCHS) \
    -isysroot $(SDKROOT) \
    -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET) \
    -fno-common -no-cpp-precomp -fexceptions 
    
  #instead of using -bundle, use -dynamiclib flag to make the lib dlopen compatible
  SHRLIBLDFLAGS = \
    -arch $(ARCHS) \
    -dynamiclib \
    -Wl,-syslibroot,$(SDKROOT) \
    -mmacosx-version-min=$(MACOSX_DEPLOYMENT_TARGET) \
    -Wl,$(LD_NAMESPACE) $(LD_UNDEFS) -Wl,-exported_symbols_list,
endif

ifeq ($(COMPUTER),SOL64)
  IS_MEX_GCC = $(shell $(MATLAB_ROOT)/bin/mex -v 2>&1 | grep CC | grep "= gcc")
  ifneq ($(IS_MEX_GCC),)
      # If mex setup for gcc, use gcc
      ANSI_OPTS = -fPIC
      CPP_ANSI_OPTS = $(ANSI_OPTS)
      CC = gcc -m64
      CPP = g++ -m64
      SHRLIBLDFLAGS = -shared -static-libgcc -Wl,-M,
  else
      # cc is the Sun Compiler, set options appropriately
      DEFAULT_OPT_OPTS = 
      ANSI_OPTS = -dalign -KPIC -xlibmil
      CPP_ANSI_OPTS = $(ANSI_OPTS)
      CC = cc -xarch=v9a
      CPP = CC -xarch=v9a -compat=5
      SHRLIBLDFLAGS = -G -Wl,-M,
  endif
endif

# To create a Quantify (from Rational) build,
# specify
#     QUANTIFY=1
# or
#     QUANTIFY=/path/to/quantify
#
# Note, may also need QUANTIFY_FLAGS=-cachedir=./q_cache
#
ifdef QUANTIFY
  ifeq ($(QUANTIFY),1)
    QUANTIFY_ROOT = /hub/$(ARCH)/apps/quantify
  else
    QUANTIFY_ROOT = $(QUANTIFY)
  endif

  INSTRUMENT_INCLUDES := -I$(QUANTIFY_ROOT)
  INSTRUMENT_LIBS     := $(QUANTIFY_ROOT)/quantify_stubs.a
  LD                  := quantify $(QUANTIFY_FLAGS) $(LD)
  OPT_OPTS            := -g
endif

# To create a Purify (from Rational) build, specify
#   PURIFY=1
#
ifeq ($(PURIFY),1)
  PURIFY_ROOT = /hub/$(ARCH)/apps/purify

  INSTRUMENT_INCLUDES := -I$(PURIFY_ROOT)

  ifeq ($(ARCH), sgi)
    INSTRUMENT_LIBS     := $(PURIFY_ROOT)/purify_stubs_n32.a
  else
    INSTRUMENT_LIBS     := $(PURIFY_ROOT)/purify_stubs.a
  endif

  CC       := purify $(CC)
  OPT_OPTS := -g
endif

# [eof] xenotools.mk
