/**
 * @file demo.c
 *
 * @author Erik Vesterlund
 *
 * @brief Program for demonstrating various features of the C
 * programming language.
 *
 * Currently this program demonstrates the const and static keywords,
 * more to come when I feel the need to. This program will, by
 * design, not compile - comments show which parts need to be
 * modified to be able to compile.
 */

#include <stdio.h>

#include "demo_utils.h"

/**
 * @brief Demonstrates the const keyword.
 *
 * The const keyword can be used to declare variables or pointers to
 * be constant. Several examples are shown below.
 */
void const_demo()
{
        int x = 5;
        int y = 7;
        const int z = 11;
        const int w = 13;

        /* First example, pointer to const. We first declare a pointer to a
         * const int and then show that the pointer can be reassigned, but
         * can not be used to change the value (via dereferencing).
         */
        const int *a = &z; /* initial declaration */
        a = &w;            /* changing the pointer is fine */
        *a = 1;            /* changing the value is not allowed;
                            * comment it out to be able to compile
                            */

        /* Second example, const pointer to value. We first declare a const
         * pointer to an int and then show that while we can change the
         * value (via dereferencing) we can't change what we point to.
         */
        int *const b = &x; /* initial declaration */
        *b = 1;            /* changing the value pointed to is fine */
        b = &y;            /* changing the pointer itself is not allowed;
                            * comment it out to be able to compile
                            */

        /* Third example, const pointer to const value. We first declare a
         * const pointer to a const int and then show that we can change
         * neither the pointer nor its value (via dereferencing).
         */
        const int * const c = &x; /* initial declaration */
        c = &y;                   /* changing the pointer is not allowed */
        *c = 1;                   /* changing the value is not allowed */

        /* Since the variable 'x' was not declared as constant, it can still
         * be changed in the "normal" way.
         */
        x = 1;
        printf("The value of x is %d\n", x);
}

/**
 * @brief Demonstrates mutable and immutable strings.
 */
void mutable_string_demo()
{
        char mut_str[] = "hello";  /* a mutable string */
        char *immut_str = "hello"; /* an immutable string */

        mut_str[0] = 'c';          /* this is allowed */
        immut_str[0] = 'c';        /* this will cause a segfault */
}

/**
 * @brief Demonstration of the keyword 'static'.
 *
 * This function is intended to demonstrate that one can call static
 * functions that are defined in the same file as the function call
 * takes place.
 */
static void foo2()
{
        printf("Called static function defined in \
                the same file as calling function.\n");
}

int main()
{
        /* Demonstration of the keyword 'const'. */
        const_demo();

        /* Demonstration of the difference between mutable and immutable
         * strings.
         */
        mutable_string_demo();

        /* Demonstration of the keyword 'static'. This function is static
         * and defined elsewhere and so cannot be called from this file
         * (program).
         */
        foo();

        /* This function is non-static and defined elsewhere and can of
         * course be called from here.
         */
        bar();

        /* This function is static and is defined here and can therefore be
         * called from here.
         */
        foo2();

        /* This function is non-static and defined elsewhere. It calls a
         * static function defined in the same file and so calling this
         * function here is OK.
         */
        bar2();

        return 0;
}
