all: compile link execute

compile:
	gcc -c -I "./include" ./src/logger.c -o ./lib/logger.o
	gcc -c -I "./include" ./src/directory_ops.c -o ./lib/directory_ops.o
	gcc -c -I "./include" ./src/permissions.c -o ./lib/permissions.o
	gcc -c -I "./include" ./src/main.c -o ./lib/main.o
	
link:
	gcc ./lib/logger.o ./lib/directory_ops.o ./lib/permissions.o ./lib/main.o -o ./bin/file_manager

execute:
	./bin/file_manager
	


