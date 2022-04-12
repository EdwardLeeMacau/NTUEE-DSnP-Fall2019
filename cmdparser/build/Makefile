################################################################################
# Build tools and commands
################################################################################

MAKEFLAGS += -j

################################################################################
# Build tools and commands
################################################################################

CC := gcc
CX := g++
AR := ar
RM := rm
LN := ln
MKDIR := mkdir
RMDIR := rmdir
MKDIR_P := mkdir -p

### Then, define global compilation and linking flags.

DEFINE_ALL :=
CFLAGS_ALL :=
LFLAGS_ALL :=

### Finally, define complete commands for all actions.

COMPILE = $(addprefix -D,$(DEFINE_ALL)) $(addprefix -D,$(DEFINE_LOCAL)) $(CFLAGS_ALL) $(CFLAGS_LOCAL) -o $@ -c $<
LINK = -o $@ $^ $(LFLAGS_ALL) $(LFLAGS_LOCAL)
ARCHIVE_STATIC = $(AR) rcsD $@ $^
ARCHIVE_SHARED = -shared -Wl,--build-id -Wl,--soname,$(basename $(basename $(notdir $@))) -o $@ \
                 -Wl,--whole-archive $^ -Wl,--no-whole-archive

################################################################################
# Build rules
################################################################################

### First, define default build target.

.PHONY: all
all: primary

### Second, define variables for collecting outputs and temporarily objects.

TARGET_BINS   :=
TARGET_DOCS   :=
TARGET_SLIBS  :=
TARGET_DLIBS  :=
TARGET_TESTS  :=
CLEAN_FILES   :=
CLEAN_DIRS    :=
CLEAN_TARGETS :=

### Then, collect the knowledge about how to build all outputs.

root := $(realpath ../)
dir := $(root)/build

BINDIR = $(root)/bin

subdir := $(root)/src
include $(subdir)/Rules.mk

################################################################################
# Dependency
################################################################################

# TODO

################################################################################
# Targets (Must be placed after "Subdirectories" section)
################################################################################

.PHONY:	primary
primary: $(TARGET_SLIBS) $(TARGET_DLIBS) $(TARGET_BINS)

.PHONY: test
test: primary $(TARGET_TESTS)

.PHONY: clean
clean:
	@printf "  %-8s$(root)\n" "CLEAN"
	$(Q)$(RM) -f $(CLEAN_FILES)
	$(Q)$(foreach d, $(CLEAN_DIRS), $(RM) -rf $(d); )

.PHONY: distclean
distclean: clean

.PHONY: help
help:
	@echo ""
	@echo "  Syntax:"
	@echo "    make [options] <target>"
	@echo ""
	@echo "  Options:"
	@echo "    V=0         Hide command lines. (default)"
	@echo "    V=1         Show command lines."
	@echo ""
	@echo "  Targets:"
	@echo "    all         Build primary outputs. (default)"
	@echo "    test        Build test programs."
	@echo "    clean       Clean all outputs."
	@echo "    distclean   Clean all outputs and configurations."
	@echo "    help        Show help messages."
	@echo ""

################################################################################
# General rules
################################################################################

%.o: %.c
	@printf "  %-8s$@\n" "CC"
	$(Q)$(CC) $(COMPILE)

%.o: %.cpp
	@printf "  %-8s$@\n" "CX"
	$(Q)$(CX) $(COMPILE)

$(BINDIR):
	$(Q)$(MKDIR_P) $@

################################################################################
# Quiet options
################################################################################

V ?= 0
ifeq ($(V),1)
	Q :=
	VOUT :=
else
	Q := @
	VOUT := 2>&1 1>/dev/null
endif

################################################################################
# Prevent make from removing any build targets, including intermediate ones
################################################################################

.SECONDARY: $(CLEAN_FILES) $(CLEAN_DIRS)
