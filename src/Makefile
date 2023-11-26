#
#    Makefile       -    Makefile for MFCX
#
#    Written by John Young, 7 August 2021
#    Copyright (c) 2021 Thistle Software
#

PROGRAM	=	MFCX

.DEFAULT_GOAL := build
.PHONY:	cleanmost clean build test ext-deps check check-xml check-html

# TOOLS
CXX            =    cl.exe
RSC            =    rc.exe
# Note 'LIBEXE' not 'LIB' so as not to clash with the environment variable of the same name.
LIBEXE         =    lib.exe
LINK           =    link.exe
MKDEP          =    mkdep.bat
BSC32          =    bscmake.exe
PERL           =    perl.exe
PYTHON         =    python
SED            =    sed.exe
CPPCHECK       =    cppcheck.exe
COVERAGE       =    OpenCppCoverage.exe

# Check that all tool dependencies are installed.
EXECS := $(CXX) $(RSC) $(BSC32) $(PERL) $(PYTHON) \
         $(SED) $(CPPCHECK) $(COVERAGE)
# Note: Use 'where' instead of 'which' to find .bat files as well.
K := $(foreach exec,$(EXECS),\
          $(if $(shell where $(exec)),some string,$(error "No '$(exec)' in PATH")))

# Pull out CPPCHECK_HOME and _HTMLREP from the CPPCHECK app location.
CPPCHECK_PATH  =    $(shell which "$(CPPCHECK)")
CPPCHECK_DIR   =    $(shell dirname "$(CPPCHECK_PATH)")
CPPCHECK_HOME  =    $(shell cygpath -m "$(CPPCHECK_DIR)")
CPPCHECK_HTMLREP=   $(CPPCHECK_HOME)/htmlreport/cppcheck-htmlreport

GTEST_VER      = 1.13.0
GTEST_BASE     = $(GTEST_HOME)/gtest-$(GTEST_VER)
GTEST_INC      = $(GTEST_BASE)/include
GTEST_LIB      = $(GTEST_BASE)/lib

# SUBROUTINES

.SUFFIXES:

ifneq ($(MAKECMDGOALS), test)
  ifneq ($(MAKECMDGOALS), check)
    ifneq ($(MAKECMDGOALS), coverage)
      # The default configuration...	
      ifeq ("$(CFG)", "")
        CFG  =    Debug
        $(warning No configuration specified. Defaulting to '$(CFG)'.)
      endif
      
      # Configuration check - put all the valid configurations here...
      ifneq "$(CFG)" "Release"
        ifneq "$(CFG)" "Debug"
          $(error Invalid configuration "$(CFG)" specified.  Use 'Release' or 'Debug')
        endif # CFG = Debug
      endif # CFG = Release
    endif # goal = coverage
  endif # goal = check
endif # goal = test

# Configuration dependent settings...
OUTDIR	  = ../lib
INTDIR	  =	../builds/$(CFG)
DEPDIR	  =	../builds/dep
TESTDIR   = ../builds/test
CHECKDIR  = ../builds/cppcheck

# FLAGS - common to _DEBUG and release builds.
# See https://docs.microsoft.com/en-us/cpp/build/reference/compiling-a-c-cpp-program?view=vs-2019 for more information.
# TODO: Check /GX is deprecated - use /EH instead.

# C++ Flags:
# - /errorReport:prompt - deprecated (use WEP instead)
# - /EHsc - Enables standard C++ stack unwinding / extern "C" functions don't throw C++ exceptions.
# - /Fd  - PDB directory.
# - /fp:precise - Floating point precise.
# - /GS Buffer security check
# - /Gd  - __cdecl calling convention.
# - /Oy- : Disable frame-pointer omission.
# - /W3  - level 3 warnings (production).
# - /WX- - Treat all warnings as errors.
# - /Zc:
#     - wchar_t - wchar_t is a native type not a typedef.
#     - inline  - Remove unreferenced functions.
#     - forScope- Enforce C++ for scoping rules.
# - /Zi  - Produce a separate PDB.
CXX_CMN_FLAGS = -Gd -GS -analyze- -Oy- -W3 -Zc:wchar_t,inline,forScope \
                -D "WIN32" -D "_WINDOWS" -D "MFCX_LOG_USELOG" \
                -D "_MBCS" -D "_AFXDLL" -D "GTEST" -WX- -Zi -fp:precise \
                -errorReport:prompt -EHsc -nologo \
                -Fd"$(INTDIR)/$(PROGRAM).pdb" -Fp"$(INTDIR)/$(PROGRAM).pch" \
                -diagnostics:classic -c

