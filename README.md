# ReversePolishNotationConverter
Allows the conversion math notations. 
Supports converting from infix (common) notation to Reverse Polish Notation (RPN).
The string input must only contain lowercase letters, the basic operators and parenthesis. Spaces and other characters are not allowed.

To do the conversion from Infix to RPN, the code searches for lowest priority precendence operator, anything within () is considered a higher priority.
That operator is added to the end of the RPN string.
Then search for the right and left parameters.
If the parameter search only has a single character, then it is the parameter returned.
This process builds the RPN notation string from the last character first, to the first character.
There are length, parameters and range checks to make sure things are safe.

## Environment requirements
*	Ubuntu 14.04 or 16.04
*	Run install script (./scripts/installEnvironment.sh)

## Compiling
*	make clean all

## Testing
### Unit test
*	make clean test

### Memory Usage Check
*	make clean mem-leak-check

### Code Coverage Check
*	make clean coverage

## Library API basics
The library defines these functions:
### convert()
Converts from infix notation to Reverse Polish Notation (RPN) or from RPN to infix notation.
See 'rpn_convert.h' for more details on parameters and usage.
