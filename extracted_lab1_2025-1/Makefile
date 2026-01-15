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


# Cross-platform Makefile for lang compiler
# Works with MinGW/GCC on Windows (via MSYS2) and Unix-like systems
# Requires: g++, flex

RM = rm -f
OBJEXT = .o
OUTEXE = lang.exe

CXX = g++
CXXFLAGS = -std=c++17 -I. -Wall
CFLAGS = -D_POSIX_C_SOURCE=200809L
LDFLAGS = -o $(OUTEXE)

OBJS = main$(OBJEXT) langlex$(OBJEXT) cSymbol$(OBJEXT) cSymbolTable$(OBJEXT)

all: $(OUTEXE)

# Generate lexer source using flex
langlex.c: lang.l
	flex -o langlex.c lang.l

clean:
	$(RM) $(OBJS)
	$(RM) langlex.c
	$(RM) $(OUTEXE)
	$(RM) test/out*.xml

test: $(OUTEXE)
	./$(OUTEXE) test/test1.lang test/out1.xml
	./$(OUTEXE) test/test2.lang test/out2.xml
	./$(OUTEXE) test/test3.lang test/out3.xml
	./$(OUTEXE) test/test4.lang test/out4.xml

.PHONY: all clean test

# Compile C++ source files to .o
main$(OBJEXT): main.cpp langlex.c cSymbol.cpp cSymbolTable.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

langlex$(OBJEXT): langlex.c
	$(CXX) $(CXXFLAGS) $(CFLAGS) -x c++ -c langlex.c -o $@

cSymbol$(OBJEXT): cSymbol.cpp
	$(CXX) $(CXXFLAGS) -c cSymbol.cpp -o $@

cSymbolTable$(OBJEXT): cSymbolTable.cpp cSymbol.h cSymbolTable.h
	$(CXX) $(CXXFLAGS) -c cSymbolTable.cpp -o $@

$(OUTEXE): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS)

.PHONY: all clean