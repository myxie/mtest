/*Code initially developed from http://www.jera.com/techinfo/jtns/jtn002.html*/

#ifndef MTEST_H
#define MTEST_H

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
/*
	Define TIME_TESTS in your test file if you want
	to use the timing functions
*/
#ifdef TIME_TESTS
#include <time.h>
#endif
/*
	TESTING API

	All public API functions have MTEST in capitals 
	prepended to the function. Functions with lower case mtest_
	are helper functions within the header file. 
*/

void mtest_SET_fAccuracy(float accuracy);

void mtest_SET_dAccuracy(double accuracy);


void mtest_ASSERT_int(int assert, int a, int b);

void mtest_ASSERT_int_array(int assert, const int *a, const int *b, unsigned long n);

void mtest_ALMOST_int_array(int assert, const int *a, const int *b, unsigned long n, int tolerance);


void mtest_ASSERT_float(int assert, float a, float b);

void mtest_ASSERT_float_array(int assert, const float *a, const float *b, unsigned long n);

void mtest_ALMOST_float_array(int assert, const float *a, const float *b, unsigned long n, float tolerance);


void mtest_ASSERT_double(int assert, double a, double b);

void mtest_ASSERT_double_array(int assert, const double *a, const double *b, unsigned long n);

void mtest_ALMOST_double_array(int assert, const double *a, const double *b, unsigned long n, double tolerance);


/*
	PUBLIC VARIABLES FOR USER
	Defining TRUE and FALSE makes things easier, and 
	means we do not need to use booleans
*/

#define TRUE  1 
#define FALSE 0

enum MTEST_output_level{
	STD_OUTPUT,
	TIME
};

/*
	The end of the header file! 
*/
#endif //MTEST_H

/*
	IMPLEMENTATION 
*/

#ifdef MTEST_LIBRARY

int test_status;
int tests_run;
int tests_passed;
float mtest_fAccuracy = 1e-7;
double mtest_dAccuracy = 1e-15;

#ifdef TIME_TESTS
clock_t start_t;
#endif

/*
	Private helper functions 
*/
void mtest_update_test_status(int test_status);
void mtest_init(void);
void mtest_close(void);
int mtest_abs_int(int a);
float mtest_abs_float(float a);
double mtest_abs_double(double a);
long double mtest_abs_ldouble(long double a);


/*
	MTEST_run_test_suite: Macro that runs a test suite, which 
	is simply a function with no return value that separates different
	'suites' of tests (based on the users preferred logical/functional
	separation). Accepts optional output assertition that affects how
	test results are displayed to the user.
*/ 
#define MTEST_run_test_suite(test_suite_name, mtest_output_level) do {\
	printf("Running tests for: '%s'\n", #test_suite_name);\
	mtest_init();\
	if(mtest_output_level == TIME){\
		test_suite_name();\
	} else{\
		test_suite_name();\
	}\
	mtest_close();	\
	printf("Finished test suite\n\n");\
} while (0)


void mtest_ASSERT_int(int assert, int a, int b) {
    if (((a == b) && (assert == TRUE)) || ((a != b) && (assert == FALSE))) {
        tests_passed++;
        test_status =  1;
    }    
    else{
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

/** Moved to an element wise check because annoyingly, when passed to a function arrays become simple pointers
 *  so it is impossible to query them for size.
 */
void mtest_ASSERT_int_array(int assert, const int *a, const int *b, unsigned long n) {
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            result = 0;
            break;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

//TODO: handle negative tolerance case
void mtest_ALMOST_int_array(int assert, const int *a, const int *b, unsigned long n, int tolerance){
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (mtest_abs_int(a[i])-mtest_abs_int(b[i]) > tolerance) {
            result = 0;
            break;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

void mtest_SET_fAccuracy(float accuracy) {
    mtest_fAccuracy = accuracy;
}

void mtest_SET_dAccuracy(double accuracy) {
    mtest_dAccuracy = accuracy;
}

void mtest_ASSERT_float(int assert, float a, float b) {
    if (((mtest_abs_float(a - b) <= mtest_fAccuracy) && (assert == TRUE)) ||
        ((mtest_abs_float(a - b) > mtest_fAccuracy) && (assert == FALSE))) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

void mtest_ASSERT_float_array(int assert, const float *a, const float *b, unsigned long n) {
    //Moving away from memcmp test for floating point values
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (mtest_abs_float(a[i] - b[i]) > mtest_fAccuracy) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

//TODO: handle negative tolerance case
void mtest_ALMOST_float_array(int assert, const float *a, const float *b, unsigned long n, float tolerance){
    //Moving away from memcmp test for floating point values
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (mtest_abs_float(a[i] - b[i]) > tolerance) {
            result = 0;
            break;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

void mtest_ASSERT_double(int assert, double a, double b) {
    if (((mtest_abs_double(a - b) <= mtest_fAccuracy) && (assert == TRUE)) ||
        ((mtest_abs_double(a - b) > mtest_fAccuracy) && (assert == FALSE))) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

void mtest_ASSERT_double_array(int assert, const double *a, const double *b, unsigned long n) {
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (mtest_abs_double(a[i] - b[i]) > mtest_fAccuracy) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

void mtest_ALMOST_double_array(int assert, const double *a, const double *b, unsigned long n, double tolerance){
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (mtest_abs_double(a[i] - b[i]) > tolerance) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    mtest_update_test_status(test_status);
}

/*
	Timing Functions
*/

#ifdef TIME_TESTS
void MTEST_start_timer(void){
    start_t = clock();
}

void MTEST_stop_timer(){
    clock_t end_t;
    double seconds;

    end_t = clock();

    seconds = (double)(end_t - start_t)/1000000.f;
    printf("CPU time is %f seconds\n", seconds);
}
#endif //TIME_TESTS

/*
	Private mtest functions that are only used in this file
*/

void mtest_init(void){
    tests_run = 0;
    tests_passed = 0;
}

void mtest_close(void){
    printf("Passed %d out of %d tests\n", tests_passed, tests_run);     
} 


void mtest_update_test_status(int test_status){
   	tests_run++;
    if(test_status > 0){
        printf("Test %d passed\n", tests_run);    
    }
    else{
        printf("Test %d failed\n", tests_run);    
    }
}

int mtest_abs_int(int a){ //Sneaky bit-hax method
    int t = a >> ((sizeof(int)*8)-1);
    return t ^ (a+t);
}

float mtest_abs_float(float a){ //Wasteful, heathen but simple implementation
    return a<0?-a:a;
}

double mtest_abs_double(double a){
    return a<0?-a:a;
}

long double mtest_abs_ldouble(long double a){
    return a<0?-a:a;
}

#endif //MTEST_LIBRARY

