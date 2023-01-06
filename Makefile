C = gcc
LIBS = -lX11

all: main.c
	$(C) $^ $(LIBS)
