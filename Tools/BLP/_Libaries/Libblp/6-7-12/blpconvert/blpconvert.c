/*
 *  blpconvert.c
 *  libblp
 *
 *  Created by Jean-Francois Roy on 27/12/2005.
 *  Copyright 2005 MacStorm. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <limits.h>

#include "blp.h"

static char *_copy_path_extension(const char *path) {
	size_t path_length = strlen(path);
	if(path_length > PATH_MAX) return NULL;
	
	const char *path_pointer = path + path_length;
	while(path_pointer < path) {
		if(*path_pointer == '.') break;
		path_pointer--;
	}
	
	if(path_pointer == path) return NULL;
	path_pointer++;
	
	size_t path_extension_size = (path + path_length) - path_pointer + 2;
	char *path_extension = malloc(path_extension_size);
	strncpy(path_extension, path_pointer, path_extension_size - 1);
	return path_extension;
}

int main (int argc, char const* argv[]) {
    if(argc < 3) {
        printf("usage: blpconvert <input file path> <output file path>\n\nAvailable output formats:\n    .tga (Targa)\n    .jpg (JPEG)\n");
        return 1;
    }
	
	int blpfd = open(argv[1], O_RDONLY);
	if(blpfd == -1) {
		printf("error: unable to open the input file\n");
		return 1;
	}
	
	struct stat sb;
	int result = fstat(blpfd, &sb);
	if(result == -1) {
		printf("error: unable to stat the input file\n");
		return 1;
	}
	
	size_t input_size = (size_t)(sb.st_size);
	
	void *input_data = malloc(input_size);
	if(input_data == NULL) {
		printf("error: unable to malloc the input file buffer\n");
		return 1;
	}
	
	ssize_t bytes_read = read(blpfd, input_data, input_size);
	if(bytes_read == -1) {
		printf("error: unable to read the input file\n");
		return 1;
	}
	
	result = BLP2Everything(input_data, input_size, argv[2]);
    if(result == 0) {
		printf("error: unable to convert the input file to the desired output format\n");
		return 1;
	}
	
    return 0;
}
