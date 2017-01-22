#!/bin/bash

VALUE=$(gcov -a src/rpn_convert.c | grep "Lines executed:")

if [ ${VALUE:15:7} == "100.00%" ] ; then 
	echo "Full coverage"
	exit 0
else
	echo "${VALUE:15:7} coverage, run 'gcov -a src/rpn_convert.c' for details"
	exit 1
fi
