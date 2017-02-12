#include "word_processor.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>


uint32_t count_lines(char* buffer, int buffer_length)
{
	uint32_t count = 0;
	for(int index = 0; index < buffer_length; ++index)
	{
		if(buffer[index] == '\n')
		{
			++count;
		}
	}
	return count;
}

/* See description in header file */
rpn_return_code_t convert(
            char* filename,
            uint32_t* lines)
{
	//~ char *input_str = filename;
	//~ char *output_str = NULL;
	//~ uint32_t *input_length = filename_length;
	//~ uint32_t *output_length = 0;
	rpn_return_code_t return_value = RC_FAILURE;
	FILE * input_file = NULL;
	size_t file_length = 0;
	char * file_buffer = NULL;

	// Open file for reading
	input_file = fopen(filename, "r");
	
	// Determine file size
	fseek(input_file, 0, SEEK_END);
	file_length = ftell(input_file);
	
	file_buffer = malloc(sizeof(char) * (file_length + 1));
	printf("File size: %ld\n", file_length);
	fseek(input_file, 0, SEEK_SET);
	//~ fseek(input_file, -100, SEEK_CUR);
	
	size_t newLen = fread(file_buffer, sizeof(char), file_length, input_file);
	if ( ferror( input_file ) != 0 ) {
		fputs("Error reading file", stderr);
		return RC_FAILURE;
	} else {
		file_buffer[newLen++] = '\0'; /* Just to be safe. */
	}
	//~ printf("read:\n%s\n", file_buffer);

	setlocale(LC_ALL, "");
	printf("études%lc\n", L'é');
	
	(*lines) = count_lines(file_buffer, file_length);
	
	free(file_buffer);
	fclose(input_file);
	
	return_value = RC_SUCCESS;
	
	return return_value;
}

#ifndef UNIT_TESTING
int main(int argc, char **argv)
{
	uint32_t lines = -1;
	convert("/usr/share/dict/words", &lines);
}
#endif /* Not UNIT_TESTING */
