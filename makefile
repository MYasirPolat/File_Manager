all: compile link execute

compile:
	gcc -c -I "./include" ./src/logger.c -o ./lib/logger.o
	gcc -c -I "./include" ./src/main.c -o ./lib/main.o
	
link:
	gcc ./lib/logger.o ./lib/main.o -o ./bin/file_manager

execute:
	./bin/file_manager
	
