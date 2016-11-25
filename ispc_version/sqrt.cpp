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
int testtest=0;
const float epsilon = 0.0001f;
const float divisor = 2.f;
const float MAX = 5;
const float MIN = 0;

/* Change this to whatever you'd like! */
const long long NUM_ROOTS=8;
int main()
{
	clock_t begin, end;
	double timeSpent, timeSpentIspc, timeSpentSeq;
	float x[NUM_ROOTS];
	float ans[NUM_ROOTS];
	unsigned long long i;
	// initialize x with random floats within [MIN, MAX]
	for (i = 0; i < NUM_ROOTS; i++)
	{
		x[i] = (float) (rand()%6);
	}
	printf("initial %f\n", x[0]);
	// CALLING SQRT IN ISPC
	int num_cores;
	int num_threads;
	// NO TASKS: 1-8 threads
  printf("ispc\n");
	for (num_threads = 1; num_threads <= 8; num_threads++)
	{
		begin = clock();
	printf("first try first %f and %f\n", x[0],x[1]);
	sqrt_ispc(NUM_ROOTS, x, ans, num_threads);
		printf("first try second %f\n", x[0]);end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

		printf("1 | %d thread(s) | %f cycles\n",
			num_threads,
			timeSpent);
	}
	printf("middle %f\n", x[0]);
	// TASKS: 2-4 cores & 1-8 threads
	for (num_cores = 2; num_cores <= 4; num_cores++)
	{
		for (num_threads = 1; num_threads <= 8; num_threads++)
		{
			begin = clock();
			sqrt_ispc_tasks(NUM_ROOTS, x, ans, num_cores, num_threads);
			end = clock();
			timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
			printf("%d | %d thread(s) | %f cycles\n",
				num_cores,
				num_threads,
				timeSpent);
		}
	}

	// print out if you want to verify

	/*
	for (i = 0 ; i <NUM_ROOTS; i++)
	{
		printf("%f: %f\n", x[i], ans[i]);
	}
	*/

	// now do it avx
	begin = clock();
	printf("moved x %f\n", x[0]);
	sqrt_avx_loop(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;
  	printf("avx, %d\n", testtest);
	printf("%f cycles\n", timeSpentSeq);

	// now do it sequentially
	begin = clock();	printf("seqential x %d\n", x[0]);
	sqrt_seq(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("sequential\n");
	printf("%f cycles\n", timeSpentSeq);

	return 0;
}

void sqrt_seq(float x[], float ans[])
{
	unsigned long long i;
	for (i = 0; i < NUM_ROOTS; i++)
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
	printf("inner %f\n", x[0]);
	unsigned long long i;
	for (i = 0; i < NUM_ROOTS/8; i++)
	{
		__m256 x_avx = _mm256_set_ps(x[8*i], x[8*i+1], x[8*i+2], x[8*i+3], x[8*i+4], x[8*i+5],x[8*i+6], x[8*i+7]);
		//printf("initialize %d %d  %d\n",i,  x[8*i], x_avx[0]);
		sqrt_avx(x_avx);
	}
}

void sqrt_avx(__m256 x_avx)
{
	__m256 twos = _mm256_set_ps(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	__m256 ans_avx = _mm256_div_ps(x_avx, twos);
	__m256 diff = _mm256_sub_ps(x_avx, ans_avx);

	bool diffmag = false;
	//printf("unrooted %d\n", x_avx[0]);
	unsigned long long i;
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
	//printf("rooted %d\n", ans_avx[0]);
}
