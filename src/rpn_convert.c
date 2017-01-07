#include "rpn_convert.h"

#include <string.h>
#include <stdio.h>


/**
 * Converts the operator to it's precedence
 * A lower number means it happens first (has precedence)
 */
int calc_precedence(char operator)
{
	int returnPrecedence = 0;
	
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
 * return RC_SUCCESS if characters are within range
 *        RC_FAILURE if any character is invalid
 */
rpn_return_code_t check_characters(char* calculation_string, 
	uint32_t calculation_string_length)
{
	rpn_return_code_t return_value = RC_FAILURE;
	int has_operator = 0;
	int has_variable = 0;
	int index = 0;
	
	for(; index < calculation_string_length; ++index)
	{
		//~ printf("[%d] = %d\n", index, calculation_string[index]);
		// look for acceptable values
		if( (calculation_string[index] >= 'a') &&
		    (calculation_string[index] <= 'z') )
		{
			has_variable = 1;
			continue;
		}
		else if ( (calculation_string[index] == '^') ||
				  (calculation_string[index] == '/') ||
				  (calculation_string[index] == '*') ||
				  (calculation_string[index] == '-') ||
				  (calculation_string[index] == '+') )
		{
			has_operator = 1;
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
		((1 == has_operator) || (2 == calculation_string_length)) )
	{
		return_value = RC_SUCCESS;
	}
	
	return return_value;
}

/**
 * Pass the complete string
 * Include every character that hasn't been used yet
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
	//~ printf("start%d stop%d\n", index_start, index_stop);
	// search backwards so that ties go to the first in the string
	for(int32_t index = index_stop; index >= index_start; --index)
	{
		//~ printf("index%d\n", index);
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
		    //~ printf("found [%d]=%c\n", index, infix[index]);
		}
	}
	//~ printf("found [%d]=%c\n", *index_min_operator, infix[*index_min_operator]);
	
	return return_value;
}

/**
 * Pass the complete string
 * Include every character that hasn't been used yet
 */
rpn_return_code_t determine_value(char* infix, 
								  int32_t index_start,
								  int32_t index_stop,
								  char* rpn, 
								  int32_t* rpn_start,
								  int32_t* rpn_stop)
{
	rpn_return_code_t return_value = RC_FAILURE;
	uint32_t current_operator_index = 0;
	
	//~ printf("rpn_start=%d rpn_stop=%d\n", *rpn_start, *rpn_stop);
	
	// If there is only one variable left, then it must be the end
	if(index_stop == index_start)
	{
		//~ printf("leaf[%d]=%c\n", index_stop, infix[index_stop]);
		// TODO: check char is a variable
		rpn[(*rpn_stop)] = infix[index_stop];
		//~ printf("rpn[%d]=%c\n", (*rpn_stop), rpn[(*rpn_stop)]);
		(*rpn_stop)--;
		return_value = RC_SUCCESS;
	}
	else
	{
		// TODO: look at return codes
		
		// There must still be an operator, find it and the parameters
		search_for_min_operator(infix, index_start, index_stop, &current_operator_index);
		//   Put it on the rightmost available
		rpn[(*rpn_stop)] = infix[current_operator_index];
		//~ printf("rpn[%d]=%c\n", (*rpn_stop), rpn[(*rpn_stop)]);
		(*rpn_stop)--;
		// determine right side
		determine_value(infix, current_operator_index + 1, index_stop, rpn, rpn_start, rpn_stop);
		//   Put in next slot
		// determine left side
		determine_value(infix, index_start, current_operator_index - 1, rpn, rpn_start, rpn_stop);
		//   Put in next slot
		return_value = RC_SUCCESS;
	}
	
	return return_value;
}

/* See description in header file */
rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t rpn_length)
{
	uint32_t last_used_char = infix_length - 1;
	int32_t rpn_start = 0;
	int32_t rpn_stop = (int32_t) last_used_char - 1;
	if(check_characters(infix, infix_length) == RC_FAILURE)
	{
		return RC_FAILURE;
	}
		
	determine_value(infix, 0, last_used_char-1, rpn, &rpn_start, &rpn_stop);
	
	return RC_SUCCESS;
}
