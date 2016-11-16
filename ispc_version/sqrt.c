#include <stdio.h>
#include <pthread.h>

const float MAX = 5;
const float MIN = 0;
/* Change this to whatever you'd like! */
const int NUM_ROOTS = 1024;

float x[NUM_ROOTS];
float ans[NUM_ROOTS];

int main()
{
	int i;
	
	// initialize x with random floats within [MIN, MAX]
	for (i = 0; i < NUM_ROOTS; i++)
	{
		x[i] = (float) rand()/(float)(RAND_MAX/MAX) + MIN;
	}
	
	// call sqrt in parallel for all NUM_ROOTS numbers
	sqrt_ispc(NUM_ROOTS, x, ans);
	
	// print out if you want to verify
	/*
	for (i = 0 ; i < NUM_ROOTS; i++)
	{
		printf("%f: %f\n", x[i], ans[i]);
	}
	*/
	
	return 0;
}