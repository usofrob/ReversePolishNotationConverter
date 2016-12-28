#include "rpn_convert.h"

#include <string.h>

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

rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t rpn_length)
{
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
