#############################################################################
# Project    : ZenomMatlab
# Created on : 
# 
#############################################################################

# Zenom
ZENOM_INCLUDES   = -I/usr/include/zenom
ZENOM_LFLAGS     = -L/usr/lib64
ZENOM_LIBS       = -lznm-controlbase 

# Xenomai
XENOMAI_CFLAGS   = -D_GNU_SOURCE -D_REENTRANT -D__XENO__
XENOMAI_INCLUDES = -I/usr/xenomai/include
XENOMAI_LFLAGS   = -L/usr/xenomai/lib  
XENOMAI_LIBS     = -lnative -lxenomai

# Compiler, tools and options
CXX           = g++
CXXFLAGS      = -pipe -O2 -Wall $(XENOMAI_CFLAGS)
INCLUDES      = -I. $(XENOMAI_INCLUDES) $(ZENOM_INCLUDES) 
LFLAGS        = $(XENOMAI_LFLAGS) $(ZENOM_LFLAGS)
LIBS          = $(XENOMAI_LIBS) $(ZENOM_LIBS)

# Files
SOURCES       = main.cpp
OBJECTS       = $(SOURCES:.cpp=.o)
TARGET        = ZenomMatlab

# Build rules
$(TARGET):  $(OBJECTS)  
	$(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET)

# Compile
.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

