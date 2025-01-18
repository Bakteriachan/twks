#ifndef WORKSPACE_H
#define WORKSPACE_H

#define WORKSPACE_MAX_SIZE 50
#define VALUE_MAX_SIZE 2048

void get_directory(char*);

void add_variable(char* workspace, char* key, char* value);

void get_variable(char* workspace, char* key, char* value);

int get_active_workspace(char* workspace);

#endif
