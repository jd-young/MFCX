#
#    Makefile       -    Makefile for MFCX
#
#    Written by John Young, 7 August 2021
#    Copyright (c) 2021 Thistle Software
#

PROGRAM	=	MFCX

.DEFAULT_GOAL := build
.PHONY:	cleanmost clean build test ext-deps check check-xml check-html

# PRED-654: Figure out how to get rid of hard-coded library path
# EXTERNAL DEPENDENCIES

# TOOLS
CXX            =    cl.exe
RSC            =    rc.exe
# Note 'LIBEXE' not 'LIB' so as not to clash with the environment variable of the same name.
LIBEXE         =    lib.exe
MKDEP          =    mkdep
BSC32          =    bscmake.exe
PERL           =    perl.exe
PYTHON         =    C:\Users\jyoung\AppData\Local\Programs\Python\Python37-32\python.exe
SED            =    sed.exe
CPPCHECK_HOME  =    C:\Program Files\Cppcheck
CPPCHECK       =    $(CPPCHECK_HOME)\cppcheck.exe
CPPCHECK_HTMLREP=   $(CPPCHECK_HOME)\src\cppcheck-1.89\htmlreport\cppcheck-htmlreport
COVERAGE       =    C:\Program Files\OpenCppCoverage\OpenCppCoverage.exe

# Global includes / libraries
HTMLHELP 	= C:\Program Files (x86)\HTML Help Workshop

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
# - /Zi  - Produce a separate PDB.

# There is a dependency on expat.
# Common
CXX_CMN_FLAGS = /Gd /GS /analyze- /Oy- /W3 /Zc:wchar_t,inline,forScope \
                /D "WIN32" /D "_WINDOWS" /D "MFCX_LOG_USELOG" \
                /D "_VC80_UPGRADE=0x0600" /D "_MBCS" /D "_AFXDLL" /WX- \
                /fp:precise /errorReport:prompt /EHsc /nologo \
                /Fd"$(INTDIR)\\" /FC \
                /Fp"$(INTDIR)\$(PROGRAM).pch" /diagnostics:classic /c

RSC_CMN_FLAGS = /D "_VC80_UPGRADE=0x0600" /D "_AFXDLL" /l 0x0409 /nologo \
                /fo"$(INTDIR)\$(PROGRAM).res" 

DEPFLAGS	=	-q

ifeq ("$(CFG)", "Debug")

  TARGET    =    $(OUTDIR)\$(PROGRAM)sd.lib

  # /FR:<bsc-dir> browser info
  # /Od   - disable optimisations.
  # /RTC1 - run-time error checking optimised for debugging.
  # /MDd  - use debug libraries.
  CXXFLAGS  = $(CXX_CMN_FLAGS) /Gy /ZI /Gm- /Od /D "_DEBUG" /RTC1 /MDd \
              /FR:"$(INTDIR)\\" 
  RSC_FLAGS = $(RSC_CMN_FLAGS) /D "_DEBUG"
 
  BSC32_FLAGS=/nologo 
  LIB_FLAGS = /OUT:"$(OUTDIR)\MFCXsd.lib" /MACHINE:X86 /NOLOGO
  # /LIST:"$(INTDIR)\MFCX.lst" - doesn't work
              
else ifeq ("$(CFG)", "Release")

  TARGET    =    $(OUTDIR)\$(PROGRAM)s.lib

  # /Gy  : function level linking.
  # /Gm- : Switch off minimum rebuild.
  # /O2  : Optimise for max speed.
  # /GF  : Eliminate Duplicate Strings
  # /WX- : Treat warnings as errors.
  # /MD  : Multithreaded DLLs.
  CXXFLAGS  = $(CXX_CMN_FLAGS) /Zi /O2 /D "NDEBUG" /GF /MD
  RSC_FLAGS = $(RSC_CMN_FLAGS) /D "NDEBUG" 
  BSC32_FLAGS=/o"$(OUTDIR)\MFCX.bsc" /nologo 
  LIB_FLAGS = /OUT:"$(OUTDIR)\MFCXs.lib" /NOLOGO 

