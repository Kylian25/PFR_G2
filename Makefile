main.out : src/main.o
	gcc -o main.out src/main.o

main.o : src/main.c
	gcc -c src/main.c -o src/main.o

clean:
	rm -f main.out src/*.o 
