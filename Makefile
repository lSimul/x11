C = gcc
LIBS = -lX11

all: capture_and_clicker command_reader

command_reader: command_reader.c grammer.o file.o string.o command.o token.o keys.o movement.o
	$(C) $^ $(LIBS) -lXtst -o $@

grammer.o: grammer.c grammer.h
	$(C) $< -c -o $@

command.o: command.c command.h
	$(C) $< -c -o $@

token.o: token.c token.h
	$(C) $< -c -o $@

file.o: file.c file.h
	$(C) $< -c -o $@

string.o: string.c string.h
	$(C) $< -c -o $@

movement.o: movement.c movement.h
	$(C) $< -c -o $@

capture_and_clicker: capture_and_clicker.c file.o movement.o
	$(C) $^ $(LIBS) -lXtst -o $@

keys.o: keys.c keys.h
	$(C) $< -c -o $@

clean:
	rm -rf *.o

doc:
	doxygen Doxyfile
