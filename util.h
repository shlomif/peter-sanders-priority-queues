// this files contains all the application independent little
// functions and macros used for the optimizer.
// In particular Peters debug macros and Dags stuff
// from dbasic.h cdefs, random,...

//////////////// stuff originally from debug.h ///////////////////////////////
// (c) 1997 Peter Sanders
// some little utilities for debugging adapted
// to the paros conventions


#ifndef UTIL
#define UTIL

// default debug level. will be overidden e.g. if debug.h is included
#ifndef DEBUGLEVEL
#define DEBUGLEVEL 3
#endif

#if DEBUGLEVEL >= 0
#define Debug0(A) A
#else
#define Debug0(A)
#endif
#if DEBUGLEVEL >= 1
#define Debug1(A) A
#else
#define Debug1(A)
#endif
#if DEBUGLEVEL >= 2
#define Debug2(A) A
#else
#define Debug2(A)
#endif
#if DEBUGLEVEL >= 3
#define Debug3(A) A
#else
#define Debug3(A)
#endif
#if DEBUGLEVEL >= 4
#define Debug4(A) A
#else
#define Debug4(A)
#endif
#if DEBUGLEVEL >= 5
#define Debug5(A) A
#else
#define Debug5(A)
#endif
#if DEBUGLEVEL >= 6
#define Debug6(A) A
#else
#define Debug6(A)
#endif

#include <iostream>

#define Assert(c) if(!(c))\
  {std::cout << "\nAssertion violation (" #c ") " << __FILE__ << ":" << __LINE__ << std::endl;}
#define Assert0(C) Debug0(Assert(C))
#define Assert1(C) Debug1(Assert(C))
#define Assert2(C) Debug2(Assert(C))
#define Assert3(C) Debug3(Assert(C))
#define Assert4(C) Debug4(Assert(C))
#define Assert5(C) Debug5(Assert(C))

#define Error(s) {std::cout << "\nError:" << s << " " << __FILE__ << ":" << __LINE__ << std::endl;}

////////////// min, max etc. //////////////////////////////////////

#ifndef Max
#define Max(x,y) ((x)>=(y)?(x):(y))
#endif

#ifndef Min
#define Min(x,y) ((x)<=(y)?(x):(y))
#endif

#ifndef Abs
#define Abs(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef PI
#define PI 3.1415927
#endif

// is this the right definition of limit?
inline double limit(double x, double bound)
{
  if      (x >  bound) { return  bound; }
  else if (x < -bound) { return -bound; }
  else                   return x;
}

/////////////////////// timing /////////////////////
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#include <errno.h>

typedef enum clockid_t {
	CLOCK_MONOTONIC,
	CLOCK_REALTIME
} clockid_t;

struct timespec64bit {
        time_t      tv_sec;  // Seconds - >= 0
		// 32-bit long, same as unix LP64
        long   tv_nsec; // Nanoseconds - [0, 999999999]
};


#define MS_PER_SEC      1000ULL     // MS = milliseconds
#define US_PER_MS       1000ULL     // US = microseconds
#define HNS_PER_US      10ULL       // HNS = hundred-nanoseconds (e.g., 1 hns = 100 ns)
#define NS_PER_US       1000ULL

#define HNS_PER_SEC     (MS_PER_SEC * US_PER_MS * HNS_PER_US)
#define NS_PER_HNS      (100ULL)    // NS = nanoseconds
#define NS_PER_SEC      (MS_PER_SEC * US_PER_MS * NS_PER_US)

inline int clock_gettime_monotonic(struct timespec64bit *tv)
{
    static LARGE_INTEGER ticksPerSec;
    LARGE_INTEGER ticks;
    double seconds;

    if (!ticksPerSec.QuadPart) {
        QueryPerformanceFrequency(&ticksPerSec);
        if (!ticksPerSec.QuadPart) {
            errno = ENOTSUP;
            return -1;
        }
    }

    QueryPerformanceCounter(&ticks);

    seconds = (double) ticks.QuadPart / (double) ticksPerSec.QuadPart;
    tv->tv_sec = (time_t)seconds;
    tv->tv_nsec = (long)((ULONGLONG)(seconds * NS_PER_SEC) % NS_PER_SEC);

    return 0;
}

inline int clock_gettime_realtime(struct timespec64bit *tv)
{
    FILETIME ft;
    ULARGE_INTEGER hnsTime;

    GetSystemTimeAsFileTime(&ft);

    hnsTime.LowPart = ft.dwLowDateTime;
    hnsTime.HighPart = ft.dwHighDateTime;

    // To get POSIX Epoch as baseline, subtract the number of hns intervals from Jan 1, 1601 to Jan 1, 1970.
    hnsTime.QuadPart -= (11644473600ULL * HNS_PER_SEC);

    // modulus by hns intervals per second first, then convert to ns, as not to lose resolution
    tv->tv_nsec = (long) ((hnsTime.QuadPart % HNS_PER_SEC) * NS_PER_HNS);
    tv->tv_sec = (long) (hnsTime.QuadPart / HNS_PER_SEC);

    return 0;
}

inline int clock_gettime(clockid_t type, struct timespec64bit *tp)
{
    if (type == CLOCK_MONOTONIC)
    {
        return clock_gettime_monotonic(tp);
    }
    else if (type == CLOCK_REALTIME)
    {
        return clock_gettime_realtime(tp);
    }

    errno = ENOTSUP;
    return -1;
}

#else
#define timespec64bit timespec
#endif

inline double wallClockTime()
{ struct timespec64bit tp;

  clock_gettime(CLOCK_REALTIME, &tp);
  return tp.tv_sec + tp.tv_nsec * 1e-9;
}

// elapsed CPU time see also /usr/include/sys/time.h
inline double cpuTime()
{ struct timespec tp;

  return clock();
//  clock_gettime(CLOCK_VIRTUAL, &tp);
//  return tp.tv_sec + tp.tv_nsec * 1e-9;
}

#endif
