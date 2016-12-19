#include "rpn_convert.h"

#include <string.h>

rpn_return_code_t convert(char* infix, 
            uint32_t infix_length,
            char* rpn,
            uint32_t rpn_length)
{
	strncpy(rpn, infix, infix_length);
	return RC_SUCCESS;
}
