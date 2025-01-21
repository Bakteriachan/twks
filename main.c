#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <json-c/json.h>
#include "workspace.h"


int main(int argc, char* argv[]) {
	char* workspace_name = malloc(WORKSPACE_MAX_SIZE), *directory = malloc(250);
	if(get_active_workspace_name(workspace_name) == -1) {
		return 1;
	}


	get_directory(directory);

	json_object *root = get_json_object(directory);

	json_object *workspace = get_workspace_json_from_root_json(root, workspace_name);


	int showUsage = 1;

	void *buf;
	if(open(directory, O_RDONLY) == -1) {
		int err_v = errno;
		fprintf(stderr, "error while locating file %s: %d - %s\n", directory, err_v, strerror(err_v));
		int fd = creat(directory, S_IRWXU);
		if(fd == -1) {
			int err_v = errno;
			char *err = strerror(err_v);
			printf("Could not create file: %s\n", err);
			return err_v;
		} 
	}
	
	// GET variable value
	if(argc == 2 && strcmp(argv[1], "-w") != 0 && strcmp(argv[1], "--workspace") != 0) {
		char* key = argv[1], *value = malloc(VALUE_MAX_SIZE);	
		get_variable(workspace, key, &value);
		if(value == NULL) {
			fprintf(stderr,"%s: No value for such key %s\n", argv[0], key);
		} else {
			fprintf(stdout, "%s", value);
		}
		showUsage = 0;
	} 

	if(argc == 2 && (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--workspace") == 0)) {
		fprintf(stdout, "Current active workspace: %s\n", workspace_name);
		showUsage = 0;
	}

	// SET variable value
	if(argc == 3 && strcmp(argv[1], "-w") != 0 && strcmp(argv[1], "--workspace") != 0) {
		char *key = argv[1], *value = argv[2];
		add_variable(workspace, key, value);
		save_json_object(root, directory);
		showUsage = 0;
	}


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
			workspace_name = argv[i+1];
			if(wr == -1) {
				int err_v = errno;
				printf("%s: Could not write to file: %s", argv[0], strerror(err_v));
				close(fd);
				json_object_put(root);
				return 1;
			}
			showUsage = 0;
			close(fd);
			break;
		}
	}

	if(argc == 5) {
		workspace = get_workspace_json_from_root_json(root, workspace_name);
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
			save_json_object(root, directory);
			printf("the json object: %s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY)); 
			showUsage = 0;
		} else {
			value = malloc(VALUE_MAX_SIZE);
			get_variable(workspace, key, &value);
			if(value == NULL) {
				fprintf(stderr, "%s: No value for such key: %s\n", argv[0], key);
			} else {
				fprintf(stdout, "%s\n", value);
			}
			showUsage = 0;
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

	json_object_put(root);
	return 0;
}
