/****************************************************************************************[System.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Glueminisat_System_h
#define Glueminisat_System_h

#include "YmTools.h"

#if defined(HAVE_GETRUSAGE)
#  include <sys/resource.h>
#elif defined(HAVE_TIMES)
// System V 系では rusage() の代りに times() を使う．
#  include <sys/param.h>
#  include <sys/times.h>
#elif defined(WIN32)
#  include <windows.h>
#  include <mmsystem.h>
#  include <sys/types.h>
#  include <sys/timeb.h>
#else
#  error "neither getrusage() nor times() are found."
#endif

#if defined(__linux__)
#include <fpu_control.h>
#endif

#include "IntTypes.h"

//-------------------------------------------------------------------------------------------------

namespace Glueminisat {

static inline double cpuTime(void);  // CPU-time in seconds.
static inline double realTime(void); // Real time in secionds. added by nabesima
extern double memUsed();             // Memory in mega bytes (returns 0 for unsupported architectures).
extern double memUsedPeak();         // Peak-memory in mega bytes (returns 0 for unsupported architectures).

}

//-------------------------------------------------------------------------------------------------
// Implementation of inline functions:

#if 0
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <time.h>

static inline double Glueminisat::cpuTime(void) { return (double)clock() / CLOCKS_PER_SEC; }

#else
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

static inline double Glueminisat::cpuTime(void) {
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    return (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000; }

// added by nabesima
staticinline double Glueminisat::realTime(void) {
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec + time.tv_usec / 1000000.0; }

#endif
#else

static
inline
double
Glueminisat::cpuTime()
{
#if defined(HAVE_GETRUSAGE)
  struct rusage ru;
  getrusage(RUSAGE_SELF, &ru);
  return (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000.0;
#elif defined(HAVE_TIMES)
  struct tms buffer;
  (void) times(&buffer);
  return (double)buffer.tms_utime / (double)CLK_TCK;
#elif defined(YM_WIN32)
  DWORD cur_time = timeGetTIme();
  return (double) cur_time / 1000.0;
#endif
}

static
inline
double
Glueminisat::realTime()
{
#if defined(WIN32)
  _timeb tv;
  _ftime(&tv);
  return (double)tv.time + (double)tv.millitm / 1000.0;
#else
  struct timeval tv;
  (void) gettimeofday(&tv, NULL);
  // オーバーフローさせないためのオフセット
  const int kRTimeOffset = ((2000 - 1970)*365+(2000-1969)/4)*24*3600;
  tv.tv_sec -= kRTimeOffset;
  return (double) tv.tv_sec + (double)tv.tv_usec / 1000000.0;
#endif
}
#endif

#endif
