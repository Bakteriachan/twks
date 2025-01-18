#include <stdio.h>
#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "workspace.h"

void get_directory(char* location) {
	char *loc = "/home/bakteria/.config/terminal-workspaces/workspaces.json";
	location = strcpy(location, loc);
}



void add_variable(char* workspace, char* key, char* value) {
	
}

void get_variable(char* workspace, char* key, char* value) {
	strcpy(value, "test");
}

// Active workspace is selected in this order:
// 		1 -> Environment variable
// 		2 -> Current active workspace (obtained from file `.active`)
// 		3 -> Default workspace
int get_active_workspace(char* workspace) {
	char* default_w = "DEFAULT";

	int isActive = 0;
	char *buff = malloc(WORKSPACE_MAX_SIZE);

	int fd = open(".active", O_RDONLY);
	if(fd != -1) {
		int r = read(fd, buff, WORKSPACE_MAX_SIZE);
		//realloc(buff, r);
		if(r == -1) {
			int err_v = errno;
			dprintf(2, "Error reading: %s\n", strerror(err_v));
			return -1;
		}
		isActive = 1;
		close(fd);
	}

	char* env_w = getenv("WORKSPACE_NAME");
	if(env_w == NULL) {
		if(isActive) {
			sprintf(workspace, "%s", buff);
		} else {
			strcpy(workspace, default_w);
		}
	} else {
		sprintf(workspace, "%s", env_w);
	}
	return 0;
}
