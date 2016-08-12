#ifndef AREACOVER_H
#define AREACOVER_H

/// @file AreaCover.h
/// @brief AreCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "sbj_nsdef.h"
#include "ym/ym_bnet.h"
#include "CutHolder.h"


BEGIN_NAMESPACE_YM_LUTMAP3

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
  AreaCover();

  /// @brief デストラクタ
  ~AreaCover();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積最小化マッピングを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  /// @return LUT数を返す．
  void
  operator()(const SbjGraph& sbjgraph,
	     const CutHolder& cut_holder,
	     const vector<const SbjNode*>& boundary_list,
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

  /// @brief FFR に印をつける．
  void
  dfs(const SbjNode* node,
      vector<const SbjNode*>& node_list);

  /// @brief fanout point マークを調べる．
  bool
  fpmark(const SbjNode* node) const;

  /// @brief fanout point マークをつける．
  void
  set_fpmark(const SbjNode* node);

  /// @brief fanout point マークを消す．
  void
  clear_fpmark(const SbjNode* node);

  /// @brief テンポラリマークを調べる．
  bool
  mark(const SbjNode* node) const;

  /// @brief テンポラリマークをつける．
  void
  set_mark(const SbjNode* node);

  /// @brief テンポラリマークを消す．
  void
  clear_mark(const SbjNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymuint mMode;

  // いくつかのフラグをまとめたもの
  vector<ymuint8> mFlags;

  // 各ノードのベスト値を記録する配列．
  vector<double> mBestCost;

  // 各入力から根の出力に抜ける経路上の重みを入れる配列
  vector<double> mWeight;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief fanout point マークを調べる．
inline
bool
AreaCover::fpmark(const SbjNode* node) const
{
  return static_cast<bool>((mFlags[node->id()] >> 0) & 1U);
}

// @brief fanout point マークをつける．
inline
void
AreaCover::set_fpmark(const SbjNode* node)
{
  mFlags[node->id()] |= (1U << 0);
}

// @brief fanout point マークを消す．
inline
void
AreaCover::clear_fpmark(const SbjNode* node)
{
  mFlags[node->id()] &= ~(1U << 0);
}

// @brief テンポラリマークを調べる．
inline
bool
AreaCover::mark(const SbjNode* node) const
{
  return static_cast<bool>((mFlags[node->id()] >> 1) & 1U);
}

// @brief テンポラリマークをつける．
inline
void
AreaCover::set_mark(const SbjNode* node)
{
  mFlags[node->id()] |= (1U << 1);
}

// @brief テンポラリマークを消す．
inline
void
AreaCover::clear_mark(const SbjNode* node)
{
  mFlags[node->id()] &= ~(1U << 1);
}

END_NAMESPACE_YM_LUTMAP3

#endif // AREACOVER_H
