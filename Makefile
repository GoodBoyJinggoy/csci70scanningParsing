all: scanner.exe parser.exe

parser.o: parser.c scan.h token.h parser.h io.h
	gcc -c parser.c

parser.exe: parser.o token.o scan.o
	gcc -o parser.exe parser.o token.o scan.o

token.o: token.c token.h
	gcc -c token.c

scan.o: scan.c scan.h token.h io.h
	gcc -c scan.c

scanner.o: scanner.c scan.h token.h io.h
	gcc -c scanner.c

scanner.exe: scanner.o scan.o token.o
	gcc -o scanner.exe scanner.o scan.o token.o

clean:
	rm *.exe *.o
