CC = gcc
OBJECTS = *.o

password_generator: pcg.o password_generator.o
	$(CC) $(OBJECTS) -o password_generator

pcg.o : pcg.c pcg.h
	$(CC)	-c pcg.c

password_generator.o: password_generator.c pcg.h
	$(CC) -c password_generator.c

.PHONY: clear

clear:
	rm -f password_generator password_generator.o pcg.o