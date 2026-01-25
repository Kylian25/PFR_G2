main : src/main.o src/menu/menu.o src/outils/outils.o src/config/config.o src/images/module_image.o src/traitement/traitement.o
	gcc -o main src/main.o src/menu/menu.o src/outils/outils.o src/config/config.o src/images/module_image.o src/traitement/traitement.o

main.o : src/main.c src/menu/menu.h
	gcc -c src/main.c -o src/main.o

menu.o : src/menu/menu.c src/menu/menu.h
	gcc -c src/menu/menu.c -o src/menu/menu.o

outils.o : src/outils/outils.c src/outils/outils.h
	gcc -c src/outils/outils.c -o src/outils/outils.o 

config.o : src/config/config.c src/config/config.h
	gcc -c src/config/config.c -o src/config/config.o

module_image.o : src/images/module_image.c src/images/module_image.h
	gcc -c src/images/module_image.c -o src/images/module_image.o

traitement.o : src/traitement/traitement.c src/traitement/traitement.h
	gcc -c src/traitement/traitement.c -o src/traitement/traitement.o

clean:
	rm -f main
	find src -name "*.o" -type f -delete   
