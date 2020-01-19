.PHONY = all clean cleanall 
 
all: scan 
 
memc.o: memc.cpp
		g++ -std=c++17 --pedantic -Wall -c memc.cpp

code.o: code.cpp
		g++ -std=c++17 --pedantic -Wall -c code.cpp

scan: parser.ypp lexer.l memc.o code.o
		bison -o parser_y.c -d parser.ypp
		flex -o lexer_l.c lexer.l 
		g++ -std=c++17 --pedantic -Wall -O -lm -o scan parser_y.c lexer_l.c memc.o code.o
 
clean: 
		rm -f *.c *.h *.o
 
cleanall: clean 
		rm -f scan