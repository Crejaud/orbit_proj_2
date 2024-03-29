//GROUP 19 Project 2
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <immintrin.h>
#include "sqrt_ispc.h"

using namespace ispc;

//initialize functions & globals
void sqrt_seq(float x[], float ans[]);
void sqrt_avx_loop(float x[], float ans[]);
void sqrt_avx(__m256 x_avx);
int testtest=0;
const float epsilon = 0.0001f;
const float divisor = 2.f;
const float MAX = 5;
const float MIN = 0;

/* Change this to whatever power of 2 you'd like! Max value 524288. 
Why is this the max value? We begin to overflow our stack and cause errors if we go larger than this. 
Once we discovered this it was too late to refactor without completely redoing all the code.
Why powers of 2? To keep the AVX intrinsics as efficent as possible since they use vectors of 8 floats*/
const long long NUM_ROOTS=524288;

int main()
{
	clock_t begin, end;
	double timeSpent, timeSpentIspc, timeSpentSeq;
	float x[NUM_ROOTS];
	float ans[NUM_ROOTS];
	unsigned long long i;
	// initialize x with random floats within [MIN, MAX]
	srand(time(NULL));
	for (i = 0; i < NUM_ROOTS; i++)
	{
		x[i] = (float) (rand()%6);
	}

	// CALLING SQRT IN ISPC
	int num_cores;
	int num_threads;
	// NO TASKS: 1-8 threads
	  printf("ispc\n");
	for (num_threads = 1; num_threads <= 8; num_threads++)
	{
		begin = clock();
		sqrt_ispc(NUM_ROOTS, x, ans, num_threads);
		end = clock();
		timeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("1 | %d thread(s) | %f cycles\n", num_threads, timeSpent);
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
			printf("%d | %d thread(s) | %f cycles\n",
				num_cores,
				num_threads,
				timeSpent);
		}
	}

	// now do it avx
	begin = clock();
	sqrt_avx_loop(x, ans);
	end = clock();
        timeSpentSeq = (double)(end - begin) / CLOCKS_PER_SEC;
	
	//Uncomment the below line and line 139 to see how many times AVX ran newton ralphson
	// printf("avx ran Netwon Ralphson %d times\n", testtest);
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

//Sequential
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

//AVX Functions
void sqrt_avx_loop(float x[], float ans[])
{
	unsigned long long i;
	for (i = 0; i < NUM_ROOTS/8; i++)
	{
		//put the contents of X into vectors, which each hold 8 floats
		__m256 x_avx = _mm256_set_ps(x[8*i], x[8*i+1], x[8*i+2], x[8*i+3], x[8*i+4], x[8*i+5],x[8*i+6], x[8*i+7]);
		sqrt_avx(x_avx);
	}
}

void sqrt_avx(__m256 x_avx)
{
	//actually do the square root with vectors
	__m256 twos = _mm256_set_ps(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	__m256 ans_avx = _mm256_div_ps(x_avx, twos);
	__m256 diff = _mm256_sub_ps(x_avx, ans_avx);

	bool diffmag = false;;
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
	//	testtest++;
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
