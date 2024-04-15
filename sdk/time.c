#include <sdk/time.h>

#include <unistd.h>
#include <time.h>

static u64 t_initial = 0; 

u64 t_time(void)
{
  struct timespec ts;
  u64 value;

  clock_gettime(CLOCK_REALTIME, &ts);
  value = (u64) ts.tv_sec * t_frequency + (u64) ts.tv_nsec;

  if (!t_initial)
    t_initial = value;

  return value;
}

f64 t_ticks(void)
{
  return ((f64) t_time() - t_initial) / ((f64) t_frequency);
}
