/*
 ** Copyright(C) 2007 INL
 ** Written by Victor Stinner <victor.stinner AT inl.fr>
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, version 2 of the License.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "libmacfly.h"
/*--------------------------------------------------------------------------*/
#include <time.h>          /* time_t */
#include <sys/timeb.h>     /* struct timeb */
#include <sys/time.h>      /* struct timeval */
#ifdef WRAP_PTHREAD
#  include <pthread.h>       /* pthread structs */
#endif
/*--------------------------------------------------------------------------*/


typedef time_t (*time_prototype) (time_t*);
typedef int (*gettimeofday_prototype) (struct timeval *tv, struct timezone *tz);
typedef int (*ftime_prototype) (struct timeb *tp);

static time_prototype real_time = NULL;
static gettimeofday_prototype real_gettimeofday = NULL;
static ftime_prototype real_ftime = NULL;

#ifdef WRAP_CLOCK_GETTIME
typedef int (*clock_gettime_prototype) (clockid_t clk_id, struct timespec *tp);
static clock_gettime_prototype real_clock_gettime = NULL;
#endif

#ifdef WRAP_PTHREAD
typedef int (*pthread_cond_timedwait_prototype) (pthread_cond_t * cond, pthread_mutex_t * mutex, const struct timespec * abstime);
static pthread_cond_timedwait_prototype real_pthread_cond_timedwait = NULL;
#endif

/*--------------------------------------------------------------------------*/

void init_wrappers()
{
    real_time = (time_prototype)get_symbol("time", 0);
    real_gettimeofday = (gettimeofday_prototype)get_symbol("gettimeofday", 0);
    real_ftime = (ftime_prototype)get_symbol("ftime", 0);
#ifdef WRAP_CLOCK_GETTIME
    real_clock_gettime = (clock_gettime_prototype)get_symbol("clock_gettime", 0);
#endif
#ifdef WRAP_PTHREAD
    real_pthread_cond_timedwait = (pthread_cond_timedwait_prototype)get_symbol("pthread_cond_timedwait", 0);
#endif
}

/*--------------------------------------------------------------------------*/

/**
 * Update a 'time_t' value with time delta
 */
static inline time_t update_time_t(time_t value)
{
    if (delta.is_negative)
        return value - delta.sec;
    else
        return value + delta.sec;
}

/**
 * Update a timeval structure with time delta
 */
static inline void update_timeval(struct timeval *tv)
{
    if (delta.is_negative) {
        tv->tv_sec -= delta.sec;
        if (tv->tv_usec < (int32_t)(delta.nanosec/1000)) {
            tv->tv_usec += 1000000;
            tv->tv_sec -= 1;
        }
        tv->tv_usec -= delta.nanosec / 1000;
    } else {
        tv->tv_sec += delta.sec;
        tv->tv_usec += delta.nanosec / 1000;
        if (999999 < tv->tv_usec) {
            tv->tv_usec -= 1000000;
            tv->tv_sec += 1;
        }
    }
}

/**
 * Update a timespec structure with time delta
 */
static inline void update_timespec(struct timespec *tp, int inverse)
{
    int negative = delta.is_negative;
    if (inverse)
        negative = !negative;
    if (negative) {
        tp->tv_sec -= delta.sec;
        if (tp->tv_nsec < (int32_t)delta.nanosec) {
            tp->tv_nsec += 1000000000;
            tp->tv_sec -= 1;
        }
        tp->tv_nsec -= delta.nanosec;
    } else {
        tp->tv_nsec += delta.nanosec;
        tp->tv_sec += delta.sec;
        if (999999999 < tp->tv_nsec) {
            tp->tv_nsec -= 1000000000;
            tp->tv_sec += 1;
        }
    }
}

/*--------------------------------------------------------------------------*/

/**
 * Wrapper for time()
 */
time_t time (time_t *t)
{
    time_t res;

    if (!real_time) {
        real_time = (time_prototype)get_symbol("time", 1);
    }
    network_update();
#ifdef DEBUG_WRAPPERS
    DEBUG_MSG("Call time()");
#endif
    res = real_time(t);
    if (res != ((time_t)-1)) {
        res = update_time_t(res);
        if (t) *t = res;
    }
    return res;
}

/**
 * Wrapper for gettimeofday()
 */
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    int err;

    if (!real_gettimeofday) {
        real_gettimeofday = (gettimeofday_prototype)get_symbol("gettimeofday", 1);
    }
    network_update();
    err = real_gettimeofday(tv, tz);
    if (!err && tv) {
        update_timeval(tv);
#ifdef DEBUG_WRAPPERS
        DEBUG_MSG("Call gettimeofday(%p, %p): sec=%i -- usec=%i",
            tv, tz, tv->tv_sec, tv->tv_usec);
    } else {
        DEBUG_MSG("Call gettimeofday(%p, %p)", tv, tz);
#endif
    }
    return err;
}

/**
 * Wrapper for ftime()
 */
int ftime (struct timeb *tp)
{
    int res;
    if (!real_ftime) {
        real_ftime = (ftime_prototype)get_symbol("ftime", 1);
    }
    network_update();
#ifdef DEBUG_WRAPPERS
    DEBUG_MSG("Call ftime()");
#endif
    res = real_ftime(tp);
    if (tp) {
        tp->time = update_time_t(tp->time);
    }
    return res;
}

#ifdef WRAP_CLOCK_GETTIME
/**
 * Wrapper for clock_gettime()
 */
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    int err;
    if (!real_clock_gettime) {
        real_clock_gettime = (clock_gettime_prototype)get_symbol("clock_gettime", 1);
    }
    network_update();
#ifdef DEBUG_WRAPPERS
    DEBUG_MSG("Call clock_gettime()");
#endif
    err = real_clock_gettime(clk_id, tp);
    if (!err && tp) {
        update_timespec(tp, 0);
}
#endif

#ifdef WRAP_PTHREAD
/**
 * Wrapper for pthread_cond_timedwait()
 */
int pthread_cond_timedwait(pthread_cond_t * cond,
              pthread_mutex_t * mutex,
              const struct timespec * abstime)
{
    struct timespec tp;

    if (!real_pthread_cond_timedwait) {
        real_pthread_cond_timedwait = (pthread_cond_timedwait_prototype)get_symbol("pthread_cond_timedwait", 1);
    }
    tp = *abstime;

    network_update();
    update_timespec(&tp, 1);
    return real_pthread_cond_timedwait(cond, mutex, &tp);
}
#endif

/*--------------------------------------------------------------------------*/