endif

CPPCHECK_FLAGS = -i Utilities/TOTDDlg.cpp -i Sockets/FTP/FTPLIST-TestCases.cpp \
                 -i Export --quiet \
                 --library="$(CPPCHECK_HOME)/cfg/mfc.cfg" \
                 --library="$(CPPCHECK_HOME)/cfg/windows.cfg" \
                 -D "WIN32" -D "_DEBUG" -D "_WINDOWS" -D "_AFXDLL" -D "_MBCS" \
                 -D "__AFXWIN_H__" -U "NDEBUG" -U "BETA" \
                 --enable=all --platform=win64 --std=c++03 --inconclusive 
                 
#--check-config

SRC       =    $(filter-out MFCX_DLL.cpp, $(wildcard *.cpp)) $(wildcard regex/*.cpp)
# Remove StdAfx.obj from the object list, because it's a special case - the precompiled header.
OBJS      =    $(addprefix $(INTDIR)/, $(filter-out StdAfx.obj, $(SRC:.cpp=.obj)))
DEPS      =    $(addprefix $(DEPDIR)/, $(SRC:.cpp=.d))

SRC_RES   =    $(PROGRAM).rc $(wildcard res/*)
RES       =    $(INTDIR)/$(PROGRAM).res

SBRS      =    $(addprefix $(INTDIR)/, StdAfx.sbr $(notdir $(OBJS:.obj=.sbr)))

PCH       =    $(INTDIR)\$(PROGRAM).pch
BSCFILE   =    $(OUTDIR)\$(PROGRAM).bsc 


# TARGETS

ifeq ("$(CFG)", "Debug")
build : $(TARGET) $(BSCFILE)
else
build : $(TARGET)
endif

cleanmost:
	@if exist $(OUTDIR)                 rmdir /S /Q "$(OUTDIR)"
	@if exist $(INTDIR)                 rmdir /S /Q "$(INTDIR)"
	@if exist $(PROGRAM).dep            del /Q $(PROGRAM).dep
	@if exist $(PROGRAM).opt            del /Q $(PROGRAM).opt
	@if exist $(PROGRAM).plg            del /Q $(PROGRAM).plg
	@if exist LastCoverageResults.log   del /Q LastCoverageResults.log


clean: cleanmost cleantest
	@echo Cleaning $(PROGRAM) $(CFG) temporary files
	@if exist $(DEPDIR)                 rmdir /S /Q "$(DEPDIR)"
	@if exist $(TESTDIR)                rmdir /S /Q "$(TESTDIR)"
	@if exist $(PROGRAM).aps            del /Q $(PROGRAM).aps
	@if exist $(PROGRAM).plg            del /Q $(PROGRAM).plg
	@if exist $(PROGRAM)Test.plg        del /Q $(PROGRAM)Test.plg
	@if exist $(PROGRAM).ncb            del /Q $(PROGRAM).ncb


# Dependency files 
$(DEPS)   :    $(DEPDIR)/%.d  :    %.cpp
	@echo Making $@ from $<
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(MKDEP) $(DEPFLAGS) $< | $(SED) "s!^\(.*\)\.cpp : !$(INTDIR)/\1.obj : !" > $@


$(OBJS)	:	$(PCH)

# Resource files
$(RES) : $(SRC_RES)

# Make the pre-compiled header file.
$(INTDIR)/StdAfx.obj $(PCH) 		:	StdAfx.h StdAfx.cpp
	@echo Creating the precompiled header (making $(@D))
	@if not exist "$(@D)" mkdir "$(@D)"
	@"$(CXX)" $(CXXFLAGS) /Yc"StdAfx.h" /Fo$@ StdAfx.cpp

# Rules making .obj from sources and puts them flat in $(INTDIR)
$(OBJS)	:	$(INTDIR)/%.obj	:	%.cpp
	@echo Building $@ from $< (making $(@D))
	@if not exist "$(@D)" mkdir "$(@D)"
	@"$(CXX)" $(CXXFLAGS) /Yu"StdAfx.h" /Fo$@ $<

#$(SBRS)	:	$(INTDIR)/%.sbr	:	%.cpp
#	@echo Creating $@ from $<
#	@if not exist "$(@D)" mkdir "$(@D)"
#	@"$(CXX)" $(CXXFLAGS) /Yu"StdAfx.h" /Fo$@ $<

$(INTDIR)/%.res	:	%.rc
	@echo Building resource $@ from $<
	@$(RSC) $(RSC_FLAGS) /fo$@ $<

$(TARGET) : $(INTDIR)/StdAfx.obj $(OBJS) $(RES)
	@echo Creating library $@ (making $(OUTDIR))
	@if not exist "$(OUTDIR)" mkdir "$(OUTDIR)"
	@$(LIBEXE) $(LIB_FLAGS) $(INTDIR)/StdAfx.obj $(OBJS) $(RES)
	
$(BSCFILE) : 
	@echo Create browser information
	@$(BSC32) $(BSC32_FLAGS) /o$(OUTDIR)/$(PROGRAM).bsc $(SBRS)


## Testing below

TEST_PROGRAM   =  $(PROGRAM)Test
TEST_PCH       =  $(TESTDIR)\$(TEST_PROGRAM).pch
TEST_TARGET    =  $(TESTDIR)\$(TEST_PROGRAM).exe


# See https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically?view=vs-2019
# for an explanation of the options.
# Note, no space allowed between options and files. For example: '/Fp$(TEST_PCH)' works, '/Fp $(TEST_PCH)' doesn't. :-(
CONSOLE_CXXFLAGS = /nologo /MDd /W3 /EHsc /GR /Zi /Od \
                   /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" \
                   /D "_AFXDLL" /D "UNIT_TEST" \
                   /Fp$(TEST_PCH) /Fd"$(TESTDIR)\\" /FD /RTC1 /c

# NOTE: To find unresolved external symbols, navigate to the VC98/Lib directory and run this command...
#       nm -A *.lib 2> /dev/null | grep <missing-symbol>
CONSOLE_LINK_FLAGS= shlwapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib \
                    comdlg32.lib advapi32.lib shell32.lib ole32.lib \
                    oleaut32.lib uuid.lib odbc32.lib odbccp32.lib version.lib \
                    winmm.lib /nologo /subsystem:console \
                    /pdb:"$(TESTDIR)\$(TEST_PROGRAM).pdb" /debug \
                    /machine:I386

# Same as SCR but without all files that reference MFCX_DLL.cpp.
UNITTEST_SRC  :=   $(wildcard ../test/*.cpp)
TEST_SRC_DIRS :=   $(addprefix ../test/, $(SRC_DIRS))
UNITTEST_SRC  :=   $(wildcard ../test/*.cpp) $(foreach dir,$(TEST_SRC_DIRS),$(wildcard $(dir)/*.cpp))

UNITTEST_DEPS :=   $(UNITTEST_SRC:.cpp=.d)

TEST_SRC      :=   $(UNITTEST_SRC) Crypt.cpp DataQueue.cpp Filename.cpp \
                    Regex.cpp RegexString.cpp Utils.cpp XString.cpp \
                    XStringList.cpp XTime.cpp
                         
TEST_OBJS     :=   $(addprefix $(TESTDIR)/, $(TEST_SRC:.cpp=.obj))
TEST_DEPS     :=   $(addprefix $(DEPDIR)/, $(notdir $(UNITTEST_DEPS)))


# TEST ----------------------------------
TXT_PATH = ../builds\exp-deps

#DOS_PATH  = $(subst /,\,$(TXT_PATH))
DOS_PATH  = $(call dospath,$(TXT_PATH))
UNIX_PATH = $(call unixpath,$(TXT_PATH))
# ---------------------------------------



debug:
	@echo TEST_TARGET        =    $(TEST_TARGET)
	@echo TEST_CPY_FILES     =    $(TEST_CPY_FILES)
	@echo TEST_SRC_DIRS      =    $(TEST_SRC_DIRS)
	@echo SRC                =    $(SRC)
	@echo UNITTEST_SRC       =    $(UNITTEST_SRC)
	@echo TEST_SRC           =    $(TEST_SRC)
	@echo TEST_OBJS          =    $(TEST_OBJS)


# Dependency files 
$(TEST_DEPS)   :    $(DEPDIR)/%.d  :    ../test/%.cpp
	@echo Making $@ from $<
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(MKDEP) $(DEPFLAGS) $< | $(SED) "s!^\(.*\)\.cpp : !$(TESTDIR)/\1.obj : !" > $@


$(TEST_OBJS)   :    $(TEST_PCH)

# Make the pre-compiled header file.
$(TESTDIR)\StdAfx.obj $(TEST_PCH) 	:	StdAfx.h StdAfx.cpp
	@echo Creating the test precompiled header 
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(CXX) $(CONSOLE_CXXFLAGS) /Yc"StdAfx.h" /Fo$(TESTDIR)\StdAfx.obj StdAfx.cpp

# Make the other object files...
$(TEST_OBJS)	:	$(TESTDIR)/%.obj	:	%.cpp
	@echo Building test $@ from $< (making $(@D))
	@if not exist "$(@D)" mkdir "$(@D)"
	@$(CXX) $(CONSOLE_CXXFLAGS) /Yu"StdAfx.h" /Fo$@ $<

$(TESTDIR)\$(TEST_PROGRAM).exe : $(TEST_OBJS)
	@echo Creating $@
	@$(LINK) $(CONSOLE_LINK_FLAGS) /out:$@ $(TESTDIR)\StdAfx.obj $(TEST_OBJS)

test : $(TEST_TARGET) test_files
	@echo Running unit tests
	@$(TESTDIR)/$(TEST_PROGRAM).exe

cleantest : 
	@echo Cleaning test files
	@if exist $(TESTDIR) rmdir /S /Q "$(TESTDIR)"

$(CHECKDIR)/cppcheck.xml : $(SRC)
	@echo Running checks...
	@if not exist "$(CHECKDIR)" mkdir "$(CHECKDIR)"
	@"$(CPPCHECK)" $(CPPCHECK_FLAGS) --xml --xml-version=2 --output-file=$@ .

check-xml : ext-deps $(CHECKDIR)/cppcheck.xml

check-html : check-xml
	@echo Creating checks HTML
	@"$(PYTHON)" "$(CPPCHECK_HTMLREP)" --file=$(CHECKDIR)/cppcheck.xml \
	             --report-dir=$(CHECKDIR)/html --source-dir=.

check     : check-html

coverage: $(TEST_TARGET) test_files
	@echo Generating test coverage statistics...
	@if not exist "..\builds\coverage" mkdir "..\builds\coverage"
	@"$(COVERAGE)" -v --config_file ..\test\coverage\OpenCppCoverage.cfg -- \
	               $(TESTDIR)\$(TEST_PROGRAM).exe \
	               2> ..\builds\coverage\OpenCppCoverage.err

trace:
	@echo CXXFLAGS   = $(CXXFLAGS)
	@echo SRC        = $(SRC)
	@echo OBJS       = $(OBJS)
	@echo SBRS       = $(SBRS)
	@echo LIBEXE     = $(LIBEXE)
	@echo LIB_FLAGS  = $(LIB_FLAGS)
	@echo RES        = $(RES)
	

# INCLUDES
ifneq ($(MAKECMDGOALS), clean)
  ifneq ($(MAKECMDGOALS), cleanmost)
    ifneq ($(MAKECMDGOALS), trace)
      -include $(DEPS)
    endif
  endif
endif

ifeq ($(MAKECMDGOALS), test)
  -include $(TEST_DEPS)
endif

# vi:ts=5