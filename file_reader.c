#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "file_reader.h"

size_t file_get_size(FILE *fp)
{
	size_t file_size;
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return file_size;
}

size_t read_file(const char * file_path, char **buffer)
{
	size_t actual_size;
	FILE *fp;
	size_t file_size;
	char *tmp;

	if (NULL == buffer) {
		return -EINVAL;
	}

	fp = fopen(file_path, "r");
	if (NULL == fp) {
		perror("Could not open file");
		return errno;
	}

	file_size = file_get_size(fp);

	tmp = malloc(file_size * sizeof(char));
	if (NULL == tmp) {
		perror("Could not allocate memory");
		return -ENOMEM;
	}

	actual_size = fread(tmp, sizeof(char),file_size, fp);
	fclose(fp);

	*buffer = tmp;
	return actual_size;
}
