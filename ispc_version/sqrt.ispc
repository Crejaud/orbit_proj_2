const float epsilon = 0.0001f;
const float divisor = 2f;

export void sqrt_ispc(uniform int N, uniform float x[], uniform float ans[], uniform int num_cores, uniform int num_threads)
{
	if (num_cores <= 1)
	{
		if (num_threads <= 1)
		{
			sqrt(N, x, ans);
		}
		else
		{
			sqrt_t(N, x ans, num_threads);
		}
	}
	else
	{
		if (num_threads <= 1)
		{
			launch[num_cores] task_sqrt(N, x, ans);
		}
		else
		{
			launch[num_cores] task_sqrt_t(N, x, ans, num_threads);
		}
	}
}

void sqrt(uniform int N, uniform float x[], uniform float ans[])
{
	foreach (i = 0 ... N)
	{
		newton_sqrt(i, x, ans);
	}
}

void sqrt_t(uniform int N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	foreach (i = 0 ... N)
        {
                int start_t, end_t, range_t;
        	range_t = N / num_threads;
        	start_t = range_t * i;
        	end_t = start_t + range_t;
		
		if (i = num_threads - 1)
			end_t = num_threads;

		int j;
		for (j = start_t; j < end_t; j++)
		{
			newton_sqrt(j, x, ans);
		}
	}
}

task void task_sqrt(uniform int N, uniform float x[], uniform float ans[])
{
	uniform int start, end, range;
	range = N / (taskCount + 1);
	start = range * taskIndex;
	end = start + range;

	if (taskIndex == taskCount - 1)
		end = N;

	foreach (i = start ... end)
	{
		newton_sqrt(i, x, ans);
	}
}

task void task_sqrt_t(uniform int N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	uniform int start, end, range;
        range = N / (taskCount + 1);
        start = range * taskIndex;
        end = start + range;

        if (taskIndex == taskCount - 1)
                end = N;

        foreach (i = start ... end)
        {
		int start_t, end_t, range_t;
		range_t = range / num_threads;
		start_t = range_t * i;
		end_t = start_t + range_t;
		int j;
		for (j = start_t, j < end_t; j++)
		{
                	newton_sqrt(j, x, ans);
		}
        }
}

void newton_sqrt(int i, uniform float x[], uniform float ans[])
{
	ans[i] = x[i];
	while (epsilon * ans[i] < abs(ans[i] - x[i]/ans[i]))
	{
        	ans[i] = (x[i]/ans[i] + ans[i])/divisor;
        }
}
