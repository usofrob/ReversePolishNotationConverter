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

To do the conversion from RPN to Infix, assume that you'll always include () whenever there is more than one operator.
That would make length of the Infix string = (num_operators - 1) * 2 + rpn_length.
Parse from right to left
*	Read operator
*	index--
*	if (not first pass), then write ')'
*	Determine parameter 2
*	index--
*	Determine parameter 1
*	index--
*	if (not first pass), then write '('
The result has a lot more '()' than the original infix notation. A future improvement would be to remove the paren's when they are determined not to be needed.


## Environment Requirements
*	Ubuntu 14.04 or 16.04
*	Run install script (./scripts/installEnvironment.sh)

## Compiling
*	make clean all

## Testing
### Unit Test
*	make clean test

### Memory Usage Check
*	make clean mem-leak-check

### Code Coverage Check
*	make clean coverage

## Library API Basics
The library defines these functions:
### convert()
Converts from infix notation to Reverse Polish Notation (RPN) or from RPN to infix notation.
See 'rpn_convert.h' for more details on parameters and usage.

## Additional Notes
###	Commit Message Format
*	NG = Not Good, this commit has an error, probably from adding a new test
*	OK = Good, this commit runs successfully
*	REF = Refactor, this commit refactors successfully running code

### Automatic Tests
This project includes a file called 'JenkinsFile' that when used with the 
github plugin for Jenkins, will automatically run through the different 
compilation and tests as described above. It can also be used as an example 
of how to run the steps manually.
