################################################
#
# Bing, Copyright (C) 1998-2000 Jean-Baptiste M. Queru
# 
# This program is distributed under the terms of the enclosed
# JBQ code license.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# License for more details.
# 
# Contact the author at:
# Jean-Baptiste M. Queru, 31 Camino Alto, Millbrae CA 94030, USA
# or by e-mail at : jb@queru.com
#
################################################

# the file name, ...

BINARY := Bing
VERSION := 0.4.0.2
LIBS := -lbe

################################################

# output directories

OBJDIR := obj
DEPDIR := dep
SRCDIR := src
DATADIR := data

# compiler, linker, ...

CC := gcc -c -pipe
CFLAGS :=
CFLAGS += -O3
CFLAGS += -fno-pic
CFLAGS += -fomit-frame-pointer
#CFLAGS += -ffast-math
#CFLAGS += -mno-ieee-fp
CFLAGS += -Wall -W -Wno-multichar -Wp,-Wall
CFLAGS += -Werror
CFLAGS += -DVERSION_STRING=\"$(VERSION)\" -DBINARY_NAME=\"$(BINARY)\"

LD := gcc
LDFLAGS := $(LIBS)

DEP := gcc -MM

ZIP := zip -r -9 -y

################################################

# the engine

MAKEFILE := GNUmakefile

FULLNAME := $(subst \ ,_,$(BINARY))-$(VERSION)

DATA := $(shell find $(DATADIR) -type f)

EXTRADATA:= $(wildcard data.zip)

BASESOURCES := $(shell cd $(SRCDIR) && ls -1 *.cpp)
SOURCES := $(addprefix $(SRCDIR)/,$(BASESOURCES))
OBJECTS := $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(BASESOURCES))))
DEPENDS := $(addprefix $(DEPDIR)/,$(addsuffix .d,$(basename $(BASESOURCES))))

BASEHEADERS := $(shell cd $(SRCDIR) && ls -1 *.h)
HEADERS := $(addprefix $(SRCDIR)/,$(BASEHEADERS))

BASEAPPINFO := $(shell cd $(SRCDIR) && ls -1 *.ai)
APPINFO := $(addprefix $(SRCDIR)/,$(BASEAPPINFO))

.PHONY : default clean binarchive sourcearchive all

.DELETE_ON_ERROR : $(BINARY)

default : $(BINARY)

clean :
	@echo cleaning
	@rm -rf $(BINARY) $(OBJDIR) $(DEPDIR) $(filter-out data.zip,$(wildcard *.zip)) *.zip~

all : sourcearchive binarchive

sourcearchive : $(FULLNAME)-src.zip

binarchive : $(FULLNAME)-$(BE_HOST_CPU).zip

$(BINARY) : $(OBJECTS) $(APPINFO)
	@echo linking $@
	@$(LD) $(LDFLAGS) $(OBJECTS) -o "$@"
	@echo adding app info to $@
	@addappinfo $(APPINFO) "$@"
	
$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@echo compiling $@
	@mkdir -p $(OBJDIR)
	@$(CC) $< $(CFLAGS) $(CFLAGS.default) -o $@

$(DEPDIR)/%.d : $(SRCDIR)/%.cpp
	@echo generating dependencies for $<
	@mkdir -p $(DEPDIR)
	@$(DEP) $< > $@
	@echo -e yf.i$(OBJDIR)/\\033t:a$(DEPDIR)/\\033pad \\033f:a $(MAKEFILE)\\033ZZ | vi $@ >/dev/null 2>/dev/null

$(FULLNAME)-$(BE_HOST_CPU).zip : $(BINARY) $(DATA) $(EXTRADATA) $(MAKEFILE)
	@rm -rf $@~
	@mkdir -p $@~/$(FULLNAME)-$(BE_HOST_CPU)/
	@copyattr -d -r $(BINARY) $(DATADIR)/* $@~/$(FULLNAME)-$(BE_HOST_CPU)/
ifneq "$(EXTRADATA)" ""
	@unzip $(EXTRADATA) -d $@~/$(FULLNAME)-$(BE_HOST_CPU)/
endif
	@find $@~ -type f | xargs mimeset
	@cd $@~ && $(ZIP) $@ $(FULLNAME)-$(BE_HOST_CPU)
	@mv -f $@~/$@ .
	@rm -rf $@~

$(FULLNAME)-src.zip : $(SOURCES) $(HEADERS) $(DATA) $(EXTRADATA) $(MAKEFILE)
	@rm -rf $@~
	@mkdir -p $@~/$(FULLNAME)-src/
	@copyattr -d -r $(SRCDIR) $(DATADIR) $(MAKEFILE) $@~/$(FULLNAME)-src/
ifneq "$(EXTRADATA)" ""
	@copyattr -d -r $(EXTRADATA) $@~/$(FULLNAME)-src/
endif
	@find $@~ -type f | xargs mimeset
	@cd $@~ && $(ZIP) $@ $(FULLNAME)-src
	@mv -f $@~/$@ .
	@rm -rf $@~

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),depend)
ifneq ($(MAKECMDGOALS),sourcearchive)
include $(DEPENDS)
endif
endif
endif
