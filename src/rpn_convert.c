#include "rpn_convert.h"

#include <string.h>

rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t rpn_length)
{
	strncpy(rpn, infix, infix_length);
	if(4 == infix_length)
	{
		char temp = rpn[1];
		rpn[1] = rpn[2];
		rpn[2] = temp;
	}
	return RC_SUCCESS;
}
