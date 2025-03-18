#ifndef WORKSPACE_H
#define WORKSPACE_H

#define WORKSPACE_MAX_SIZE 50
#define VALUE_MAX_SIZE 2048

#include <json-c/json.h>

json_object* get_json_object(char*);

json_object* get_workspace_json_from_root_json(json_object*, char*);

void get_directory(char*);

void add_variable(json_object* workspace, char* key, char* value);

void get_variable(json_object* workspace, char* key, char** value);

void delete_key(json_object* workspace, char* key);

void save_json_object(json_object* root, char* directory);

int get_active_workspace_name(char* workspace);

void get_active_workspace_keys(json_object* workspace, char **buff);

#endif
