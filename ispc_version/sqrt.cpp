#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "sqrt_ispc.h"

using namespace ispc;

void sqrt_seq(float x[], float ans[]);

const float epsilon = 0.0001f;
const float divisor = 2.f;
const float MAX = 5;
const float MIN = 0;
/* Change this to whatever you'd like! */
const int NUM_ROOTS = 16777216;

int main()
{
	clock_t begin, end;
	double timeSpent, timeSpentIspc, timeSpentSeq;
	float x[NUM_ROOTS];
	float ans[NUM_ROOTS];
	int i;

	// initialize x with random floats within [MIN, MAX]
	for (i = 0; i < NUM_ROOTS; i++)
	{
		x[i] = (float) rand()/(float)(RAND_MAX/MAX) + MIN;
	}

	// CALLING SQRT IN ISPC
	int num_cores;
	int num_threads;
	// NO TASKS: 1-8 threads
	for (num_threads = 1; num_threads <= 8; num_threads++)
	{
		begin = clock();
		sqrt_ispc(NUM_ROOTS, x, ans, num_threads);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("[sqrt ispc] [%d elements] [1 core | %d thread(s)]: [%f] cycles\n",
			NUM_ROOTS,
			num_threads,
			timeSpent);
	}

	// TASKS: 2-4 cores & 1-8 threads
	for (num_cores = 2; num_cores <= 4; num_cores++)
	{
		for (num_threads = 1; num_threads <= 8; num_threads++)
		{
			begin = clock();
			sqrt_ispc_tasks(NUM_ROOTS, x, ans, num_cores, num_threads);
			end = clock();
			timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

			printf("[sqrt ispc] [%d elements] [%d cores | %d thread(s)]: [%f] cycles\n",
				NUM_ROOTS,
				num_cores,
				num_threads,
				timeSpent);
		}
	}

	// print out if you want to verify
	/*
	for (i = 0 ; i < NUM_ROOTS; i++)
	{
		printf("%f: %f\n", x[i], ans[i]);
	}
	*/

	// now do it sequentially
	begin = clock();
	sqrt_seq(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("[sqrt seq] [%d elements]: [%f] cycles\n", NUM_ROOTS, timeSpentSeq);

	// free all allocated memory
	free(x);
	free(ans);

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
