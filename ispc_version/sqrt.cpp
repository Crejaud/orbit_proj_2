#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <immintrin.h>
#include "sqrt_ispc.h"

using namespace ispc;

void sqrt_seq(float x[], float ans[]);
void sqrt_avx_loop(float x[], float ans[]);
void sqrt_avx(__m256 x_avx);

const float epsilon = 0.0001f;
const float divisor = 2.f;
const float MAX = 5;
const float MIN = 0;
/* Change this to whatever you'd like! */

int main()
{
	clock_t begin, end;
	double timeSpent, timeSpentIspc, timeSpentSeq;
	float x[4194304];
	float ans[4194304];
	int i;
	printf("starting\n");
	// initialize x with random floats within [MIN, MAX]
	for (i = 0; i < 4194304; i++)
	{
		x[i] = (float) rand()/(float)(RAND_MAX/MAX) + MIN;
	}

	// CALLING SQRT IN ISPC
	int num_cores;
	int num_threads;
	// NO TASKS: 1-8 threads
  printf("ispc\n");
	for (num_threads = 1; num_threads <= 8; num_threads++)
	{
		begin = clock();
		sqrt_ispc(4194304, x, ans, num_threads);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("1 | %d thread(s) | %f cycles\n",
			num_threads,
			timeSpent);
printf("test0\n");
	}
printf("test1\n");
	// TASKS: 2-4 cores & 1-8 threads
	for (num_cores = 2; num_cores <= 4; num_cores++)
	{
		for (num_threads = 1; num_threads <= 8; num_threads++)
		{
			begin = clock();
			sqrt_ispc_tasks(4194304, x, ans, num_cores, num_threads);printf("test2\n");
			end = clock();printf("test3\n");
			timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
printf("test4\n");
			printf("%d | %d thread(s) | %f cycles\n",
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

	// now do it avx
	begin = clock();
	sqrt_avx_loop(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;
  	printf("avx\n");
	printf("%f cycles\n", timeSpentSeq);

	// now do it sequentially
	begin = clock();
	sqrt_seq(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("sequential\n");
	printf("%f cycles\n", timeSpentSeq);

	return 0;
}

void sqrt_seq(float x[], float ans[])
{
	int i;
	for (i = 0; i < 4194304; i++)
        {
                ans[i] = x[i];
                while (epsilon * ans[i] < fabsf(ans[i] - x[i]/ans[i]))
                {
                        ans[i] = (x[i]/ans[i] + ans[i])/divisor;
                }
        }
}

void sqrt_avx_loop(float x[], float ans[])
{
	int i;
	for (i = 0; i < 4194304/8; i++)
	{
		__m256 x_avx = _mm256_set_ps(x[8*i], x[8*i+1], x[8*i+2], x[8*i+3], x[8*i+4], x[8*i+5],x[8*i+6], x[8*i+7]);
		sqrt_avx(x_avx);
	}
}

void sqrt_avx(__m256 x_avx)
{
	__m256 twos = _mm256_set_ps(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	__m256 ans_avx = _mm256_div_ps(x_avx, twos);
	__m256 diff = _mm256_sub_ps(x_avx, ans_avx);

	bool diffmag = false;

	int i;
	for (i = 0; i < 8; i++)
	{
		if (fabsf(diff[i]) > epsilon)
		{
			diffmag = true;
			break;
		}
	}

	while (diffmag)
	{
		__m256 temp_ans_avx = ans_avx;
		ans_avx = _mm256_div_ps (_mm256_add_ps (ans_avx, _mm256_div_ps (x_avx, ans_avx)), twos);
		diff = _mm256_sub_ps (temp_ans_avx, ans_avx);
		diffmag = false;
		for (i = 0; i < 8; i++)
		{
			if (fabsf(diff[i]) > epsilon)
			{
				diffmag = true;
				break;
			}
		}
	}
}
