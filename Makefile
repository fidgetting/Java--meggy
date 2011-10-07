#
# Makefile for comp project
#
# Created on: May 27, 2011
#     Author: norton
#

YACC    = bison
LEX     = flex
CXX     = g++
CFLAGS  = -g -Wall -O2 -std=c++0x
INCPATH = -I.
EXE     = meggy

OBJS = parser.o        \
       lexer.o         \
       visitor.o       \
       node.o          \
       basic_visitor.o \
       type.o          \
       scope.o         \
       symtable.o      \
       type_checker.o  \

HEAD = visitor.hpp       \
       node.hpp          \
       type.hpp          \
       basic_visitor.hpp \
       scope.hpp         \
       symtable.hpp      \
       type_checker.hpp  \
       Makefile          \

GENR = node.cpp    \
       visitor.cpp \


all: $(EXE)
$(EXE): $(OBJS) $(EXE).cpp $(HEAD)
	$(CXX) $(INCPATH) $(CFLAGS) $@.cpp $(OBJS) -o $@

parser.cpp: parser.y $(GENR) $(HEAD)
	$(YACC) $< -o $@ -d

lexer.cpp: lexer.lex parser.y $(GENR) $(HEAD)
	$(LEX) --nounput -o $@ $<

$(OBJS): %.o: %.cpp $(HEAD)
	$(CXX) -c $(INCPATH) $(CFLAGS) $<

$(GENR): nodes.in astgen.py
	./astgen.py

clean:
	rm -f $(EXE) *.o parser.cpp lexer.cpp *.dot *.png
	rm -f TestCases/*.png TestCases/*.ast TestCases/*.sym
	rm -f $(GENR)
