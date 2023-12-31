#
#    Makefile for MFCX's documentation
#
#    Written by John Young, 23 September 2022
#    Copyright (c) 2022 Thistle Software
#

PROGRAM   =    MFCX

.PHONY: clean doxygen

# Tools
DOX    = doxygen.exe

# Default to the full format (which takes time).
ifeq ("$(FORMAT)", "")
  FORMAT = full
endif


ifeq ($(MAKECMDGOALS), doxygen)
  ifneq ("$(FORMAT)", "full")
    ifneq ("$(FORMAT)", "mini")
      $(error Invalid FORMAT "$(FORMAT)" specified.  Please use 'mini', or 'full' (the default))
    endif
  endif
endif

# Parse out the version and year from the Version.h file...
#define   VER_STRING     "2, 2, 0, 1175\0"
#define   VER_COPYRIGHT  "Copyright � 2019 Thistle Software\0"
#VER_CMD   = $(SED) -rn "/VER_STRING/s/^.*\"([0-9]+), ([0-9]+), ([0-9]+), ([0-9]+).*/\1.\2.\3.\4/p" ../src/Version.h
#YEAR_CMD  = $(SED) -rn "s/^.* ([0-9]+) Thistle Software.*/\1/p" ../src/Version.h

# PRED-656: use these to populate the PROJECT_NUMBER in the doxyfile.
#VERSION   = $(shell $(VER_CMD))
#YEAR      = $(shell $(YEAR_CMD))

# TODO: Add MFCX.chm file.  GENERATE_HTMLHELP = YES
BUILDS_DIR   =    ../../builds/doc
ifeq ($(FORMAT), mini)
  DOXYFILE   =    $(PROGRAM)-mini.dox
  HTML_DIR   =    $(BUILDS_DIR)/html-mini
else
  DOXYFILE   =    $(PROGRAM).dox
  HTML_DIR   =    $(BUILDS_DIR)/html
endif

LOG       =    $(BUILDS_DIR)/$(PROGRAM)-dox.log
ERR       =    $(BUILDS_DIR)/$(PROGRAM)-dox.err


#$(DOXYFILE) : ../../src/Version.h
SRC_DIR   =    ../../src
SRC       =    $(filter-out MFCX_DLL.cpp, $(wildcard $(SRC_DIR)/*.cpp)) \
               $(wildcard $(SCR_DIR)/regex/*.cpp)

DOXYFILES = $(DOXYFILE) doxygen.css History.html MFCX-MainPage.dox \
            MFCXHeader.html MFCXFooter.html

$(HTML_DIR)/index.html : $(DOXYFILES) $(SRCS)

doxygen : $(HTML_DIR)/index.html
	@echo "Running doxygen ($(FORMAT)) for $(PROGRAM)"
	@mkdir -p $(HTML_DIR)
	@cp -p History.html $(HTML_DIR)
	@echo "See $(LOG) for messages"
	@echo "See $(ERR) for errors"
	@$(DOX) $(DOXYFILE) > $(LOG) 2> $(ERR)

clean:
	@rm -rf $(BUILDS_DIR)
	@rm -f $(LOG)
	@rm -f $(ERR)

trace:
	@echo VERSION = $(VERSION)
	@echo YEAR    = $(YEAR)
	@echo DOX     = $(DOX)
	@echo DOXYFILE = $(DOXYFILE)
