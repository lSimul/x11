C = gcc
LIBS = -lX11

all: presskey capture_and_clicker grammer

grammer: grammer.c file.o string.o
	$(C) $^ -o $@

file.o: file.c file.h
	$(C) $< -c -o $@

string.o: string.c string.h
	$(C) $< -c -o $@

movement.o: movement.c movement.h
	$(C) $< -c -o $@

capture_and_clicker: capture_and_clicker.c file.o movement.o
	$(C) $^ $(LIBS) -lXtst -o $@

presskey: presskey.c
	$(C) $^ $(LIBS) -lXtst -o $@
