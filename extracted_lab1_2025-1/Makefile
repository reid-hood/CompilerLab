#**************************************
# Makefile
#
# Makefile for lang compiler
#
# Author: Phil Howard 
# phil.howard@oit.edu
#
# Date: Jan. 12, 2016
#


# Windows-only Makefile (MSVC)
# Assumes: building from Developer Command Prompt or VS tools initialized, and `flex` is available in PATH.

RM = del /Q
OBJEXT = .obj
OUTEXE = lang.exe

CXX = cl
CXXFLAGS = /nologo /W3 /EHsc /MD /Zi
LNK = link
LNKFLAGS = /nologo /OUT:$(OUTEXE)

OBJS = main$(OBJEXT) langlex$(OBJEXT) cSymbol$(OBJEXT) cSymbolTable$(OBJEXT)

all: $(OUTEXE)

clean:
    $(RM) $(OBJS) 2>nul || @echo.
    $(RM) langlex.c 2>nul || @echo.
    $(RM) $(OUTEXE) 2>nul || @echo.
    $(RM) out.xml 2>nul || @echo.
    $(RM) out2.xml 2>nul || @echo.

# Generate lexer source using flex (assumes flex.exe is available on PATH)
langlex.c: lang.l
    flex -o langlex.c lang.l

# Compile C++ source files to .obj
main$(OBJEXT): main.cpp langlex.c cSymbol.cpp cSymbolTable.cpp
    $(CXX) $(CXXFLAGS) /c main.cpp /Fo$@

langlex$(OBJEXT): langlex.c
    $(CXX) $(CXXFLAGS) /c langlex.c /Fo$@

cSymbol$(OBJEXT): cSymbol.cpp
    $(CXX) $(CXXFLAGS) /c cSymbol.cpp /Fo$@

cSymbolTable$(OBJEXT): cSymbolTable.cpp cSymbol.h cSymbolTable.h
    $(CXX) $(CXXFLAGS) /c cSymbolTable.cpp /Fo$@

$(OUTEXE): $(OBJS)
    $(LNK) $(LNKFLAGS) $(OBJS)

.PHONY: all clean