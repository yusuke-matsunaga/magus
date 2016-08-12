#ifndef AREACOVER_H
#define AREACOVER_H

/// @file AreaCover.h
/// @brief AreCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;
class CutHolder;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @brief 面積モードの DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] mode モード
  AreaCover(ymuint mode);

  /// @brief デストラクタ
  ~AreaCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // node から各入力にいたる経路の重みを計算する．
  void
  calc_weight(const SbjNode* node,
	      const Cut* cut,
	      double cur_weight);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モード
  ymuint mMode;

  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

};

END_NAMESPACE_YM_LUTMAP

#endif // AREACOVER_H
