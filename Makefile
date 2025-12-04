main.out : src/main.o
	gcc -o src/main.out src/main.o

main.o : src/main.c
	gcc -c src/main.c -o src/main.o

clean:
	rm -f all *.o
