#ifndef _TIME_h
#define _TIME_h 1

#include <sdk/sdk.h>

// 1ns = 1e-9s
static const u64 t_frequency = 1e+9;

typedef struct timing_t
{
  f64 start, stop, elapsed;
} timing_t;

u64 t_time(void);
f64 t_ticks(void);

static inline f64 timing_start(timing_t *timer)
{
  return timer->start = t_ticks();
}

static inline f64 timing_stop(timing_t *timer)
{
  timer->stop = t_ticks();
  
  return (timer->elapsed = timer->stop - timer->start);
}

static inline f64 timing_restart(timing_t *timer)
{
  f64 elasped = timing_stop(timer);
  timing_start(timer);

  return elasped;
}

static inline f64 timing_elapsed(timing_t *timer)
{
  return t_ticks() - timer->start;
}

#endif
