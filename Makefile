build:
	gcc main.c $(shell python3.8-config --ldflags --libs --embed --cflags) -o main
