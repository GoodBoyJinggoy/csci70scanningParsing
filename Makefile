all: scanner.exe parser.exe

parser.o: parser.c scanparse.h token.h 
	gcc -c parser.c

parser.exe: parser.o token.o scanparse.o
	gcc -o parser.exe parser.o token.o scanparse.o

token.o: token.c token.h
	gcc -c token.c

scanparse.o: scanparse.c scanparse.h token.h 
	gcc -c scanparse.c

scanner.o: scanner.c scanparse.h token.h 
	gcc -c scanner.c

scanner.exe: scanner.o scanparse.o token.o
	gcc -o scanner.exe scanner.o scanparse.o token.o

clean:
	rm *.exe *.o

cleanscan:
	rm *_scan.txt

cleanparse:
	rm *_parse.txt
