################################################################################
# [DON'T TOUCH] Traverse directory tree
################################################################################

sp             := $(sp).x
dirstack_$(sp) := $(dir)
dir            := $(subdir)

################################################################################
# Subdirectories, in random order
################################################################################

subdir := $(dir)/cmdparser
include $(subdir)/Rules.mk

subdir := $(dir)/util
include $(subdir)/Rules.mk

################################################################################
# [EDIT] Specify source files and include directories
################################################################################

SRCS_$(dir) := $(wildcard $(dir)/*.cpp)
INCS_$(dir) := $(root)/src/cmdparser

################################################################################
# [DON'T TOUCH] Calculate corresponding object files and auto-dependencies
################################################################################

OBJS_$(dir) := $(SRCS_$(dir):.cpp=.o)
DEPS_$(dir) := $(SRCS_$(dir):.cpp=.d)

$(OBJS_$(dir)):	CFLAGS_LOCAL := $(addprefix -iquote,$(INCS_$(dir)))

-include $(DEPS_$(dir))

################################################################################
# Targets
################################################################################

TARGET_BIN_$(dir) := $(BINDIR)/cmdreader

$(TARGET_BIN_$(dir)): $(OBJS_$(dir)/cmdparser) $(OBJS_$(dir)/util) $(OBJS_$(dir)) | $(BINDIR)
	@printf "  %-8s$@\n" "CXLD"
	$(Q)$(CX) $(LINK)

TARGET_BINS += $(TARGET_BIN_$(dir))
CLEAN_FILES += $(TARGET_BIN_$(dir))
CLEAN_DIRS  += $(BINDIR)

################################################################################
# Traverse directory tree
################################################################################

dir		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
