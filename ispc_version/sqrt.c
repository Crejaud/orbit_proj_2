#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "sqrt_ispc.h"

using namespace ispc;

const float epsilon = 0.0001f;
const float divisor = 2f;
const float MAX = 5;
const float MIN = 0;
/* Change this to whatever you'd like! */
const int NUM_ROOTS = 1024;

int main()
{
	clock_t begin, end;
	double timeSpent;
	float x[NUM_ROOTS], x_seq[NUM_ROOTS];
	float ans[NUM_ROOTS], ans_seq[NUM_ROOTS];

	int i;
	
	// initialize x with random floats within [MIN, MAX]
	for (i = 0; i < NUM_ROOTS; i++)
	{
		float rand_num = (float) rand()/(float)(RAND_MAX/MAX) + MIN;
		x[i] = rand_num;
		x_seq[i] = rand_num;
	}
	
	// call sqrt in parallel for all NUM_ROOTS numbers
	begin = clock();
	sqrt_ispc(NUM_ROOTS, x, ans);
	end = clock();
        timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("Time for ispc sqrt of %d elements: %f", NUM_ROOTS, timeSpent);

	// print out if you want to verify
	/*
	for (i = 0 ; i < NUM_ROOTS; i++)
	{
		printf("%f: %f\n", x[i], ans[i]);
	}
	*/

	// now do it sequentially
	begin = clock();
	sqrt_seq(x_seq, ans_seq);
	end = clock();
        timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("Time for sequential sqrt of %d elements: %f", NUM_ROOTS, timeSpent);

	return 0;
}

void sqrt_seq(float x[], float ans[])
{
	int i;
	for (i = 0; i < NUM_ROOTS; i++)
        {
                ans[i] = x[i];
                while (epsilon * ans[i] < fabsf(ans[i] - x[i]/ans[i]))
                {
                        ans[i] = (x[i]/ans[i] + ans[i])/divisor;
                }
        }
}
