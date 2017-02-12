#include "word_processor.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

typedef struct word
{
	struct word *prev;
	struct word *next;
	char * buffer;
	int length;
	int index;
} word_t;

void cleanup(word_t * head)
{
	word_t *temp = NULL;

#ifndef UNIT_TESTING
	if(head == NULL)
	{
		printf("HEAD is null\n");
		return;
	}
#endif /* Not UNIT_TESTING */
	
	while(head != NULL)
	{
		//~ head->prev = NULL;
		//~ printf("free %s\n", head->buffer);
		free(head->buffer);
		//~ head->buffer = NULL;
		temp = head->next;
		free(head);
		head = temp;
	}
}

uint32_t count_lines(char* buffer, int buffer_length, word_t ** list_head)
{
	//~ word_t * word_list_head = (*list_head);
	word_t * word_list_prev = NULL;
	word_t * word_list_current = NULL;
	uint32_t count = 0;
	int32_t last_newline = -1;
	int32_t new_length = -1;
	for(int index = 0; index < buffer_length; ++index)
	//~ for(int index = 0; index < 1000; ++index)
	{
		if((buffer[index] == '\n') && (index - last_newline > 1))
		{
			word_list_current = (word_t*) malloc(sizeof(word_t));
			if((*list_head) == NULL)
			{
				(*list_head) = word_list_current;
			}
			new_length = index - last_newline;
			// Create space for new word and copy
			word_list_current->buffer = (char*) malloc(new_length);
			memcpy(word_list_current->buffer, buffer + (last_newline + 1), new_length - 1);
			word_list_current->buffer[new_length - 1] = '\0';
			
			//~ printf("new %s, len=%d\n", word_list_current->buffer, new_length);
			word_list_current->length = new_length;
			word_list_current->index = count;
			word_list_current->prev = word_list_prev;
			word_list_current->next = NULL;
			if(word_list_prev != NULL)
			{
				word_list_prev->next = word_list_current;
			}
			
			last_newline = index;
			++count;
			word_list_prev = word_list_current;
			word_list_current = NULL;
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
	word_t * word_list_head = NULL;

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
#ifndef UNIT_TESTING
		fputs("Error reading file", stderr);
		return RC_FAILURE;
#endif /* Not UNIT_TESTING */
	} else {
		file_buffer[newLen++] = '\0'; /* Just to be safe. */
	}
	//~ printf("read:\n%s\n", file_buffer);

	//~ setlocale(LC_ALL, "");
	//~ printf("études%lc\n", L'é');
	
	(*lines) = count_lines(file_buffer, file_length, &word_list_head);
	printf("Number of words: %d\n", *lines);
	
	// General cleanup
	free(file_buffer);
	file_buffer = NULL;
	fclose(input_file);
	input_file = NULL;
	cleanup(word_list_head);
	word_list_head = NULL;
	
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
