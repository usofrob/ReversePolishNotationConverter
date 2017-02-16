#include "word_processor.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

typedef struct word
{
	struct word *next;
	char * buffer;
} word_t;

typedef struct boggle_used
{
	char used[16];
} boggle_used_t;

word_t * found_list;

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
		free(head->buffer);
		temp = head->next;
		free(head);
		head = temp;
	}
}

uint32_t count_lines(char* buffer, int buffer_length, word_t ** list_head)
{
	word_t * word_list_prev = NULL;
	word_t * word_list_current = NULL;
	uint32_t count = 0;
	int32_t last_newline = -1;
	int32_t new_length = -1;
	for(int index = 0; index < buffer_length; ++index)
	{
		// convert everything to lowercase
		if((buffer[index] >= 'A') && (buffer[index] <= 'Z'))
		{
			buffer[index] += 32;
		}
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

void add_to_found_list(char * new_word)
{
	word_t * word_list_prev = NULL;
	word_t * word_list_current = found_list;
		
	if(found_list == NULL)
	{
		word_list_current = (word_t*) malloc(sizeof(word_t));
		found_list = word_list_current;
		word_list_current->buffer = new_word;
		word_list_current->next = NULL;
		return;
	}
	
	
	// search for duplicates
	while(word_list_current != NULL)
	{
		if(strcmp(new_word, word_list_current->buffer) == 0)
		{
			return; // already found
		}
		word_list_prev = word_list_current;
		word_list_current = word_list_current->next;
	}
	// not found, so add
	
	word_list_current = (word_t*) malloc(sizeof(word_t));
	word_list_current->buffer = new_word;
	word_list_current->next = NULL;
	word_list_prev->next = word_list_current;
}

void print_list()
{
	word_t * word_list_current = found_list;
	word_t *temp = NULL;
	while(word_list_current != NULL)
	{
		printf("%s\n", word_list_current->buffer);
		temp = word_list_current->next;
		free(word_list_current);
		word_list_current = temp;
	}
}

rpn_return_code_t search_from_start(char* test_word, char* boggle_map, boggle_used_t used_map, int last_index)
{
	int search_pattern[8] = {-5, -4, -3, -1, 1, 3, 4, 5};
	// search all safe directions
	if(test_word[0] == '\0')
	{
		return RC_SUCCESS;
	}
	if((test_word[0] < 'a') || (test_word[0] > 'z'))
	{
		// go to next letter
		if(search_from_start(test_word + 1, boggle_map, used_map, last_index) == RC_SUCCESS)
		{
			return RC_SUCCESS;
		}
	}
	
	for(int direction = 0; direction < 8; ++direction)
	{
		int next_index = last_index + search_pattern[direction];
		if((next_index >=0) && (next_index <=15) &&
		   (used_map.used[next_index] == 0) && 
		   (test_word[0] == boggle_map[next_index]))
		{
			used_map.used[next_index] = 1;
			if(test_word[1] == '\0')
			{
				return RC_SUCCESS;
			}
			if(search_from_start(test_word + 1, boggle_map, used_map, next_index) == RC_SUCCESS)
			{
				return RC_SUCCESS;
			}
			
		}
	}
	
	return RC_FAILURE;
}

void match_word(word_t * list_head,
				char* boggle_map)
{
	boggle_used_t used;
	// for each word
	while(list_head != NULL)
	{
		// for each letter on the map
		for (int start_index = 0; start_index < 16; ++start_index)
		{
			memset(&used, 0, sizeof(boggle_used_t));
			// check to see if the next letter matches
			if(list_head->buffer[0] == boggle_map[start_index])
			{
				// record matched letters
				used.used[start_index] = 1;
				if(search_from_start(list_head->buffer + 1,
								boggle_map,
								used,
								start_index) == RC_SUCCESS)
				{
					//~ printf("word:%s\n", list_head->buffer);
					add_to_found_list(list_head->buffer);
					break;
				}
			}
		}
		list_head = list_head->next;
	}
}

/* See description in header file */
rpn_return_code_t convert(
            char* filename,
            char* boggle_map)
{
	rpn_return_code_t return_value = RC_FAILURE;
	FILE * input_file = NULL;
	size_t file_length = 0;
	char * file_buffer = NULL;
	word_t * word_list_head = NULL;
	found_list = NULL;

	// Open file for reading
	input_file = fopen(filename, "r");
	
	// Determine file size
	fseek(input_file, 0, SEEK_END);
	file_length = ftell(input_file);
	
	file_buffer = malloc(sizeof(char) * (file_length + 1));
	//~ printf("File size: %ld\n", file_length);
	fseek(input_file, 0, SEEK_SET);
	
	size_t newLen = fread(file_buffer, sizeof(char), file_length, input_file);
	if ( ferror( input_file ) != 0 ) {
#ifndef UNIT_TESTING
		fputs("Error reading file", stderr);
		return RC_FAILURE;
#endif /* Not UNIT_TESTING */
	} else {
		file_buffer[newLen++] = '\0'; /* Just to be safe. */
	}
	
	count_lines(file_buffer, file_length, &word_list_head);
	//~ printf("Number of words: %d\n", *lines);
	
	
	match_word(word_list_head, boggle_map);
	print_list();
	
	
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
	if(argc != 3)
	{
		printf("a.out /usr/share/dict/words abanzqzdrrorrnrr\n");
		return -1;
	}

	convert(argv[1], argv[2]);
}
#endif /* Not UNIT_TESTING */
