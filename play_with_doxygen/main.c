/**
 * @file main.c
 *
 * @author Erik Vesterlund
 *
 * @brief File for femonstrating Doxygen documentation.
 * This line will belong to the brief.
 *
 * This part of the description is separated from the brief section
 * by an empty line, and so will not show up as part of the brief.
 *
 * We can have several paragraphs in the detailed description simply
 * by separating intended paragraphs with an empty line. Thus, the
 * above paragraph will be the first paragraph, and this the second.
 *
 * It seems like single-starred comment blocks aren't picked up at
 * all, so they can be used for internal purposes if so desired.
 *
 * If by some chance you're reading this from the actual HTML output
 * then note that the documentation itself is not intended to be
 * enlightening (the code itself is only mock-up anyway), it's the
 * documentation formatting in the source file that's the point, so
 * to speak.
 *
 * Oh and in case I forget how to generate this: you can do
 * $ doxygen main.c
 * but if you want more customization etc then do
 * $ doxygen -g
 * and then
 * $ doxygen Doxyfile
 */

// Single-line comments don't seem to be picked up either - good

#include <stdio.h> /* comments here should not be double-starred */
#include <stdlib.h>
#include <pthread.h>

/**
 * @brief Struct documentation.
 * Again, this line will belong to the brief.
 *
 * A more detailed description should start here.
 *
 * Several paragraphs is quite alright when describing structs,
 * as well.
 */
struct mystruct_s {
	int x; /**< This is one way to describe struct members. */

	int y; /**< This won't show up as brief */

	/**
	 * @brief Brief description of struct member
	 *
	 * You can describe struct members in more detail like this. I
	 * don't know which method is preferred. The ATbrief macro needn't
	 * be used, but the output will be slightly different.
	 */
	int z;

	int w; /**< @brief Will this show as brief? */
};

/**
 * @brief Occupy cores
 *
 * Useless is the probably the best way to describe this function,
 * and that's really all the details we need.
 *
 * @param arg A void pointer
 * @return void pointer
 */
void *work(void *arg) {
  while(1)
    ;
}

/**
 * @brief Classic foobar
 *
 * Quite clearly another useless function. Naturally the word foobar
 * had to appear somewhere in this file.
 *
 * @param x A value not used for anything
 * @return 0 No matter what
 */
int foo(int x)
{
	printf("bar\n");

	return 0;
}

/**
 * @brief The main function (duh)
 *
 * Should the main function even be documented? If so, we should do
 * it the same way as above. Describing the main function doesn't
 * seem to make it show up under 'Function Documentation' though,
 * which makes sense. Could be because all this shows up in the
 * 'Module' section due to the group definition below.
 *
 * @param argc The number of arguments
 * @param argv The vector of arguments (or whatever the v stands for)
 * @return 0 on success, -1 otherwise (I guess?)
 */
int main(int argc, char** argv) {
	/* Use single-starred comments to make internal "invisible" comments. */
  int nthreads = 4;
  if (argc==2) {
    if (atoi(argv[1])>1)
      nthreads = atoi(argv[1]);
  }

	/** Double-starred comments will be picked up, of course. */
  pthread_t threads[nthreads];
  int i;

	/**
	 * \defgroup super_duper_group
	 *
	 * Groups can be defined this way. Use addgroup to add more stuff
	 * to it. I don't know the use of it really, I suppose this file
	 * isn't even large enough to warrant using groups, but at least
	 * this demonstrates how to use them. Sort of, because the output
	 * of all this doesn't look very good. Anyway...
	 *
   * @{
	 */
  for (i=0; i<nthreads; i++)
    pthread_create(&threads[i], NULL, work, NULL);
	/** @} */ /* this final AT closes the group */

  for (i=0; i<nthreads; i++)
    pthread_join(threads[i], NULL);

  return 0;
}
