/**
 * Just a basic test right now to get the Makefile setup
 */

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "rpn_convert.h"

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
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(expected_infix_return_code == return_code,
		"Was expecting %d, but found %d", expected_infix_return_code, return_code);
	ck_assert_str_eq(rpn, rpn_output);
	ck_assert_msg(rpn_length == strlen(rpn_output) + 1, "Was expecting length of %d, but found %d", strlen(rpn_output) + 1, rpn_length);
	
	/* Check rpn to infix */
	infix_length = sizeof(infix);
	memset(infix, 0, infix_length);
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	
	ck_assert_msg(expected_rpn_return_code == return_code,
		"Was expecting %d, but found %d", expected_rpn_return_code, return_code);
	ck_assert_str_eq(infix, infix_output);
	ck_assert_msg(infix_length == strlen(infix_output) + 1, "Was expecting length of %d, but found %d", strlen(infix_output) + 1, infix_length);

	/* consistency check */
	rpn_length = sizeof(rpn);
	memset(rpn, 0, rpn_length);
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(expected_infix_return_code == return_code,
		"Was expecting %d, but found %d", expected_infix_return_code, return_code);
	ck_assert_str_eq(rpn, rpn_output);
	ck_assert_msg(rpn_length == strlen(rpn_output) + 1, "Was expecting length of %d, but found %d", strlen(rpn_output) + 1, rpn_length);
	}

START_TEST (basic)
{
	forward_and_reverse("a", RC_SUCCESS, "a", RC_SUCCESS, "a", __LINE__);
}
END_TEST

START_TEST (basic_plus_minus_multiply_divide_exponent)
{
	forward_and_reverse("a+b", RC_SUCCESS, "ab+", RC_SUCCESS, "a+b", __LINE__);
	forward_and_reverse("a-b", RC_SUCCESS, "ab-", RC_SUCCESS, "a-b", __LINE__);
	forward_and_reverse("a*b", RC_SUCCESS, "ab*", RC_SUCCESS, "a*b", __LINE__);
	forward_and_reverse("a/b", RC_SUCCESS, "ab/", RC_SUCCESS, "a/b", __LINE__);
	forward_and_reverse("a^b", RC_SUCCESS, "ab^", RC_SUCCESS, "a^b", __LINE__);
}
END_TEST

START_TEST (three_variables)
{
	forward_and_reverse("a+b-c", RC_SUCCESS, "abc-+", RC_SUCCESS, "a+(b-c)", __LINE__);
	forward_and_reverse("a-b+c", RC_SUCCESS, "ab-c+", RC_SUCCESS, "(a-b)+c", __LINE__);
	forward_and_reverse("a-b^c", RC_SUCCESS, "abc^-", RC_SUCCESS, "a-(b^c)", __LINE__);
	forward_and_reverse("a/b*c", RC_SUCCESS, "ab/c*", RC_SUCCESS, "(a/b)*c", __LINE__);
	forward_and_reverse("a^b+c", RC_SUCCESS, "ab^c+", RC_SUCCESS, "(a^b)+c", __LINE__);
	forward_and_reverse("a+b+c", RC_SUCCESS, "ab+c+", RC_SUCCESS, "(a+b)+c", __LINE__);
}
END_TEST

START_TEST (more_variables)
{
	forward_and_reverse("a+b-c*d", RC_SUCCESS, "abcd*-+", RC_SUCCESS, "a+(b-(c*d))", __LINE__);
	forward_and_reverse("d/a-b/c", RC_SUCCESS, "da/bc/-", RC_SUCCESS, "(d/a)-(b/c)", __LINE__);
	forward_and_reverse("a-b^c^d/e", RC_SUCCESS, "abc^d^e/-", RC_SUCCESS, "a-(((b^c)^d)/e)", __LINE__);
	forward_and_reverse("l/m^n*o-p", RC_SUCCESS,  "lmn^/o*p-", RC_SUCCESS, "((l/(m^n))*o)-p", __LINE__);
	forward_and_reverse("a+b-c*d/e^f+g-h*i/j^k", RC_SUCCESS, "abcdef^/*-+ghijk^/*-+", RC_SUCCESS, "(a+(b-(c*(d/(e^f)))))+(g-(h*(i/(j^k))))", __LINE__);
	forward_and_reverse("a^b/c*d-e+f^g/h*i-j+k", RC_SUCCESS, "ab^c/d*e-fg^h/i*j-+k+", RC_SUCCESS, "(((((a^b)/c)*d)-e)+((((f^g)/h)*i)-j))+k", __LINE__);
}
END_TEST

