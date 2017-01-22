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
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);

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
	uint32_t rpn_length = sizeof(rpn);

	rpn_return_code_t return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab+");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a-b");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab-");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a*b");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab*");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a/b");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab/");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a^b");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab^");
}
END_TEST

START_TEST (three_variables)
{
	char infix[] = "a+b-c";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);

	/* unit test code */
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abc-+");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a-b+c");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab-c+");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a-b^c");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abc^-");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a/b*c");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab/c*");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a^b+c");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab^c+");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+b+c");
	return_code = convert(1, infix, sizeof(infix), rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab+c+");
}
END_TEST

START_TEST (more_variables)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = RC_FAILURE;

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+b-c*d");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abcd*-+");

	rpn_length = sizeof(rpn);
	strcpy(infix, "d/a-b/c");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "da/bc/-");

	rpn_length = sizeof(rpn);
	strcpy(infix, "a-b^c^d/e");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abc^d^e/-");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "l/m^n*o-p");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "lmn^/o*p-");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a+b-c*d/e^f+g-h*i/j^k");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "abcdef^/*-+ghijk^/*-+");
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a^b/c*d-e+f^g/h*i-j+k");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab^c/d*e-fg^h/i*j-+k+");
}
END_TEST

START_TEST (basic_paren)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = RC_FAILURE;

	// Are allowed to have () as the outside chars
	rpn_length = sizeof(rpn);
	strcpy(infix, "(a)");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting failure, but found %d", return_code);
		
	rpn_length = sizeof(rpn);
	strcpy(infix, "(a+b)-c");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ab+c-");
}
END_TEST

START_TEST (advanced_paren)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = RC_FAILURE;
		
	rpn_length = sizeof(rpn);
	strcpy(infix, "((l/(m^n))*o)-p");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "lmn^/o*p-");
		
	rpn_length = sizeof(rpn);
	strcpy(infix, "((v/w)^x)*(y-z)");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "vw/x^yz-*");
		
	rpn_length = sizeof(rpn);
	strcpy(infix, "(a+g)*(((b-a)+c)^(c+(e*(d^f))))");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);
	ck_assert_str_eq(rpn, "ag+ba-c+cedf^*+^*");
}
END_TEST


START_TEST (error_checking)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = RC_SUCCESS;

	rpn_length = sizeof(rpn);
	strcpy(infix, "A");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "&");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "---");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, " a");
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+b+c+d+e");
	rpn_length = 6;
	return_code = convert(1, infix, strlen(infix) + 1, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);
		
}
END_TEST

START_TEST (basic_rpn_to_infix)
{
	char infix[] = "a";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = sizeof(rpn);
	rpn_return_code_t return_code = convert(0, infix, sizeof(infix), rpn, &rpn_length);

	/* unit test code */
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting success, but found %d", return_code);

	ck_assert_str_eq(rpn, "a");
}
END_TEST

Suite * test_suite(void)
{
	Suite *s;
	TCase *tc_core;
	
	s = suite_create("Testing");
	
	tc_core = tcase_create("Infix_to_RPN");
	
	tcase_add_test(tc_core, basic);
	tcase_add_test(tc_core, basic_paren);
	tcase_add_test(tc_core, advanced_paren);
	tcase_add_test(tc_core, basic_plus_minus_multiply_divide_exponent);
	tcase_add_test(tc_core, three_variables);
	tcase_add_test(tc_core, more_variables);
	tcase_add_test(tc_core, error_checking);
	tcase_add_test(tc_core, basic_rpn_to_infix);
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
