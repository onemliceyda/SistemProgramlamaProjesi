CC = gcc
OBJ = main.o ./libfdr/libfdr.a

all: kripto

kripto: main.o ./libfdr/libfdr.a
	$(CC) -w $(OBJ) -o $@

main.o: main.c
	$(CC) -w -c $<

clean:
	rm -f *.o kripto

cleanall:
	rm -f *.o kripto encripted decripted *.txt ornek_metin .kilit *.a

run:
	make -s clean
	make -s
	./kripto -e ornek_metin encripted
	./kripto -d encripted decripted
