main.out : src/main.o src/menu/menu.o src/outils/outils.o
	gcc -o main.out src/main.o src/menu/menu.o src/outils/outils.o

main.o : src/main.c src/menu/menu.h
	gcc -c src/main.c -o src/main.o

menu.o : src/menu/menu.c src/menu/menu.h
	gcc -c src/menu/menu.c -o src/menu/menu.o

outils.o : src/outils/outils.c src/outils/outils.h
	gcc -c src/outils/outils.c -o src/outils/outils.o 

clean:
	rm -f main.out
	find src -name "*.o" -type f -delete   
