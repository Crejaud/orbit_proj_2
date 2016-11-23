#include <stdio.h>

#include <immintrin.h>
int main(){
	float epsilon = 0.0001;
	__m256 n = _mm256_set_ps(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
	int k;
	for(k=0; k<8; ++k){
		printf("%f ", n[k]);
	}
	printf("\n");
	__m256 twos = _mm256_set_ps(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	__m256 i = _mm256_div_ps(n, twos);
	__m256 diff = _mm256_sub_ps(n, i);
	bool diffmag=false;
	int l;
	for(l=0; l<8; ++l){
		if(abs(diff[l])>epsilon){
			diffmag=true;
			break;
		}
	}
	while(diffmag){
		__m256 j=i;
		i= _mm256_div_ps(_mm256_add_ps(i, _mm256_div_ps(n,i)),twos);
		diff=_mm256_sub_ps(j, i);
		diffmag=false;
		for(l=0; l<8; ++l){
			if(abs(diff[l])>epsilon){
				diffmag=true;
				break;
			}
		}
	}
	for(k=0; k<8; ++k){
		printf("%f ", i[k]);
	}
	printf("\n");	
}
