
/// @file StopWatch.sip
/// @brief StopWatch の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class StopWatch
{
%TypeHeaderCode
#include "utils/StopWatch.h"
using nsYm::StopWatch;
%End
public:

  /// @brief コンストラクタ．
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態で初期化される．
  StopWatch();

  /// @brief "リセット" する.
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態にリセットする．計測途中の場合には今までの累積値は捨てられる．
  void
  reset();

  /// @brief 計測開始.
  /// @note すでに計時中だったらなにもしない．
  void
  start();

  /// @brief 計測を止める.
  /// @note
  /// - すでに停止状態だったらなにもしない．
  /// - 計測状態だったら直前の start() から今までの経過時間を
  /// 累積時間に足す．
  void
  stop();

  /// @brief 累積時間の取得
  /// @retval ラップタイム 計測状態の場合
  /// @retval 経過累積時間 停止状態
  USTime
  time() const;

};