RSC_CMN_FLAGS = -D "_AFXDLL" -l 0x0409 -nologo -fo"$(INTDIR)/$(PROGRAM).res" 

DEPFLAGS	=	-q

ifeq ("$(CFG)", "Debug")

  TARGET    =    $(OUTDIR)/$(PROGRAM)sd.lib

  # -FR:<bsc-dir> browser info
  # -Od   - disable optimisations.
  # -RTC1 - run-time error checking optimised for debugging.
  # -MDd  - use debug libraries.
  CXXFLAGS  = $(CXX_CMN_FLAGS) -Od -D "_DEBUG" -RTC1 -MDd \
              -FR:"$(INTDIR)/" 
         		# -Gy -Gm-
  RSC_FLAGS = $(RSC_CMN_FLAGS) -D "_DEBUG"
 
  BSC32_FLAGS=-nologo 
  LIB_FLAGS = -OUT:"$(OUTDIR)/MFCXsd.lib" -MACHINE:X86 -NOLOGO
  # -LIST:"$(INTDIR)/MFCX.lst" - doesn't work
              
else ifeq ("$(CFG)", "Release")

  TARGET    =    $(OUTDIR)/$(PROGRAM)s.lib

  # -Gy  : function level linking.
  # -Gm- : Switch off minimum rebuild.
  # -O2  : Optimise for max speed.
  # -GF  : Eliminate Duplicate Strings
  # -WX- : Treat warnings as errors.
  # -MD  : Multithreaded DLLs.
  CXXFLAGS  = $(CXX_CMN_FLAGS) -O2 -D "NDEBUG" -GF -MD
  RSC_FLAGS = $(RSC_CMN_FLAGS) -D "NDEBUG" 
  BSC32_FLAGS=-o"$(OUTDIR)/MFCX.bsc" -nologo 
  LIB_FLAGS = -OUT:"$(OUTDIR)/MFCXs.lib" -NOLOGO 

endif

CPPCHECK_FLAGS = --quiet \
                 --library="$(CPPCHECK_HOME)/cfg/mfc.cfg" \
                 --library="$(CPPCHECK_HOME)/cfg/windows.cfg" \
                 -D "WIN32" -D "_DEBUG" -D "_WINDOWS" -D "_AFXDLL" -D "_MBCS" \
                 -D "__AFXWIN_H__" -U "NDEBUG" -U "BETA" \
                 --enable=all --platform=win64 --std=c++14 --inconclusive \
                 --inline-suppr --suppress=unusedFunction
                 
#--check-config

