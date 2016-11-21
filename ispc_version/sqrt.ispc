const float epsilon = 0.0001f;
const float divisor = 2f;

export void sqrt_ispc(uniform int N, uniform float x[], uniform float ans[])
{
	foreach (i = 0 ... N)
	{
		ans[i] = x[i];
		while (epsilon * ans[i] < fabsf(ans[i] - x[i]/ans[i]))
		{
			ans[i] = (x[i]/ans[i] + ans[i])/divisor;
		}
	}
}
