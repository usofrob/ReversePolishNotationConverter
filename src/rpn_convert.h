
#ifndef RPN_CONVERT_H
#define RPN_CONVERT_H

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
 * Converts infix to RPN
 * 
 * Input:
 *   infix_to_rpn : Boolean value, 
 *                  true (1) if going from infix to RPN
 *                  false (0) if going from RPN to infix
 * 
 * Input/Output: depending on infix_to_rpn
 *   infix : The pre-allocated character string in infix notation
 *   infix_length : The length of the infix string, including null 
 *                  terminator
 * Input/Output: depending on infix_to_rpn
 *   rpn : The pre-allocated character string for the RPN return value
 *   rpn_length : The length of the space reserved for rpn string,
 *                including space for null terminator
 *                Updated with used length
 * 
 * NOTE: Valid characters for the infix notation are any lower case
 * 		 letter 'a' through 'z' and these characters: '+', '-', '*',
 * 		 '/', '^', '(' and ')'.
 *       Valid characters for the RPN are any lower case
 * 		 letter 'a' through 'z' and these characters: '+', '-', '*',
 * 		 '/', and '^'.
 * 
 * returns type rpn_return_code_t
 */
rpn_return_code_t convert(int infix_to_rpn,
            char* infix, 
            uint32_t* infix_length,
            char* rpn,
            uint32_t* rpn_length);

#endif /* RPN_CONVERT_H */
