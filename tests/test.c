/**
 * Just a basic test right now to get the Makefile setup
 */

#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "word_processor.h"


START_TEST (basic)
{
	rpn_return_code_t return_code = RC_FAILURE;
	uint32_t lines = -1;
	//~ forward_and_reverse("a", RC_SUCCESS, "a", RC_SUCCESS, "a", __LINE__);
	return_code = convert("/usr/share/dict/words", "abanzqzdrrorrnrr");
	ck_assert_msg(RC_SUCCESS == return_code,
		"Was expecting %d, but found %d", RC_SUCCESS, return_code);
	//~ ck_assert_msg(lines == 99171,
		//~ "Was expecting %d, but found %d", 99171, lines);
	return_code = convert("/usr/share/dict/words", "zzzzzzbzhzezazzz");
	return_code = convert("/usr/share/dict/words", "znszzzbzhzezazzz");
	
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
