const float epsilon = 0.0001f;
const float divisor = 2f;

export void sqrt_ispc(uniform int N, uniform float x[], uniform float ans[], uniform int num_threads)
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

export void sqrt_ispc_tasks(uniform int N, uniform float x[], uniform float ans[], uniform int num_cores, uniform int num_threads)
{
	if (num_threads <= 1)
	{
		launch[num_cores] task_sqrt(N, x, ans);
	}
	else
	{
		launch[num_cores] task_sqrt_t(N, x, ans, num_threads);
	}
	sync;
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
                int first_t, last_t, span_t;
        	span_t = N / num_threads;
        	first_t = i * span_t;
        	last_t = min((i+1) * span_t, N);

		int i_t;
		for (i_t = first_t; i_t < last_t; i_t++)
		{
			newton_sqrt(i_t, x, ans);
		}
	}
}

task void task_sqrt(uniform int N, uniform float x[], uniform float ans[])
{
	uniform int first, last, span;
	span = N/taskCount;
	first = taskIndex * span;
	last = min((taskIndex+1) * span, N);

	foreach (i = first ... last)
	{
		newton_sqrt(i, x, ans);
	}
}

task void task_sqrt_t(uniform int N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	uniform int first, last, span;
        span = N/taskCount;
        first = taskIndex * span;
        last = min((taskIndex+1) * span, N);

        foreach (i = first ... last)
        {
		// in this scope:
		// we must split work into partitions
		// so total count s the span
		// i is the start of the span for threads
		int first_t, last_t, span_t;
		span_t = span / num_threads;
		first_t = i * span_t;
		last_t = min((i+1) * span_t, span);
		int i_t;
		for (i_t = first_t, i_t < last_t; i_t++)
		{
                	newton_sqrt(i_t, x, ans);
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
