traitement.o : traitement.h traitement.c
	gcc -c traitement.c -o traitement.o

main.o : traitement.h main.c
	gcc -c main.c -o main.o

main.out : traitement.o main.o
	gcc -o main.out traitement.o main.o

clear :
	rm *.o