build:
	gcc main.c cell.c networking.c graphics.c maze_import.c -g $(shell python3.10-config --ldflags --libs --embed --cflags) -lGL -lGLU -lGLEW -lX11 -lm -lrt -lpng -lglfw -o main
