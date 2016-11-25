#include <iostream>
#include <cstdlib>
void newton_sqrt(unsigned int i,  float x[],  float ans[])
{
	ans[i] = x[i];
	while ((0.0001 < (ans[i] - x[i]/ans[i]))||(-0.0001 > (ans[i] - x[i]/ans[i])))
	{
        	ans[i] = (x[i]/ans[i] + ans[i])/2.0;
        }
	std::cout<<"x[i] "<<x[i]<<" has root ans[i] "<<ans[i]<<"\n";
}

int main(){
	float x[6]={0,1,2,3,4,5};
	float ans[6];
	for(int i=0; i<6; ++i){
		newton_sqrt(i, x, ans);
	}
}
