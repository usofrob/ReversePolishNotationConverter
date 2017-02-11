
#ifndef WORD_PROCESSOR_H
#define WORD_PROCESSOR_H

#include <stdint.h>

/**
 * Generic return code meanings
 */
typedef enum rpn_return_code_t
{
	RC_SUCCESS, // Successful conversion
	RC_FAILURE,  // Generic failed conversion
	RC_INVALID_CHAR,
	RC_INVALID_INPUT_LENGTH
} rpn_return_code_t;


/**
 * Converts something with a word list
 * 
 * Input:
 *   filename : The pre-allocated character string of the file name
 *   filename_length : The length of the filename string, including null 
 *                  terminator
 * 
 * returns type rpn_return_code_t
 */
rpn_return_code_t convert(
            char* filename);

#endif /* WORD_PROCESSOR_H */
