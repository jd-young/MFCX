#
#    Makefile for MFCX
#
#    Written by John Young, 7 August 2021
#    Copyright (c) 2021 Thistle Software
#
## USAGE
#    make [-f <make-file>] [CFG = <config>] [<target>]
#
#    where 
#         -f <make-file>      Specifies the make file to use.  Normally, this 
#                             is omitted, in which case make looks for 
#                             'GNUmakefile', 'makefile', and `Makefile', in 
#                             that order, and use the first of these three.
#                             This file is called 'Makefile', so make will 
#                             probably use this one!
#
#         CFG = <config>      Specifies the configuration to build.  These can 
#                             be 'Debug' or 'Release'.  If this omitted, then 
#                             the default is 'Debug'.
#
#         <target>            The target.  If omitted, then the default target 
#                             is used (help).

## TARGETS
#
#    'check'        Runs source code checks using cppcheck.
#
#    'clean'        Cleans all the intermediate files for a given configuration.
#
#    'cleanall'     Cleans all files associated with creating a 'dist' package, 
#                   for all configurations.
#
#    'cleanmost'    Cleans common files, but leaves some files intact (.pch, 
#                   etc).
#
#    'help'         Prints the makefile's help - a brief, one-line description 
#                   of each target.
#
#    'lib'          Build the MFCX.lib file.
#
#    'package'      Build the MFCX.zip file, which contains the includes and 
#                   libs.
#
#    'targets'      Prints out this page.
#
#    'trace'        Prints out information that is useful for debugging 
#                   problems. 
#
#    'usage'        Prints out the usage page.
#

# TODO: targets
#    'chm'          Creates the compiled help file for MFCX: MFCX.chm.
#                   This is the same for all configurations, and so if you build
#                   with CFG=Debug, and then CFG=Release, then nothing further
#                   will be done.
#
#    'coverage'     Creates a test coverage report.
#
#    'doc'          Builds the documentation for MFCX (doxygen, and anything 
#                   else that is relevant).
#
#    'package'      Creates a zip distribution package for the MFCX application
#                   containing the binaries and source.
#
#    'test'         Runs all the tests for the MFCX project.  Continuous 
#                   integration tools will typically run this when updates are 
#                   made to the project's source code control (currently Git).
#


ifneq ("$(MINGW_PREFIX)", "")
     $(error Please run under a Windows Console (not mingw64).)
endif

PROGRAM	=	MFCX

.DEFAULT_GOAL := help

# TOOLS

AWK  = awk.exe
GREP = grep.exe
SED  = sed.exe
SORT = sort.exe

7Z_VER = 22.01
7Z     = $(7ZIP_HOME)/7-Zip-$(7Z_VER)/7z

.PHONY: cleanmost clean cleanall help lib chm doc build deploy test

# TARGETS

targets: ## Prints a full description of the targets.
	@$(SED) -n "/## TARGET/,/^$$/ s/^##\? \?\(.*\)/\1/p" Makefile

usage: ## Prints the usage message.
	@$(SED) -n "/## USAGE/,/^$$/ s/^##\? \?\(.*\)/\1/p" Makefile


help: ## Prints out this help message
	@echo Welcome to the $(PROGRAM) project makefile
	@echo Targets:
	@$(SED) -n "s/\(^[a-z]*\) *:.* ## *\(.*\)/\1:\2/p" Makefile | \
	     $(AWK) -F: '{ printf "   %-10s %s\n", $$1, $$2 }' | \
	     sort

cleanmost: ## Cleans most common files, but leaves rarely changed files (.pch, etc).
	@cd src && $(MAKE) cleanmost
	@cd doc && $(MAKE) clean

clean: ## Cleans the project of intermediate files for a given configuration.
	@cd src && $(MAKE) clean
	@cd doc && $(MAKE) clean

cleanall: ## Cleans all generated files and directories for Debug and Release builds.
	@cd src && $(MAKE) clean CFG=Debug
	@cd src && $(MAKE) clean CFG=Release
	@cd doc && $(MAKE) clean
	@rm -fr lib
	@rm -fr builds

lib: ## Builds the MFCX.lib file.
	@cd src && $(MAKE) build

package: lib
	@"$(7Z)" a -tzip builds/mfcx.zip lib/* include/*

doc:   ## Creates the documentation (including doxygen).
	@cd doc && $(MAKE) doc

test: ## Runs all the unit tests for MFCX.
	@cd src && $(MAKE) test

check: ## Runs source code checks.
	@cd src && $(MAKE) check

coverage: ## Creates a test coverage report.
	@cd src && $(MAKE) coverage

trace: ## Prints out useful information that helps with testing / debugging.
	@echo PROGRAM  :  $(PROGRAM)
	@cd src && $(MAKE) trace
