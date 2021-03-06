﻿#ifndef DAGCOVER_H
#define DAGCOVER_H

/// @file DagCover.h
/// @brief AreCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_LUTMAP

class CutHolder;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class DagCover DagCover.h "DagCover.h"
/// @brief DAG covering のヒューリスティックを表すクラス
//////////////////////////////////////////////////////////////////////
class DagCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] fanout_mode ファンアウトモード
  DagCover(bool fanout_mode);

  /// @brief デストラクタ
  virtual
  ~DagCover() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec) = 0;

  /// @brief ファンアウトモードの時 true を返す．
  bool
  fanout_mode();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンアウトモードを使う時 true にするフラグ
  bool mFoMode;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fanout_mode ファンアウトモード
inline
DagCover::DagCover(bool fanout_mode) :
  mFoMode(fanout_mode)
{
}

// @brief ファンアウトモードの時 true を返す．
inline
bool
DagCover::fanout_mode()
{
  return mFoMode;
}

END_NAMESPACE_LUTMAP

#endif // DAGCOVER_H
