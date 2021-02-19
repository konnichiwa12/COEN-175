CXX		= g++ -std=c++11
CXXFLAGS	= -g -Wall
OBJS		= checker.o lexer.o parser.o scope.o string.o symbol.o type.o
PROG		= scc

all:		$(PROG)

$(PROG):	$(OBJS)
		$(CXX) -o $(PROG) $(OBJS)

clean:;		$(RM) $(PROG) core *.o
