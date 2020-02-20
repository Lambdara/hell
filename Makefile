build:
	gcc main.c cell.c -g $(shell python3.8-config --ldflags --libs --embed --cflags) -o main
