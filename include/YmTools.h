#ifndef YMTOOLS_H
#define YMTOOLS_H

/// @file YmTools.h
/// @brief YmTools のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmConfig.h"

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_LIMITS_H)
#include <limits.h>
#endif

#if defined(HAVE_FLOAT_H)
#include <float.h>
#endif

#if defined(HAVE_MATH_H)
#include <math.h>
#endif

#if defined(HAVE_SYS_TIME_H) && defined(HAVE_TIME_H)
#include <sys/time.h>
#include <time.h>
#elif defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#elif defined(HAVE_TIME_H)
#include <time.h>
#else
#error "Neither <sys/time.h> nor <time.h> are not found"
#endif

#if defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>
#endif

#include "YmUtils/nsdef.h"
#include "YmUtils/stl.h"
#include "YmUtils/assert.h"


#endif // YMTOOLS_H
