##############################################
# Makefile for CST320 Lab 3
#
# Author: Philip Howard
# phil.howard@oit.edu
#

COPTS=-Wall -g -c -O0
OBJS=main.o \
	 calclex.o \
	 parse.o \
	 utils.o

all: calc

clean:
	rm -f $(OBJS)
	rm -f calclex.c
	rm -f calc
	rm -f out

.c.o:
	g++ $(COPTS) $? -o $@

.cpp.o:
	g++ $(COPTS) $? -o $@

main.o: main.cpp calclex.c parse.h lex.h
	g++ $(COPTS) main.cpp -o main.o

parse.o: parse.cpp parse.h lex.h utils.h
	g++ $(COPTS) parse.cpp -o parse.o

utils.o: utils.cpp utils.h lex.h
	g++ $(COPTS) utils.cpp -o utils.o

calclex.c: calc.l
	flex -o calclex.c calc.l

calclex.o: calclex.c lex.h
	g++ $(COPTS) -Wno-sign-compare calclex.c -o calclex.o

calc: $(OBJS)
	g++ $(OBJS) -o calc