
#ifndef RPN_CONVERT_H
#define RPN_CONVERT_H

#include <stdint.h>

/**
 * Generic return code meanings
 */
typedef enum rpn_return_code_t
{
	RC_SUCCESS, // Successful conversion
	RC_FAILURE  // Failed conversion
} rpn_return_code_t;

/**
 * Converts infix to RPN
 * Input:
 *   infix : The pre-allocated character string in infix notation
 *   infix_length : The length of the infix string, including null 
 *                  terminator
 * Input/Output:
 *   rpn : The pre-allocated character string for the RPN return value
 *   rpn_length : The length of the space reserved for rpn string,
 *                including space for null terminator
 *                Updated with used length
 * 
 * NOTE: Valid characters for the infix notation is any lower case
 * 		 letter 'a' through 'z' and these characters: '+', '-', '*',
 * 		 '/', '^', '(' and ')'.
 * 
 * returns type rpn_return_code_t
 */
rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t* rpn_length);

#endif /* RPN_CONVERT_H */
