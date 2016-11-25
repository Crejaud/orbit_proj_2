//GROUP 19 Project 2

const uniform float epsilon = 0.0001f;
const uniform float divisor = 2f;

//-------------------Computation method, actually performs newton's method-------------------//
static void newton_sqrt(unsigned int64 i, uniform float x[], uniform float ans[])
{
	ans[i] = x[i];
	while (((epsilon < (ans[i] - x[i]/ans[i]))||((-1*epsilon)>(ans[i]-(x[i]/ans[i]))))&&(x[i]!=0))
	{
	      	ans[i] = (x[i]/ans[i] + ans[i])/divisor;
	}
}

//-----the following are helper functions that call newton_sqrt differently based on the thread & core parameters-----//

static void sqrt_no_task(uniform unsigned int64 N, uniform float x[], uniform float ans[])
{
	foreach (i = 0 ... N)
	{
		newton_sqrt(i, x, ans);
	}
}

static void sqrt_t_no_task(uniform unsigned int64 N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	foreach (i = 0 ... N)
        {
                unsigned int64 first_t, last_t, span_t;
        	span_t = N / num_threads;
        	first_t = i * span_t;
        	last_t = min((i+1) * span_t, N);

		unsigned int64 i_t;
		for (i_t = first_t; i_t < last_t; i_t++)
		{
			newton_sqrt(i_t, x, ans);
		}
	}
}

static task void sqrt_task(uniform unsigned int64 N, uniform float x[], uniform float ans[])
{
	uniform unsigned int64 first, last, span;
	span = N/taskCount;
	first = taskIndex * span;
	last = min((taskIndex+1) * span, N);

	foreach (i = first ... last)
	{
		newton_sqrt(i, x, ans);
	}
}

static task void sqrt_t_task(uniform unsigned int64 N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	uniform unsigned int64 first, last, span;
        span = N/taskCount;
        first = taskIndex * span;
        last = min((taskIndex+1) * span, N);

        foreach (i = first ... last)
        {
		// in this scope:
		// we must split work into partitions
		// so total count s the span
		// i is the start of the span for threads
		unsigned int64 first_t, last_t, span_t;
		span_t = span / num_threads;
		first_t = i * span_t;
		last_t = min((i+1) * span_t, span);
		unsigned int64 i_t;
		for (i_t = first_t; i_t < last_t; i_t++)
		{
                	newton_sqrt(i_t, x, ans);
		}
        }
}

//-------------------The following are the functions that are called from main-------------------//

export void sqrt_ispc(uniform unsigned int64 N, uniform float x[], uniform float ans[], uniform int num_threads)
{
	if (num_threads <= 1)
	{
		sqrt_no_task(N, x, ans);
	}
	else
	{
		sqrt_t_no_task(N, x, ans, num_threads);
	}
}

export void sqrt_ispc_tasks(uniform unsigned int64 N, uniform float x[], uniform float ans[], uniform int num_cores2, uniform int num_threads)
{
	if (num_threads <= 1)
	{
		launch[num_cores2] sqrt_task(N, x, ans);
	}
	else
	{
		launch[num_cores2] sqrt_t_task(N, x, ans, num_threads);
	}
	sync;
}
