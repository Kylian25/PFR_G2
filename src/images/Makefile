main: test_module_image.o module_image.o
	gcc test_module_image.o module_image.o -o main.out

test: test_module_image.c 
	gcc -c test_module_image.c -o test_module_image.o

module_image: module_image.c
	gcc -c module_image.c -o module_image.o

clean:
	rm -rf *.o
