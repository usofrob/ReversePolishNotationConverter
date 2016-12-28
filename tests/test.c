/**
 * Just a basic test right now to get the Makefile setup
 */

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "rpn_convert.h"

#define MAX_STRING_LENGTH 128

START_TEST (basic)
{
	char infix[] = "a";
	char rpn[MAX_STRING_LENGTH] = "";
	rpn_return_code_t return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));

	/* unit test code */
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);

	ck_assert_str_eq(rpn, "a");
}
END_TEST

START_TEST (basic_plus_minus_multiply_divide_exponent)
{
	char infix[] = "a+b";
	char rpn[MAX_STRING_LENGTH] = "";

	rpn_return_code_t return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab+");
	
	strcpy(infix, "a-b");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab-");
	
	strcpy(infix, "a*b");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab*");
	
	strcpy(infix, "a/b");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab/");
	
	strcpy(infix, "a^b");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab^");
}
END_TEST

START_TEST (three_variables)
{
	char infix[] = "a+b-c";
	char rpn[MAX_STRING_LENGTH] = "";
	rpn_return_code_t return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));

	/* unit test code */
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abc-+");

	strcpy(infix, "a-b+c");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab-c+");

	strcpy(infix, "a-b^c");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abc^-");

	strcpy(infix, "a/b*c");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab/c*");

	strcpy(infix, "a^b+c");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab^c+");

	strcpy(infix, "a+b+c");
	return_code = convert(infix, sizeof(infix), rpn, sizeof(rpn));
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab+c+");
}
END_TEST

Suite * test_suite(void)
{
	Suite *s;
	TCase *tc_core;
	
	s = suite_create("Testing");
	
	tc_core = tcase_create("Infix_to_RPN");
	
	tcase_add_test(tc_core, basic);
	tcase_add_test(tc_core, basic_plus_minus_multiply_divide_exponent);
	tcase_add_test(tc_core, three_variables);
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
