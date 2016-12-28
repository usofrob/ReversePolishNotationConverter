#include "rpn_convert.h"

#include <string.h>
#include <stdio.h>


/**
 * Converts the operator to it's precedence
 * A lower number means it happens first (has precedence)
 */
int precedence(char operator)
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

/* See description in header file */
rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t rpn_length)
{
	if(check_characters(infix, infix_length) == RC_FAILURE)
	{
		return RC_FAILURE;
	}
	
	strncpy(rpn, infix, infix_length);
	if(2 == infix_length)
	{
		// Nothing to do
	}
	else if(4 == infix_length)
	{
		char temp = rpn[1];
		rpn[1] = rpn[2];
		rpn[2] = temp;
	}
	else if(6 == infix_length)
	{
		// If the second operator has precedence over the first
		if (precedence(infix[3]) < precedence(infix[1]))
		{
			rpn[1] = infix[2];
			rpn[2] = infix[4];
			rpn[3] = infix[3];
			rpn[4] = infix[1];
		}
		else
		{
			rpn[1] = infix[2];
			rpn[2] = infix[1];
			rpn[3] = infix[4];
			rpn[4] = infix[3];
		}
	}
	else
	{
		return RC_FAILURE;
	}
	
	return RC_SUCCESS;
}
