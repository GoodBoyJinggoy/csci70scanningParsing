all: scanit.exe parser.exe

parser.o: parser.c scan.h token.h
	gcc -c parser.c

parser.exe: parser.o token.o scan.o
	gcc -o parser.exe parser.o token.o scan.o

token.o: token.c token.h
	gcc -c token.c

scan.o: scan.c scan.h token.h
	gcc -c scan.c

scanit.o: scanit.c scan.h token.h
	gcc -c scanit.c

scanit.exe: scanit.o scan.o token.o
	gcc -o scanit.exe scanit.o scan.o token.o

clean:
	rm *.exe *.o

test1: sample1-quad-formula.txt
	./scanit.exe sample1-quad-formula.txt

test2: sample2-just-tokens.txt
	./scanit.exe sample2-just-tokens.txt

test3: samp3.txt
	./scanit.exe samp3.txt

test4: samp4.txt
	./scanit.exe samp4.txt

test5: samp5.txt
	./scanit.exe samp5.txt

