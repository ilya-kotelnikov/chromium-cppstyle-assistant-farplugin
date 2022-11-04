# Plugin stuff.
#
# The following user-defines are supported:
# DEBUG               - build the library for debugging;
# MINICRT=1           - build the library without CRT for minimal binary size;
# USE_PROTOZOA_STL=1  - build the library with STL replacement classes (makes
#                       sense only without MINICRT).
#
NAME = CCAssistant
MAIN_CPPFILE_TITLE = cc_assistant
SRCS = $(MAIN_CPPFILE_TITLE).cpp \
       config_settings.cpp \
       constants.cpp \
       dlgbuilderex/bindings/color_edit_field_item_binding.cpp \
       dlgbuilderex/bindings/uint_edit_field_item_binding.cpp \
       dlgbuilderex/plugin_dialog_builder_ex.cpp \
       dllmain.cpp \
       editor_filename_match_cache.cpp \
       globals.cpp

ifdef MINICRT
SRCS += protozoa/crt/crt_gcc.cpp \
        protozoa/crt/memory.cpp \
        protozoa/crt/string.cpp
endif

# System stuff (copied and adapted from Far sources).
TOOLSDIR = ./far/buildtools/

ifneq ($(shell echo %comspec%),%comspec%)
 os_name = $(subst /,\,$(1))
 UTILS_PREFIX = $(call os_name,$(TOOLSDIR)gnu.cmd)
 TOOLS_PREFIX = $(TOOLSDIR)
 HOST_TYPE = Windows
else
 os_name = $(1)
 HOST_TYPE = $(shell uname -o)
 ifneq (Msys,$(HOST_TYPE))
  ifneq (Cygwin,$(HOST_TYPE))
   HOST_TYPE = Unix
   WINE_CMD = wine #space required
  endif
 else
  TOOLS_PREFIX = $(TOOLSDIR)
  HOST_TYPE = Windows
 endif
endif

LS=$(UTILS_PREFIX) ls
MKDIR = $(UTILS_PREFIX) mkdir -p
RM = $(UTILS_PREFIX) rm -f
CP = $(UTILS_PREFIX) cp -f
MV = $(UTILS_PREFIX) mv -f

CXX = $(GCC_PREFIX)g++$(DW2)
CC = $(GCC_PREFIX)gcc$(DW2)
WINDRES = $(GCC_PREFIX)windres
DLLTOOL = $(GCC_PREFIX)dlltool
RANLIB = $(GCC_PREFIX)ranlib
AR = $(GCC_PREFIX)ar

ifndef DIRBIT
 ifneq (,$(findstring i686-w64,$(shell $(CC) -dumpmachine)))
  DIRBIT = 32
 else
  ifeq (,$(findstring 64,$(shell $(CC) -dumpmachine)))
   DIRBIT = 32
  else
   DIRBIT = 64
  endif
 endif
endif

ifeq ($(DIRBIT),32)
 WINDRES += -F pe-i386
else
 WINDRES += -F pe-x86-64
endif

USERCPP += -D DIRBIT=$(DIRBIT)

GAWK = $(strip $(call os_name, $(TOOLS_PREFIX)gawk))
DEP_FILTER := $(TOOLSDIR)dep_filter.awk

CXXWIDE = -D UNICODE -D _UNICODE
CCWIDE = -D UNICODE -D _UNICODE
RCWIDE = -D UNICODE -D _UNICODE

ifndef DEBUG
DIRNAME := out.final
C_DEBUG := -Os -fomit-frame-pointer -D NDEBUG
L_DEBUG := -s
else
DIRNAME := out.debug
C_DEBUG := -O0 -g -D _DEBUG
L_DEBUG := -O0 -g
endif

ifdef MINICRT
DIRNAME := $(DIRNAME).minicrt
USERCPP += -D MINICRT -I protozoa/stl -I protozoa/crt
USERRC += -D MINICRT
else
ifdef USE_PROTOZOA_STL
DIRNAME := $(DIRNAME).protozoa_stl
USERCPP += -D USE_PROTOZOA_STL -I protozoa/stl
USERRC += -D USE_PROTOZOA_STL
endif
endif

BASEDIR = ./$(DIRNAME).$(DIRBIT).gcc
OBJDIR := $(BASEDIR)/obj
REOBJDIR = \.\/$(DIRNAME)\.$(DIRBIT)\.gcc\/obj\/
DLLDIR ?= $(BASEDIR)$(ADDOUTDIR)
COMINC = far/pluginapi
COMUNIINC = far/pluginapi/unicode
EXT ?= dll
DLLNAME = $(NAME).$(EXT)
DLLFULLNAME = $(DLLDIR)/$(DLLNAME)

