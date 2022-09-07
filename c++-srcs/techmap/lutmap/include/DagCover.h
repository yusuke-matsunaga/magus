#ifndef DAGCOVER_H
#define DAGCOVER_H

/// @file DagCover.h
/// @brief DagCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
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
  DagCover(
    bool fanout_mode ///< [in] ファンアウトモード
  ): mFoMode{fanout_mode}
  {
  }

  /// @brief デストラクタ
  virtual
  ~DagCover() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  virtual
  void
  record_cuts(
    const SbjGraph& sbjgraph,    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder, ///< [in] 各ノードのカットを保持するオブジェクト
    MapRecord& maprec            ///< [out] マッピング結果を記録するオブジェクト
  ) = 0;

  /// @brief ファンアウトモードの時 true を返す．
  bool
  fanout_mode()
  {
    return mFoMode;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンアウトモードを使う時 true にするフラグ
  bool mFoMode;

};

END_NAMESPACE_LUTMAP

#endif // DAGCOVER_H
