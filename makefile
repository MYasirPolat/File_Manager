all: compile link execute

compile:
	gcc -c -I "./include" ./src/main.c -o ./lib/main.o
	
link:
	gcc ./lib/main.o -o ./bin/file_manager

execute:
	./bin/file_manager
	


