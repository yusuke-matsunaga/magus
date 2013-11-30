#ifndef YM_UTILS_MSTOPWATCH_H
#define	YM_UTILS_MSTOPWATCH_H

/// @file ym_utils/MStopWatch.h
/// @brief StopWatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/USTime.h"
#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MStopWatch MStopWatch.h "ym_utils/MStopWatch.h"
/// @ingroup YmUtils
/// @brief 複数のストップウォッチを切替えるためのクラス
/// @sa USTime, StopWatch
//////////////////////////////////////////////////////////////////////
class MStopWatch
{
public:

  /// @brief コンストラクタ.
  /// @param[in] n 時計の数
  /// @param[in] id 最初にアクティブな時計番号
  MStopWatch(ymuint n,
	     ymuint id = 0);

  /// @brief デストラクタ
  ~MStopWatch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief アクティブな時計を切替える.
  /// @param[in] new_id アクティブにする時計番号
  /// @return 昔の時計番号を返す
  ymuint
  change(ymuint new_id);

  /// @brief 現在アクティブな時計番号を返す.
  /// @return 現在アクティブな時計番号を返す.
  ymuint
  cur_id() const;

  /// @brief 経過時間の取得
  /// @param[in] id 対象の時計番号
  /// @return id で指定された時計の経過時間
  USTime
  time(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 時計の数
  ymuint32 mNum;

  // 現在アクティブな時計の番号
  ymuint32 mCur;

  // StopWatchの配列
  StopWatch* mWarray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 現在アクティブな時計番号を返す．
inline
ymuint
MStopWatch::cur_id() const
{
  return mCur;
}

// idで指定された時計の経過時間を返す
inline
USTime
MStopWatch::time(ymuint id) const
{
  // 範囲外の場合は補正する．
  if ( id >= mNum ) {
    id = 0;
  }
  return mWarray[id].time();
}

END_NAMESPACE_YM

#endif // YM_UTILS_MSTOPWATCH_H
