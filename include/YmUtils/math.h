#ifndef YMUTILS_MATH_H
#define YMUTILS_MATH_H

/// @file YmUtils/math.h
/// @brief 数値関係の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


BEGIN_NAMESPACE_YM

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(float a,
	   float b)
{
  float delta = fabsf(a - b);
  return delta <= (FLT_EPSILON * a);
}

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(double a,
	   double b)
{
  double delta = fabs(a - b);
  return delta <= (DBL_EPSILON * a);
}

/// @brief 2つの数値が誤差を考慮して等しいか判断する．
inline
bool
is_similar(long double a,
	   long double b)
{
  long double delta = fabsl(a - b);
  return delta <= (LDBL_EPSILON * a);
}

END_NAMESPACE_YM

#endif // YMUTILS_MATH_H
