C = gcc
LIBS = -lX11

all: presskey clicker

clicker: main.c
	$(C) $^ $(LIBS) -o $@

presskey: presskey.c
	$(C) $^ $(LIBS) -lXtst -o $@
