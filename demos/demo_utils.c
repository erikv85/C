#include <stdio.h>

#include "demo_utils.h"

static void foo() {
	printf("Called the static function 'foo'\n");
}

void bar() {
	printf("Called the non-static function 'bar'\n");
}

void bar2() {
	printf("Called the non-static function 'bar2'. I'm calling a function\
		defined in the same file as me:\n\t");
	foo();
}
