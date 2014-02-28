
/// @file USTime.sip
/// @brief USTime の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class USTime {
%TypeHeaderCode
#include "utils/USTime.h"
using nsYm::USTime;
%End

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

  const char*
  __repr__() const;
  %MethodCode
  using std::ostringstream;
  ostringstream buf;
  buf << *sipCpp;
  sipRes = buf.str().c_str();
  %End
};

USTime
operator+(USTime left,
	  USTime right);

USTime
operator-(USTime left,
	  USTime right);
