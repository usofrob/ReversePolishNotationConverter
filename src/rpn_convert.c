#include "rpn_convert.h"

#include <string.h>

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
		if ('-' == infix[3])
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