ifndef DEF_NAME
DEF_NAME = $(MAIN_CPPFILE_TITLE)
endif
ifeq ($(DEF_NAME),-)
DEF =
else
DEF = $(DEF_NAME).def
endif

RC_NAME ?= $(MAIN_CPPFILE_TITLE)
RES = $(OBJDIR)/$(MAIN_CPPFILE_TITLE).res.o

MAP = $(patsubst %.$(EXT),%.map,$(DLLFULLNAME))
MAP_OPT = -Xlinker -Map -Xlinker $(MAP)

ifdef MINICRT
STDLIBFLAGS := -nostdlib -e $(if $(findstring 32,$(DIRBIT)),_DllMain,DllMain)
else
STDLIBFLAGS := -static-libgcc
endif

C_FLAGS  := -m$(DIRBIT) $(C_DEBUG) -Wall \
            -funsigned-char -fstrict-aliasing -fno-exceptions
CXXFLAGS := $(C_FLAGS) -std=c++17 -fno-rtti \
            -I . -I $(COMINC) -I $(COMUNIINC) $(CXXWIDE) $(USERCPP)
CCFLAGS  := $(C_FLAGS) \
            -I . -I $(COMINC) -I $(COMUNIINC) $(CCWIDE) $(USERCPP)
LNKFLAGS := -m$(DIRBIT) $(L_DEBUG) $(STDLIBFLAGS) -static -shared \
            -Wl,--kill-at -Wl,--enable-stdcall-fixup $(MAP_OPT) \
            -luser32 -lkernel32 -ladvapi32 -lshell32 -lole32 $(USERLIBS)
RCFLAGS  := -I $(COMINC) -I $(COMUNIINC) $(RCWIDE) $(USERRC)

C_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(SRCS)))
CPP_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(SRCS)))
OBJS = $(strip $(C_OBJS) $(CPP_OBJS) $(RES))
ifndef DISABLEAUTODEPS
C_DEPS = $(patsubst %.c,$(OBJDIR)/%.d,$(filter %.c,$(SRCS)))
CPP_DEPS = $(patsubst %.cpp,$(OBJDIR)/%.d,$(filter %.cpp,$(SRCS)))
DEPS = $(strip $(C_DEPS) $(CPP_DEPS))
endif

all: $(DLLFULLNAME)

ifndef DISABLEAUTODEPS
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
$(OBJDIR)/%.d: %.cpp
	@echo making depends for $<
	@$(MKDIR) $(@D)
	@$(CXX) -c -MM $(CXXFLAGS) $< | \
            $(GAWK) -f $(DEP_FILTER) -v n=$(notdir $(basename $@)) \
                                     -v r=$(patsubst %/,%,$(dir $@)) \
                                                            > $(call os_name,$@)

$(OBJDIR)/%.d: %.c
	@echo making depends for $<
	@$(MKDIR) $(@D)
	@$(CC) -c -MM $(CCFLAGS) $< | \
            $(GAWK) -f $(DEP_FILTER) -v n=$(notdir $(basename $@)) \
                                     -v r=$(patsubst %/,%,$(dir $@)) \
                                                            > $(call os_name,$@)
endif
endif

ifneq ($(MINICRT)$(USE_PROTOZOA_STL),)
protozoa/stl/%:
endif

$(OBJDIR)/%.o: %.cpp
	@echo compiling $<
	@$(MKDIR) -p $(@D)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	@echo compiling $<
	@$(MKDIR) -p $(@D)
	@$(CC) $(CCFLAGS) -c -o $@ $<

$(RES): $(RC_NAME).rc $(COMINC)/plugin_version.rc $(COMINC)/farversion.hpp $(COMUNIINC)/plugin.hpp 
	@echo compiling $<
	@$(MKDIR) -p $(@D)
	@$(WINDRES) $(RCFLAGS) -i $< -o $@

$(DLLFULLNAME): $(OBJS) $(DEF)
	@echo linking $@
	@$(MKDIR) -p $(@D)
	@$(CXX) -o $@ $(DEF) $(OBJS) $(LNKFLAGS)
	@echo copy distrib files
	@$(CP) distrib/* $(DLLDIR)

ifndef SKIPCLEAN
clean:
	@$(RM) -r $(BASEDIR)/obj $(BASEDIR)/lib $(BASEDIR)/Formats $(BASEDIR)/*
endif

ifndef DISABLEAUTODEPS
-include $(DEPS)
endif
