
.PHONY: all clean

CPPFLAGS = -Wall -g -std=c++14 ${FLAGS} 

all : tcli.out calculator.o

tcli.out : test_cli.cpp calculator.o
	g++ -o tcli.out ${CPPFLAGS} test_cli.cpp calculator.o

calculator.o: calculator.h calculator.cpp
	g++ -o calculator.o ${CPPFLAGS} -c calculator.cpp

clean:
	rm tcli.out calculator.o
