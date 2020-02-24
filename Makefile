build:
	gcc main.c cell.c -g $(shell python3.8-config --ldflags --libs --embed --cflags) -lGL -lglut -lGLU -lGLEW -lX11 -lm -lrt -lpng -o main
