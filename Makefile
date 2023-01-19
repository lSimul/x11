C = gcc
LIBS = -lX11

all: presskey clicker capture capture_and_clicker

clicker: clicker.c
	$(C) $^ $(LIBS) -o $@

capture: capture.c
	$(C) $^ $(LIBS) -o $@

capture_and_clicker: capture_and_clicker.c
	$(C) $^ $(LIBS) -lXtst -o $@

presskey: presskey.c
	$(C) $^ $(LIBS) -lXtst -o $@
