C = gcc
LIBS = -lX11

all: presskey clicker capture capture_and_clicker file

file: file.c pixel.h
	$(C) $^ -o $@

clicker: clicker.c
	$(C) $^ $(LIBS) -o $@

capture: capture.c
	$(C) $^ $(LIBS) -o $@

capture_and_clicker: capture_and_clicker.c pixel.h
	$(C) $^ $(LIBS) -lXtst -o $@

presskey: presskey.c
	$(C) $^ $(LIBS) -lXtst -o $@
