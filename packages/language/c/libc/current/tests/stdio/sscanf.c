//=================================================================
//
//        sscanf.c
//
//        Testcase for C library sscanf()
//
//=================================================================
//####COPYRIGHTBEGIN####
//
// -------------------------------------------
// The contents of this file are subject to the Cygnus eCos Public License
// Version 1.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://sourceware.cygnus.com/ecos
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See the
// License for the specific language governing rights and limitations under
// the License.
// 
// The Original Code is eCos - Embedded Cygnus Operating System, released
// September 30, 1998.
// 
// The Initial Developer of the Original Code is Cygnus.  Portions created
// by Cygnus are Copyright (C) 1998 Cygnus Solutions.  All Rights Reserved.
// -------------------------------------------
//
//####COPYRIGHTEND####
//=================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     ctarpy@cygnus.co.uk, jlarmour@cygnus.co.uk
// Contributors:    jlarmour@cygnus.co.uk
// Date:          1998/6/3
// Description:   Contains testcode for C library sscanf() function
//
//
//####DESCRIPTIONEND####

// Declarations for test system:
//
// TESTCASE_TYPE=CYG_TEST_MODULE


// CONFIGURATION

#include <pkgconf/libc.h>   // Configuration header

// INCLUDES

#include <stdio.h>
#include <cyg/infra/testcase.h>
#include <sys/cstartup.h>          // C library initialisation

// HOW TO START TESTS

#if defined(CYGPKG_LIBC) && defined(CYGPKG_LIBC_STDIO) && defined(CYGFUN_LIBC_STDIO_ungetc)

# define START_TEST( test ) test(0)

#else

# define START_TEST( test ) CYG_EMPTY_STATEMENT

#endif
        

// FUNCTIONS

externC void
cyg_package_start( void )
{
#ifdef CYGPKG_LIBC
    cyg_iso_c_start();
#else
    (void)main(0, NULL);
#endif
} // cyg_package_start()

#if defined(CYGPKG_LIBC) && defined(CYGPKG_LIBC_STDIO) && defined(CYGFUN_LIBC_STDIO_ungetc)

// Functions to avoid having to use libc strings

static int
my_strcmp(const char *s1, const char *s2)
{
    for ( ; *s1 == *s2 ; s1++,s2++ )
    {
        if ( *s1 == '\0') 
            break;
    } // for

    return (*s1 - *s2);
} // my_strcmp()


static char *
my_strcpy(char *s1, const char *s2)
{
    while (*s2 != '\0') {
        *(s1++) = *(s2++);
    }
    *s1 = '\0';

    return s1; 
} // my_strcpy()


