/**
Copyright (C) 2018 RW Bunney, N Pritchard

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

/*Code initially developed from http://www.jera.com/techinfo/jtns/jtn002.html*/

#ifndef TUNIC_H
#define TUNIC_H

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
    PUBLIC VARIABLES FOR USER
    Defining TRUE and FALSE makes things easier, and
    means we do not need to use booleans
*/

#define TRUE  1
#define FALSE 0


enum tunic_output_level{
    STD_OUTPUT,
    TIME
};


/*
    TESTING API

    All public API functions have tunic in lower case followed by capitalised
    functionality (ASSERT, SET etc.) Functions with lower case tunic_
    are helper functions within the header file.
*/

/*
    INTEGER
*/
void tunic_ASSERT_int(int assert, int a, int b);
void tunic_ASSERT_int_array(int assert, const int *a, const int *b, unsigned long n);
void tunic_ALMOST_int_array(int assert, const int *a, const int *b, unsigned long n, int tolerance);

/*
    FLOAT
*/

void tunic_ASSERT_float(int assert, float a, float b);
void tunic_ASSERT_float_array(int assert, const float *a, const float *b, unsigned long n);
void tunic_ALMOST_float_array(int assert, const float *a, const float *b, unsigned long n, float tolerance);

/*
    DOUBLE
*/

void tunic_ASSERT_double(int assert, double a, double b);
void tunic_ASSERT_double_array(int assert, const double *a, const double *b, unsigned long n);
void tunic_ALMOST_double_array(int assert, const double *a, const double *b, unsigned long n, double tolerance);


/*
  The end of the header file!
*/
#endif //tunic_H

/*
  IMPLEMENTATION
*/

#ifdef TUNIC_LIBRARY

int test_status;
int tests_run;
int tests_passed;
float tunic_fAccuracy = 1e-7;
double tunic_dAccuracy = 1e-15;

#ifdef TIME_TESTS
clock_t start_t;
#endif

/*
  Private helper functions
*/
void tunic_update_test_status(int test_status);
void tunic_init(void);
void tunic_close(void);
int tunic_abs_int(int a);
float tunic_abs_float(float a);
double tunic_abs_double(double a);
long double tunic_abs_ldouble(long double a);


/*
===============================================================================
TEST-RUNNER MACROS
===============================================================================
*/

/*
    tunic_run_test_suite:
    Runs a test suite, which is simply a function with no return
    value that separates different 'suites' of tests (based on the
    users preferred logical/functional separation). Accepts optional
    output assertition that affects how test results are displayed
    to the user [TODO].
*/
#define tunic_run_test_suite(test_suite_name, tunic_output_level) do {\
        printf("Running tests for: '%s'\n", #test_suite_name);        \
        tunic_init();                                                 \
        if(tunic_output_level == TIME){                               \
            test_suite_name();                                        \
        } else{\
            test_suite_name();                  \
        }                                       \
        tunic_close();\
        printf("Finished test suite\n\n");      \
} while (0)


/*
===============================================================================
INTEGER
===============================================================================
*/

void tunic_ASSERT_int(int assert, int a, int b) {
    if (((a == b) && (assert == TRUE)) || ((a != b) && (assert == FALSE))) {
        tests_passed++;
        test_status =  1;
    }
    else{
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}

/*
    Element wise check due to pointer depreciation of arrays
    when passed as a parameter to a function.
*/
void tunic_ASSERT_int_array(int assert, const int *a, const int *b, unsigned long n) {
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
    tunic_update_test_status(test_status);
}

//TODO: handle negative tolerance case
void tunic_ALMOST_int_array(int assert, const int *a, const int *b, unsigned long n, int tolerance){
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (tunic_abs_int(a[i])-tunic_abs_int(b[i]) > tolerance) {
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
    tunic_update_test_status(test_status);
}

/*
===============================================================================
FLOAT
===============================================================================
*/

void tunic_ASSERT_float(int assert, float a, float b) {
    if (((tunic_abs_float(a - b) <= tunic_fAccuracy) && (assert == TRUE)) ||
        ((tunic_abs_float(a - b) > tunic_fAccuracy) && (assert == FALSE))) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}

void tunic_ASSERT_float_array(int assert, const float *a, const float *b, unsigned long n) {
    //Moving away from memcmp test for floating point values
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (tunic_abs_float(a[i] - b[i]) > tunic_fAccuracy) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}

//TODO: handle negative tolerance case
void tunic_ALMOST_float_array(int assert, const float *a, const float *b, unsigned long n, float tolerance) {
    //Moving away from memcmp test for floating point values
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (tunic_abs_float(a[i] - b[i]) > tolerance) {
            result = 0;
            break;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;

        tunic_update_test_status(test_status);
    }
}


/*
===============================================================================
DOUBLE
===============================================================================
*/

void tunic_ASSERT_double(int assert, double a, double b){
    if (((tunic_abs_double(a - b) <= tunic_dAccuracy) && (assert == TRUE)) ||
        ((tunic_abs_double(a - b) > tunic_dAccuracy) && (assert == FALSE))) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}


void tunic_ASSERT_double_array(int assert, const double *a, const double *b, unsigned long n) {
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (tunic_abs_double(a[i] - b[i]) > tunic_dAccuracy) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}

void tunic_ALMOST_double_array(int assert, const double *a, const double *b, unsigned long n, double tolerance){
    int i, result = 1;
    for (i = 0; i < n; ++i) {
        if (tunic_abs_double(a[i] - b[i]) > tolerance) {
            result = 0;
        }
    }
    if ((result == 1 && assert == TRUE) || (result == 0 && assert == FALSE)) {
        tests_passed++;
        test_status = 1;
    } else {
        test_status = 0;
    }
    tunic_update_test_status(test_status);
}


/*
===============================================================================
TIME
===============================================================================
*/

#ifdef TIME_TESTS
void tunic_start_timer(void){
    start_t = clock();
}

void tunic_stop_timer(){
    clock_t end_t;
    double seconds;

    end_t = clock();

    seconds = (double)(end_t - start_t)/1000000.f;
    printf("CPU time is %f seconds\n", seconds);
}
#endif //TIME_TESTS

/*
===============================================================================
PRIVATE FUNCTIONS
===============================================================================
*/

void tunic_init(void){
    tests_run = 0;
    tests_passed = 0;
}

void tunic_close(void){
    printf("Passed %d out of %d tests\n", tests_passed, tests_run);
}


void tunic_update_test_status(int test_status){
    tests_run++;
    if(test_status > 0){
        printf("Test %d passed\n", tests_run);
    }
    else{
        printf("Test %d failed\n", tests_run);
    }
}

int tunic_abs_int(int a){ //Sneaky bit-hax method
    int t = a >> ((sizeof(int)*8)-1);
    return t ^ (a+t);
}

float tunic_abs_float(float a){ //Wasteful, heathen but simple implementation
    return a<0?-a:a;
}

double tunic_abs_double(double a){
    return a<0?-a:a;
}

long double tunic_abs_ldouble(long double a){
    return a<0?-a:a;
}

#endif //TUNIC_LIBRARY
