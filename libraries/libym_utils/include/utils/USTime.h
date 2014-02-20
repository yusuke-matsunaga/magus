#ifndef UTILS_USTIME_H
#define UTILS_USTIME_H

/// @file utils/USTime.h
/// @brief USTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.

#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class USTime StopWatch.h "utils/StopWatch.h"
/// @ingroup YmUtils
/// @brief ユーザー時間とシステム時間および実時間をもつクラス
/// @sa StopWatch, MStopWatch
//////////////////////////////////////////////////////////////////////
class USTime
{
public:

  /// @brief 空のコンストラクタ
  USTime();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] u ユーザー時間(usec)
  /// @param[in] s システム時間(usec)
  /// @param[in] r 実経過時間(usec)
  USTime(double u,
	 double s,
	 double r);

  /// @brief デストラクタ
  ~USTime();

  // コピーコンストラクタと代入はデフォルトで大丈夫


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定用関数
  /// @param[in] u ユーザー時間(usec)
  /// @param[in] s システム時間(usec)
  /// @param[in] r 実経過時間(usec)
  void
  set(double u,
      double s,
      double r);

  /// @brief ユーザー時間の取得
  /// @return ユーザー時間(usec)
  /// @sa sys_time(), real_time()
  double
  usr_time_usec() const;

  /// @brief システム時間の取得
  /// @return システム時間(usec)
  /// @sa usr_time(), real_time()
  double
  sys_time_usec() const;

  /// @brief 実経過時間を取り出す．
  /// @return 実経過時間(usec)
  /// @sa usr_time(), sys_time()
  double
  real_time_usec() const;

  /// @brief ユーザー時間の取得
  /// @return ユーザー時間(sec)
  /// @sa sys_time(), real_time()
  double
  usr_time() const;

  /// @brief システム時間の取得
  /// @return システム時間(sec)
  /// @sa usr_time(), real_time()
  double
  sys_time() const;

  /// @brief 実経過時間を取り出す．
  /// @return 実経過時間(sec)
  /// @sa usr_time(), sys_time()
  double
  real_time() const;

  /// @brief 加算付き代入文
  /// @param[in] a 第一引数
  /// @return 自分自身に a を足したもの
  /// @sa operator+(const USTime& a, const USTime& b);
  const USTime&
  operator+=(const USTime& a);

  /// @brief 減算付き代入文
  /// @param[in] a 第一引数
  /// @return 自分自身から a を引いたもの
  /// @sa operator-(const USTime& a, const USTime& b)
  const USTime&
  operator-=(const USTime& a);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ユーザー時間 in microseconds
  double mUtime;

  // システム時間 in microseconds
  double mStime;

  // 実時間 in microseconds
  double mRtime;

};


/// @name 時間計測用関数
/// @{

/// @relates USTime
/// @ingroup YmUtils
/// @brief 加算
/// @param[in] a 第一引数
/// @param[in] b 第二引数
/// @return a + b
/// @sa USTime::operator+=(const USTime& a)
USTime
operator+(const USTime& a,
	  const USTime& b);

/// @relates USTime
/// @ingroup YmUtils
/// @brief 減算
/// @param[in] a 第一引数
/// @param[in] b 第二引数
/// @return a - b
/// @sa USTime::operator-=(const USTime& a)
USTime
operator-(const USTime& a,
	  const USTime& b);

/// @relates USTime
/// @ingroup YmUtils
/// @brief 時間をストリームに出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] time 出力対象の時間
/// @return ストリーム s
ostream&
operator<<(ostream& s,
	   const USTime& time);

/// @}


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
inline
USTime::USTime()
{
}

// 値を設定するコンストラクタ
inline
USTime::USTime(double u,
	       double s,
	       double r) :
  mUtime(u),
  mStime(s),
  mRtime(r)
{
}

// @brief デストラクタ
inline
USTime::~USTime()
{
}

// 設定
inline
void
USTime::set(double u,
	    double s,
	    double r)
{
  mUtime = u;
  mStime = s;
  mRtime = r;
}

// ユーザー時間を取り出す．
inline
double
USTime::usr_time_usec() const
{
  return mUtime;
}

// システム時間を取り出す
inline
double
USTime::sys_time_usec() const
{
  return mStime;
}

// 実時間を取り出す
inline
double
USTime::real_time_usec() const
{
  return mRtime;
}

// ユーザー時間を取り出す．
inline
double
USTime::usr_time() const
{
  return mUtime / (1000 * 1000);
}

// システム時間を取り出す
inline
double
USTime::sys_time() const
{
  return mStime / (1000 * 1000);
}

// 実時間を取り出す
inline
double
USTime::real_time() const
{
  return mRtime / (1000 * 1000);
}

// 加算
inline
USTime
operator+(const USTime& a,
	  const USTime& b)
{
  return USTime(a).operator+=(b);
}

// 加算付き代入
inline
const USTime&
USTime::operator+=(const USTime& a)
{
  mUtime += a.mUtime;
  mStime += a.mStime;
  mRtime += a.mRtime;
  return *this;
}

// 減算
inline
USTime
operator-(const USTime& a,
	  const USTime& b)
{
  return USTime(a).operator-=(b);
}

// 減算つき代入
inline
const USTime&
USTime::operator-=(const USTime& a)
{
  mUtime -= a.mUtime;
  mStime -= a.mStime;
  mRtime -= a.mRtime;
  return *this;
}

END_NAMESPACE_YM

#endif // UTILS_USTIME_H
