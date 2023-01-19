C = gcc
LIBS = -lX11

all: presskey clicker capture

clicker: main.c
	$(C) $^ $(LIBS) -o $@

capture: capture.c
	$(C) $^ $(LIBS) -o $@

presskey: presskey.c
	$(C) $^ $(LIBS) -lXtst -o $@