START_TEST (basic_paren)
{
	forward_and_reverse("(a)", RC_SUCCESS, "a", RC_SUCCESS, "a", __LINE__);
	forward_and_reverse("(a+b)-c", RC_SUCCESS, "ab+c-", RC_SUCCESS, "(a+b)-c", __LINE__);
}
END_TEST

START_TEST (advanced_paren)
{
	forward_and_reverse("((l/(m^n))*o)-p", RC_SUCCESS, "lmn^/o*p-", RC_SUCCESS, "((l/(m^n))*o)-p", __LINE__);
	forward_and_reverse("((v/w)^x)*(y-z)", RC_SUCCESS, "vw/x^yz-*", RC_SUCCESS, "((v/w)^x)*(y-z)", __LINE__);
	forward_and_reverse("(a+g)*(((b-a)+c)^(c+(e*(d^f))))", RC_SUCCESS, "ag+ba-c+cedf^*+^*", RC_SUCCESS, "(a+g)*(((b-a)+c)^(c+(e*(d^f))))", __LINE__);
}
END_TEST


START_TEST (error_checking)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = 0, infix_length = 0;
	rpn_return_code_t return_code = RC_SUCCESS;

	rpn_length = sizeof(rpn);
	strcpy(infix, "A");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "&");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "---");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, " a");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "a+((b");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+))b");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+)b");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+()b");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a(+)b");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "(((((a+b)))))");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting failure, but found %d", return_code);
	
	rpn_length = sizeof(rpn);
	strcpy(infix, "((((+(a)))))");
	infix_length = strlen(infix) + 1;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	rpn_length = sizeof(rpn);
	strcpy(infix, "a+b+c+d+e");
	infix_length = strlen(infix) + 1;
	rpn_length = 6;
	return_code = convert(CONVERT_INFIX_TO_RPN, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_INPUT_LENGTH == return_code,
		"Was expecting failure, but found %d", return_code);
		
}
END_TEST

START_TEST (error_checking_infix)
{
	char infix[MAX_STRING_LENGTH] = "";
	char rpn[MAX_STRING_LENGTH] = "";
	uint32_t rpn_length = 0, infix_length = 0;
	rpn_return_code_t return_code = RC_SUCCESS;

	infix_length = sizeof(rpn);
	strcpy(rpn, "A");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, "&");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, "---");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, " a");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);
	
	infix_length = sizeof(rpn);
	strcpy(rpn, "(a)");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);
	
	infix_length = sizeof(rpn);
	strcpy(rpn, "a+");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, "abcd++");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_CHAR == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, "a+bc++d");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);
	
	infix_length = sizeof(rpn);
	strcpy(rpn, "+abcd++");
	rpn_length = strlen(rpn) + 1;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_FAILURE == return_code,
		"Was expecting failure, but found %d", return_code);

	infix_length = sizeof(rpn);
	strcpy(rpn, "abcde++++");
	rpn_length = strlen(rpn) + 1;
	infix_length = 6;
	return_code = convert(CONVERT_RPN_TO_INFIX, infix, &infix_length, rpn, &rpn_length);
	ck_assert_msg(RC_INVALID_INPUT_LENGTH == return_code,
		"Was expecting failure, but found %d", return_code);
		
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
	tcase_add_test(tc_core, error_checking_infix);
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
