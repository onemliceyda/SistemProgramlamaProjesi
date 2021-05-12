CC = gcc
OBJ = main.o libfdr/fields.o libfdr/jval.o libfdr/jrb.o

all: kripto

kripto: main.o libfdr/fields.o libfdr/jval.o libfdr/jrb.o
	$(CC) $(OBJ) -o $@

main.o: main.c
	$(CC) -c $<

libdr/fields.o: libfdr/fields.c libfdr/fields.h
	$(CC) -c $<

libdr/jval.o: libfdr/jval.c libfdr/jval.h
	$(CC) -c $<

libdr/jrb.o: libfdr/jrb.c libfdr/jrb.h
	$(CC) -c $<

clean:
	rm *.o

vclean:	
	rm -rf *.o
	rm kripto