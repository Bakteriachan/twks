#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "workspace.h"


int main(int argc, char* argv[]) {
	char* workspace = malloc(WORKSPACE_MAX_SIZE), *directory = malloc(250);
	if(get_active_workspace(workspace) == -1) {
		return 1;
	}

	printf("workspace: %s\n", workspace);
	get_directory(directory);

	void *buf;
	if(stat(directory, buf) == -1) {
		int fd = creat(directory, S_IRWXU);
		if(fd == -1) {
			int err_v = errno;
			char *err = strerror(err_v);
			printf("Could not create file: %s\n", err);
			return err_v;
		} 
	} else {
		printf("file exists\n");
	}


	
	// GET variable value
	if(argc == 2 && strcmp(argv[1], "-w") != 0 && strcmp(argv[1], "--workspace") != 0) {
		char* key = argv[1], *value = malloc(VALUE_MAX_SIZE);	
		printf("%s", key);
		get_variable(workspace, key, value);
		return 0;
	} 

	// SET variable value
	if(argc == 3 && strcmp(argv[1], "-w") != 0 && strcmp(argv[1], "--workspace") != 0) {
		char *key = argv[1], *value = argv[2];
		add_variable(workspace, key, value);
		return 0;
	}

	int showUsage = 1;

	// Set active workspace
	for(int i = 1; i < argc - 1; i++) {
		if(strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--workspace") == 0) {
			int fd = open(".active", O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
			if(fd == -1) {
				int err_v = errno;
				printf("%s: Could not open file: %s\n", argv[0], strerror(err_v));
				return 1;
			}
			ssize_t wr = write(fd, argv[i+1], strlen(argv[i+1]));
			workspace = argv[i+1];
			if(wr == -1) {
				int err_v = errno;
				printf("%s: Could not write to file: %s", argv[0], strerror(err_v));
				close(fd);
				return 1;
			}
			showUsage = 0;
			close(fd);
			break;
		}
	}

	if(argc == 5) {
		char *key, *value = 0;
		int isKey = 1;
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-w") != 0 && strcmp(argv[i], "--worskpace") != 0) {
				if(isKey) {
					key = argv[i];
					isKey = 0;
				} else {
					value = argv[i];
				}
			}
		}
		if(value != 0) {
			add_variable(workspace, key, value);
			return 0;
		} else {
			value = malloc(VALUE_MAX_SIZE);
			get_variable(workspace, key, value);
			printf("%s\n", value);
			return 0;
		}
	}

	char* usage = \
		"USAGE\n"
		"    %s KEY [VALUE] [--workspace <workspace>]"
		"\n\nOPTIONS:\n"
		"    -w, --workspace <workspace>\n"
		"            Use <workspace> as the active workspace\n";
	if(showUsage)
		printf(usage, argv[0]);

	return 0;
}