static void
test( CYG_ADDRWORD data )
{
    static char x[300];
    static char y[300];
    char z[20];
    int ret;
    int int_test, int_test2, int_test3, int_test4;

    // Check 1
    my_strcpy(x, "20");
    ret = sscanf(x, "%d", &int_test);
    CYG_TEST_PASS_FAIL(int_test==20, "%d test");
    CYG_TEST_PASS_FAIL(ret == 1, "%d test return code");

    // Check 2
    my_strcpy(y, "PigsNoses");
    ret = sscanf(y, "%s", x);
    CYG_TEST_PASS_FAIL(my_strcmp(x, y)==0, "%s test");
    CYG_TEST_PASS_FAIL(ret == 1, "%s test return code");

    // Check 3
    my_strcpy(x, "FE8C");
    ret = sscanf(x, "%X", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 65164, "hex read leading zeroes");
    CYG_TEST_PASS_FAIL(ret == 1, "hex read leading zeroes return code");

    // Check 4
    my_strcpy(x, "000053Ba");
    ret = sscanf(x, "%x", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 21434, "hex read leading zeroes");
    CYG_TEST_PASS_FAIL(ret == 1, "hex read leading zeros return code");

    // Check 5
    my_strcpy(x, "53Ba");
    ret = sscanf(x, "%3x", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 1339, "hex read constrained width");
    CYG_TEST_PASS_FAIL(ret == 1, "hex read constrained width return code");

    // Check 6
    my_strcpy(x, "get the first char test");
    ret = sscanf(x, "%c", &y[0]);
    CYG_TEST_PASS_FAIL(y[0] == 'g', "%c test");
    CYG_TEST_PASS_FAIL(ret == 1, "%c test return code");

    // Check 7
    my_strcpy(x, "-5");
    ret = sscanf(x, "%d", &int_test);
    CYG_TEST_PASS_FAIL(int_test == -5, "negative integer");
    CYG_TEST_PASS_FAIL(ret == 1, "negative integer return code");
    
    // Check 8
    my_strcpy(x, "53");
    ret = sscanf(x, "%o", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 43, "%o test");
    CYG_TEST_PASS_FAIL(ret == 1, "%o test return code");
    
    // Check 9
    int_test=10;
    my_strcpy(x, "-5 0");
    ret = sscanf(x, "%*d %d", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 0, "assignment suppression");
    CYG_TEST_PASS_FAIL(ret == 1, "assignment suppression return code");
    
    // Check 10
    int_test=0;
    my_strcpy(x, "052");
    ret = sscanf(x, "%i", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 42, "octal with %i");
    CYG_TEST_PASS_FAIL(ret == 1, "octal with %i return code");
    
    // Check 10
    int_test=0;
    my_strcpy(x, "0x05f");
    ret = sscanf(x, "%i", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 95, "hex with %i");
    CYG_TEST_PASS_FAIL(ret == 1, "hex with %i return code");
    
    // Check 11
    int_test=0;
    my_strcpy(x, "+023456");
    ret = sscanf(x, "%u", &int_test);
    CYG_TEST_PASS_FAIL(int_test == 23456, "unsigned int with %u");
    CYG_TEST_PASS_FAIL(ret == 1, "unsigned int with %u return code");
    
    // Check 12
    my_strcpy(x, "aString 2747 D");
    ret = sscanf(x, "%s %d %c", y, &int_test, z);
    CYG_TEST_PASS_FAIL(  (my_strcmp(y, "aString")==0) &&
                     (int_test == 2747) &&
                     (z[0] == 'D'), "A few combinations #1");
    CYG_TEST_PASS_FAIL(ret == 3, "Combinations #1 return code");

    
    // Check 13
    my_strcpy(x, "-6 52 awurble 2747 xxx");
    ret = sscanf(x, "%d %u %s %2d %n %c",
                 &int_test, &int_test2, y, &int_test3, &int_test4, z);
    CYG_TEST_PASS_FAIL(  (int_test == -6) && (int_test2 == 52) &&
                         (my_strcmp(y, "awurble")==0) &&
                         (int_test3 == 27) &&
                         (int_test4 == 16) &&
                         (z[0] == '4'), "A few combinations #2");
    CYG_TEST_PASS_FAIL(ret == 5, "Combinations #2 return code");


#ifdef CYGSEM_LIBC_STDIO_PRINTF_FLOATING_POINT

// How much _relative_ error do we allow?
#define FLT_TOLERANCE 1.0e-3

#define MY_FABS(x) ((x) < 0.0 ? -(x) : (x))

    CYG_TEST_INFO("Starting floating point specific tests");

    {
        float flt_test, wanted;

        // Check 14
        my_strcpy(x, "2.5");
        wanted = 2.5;
        ret = sscanf(x, "%f", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test - wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %f test #1" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %f test #1 return code" );


        // Check 15
        my_strcpy(x, "-23.472345");
        wanted = -23.472345;
        ret = sscanf(x, "%f", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test - wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %f test #2" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %f test #2 return code" );

        // Check 16
        my_strcpy(x, "0.0");
        ret = sscanf(x, "%f", &flt_test);
        CYG_TEST_PASS_FAIL( flt_test==0.0, "Simple %f test 0.0" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %f test 0.0 return code" );


        // Check 17
        my_strcpy(x, "-2.6334e00");
        wanted = -2.6334;
        ret = sscanf(x, "%e", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test - wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %e test #1" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %e test #1 return code" );


        // Check 18
        my_strcpy(x, "-2.56789e-06");
        wanted = -2.56789e-06;
        ret = sscanf(x, "%e", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test-wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %e test #2" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %e test #2 return code" );

        
        // Check 19
        my_strcpy(x, "-2.12389e-06");
        wanted = -2.12389e-06;
        ret = sscanf(x, "%g", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test-wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %g test #1" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %g test #1 return code" );

        // Check 20
        my_strcpy(x, "1.3423345");
        wanted = 1.342335;
        ret = sscanf(x, "%g", &flt_test);
        CYG_TEST_PASS_FAIL( MY_FABS(flt_test-wanted) <
                            MY_FABS(wanted * FLT_TOLERANCE),
                            "Simple %g test #2" );
        CYG_TEST_PASS_FAIL( ret == 1, "Simple %g test #2 return code" );

        
    } // compound 

#else
    CYG_TEST_PASS("Floating point tests skipped - not configured");
#endif // ifdef CYGSEM_LIBC_STDIO_PRINTF_FLOATING_POINT


    CYG_TEST_FINISH("Finished tests from testcase " __FILE__ " for C library "
                    "sscanf() function");

} // test()

#endif // if defined(CYGPKG_LIBC) && defined(CYGPKG_LIBC_STDIO) && defined(CYGFUN_LIBC_STDIO_ungetc)


int
main(int argc, char *argv[])
{
    CYG_TEST_INIT();

    CYG_TEST_INFO("Starting tests from testcase " __FILE__ " for C library "
                  "sscanf() function");

    START_TEST( test );

    CYG_TEST_PASS_FINISH("Testing is not applicable to this configuration");
} // main()


// EOF sscanf.c
