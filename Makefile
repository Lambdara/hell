build:
	gcc main.c cell.c networking.c graphics.c -g $(shell python3.8-config --ldflags --libs --embed --cflags) -lGL -lGLU -lGLEW -lX11 -lm -lrt -lpng -lglfw -o main
