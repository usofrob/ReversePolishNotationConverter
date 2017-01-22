#include "rpn_convert.h"

#include <string.h>
#include <stdio.h>


/**
 * Converts the operator to it's precedence
 * A lower number means it happens first (has precedence)
 * 
 * returns precedence of an operator, 0 if not found
 */
int calc_precedence(char operator)
{
	int returnPrecedence = 0;
	void *something = malloc(1234);
	
	switch(operator)
	{
		case '^':
			returnPrecedence = 1;
			break;
		case '/':
			returnPrecedence = 2;
			break;
		case '*':
			returnPrecedence = 3;
			break;
		case '-':
			returnPrecedence = 4;
			break;
		case '+':
			returnPrecedence = 5;
			break;
	}
		
	return returnPrecedence;
}

/**
 * Check for valid characters in the calculation string
 * 
 * Input:
 *   calculation_string : The pre-allocated character string to check
 *   calculation_string_length : Length of string to check
 * Output:
 *   determined_rpn_length : calculated length of rpn string
 * return RC_SUCCESS if characters are within range
 *        RC_FAILURE if any character is invalid
 */
rpn_return_code_t check_characters(char* calculation_string, 
	uint32_t calculation_string_length,
	uint32_t* determined_rpn_length)
{
	rpn_return_code_t return_value = RC_FAILURE;
	int has_operator = 0;
	int has_variable = 0;
	int index = 0;
	
	for(; index < calculation_string_length; ++index)
	{
		// look for acceptable values
		if( (calculation_string[index] >= 'a') &&
		    (calculation_string[index] <= 'z') )
		{
			has_variable = 1;
			(*determined_rpn_length)++;
			continue;
		}
		else if ( (calculation_string[index] == '^') ||
				  (calculation_string[index] == '/') ||
				  (calculation_string[index] == '*') ||
				  (calculation_string[index] == '-') ||
				  (calculation_string[index] == '+') )
		{
			has_operator = 1;
			(*determined_rpn_length)++;
			continue;
		}
		else if ( (calculation_string[index] == '(') ||
				  (calculation_string[index] == ')') )
		{
			continue;
		}
		else
		{
			break;
		}
	}
	
	/* If the break occured when the index == length, then we went 
	 * through the entire string
	 * AND
	 * There is a variable, and there is an operation for any 
	 * calculation longer than 1 variable
	 */
	if( (index + 1 == calculation_string_length) &&
		(1 == has_variable) &&
		((1 == has_operator) || (1 == (*determined_rpn_length))) )
	{
		return_value = RC_SUCCESS;
	}
	
	return return_value;
}

/**
 * Determines the lowest precedence operator from the given range.
 * Pass the complete string
 * Include every character that hasn't been used yet
 * 
 * Input:
 *   infix : The pre-allocated character string in infix notation
 *   index_start : The first character of the infix string to search
 *   index_stop : The last character of the infix string to search (inclusive)
 * Output:
 *   index_min_operator : index into infix of the least precedence operator
 * Returns type rpn_return_code_t
 */
rpn_return_code_t search_for_min_operator(char* infix, 
										  int32_t index_start,
										  int32_t index_stop,
										  uint32_t* index_min_operator)
{
	rpn_return_code_t return_value = RC_FAILURE;
	// Use the max value of the operator as the default
	int precedence = 0;
	int max_precedence = 0;
	int paren_count = 0; // Keep track of the number of parens we are deep
	//~ printf("start%d stop%d\n", index_start, index_stop);
	
	// search backwards so that ties go to the first in the string
	for(int32_t index = index_stop; index >= index_start; --index)
	{
		// Ignore anything within a parenthesis
		if(')' == infix[index])
		{
			paren_count++;
		}
		if(paren_count > 0)
		{
			// always continue, but make sure to reduce count
			if('(' == infix[index])
			{
				paren_count--;
			}		
			continue;
		}
		
		//~ printf("[%d]=%c\n", index, infix[index]);
		// If the precedence is greater than the previous max
		precedence = calc_precedence(infix[index]);
		if(precedence > max_precedence)
		{
			// We found a valid operator and it's of lower precedence
			// then the last one found
			return_value = RC_SUCCESS;
			max_precedence = precedence;
			(*index_min_operator) = index;
		}
	}
	//~ printf("found [%d]=%c\n", *index_min_operator, infix[*index_min_operator]);
	
	return return_value;
}

