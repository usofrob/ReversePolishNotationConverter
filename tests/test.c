/**
 * Just a basic test right now to get the Makefile setup
 */

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "word_processor.h"

#define MAX_STRING_LENGTH 128

void forward_and_reverse(char* infix_input,
                         rpn_return_code_t expected_infix_return_code,
                         char* rpn_output,
                         rpn_return_code_t expected_rpn_return_code,
                         char* infix_output,
                         int line_number)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = 0, infix_length = 0;
	rpn_return_code_t return_code = RC_FAILURE;
	
	//~ printf("parameters %s %d %s %d %s %d\n", infix_input, expected_infix_return_code, rpn_output, expected_rpn_return_code, infix_output, line_number);
	/* unit test code */
	rpn_length = sizeof(rpn);
	strcpy(infix, infix_input);
	infix_length = strlen(infix) + 1;
	//~ return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(expected_infix_return_code == return_code,
		"Was expecting %d, but found %d", expected_infix_return_code, return_code);
	ck_assert_str_eq(rpn, rpn_output);
	ck_assert_msg(rpn_length == strlen(rpn_output) + 1, "Was expecting length of %d, but found %d", strlen(rpn_output) + 1, rpn_length);
	
	/* Check rpn to infix */
	infix_length = sizeof(infix);
	memset(infix, 0, infix_length);
	rpn_length = strlen(rpn) + 1;
	//~ return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	
	ck_assert_msg(expected_rpn_return_code == return_code,
		"Was expecting %d, but found %d", expected_rpn_return_code, return_code);
	ck_assert_str_eq(infix, infix_output);
	ck_assert_msg(infix_length == strlen(infix_output) + 1, "Was expecting length of %d, but found %d", strlen(infix_output) + 1, infix_length);

	/* consistency check */
	rpn_length = sizeof(rpn);
	memset(rpn, 0, rpn_length);
	infix_length = strlen(infix) + 1;
	//~ return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(expected_infix_return_code == return_code,
		"Was expecting %d, but found %d", expected_infix_return_code, return_code);
	ck_assert_str_eq(rpn, rpn_output);
	ck_assert_msg(rpn_length == strlen(rpn_output) + 1, "Was expecting length of %d, but found %d", strlen(rpn_output) + 1, rpn_length);
	}

START_TEST (basic)
{
	rpn_return_code_t return_code = RC_FAILURE;
	uint32_t lines = -1;
	//~ forward_and_reverse("a", RC_SUCCESS, "a", RC_SUCCESS, "a", __LINE__);
	return_code = convert("/usr/share/dict/words", &lines);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting %d, but found %d", RC_SUCCESS, return_code);
	ck_assert_msg(lines == 99171,
		"Was expecting %d, but found %d", 99171, lines);
}
END_TEST


Suite * test_suite(void)
{
	Suite *s;
	TCase *tc_core;
	
	s = suite_create("Testing");
	
	tc_core = tcase_create("word_processor");
	
	tcase_add_test(tc_core, basic);
	suite_add_tcase(s, tc_core);
	
	return s;
}

int main(void)
{
    
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = test_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