SRC       =    $(filter-out MFCX_DLL.cpp, $(wildcard *.cpp)) $(wildcard regex/*.cpp)
# Remove StdAfx.obj from the object list, because it's a special case - the precompiled header.
OBJS      =    $(addprefix $(INTDIR)/, $(filter-out StdAfx.obj, $(SRC:.cpp=.obj)))
DEPS      =    $(addprefix $(DEPDIR)/, $(SRC:.cpp=.d))

SRC_RES   =    $(PROGRAM).rc $(wildcard res/*)
RES       =    $(INTDIR)/$(PROGRAM).res

SBRS      =    $(addprefix $(INTDIR)/, StdAfx.sbr $(notdir $(OBJS:.obj=.sbr)))

PCH       =    $(INTDIR)/$(PROGRAM).pch
BSCFILE   =    $(OUTDIR)/$(PROGRAM).bsc 
PDBFILE   =    $(OUTDIR)/$(PROGRAM).pdb

# TARGETS

ifeq ("$(CFG)", "Debug")
  build   : $(TARGET) $(BSCFILE) $(PDBFILE)
else
  build   : $(TARGET)
endif

cleanmost:
	@rm -fr "$(OUTDIR)"
	@rm -fr "$(INTDIR)"
	@rm -f $(PROGRAM).dep
	@rm -f $(PROGRAM).opt
	@rm -f $(PROGRAM).plg
	@rm -f LastCoverageResults.log


clean: cleanmost cleantest
	@echo Cleaning $(PROGRAM) $(CFG) temporary files
	@rm -fr "$(DEPDIR)"
	@rm -fr "$(TESTDIR)"
	@rm -f $(PROGRAM).aps
	@rm -f $(PROGRAM).plg
	@rm -f $(PROGRAM)Test.plg
	@rm -f $(PROGRAM).ncb


# Dependency files 
$(DEPS)   :    $(DEPDIR)/%.d  :    %.cpp
	@echo Making $@ from $<
	@mkdir -p "$(@D)"
	@$(MKDEP) $(DEPFLAGS) $< | $(SED) "s!^\(.*\)\.cpp : !$(INTDIR)/\1.obj : !" > $@


$(OBJS)	:	$(PCH)

# Resource files
$(RES) : $(SRC_RES)

# Make the pre-compiled header file.
$(INTDIR)/StdAfx.obj $(PCH) 		:	StdAfx.h StdAfx.cpp
	@echo "Creating the precompiled header (making $(@D))"
	@mkdir -p "$(@D)"
	@"$(CXX)" $(CXXFLAGS) -Yc"StdAfx.h" -Fo$@ StdAfx.cpp

# Rules making .obj from sources and puts them flat in $(INTDIR)
$(OBJS)	:	$(INTDIR)/%.obj	:	%.cpp
	@echo "Building $@ from $< (making $(@D))"
	@mkdir -p "$(@D)"
	@"$(CXX)" $(CXXFLAGS) -Yu"StdAfx.h" -Fo$@ $<

#$(SBRS)	:	$(INTDIR)/%.sbr	:	%.cpp
#	@echo Creating $@ from $<
#	@if not exist "$(@D)" mkdir "$(@D)"
#	@"$(CXX)" $(CXXFLAGS) /Yu"StdAfx.h" /Fo$@ $<

$(INTDIR)/%.res	:	%.rc
	@echo Building resource $@ from $<
	@$(RSC) $(RSC_FLAGS) -fo$@ $<

$(TARGET) : $(INTDIR)/StdAfx.obj $(OBJS) $(RES)
	@echo "Creating library $@ (making $(OUTDIR))"
	@mkdir -p "$(OUTDIR)"
	@$(LIBEXE) $(LIB_FLAGS) $(INTDIR)/StdAfx.obj $(OBJS) $(RES)
	
	
$(BSCFILE) : 
	@echo Create browser information
	@$(BSC32) $(BSC32_FLAGS) -o$(OUTDIR)/$(PROGRAM).bsc $(SBRS)

$(PDBFILE) : 
	@cp -p $(INTDIR)/$(PROGRAM).pdb $(OUTDIR)

## Testing below

TEST_PROGRAM   =  $(PROGRAM)Test
TEST_PCH       =  $(TESTDIR)/$(TEST_PROGRAM).pch
TEST_TARGET    =  $(TESTDIR)/$(TEST_PROGRAM).exe


# See https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=vs-2019
# for an explanation of the options.
# Note, no space allowed between options and files. For example: '/Fp$(TEST_PCH)' works, '/Fp $(TEST_PCH)' doesn't. :-(
CONSOLE_CXXFLAGS = -nologo -MDd -W3 -EHsc -GR -Zi -Od -I "$(GTEST_INC)" \
                   -D "WIN32" -D "_DEBUG" -D "_CONSOLE" -D "_MBCS" \
                   -D "_AFXDLL"  -D "GTEST" \
                   -Fp$(TEST_PCH) -Fd"$(TESTDIR)/" -FD -RTC1 -c

# NOTE: To find unresolved external symbols, navigate to the VC98/Lib directory and run this command...
#       nm -A *.lib 2> /dev/null | grep <missing-symbol>
CONSOLE_LINK_FLAGS= shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib \
                    comdlg32.lib advapi32.lib shell32.lib ole32.lib \
                    oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib \
                    winmm.lib gtestsd.lib gmocksd.lib -nologo -subsystem:console \
                    -pdb:"$(TESTDIR)/$(TEST_PROGRAM).pdb" -debug \
                    -libpath:"$(GTEST_LIB)" -machine:I386

# Same as SCR but without all files that reference MFCX_DLL.cpp.
TEST_SRC_DIRS :=   $(addprefix ../test/, $(SRC_DIRS)) ../test/win32
UNITTEST_SRC  :=   $(wildcard ../test/*.cpp) $(foreach dir,$(TEST_SRC_DIRS),$(wildcard $(dir)/*.cpp))

UNITTEST_DEPS :=   $(UNITTEST_SRC:.cpp=.d)

TEST_SRC      :=   $(UNITTEST_SRC) Crypt.cpp Directory.cpp DataQueue.cpp \
                    EnvVars.cpp Filename.cpp MsgPoster.cpp \
                    Regex.cpp RegexString.cpp RegKey.cpp StringUtil.cpp \
                    SysError.cpp Thread.cpp Utils.cpp XString.cpp \
                    XStringList.cpp XTime.cpp \
                    regex/pcre.cpp regex/chartables.cpp

# TODO: The test objects look like this: ../builds/test/../test/CryptTest.obj, 
#       that is, with that weird, redundant test/../test in the path.
TEST_OBJS     :=   $(addprefix $(TESTDIR)/, $(TEST_SRC:.cpp=.obj))
TEST_DEPS     :=   $(addprefix $(DEPDIR)/, $(notdir $(UNITTEST_DEPS)))


# TEST ----------------------------------
TXT_PATH = ../builds/exp-deps

#DOS_PATH  = $(subst /,\,$(TXT_PATH))
DOS_PATH  = $(call dospath,$(TXT_PATH))
UNIX_PATH = $(call unixpath,$(TXT_PATH))
# ---------------------------------------

# Dependency files 
$(TEST_DEPS)   :    $(DEPDIR)/%.d  :    ../test/%.cpp
	@echo Making $@ from $<
	@mkdir -p "$(@D)"
	@$(MKDEP) $(DEPFLAGS) $< | $(SED) "s!^\(.*\)\.cpp : !$(TESTDIR)/\1.obj : !" > $@


$(TEST_OBJS)   :    $(TEST_PCH)

# Make the pre-compiled header file.
$(TESTDIR)/StdAfx.obj $(TEST_PCH) 	:	StdAfx.h StdAfx.cpp
	@echo "Creating the test precompiled header (creating $(@D))"
	@mkdir -p "$(@D)"
	@$(CXX) $(CONSOLE_CXXFLAGS) -Yc"StdAfx.h" -Fo$(TESTDIR)/StdAfx.obj StdAfx.cpp

# Make the other object files...
$(TEST_OBJS)	:	$(TESTDIR)/%.obj	:	%.cpp
	@echo "Building test $@ from $< (making $(@D))"
	@mkdir -p "$(@D)"
	@$(CXX) $(CONSOLE_CXXFLAGS) -Yu"StdAfx.h" -Fo$@ $<

# TODO: Can we remove the source objects and just link to the .lib file instead?
$(TESTDIR)/$(TEST_PROGRAM).exe : $(TEST_OBJS)
	@echo "Creating $@"
	@$(LINK) $(CONSOLE_LINK_FLAGS) -out:$@ $(TESTDIR)/StdAfx.obj $(TEST_OBJS)

ifeq "$(tests)" ''
     # TestVolume should run within 200 ms but takes way over that when running
     # coverage.
     ifeq ($(MAKECMDGOALS), coverage)
       tests_filter :=  --gtest_filter=-DataQueueTest.TestVolume
     else
       tests_filter :=
     endif
else
     tests_filter :=  --gtest_filter=$(tests)
endif

test : $(TEST_TARGET)
	@mkdir -p "$(TESTDIR)/results"
	@$(TESTDIR)/$(TEST_PROGRAM).exe \
	     --gtest_output="xml:$(TESTDIR)/results/$(PROGRAM)Test-results.xml" \
	     $(tests_filter)

cleantest : 
	@echo Cleaning test files
	@rm -fr "$(TESTDIR)"

$(CHECKDIR)/cppcheck.xml : $(SRC)
	@echo Running checks...
	@mkdir -p "$(CHECKDIR)"
	@"$(CPPCHECK)" $(CPPCHECK_FLAGS) --xml --xml-version=2 --output-file=$@ .

check-xml : ext-deps $(CHECKDIR)/cppcheck.xml

check-html : check-xml
	@echo Creating checks HTML
	@"$(PYTHON)" "$(CPPCHECK_HTMLREP)" --file=$(CHECKDIR)/cppcheck.xml \
	             --report-dir=$(CHECKDIR)/html --source-dir=. \
	             --title="$(PROGRAM)" --blame-options=w

check     : check-html

coverage: $(TEST_TARGET) 
	@echo Generating test coverage statistics...
	@mkdir -p "../builds/coverage"
	@"$(COVERAGE)" -v --config_file ../test/OpenCppCoverage.cfg -- \
	               $(TESTDIR)/$(TEST_PROGRAM).exe $(tests_filter) \
	               2> ../builds/coverage/OpenCppCoverage.err

trace:
	@echo CXXFLAGS   = $(CXXFLAGS)
	@echo SRC        = $(SRC)
	@echo TEST_OBJS  = $(TEST_OBJS)
	@echo SBRS       = $(SBRS)
	@echo LIBEXE     = $(LIBEXE)
	@echo LIB_FLAGS  = $(LIB_FLAGS)
	@echo RES        = $(RES)
	

# INCLUDES
ifneq ($(MAKECMDGOALS), clean)
  ifneq ($(MAKECMDGOALS), cleanmost)
    ifneq ($(MAKECMDGOALS), cleantest)
      ifneq ($(MAKECMDGOALS), trace)
        -include $(DEPS)
      endif
    endif
  endif
endif

ifeq ($(MAKECMDGOALS), test)
  -include $(TEST_DEPS)
endif



# vi:ts=5