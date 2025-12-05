main.out : src/main.o src/menu/menu.o
	gcc -o main.out src/main.o src/menu/menu.o

main.o : src/main.c src/menu/menu.h
	gcc -c src/main.c -o src/main.o

menu.o : src/menu/menu.c src/menu/menu.h
	gcc -c src/menu/menu.c -o src/menu/menu.o

clean:
	rm -f main.out src/*.o 
