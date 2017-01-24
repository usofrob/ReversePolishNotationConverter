#include "rpn_convert.h"

#include <string.h>
#include <stdio.h>



/**
 * Generic return code meanings
 */
typedef enum param_side_t
{
	PARAM_FIRST,
	PARAM_SECOND
} param_side_t;

/**
 * Converts the operator to it's precedence
 * A lower number means it happens first (has precedence)
 * 
 * returns precedence of an operator, 0 if not found
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
 * 
 * Input:
 *   calculation_string : The pre-allocated character string to check
 *   calculation_string_length : Length of string to check
 * Output:
 *   determined_rpn_length : calculated length of rpn string
 * return RC_SUCCESS if characters are within range
 *        RC_FAILURE if any character is invalid
 */
rpn_return_code_t check_characters(int infix_to_rpn,
	char* calculation_string, 
	uint32_t calculation_string_length,
	uint32_t* determined_length)
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
			(*determined_length)++;
			continue;
		}
		else if ( (calculation_string[index] == '^') ||
				  (calculation_string[index] == '/') ||
				  (calculation_string[index] == '*') ||
				  (calculation_string[index] == '-') ||
				  (calculation_string[index] == '+') )
		{
			// Add 2 for every operator to account for () for every 
			// operator more than 1
			if (infix_to_rpn)
			{
				(*determined_length)++;
			}
			else
			{
				if (has_operator)
				{
					(*determined_length) += 3;
				}
				else
				{
					(*determined_length)++;
				}
			}
			has_operator = 1;
			continue;
		}
		else if ((infix_to_rpn) && 
				 ((calculation_string[index] == '(') ||
				  (calculation_string[index] == ')')))
		{
			continue;
		}
		else
		{
			break;
		}
	}
	
	// TODO: Check the correct ratio of operators to variables
	
	/* IF the break occured when the index == length, 
	 * THEN we went through the entire string
	 * AND There is a variable
	 * AND there is an operation for any calculation longer than 1 variable
	 */
	if( (index + 1 == calculation_string_length) &&
		(1 == has_variable) &&
		((1 == has_operator) || (1 == (*determined_length))) )
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
rpn_return_code_t determine_rpn(char* infix,
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
		determine_rpn(infix, index_start + 1, index_stop - 1, rpn, rpn_stop);
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
		determine_rpn(infix, current_operator_index + 1, index_stop, rpn, rpn_stop);
		
		// determine left side
		determine_rpn(infix, index_start, current_operator_index - 1, rpn, rpn_stop);
		
		return_value = RC_SUCCESS;
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
rpn_return_code_t determine_infix(char* rpn,
								  int32_t* index_stop,
								  char* infix,
								  int32_t* infix_stop,
								  param_side_t param)
{
	rpn_return_code_t return_value = RC_FAILURE;
	char next_operator = 0;
	
	//~ printf("index_stop=%d\n", *index_stop);
	//~ printf("infix_stop=%d\n", *infix_stop);
	//~ printf("param=%d\n", param);
	// Check important parameters, only needed during development
	//~ if(*index_stop < 0)
	//~ {
		//~ return RC_FAILURE;
	//~ }
	
	// check char is a variable, then return
	if( (rpn[*index_stop] >= 'a') &&
		(rpn[*index_stop] <= 'z') )
	{
		//~ printf("1rpn[%d]=%c\n", *index_stop, rpn[*index_stop]);
		infix[*infix_stop] = rpn[*index_stop];
		//~ printf("1infix[%d]=%c\n", *infix_stop, infix[*infix_stop]);
		(*index_stop)--;
		(*infix_stop)--;
		return_value = RC_SUCCESS;
	}
	else
	{
		// TODO: look at return codes
		
		// There must still be an operator, determine next set
		next_operator = rpn[*index_stop];
		//~ printf("3rpn[%d]=%c\n", *index_stop, rpn[*index_stop]);
		(*index_stop)--;
		
		// If not first pass, then set right paren
		if (param == PARAM_SECOND)
		{
			infix[*infix_stop] = ')';
			//~ printf("1infix[%d]=%c\n", *infix_stop, infix[*infix_stop]);
			(*infix_stop)--;
		}
		
		// determine right side
		// pass the next index to use, fills in parens as needed, returns new range
		determine_infix(rpn, index_stop, infix, infix_stop, PARAM_SECOND);

		// Store the operator in the next space
		infix[*infix_stop] = next_operator;
		//~ printf("1infix[%d]=%c\n", *infix_stop, infix[*infix_stop]);
		(*infix_stop)--;
		
		// determine left side
		determine_infix(rpn, index_stop, infix, infix_stop, PARAM_SECOND);
		
		// If not first pass, then set left paren
		if (param == PARAM_SECOND)
		{
			infix[*infix_stop] = '(';
			//~ printf("1infix[%d]=%c\n", *infix_stop, infix[*infix_stop]);
			(*infix_stop)--;
		}
		
		return_value = RC_SUCCESS;
	}
	
	return return_value;
}

/* See description in header file */
rpn_return_code_t convert(int infix_to_rpn,
            char* infix, 
            uint32_t* infix_length,
            char* rpn,
            uint32_t* rpn_length)
{
	char *input_str = NULL;
	char *output_str = NULL;
	uint32_t *input_length = 0;
	uint32_t *output_length = 0;
	int32_t last_used_char = 0;
	int32_t output_stop = 0;
	uint32_t determined_length = 0;
	
	if (infix_to_rpn)
	{
		input_str = infix;
		input_length = infix_length;
		output_str = rpn;
		output_length = rpn_length;
	}
	else
	{
		input_str = rpn;
		input_length = rpn_length;
		output_str = infix;
		output_length = infix_length;
	}
	last_used_char = (*input_length) - 1;
	//~ printf("convert %d %s %d %s %d %d\n", infix_to_rpn, input_str, *input_length, output_str, *output_length, last_used_char);

	if(check_characters(infix_to_rpn, input_str, *input_length, &determined_length) != RC_SUCCESS)
	{
		return RC_INVALID_CHAR;
	}
	if(determined_length + 1 > (*output_length))
	{
		return RC_INVALID_INPUT_LENGTH;
	}
	
	// The last character index to use is one less than the length
	output_stop = determined_length - 1;
	output_str[determined_length] = 0; // Ensure null terminated
	(*output_length) = determined_length + 1;
	
	//~ printf("determined_length %d\n", determined_length);
	
	if (infix_to_rpn)
	{
		return determine_rpn(input_str, 0, last_used_char-1, output_str, &output_stop);
	}
	else
	{
		last_used_char--;
		return determine_infix(input_str, &last_used_char, output_str, &output_stop, PARAM_FIRST);
	}
}
