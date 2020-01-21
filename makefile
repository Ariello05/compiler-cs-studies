.PHONY = all clean cleanall 
 
all: scan 
 
memc.o: memc.cpp
		g++ -std=c++17 --pedantic -Wall -c memc.cpp

code.o: code.cpp
		g++ -std=c++17 --pedantic -Wall -c code.cpp

forController.o: forController.cpp
		g++ -std=c++17 --pedantic -Wall -c forController.cpp

scan: parser.ypp lexer.l memc.o code.o forController.o
		bison -o parser_y.c -d parser.ypp
		flex -o lexer_l.c lexer.l 
		g++ -std=c++17 --pedantic -Wall -O -lm -o scan parser_y.c lexer_l.c memc.o code.o forController.o
 
clean: 
		rm -f *.c *.h *.o
 
cleanall: clean 
		rm -f scan