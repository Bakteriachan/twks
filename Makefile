workspace: main.c workspace.c workspace.h
	gcc main.c workspace.c -L/usr/local/lib -ljson-c -o workspace
