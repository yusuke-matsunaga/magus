
/// @file MStopWatch.sip
/// @brief StopWatch の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class MStopWatch
{
%TypeHeaderCode
#include "utils/MStopWatch.h"
using nsYm::MStopWatch;
%End

public:

  /// @brief コンストラクタ.
  /// @param[in] n 時計の数
  /// @param[in] id 最初にアクティブな時計番号
  MStopWatch(unsigned int n,
	     unsigned int id = 0);

  /// @brief アクティブな時計を切替える.
  /// @param[in] new_id アクティブにする時計番号
  /// @return 昔の時計番号を返す
  unsigned int
  change(unsigned int new_id);

  /// @brief 現在アクティブな時計番号を返す.
  /// @return 現在アクティブな時計番号を返す.
  unsigned int
  cur_id() const;

  /// @brief 経過時間の取得
  /// @param[in] id 対象の時計番号
  /// @return id で指定された時計の経過時間
  USTime
  time(unsigned int id) const;

};