/**
 * Determines the index of the matching '('
 * index_start should refer to the '(' to match
 * 
 * Input:
 *   infix : The pre-allocated character string in infix notation
 *   index_start : Index to the first '(' of the infix string to search
 *   index_stop : The last char to check (inclusive)
 * Output:
 *   index_matching : index of matching ')'
 * Returns type rpn_return_code_t
 */
rpn_return_code_t matching_paren(char* infix, 
								 int32_t index_start,
								 int32_t index_stop,
								 int32_t* index_matching)
{
	rpn_return_code_t return_value = RC_FAILURE;
	int paren_count = 0; // Keep track of the number of parens we are deep
	*index_matching = -1;
	
	if(('(' != infix[index_start]) && (index_start >= index_stop))
	{
		return RC_FAILURE;
	}
	
	for(int index = index_start; index <= index_stop; ++index)
	{
		
		if('(' == infix[index])
		{
			paren_count++;
		}
		else if(')' == infix[index])
		{
			paren_count--;
			if (paren_count == 0)
			{
				*index_matching = index;
				return_value = RC_SUCCESS;
				break;
			}
		}		
		continue;
	}
	
	return return_value;
}								 

/**
 * Determines a parmeter to an operation, automatically fills in rpn
 * Pass the complete string
 * Include every character that hasn't been used yet
 * 
 * Input:
 *   infix : The pre-allocated character string in infix notation
 *   index_start : The first character of the infix string to search
 *   index_stop : The last character of the infix string to search (inclusive)
 * Input/Output:
 *   rpn : The pre-allocated character string for the RPN return value
 *   rpn_stop : The last undefined character of the rpn string
 * Returns type rpn_return_code_t
 */
rpn_return_code_t determine_value(char* infix,
								  int32_t index_start,
								  int32_t index_stop,
								  char* rpn,
								  int32_t* rpn_stop)
{
	rpn_return_code_t return_value = RC_FAILURE;
	uint32_t current_operator_index = 0;
	int32_t matching_index = 0;
	
	//~ printf("rpn_stop=%d\n", *rpn_stop);
	
	// Check important parameters
	if(rpn_stop < 0)
	{
		return RC_FAILURE;
	}
	
	if(RC_SUCCESS == matching_paren(infix, index_start, index_stop, &matching_index))
	{
		
	}
	
	// If there is only one variable left, then it must be the end
	if(index_stop == index_start)
	{
		// TODO: check char is a variable
		rpn[(*rpn_stop)] = infix[index_stop];
		//~ printf("rpn[%d]=%c\n", (*rpn_stop), rpn[(*rpn_stop)]);
		(*rpn_stop)--;
		return_value = RC_SUCCESS;
	}
	// Check to see if the first and last characters are ()
	else if(('(' == infix[index_start]) && 
			(RC_SUCCESS == matching_paren(infix, index_start, index_stop, &matching_index)) &&
			matching_index == index_stop)
	{
		// Remove them from the search range
		determine_value(infix, index_start + 1, index_stop - 1, rpn, rpn_stop);
		return_value = RC_SUCCESS;
	}
	else
	{
		// TODO: look at return codes
		
		// There must still be an operator, find it and the parameters
		search_for_min_operator(infix, index_start, index_stop, &current_operator_index);
		
		rpn[(*rpn_stop)] = infix[current_operator_index];
		//~ printf("rpn[%d]=%c\n", (*rpn_stop), rpn[(*rpn_stop)]);
		(*rpn_stop)--;
		
		// determine right side
		determine_value(infix, current_operator_index + 1, index_stop, rpn, rpn_stop);
		
		// determine left side
		determine_value(infix, index_start, current_operator_index - 1, rpn, rpn_stop);
		
		return_value = RC_SUCCESS;
	}
	
	return return_value;
}

/* See description in header file */
rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t* rpn_length)
{
	uint32_t last_used_char = infix_length - 1;
	int32_t rpn_stop = 0;
	uint32_t determined_rpn_length = 0;
	if(check_characters(infix, infix_length, &determined_rpn_length) != RC_SUCCESS)
	{
		return RC_FAILURE;
	}
	if(determined_rpn_length + 1 > (*rpn_length))
	{
		return RC_FAILURE;
	}
	
	// The last character index to use is one less than the length
	rpn_stop = determined_rpn_length - 1;
	rpn[determined_rpn_length] = 0; // Ensure null terminated
	(*rpn_length) = determined_rpn_length + 1;
	
	return determine_value(infix, 0, last_used_char-1, rpn, &rpn_stop);
}
