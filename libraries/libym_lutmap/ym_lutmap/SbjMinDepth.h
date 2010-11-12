#ifndef YM_LUTMAP_SBJMINDEPTH_H
#define YM_LUTMAP_SBJMINDEPTH_H

/// @file ym_lutmap/SbjGraph.h
/// @brief SbjGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SbjGraph.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_lutmap/SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class SbjMinDepth SbjMinDepth.h "SbjMinDepth.h"
/// @brief SbjGraph を k-LUT にマッピングしたときの最小段数を求めるクラス
//////////////////////////////////////////////////////////////////////
class SbjMinDepth
{
public:

  /// @brief コンストラクタ
  SbjMinDepth();

  /// @brief デストラクタ
  ~SbjMinDepth();


public:
  //////////////////////////////////////////////////////////////////////
  // minimum depth 関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 各ノードの minimum depth を求める．
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] k LUT の最大入力数
  ymuint
  get_min_depth(const SbjGraph& sbjgraph,
		ymuint k);

  /// @brief 各ノードの深さを求める．
  ymuint
  node_depth(const SbjNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // mindepth 関係の関数
  //////////////////////////////////////////////////////////////////////

  // node を根とする深さ d - 1 の k-feasible cut が存在するかどうか調べる．
  bool
  find_k_cut(const SbjNode* node,
	     const vector<const SbjNode*>& input_list,
	     ymuint k,
	     ymuint d);

  // node およびその TFI に rmark を付ける．
  // depth が d のノードに tmark を付ける．
  void
  mark_tfi(const SbjNode* node,
	   ymuint d);

  // node のファンアウトを探索する．
  bool
  dfs_fanout(const SbjNode* node);

  bool
  dfs(const SbjNode* cur_node,
      int dir);


private:
  //////////////////////////////////////////////////////////////////////
  // node のマークを操作する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief range マークを調べる．
  /// @note 副作用として range マークを付ける．
  bool
  check_rmark(const SbjNode* node);

  /// @brief range マークを返す．
  bool
  rmark(const SbjNode* node);

  /// @brief target マークを返す．
  bool
  tmark(const SbjNode* node);

  /// @brief target マークを付ける．
  void
  set_tmark(const SbjNode* node);

  /// @brief flow 用のマークを返す．
  bool
  fmark(const SbjNode* node);

  /// @brief flow 用のマークを付ける．
  void
  set_fmark(const SbjNode* node);

  /// @brief range/target/flow マークを消す．
  void
  clear_rtfmark(const SbjNode* node);

  /// @brief 入力側の visit フラグを調べる．
  /// @note 副作用として v1 マークを付ける．
  bool
  check_vmark1(const SbjNode* node);

  /// @brief 出力側の visit フラグを調べる．
  /// @note 副作用として v2 マークを付ける．
  bool
  check_vmark2(const SbjNode* node);

  /// @brief visit フラグを消す．
  void
  clear_vmark(const SbjNode* node);

  /// @brief ノードのマークにアクセスする関数
  ymuint32&
  mark(const SbjNode* node);

  /// @brief flow が流れている時 true を返す．
  bool
  flow(const SbjEdge* edge);

  /// @brief flow フラグをセットする．
  void
  set_flow(const SbjEdge* edge);

  /// @brief flow フラグをクリアする．
  void
  clear_flow(const SbjEdge* edge);


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mMarkArray の演算で用いる定数
  static
  const int kRangeShift = 0;
  static
  const int kTargetShift = 1;
  static
  const int kFlowShift = 2;
  static
  const int kPoShift = 3;
  static
  const int kV1Shift = 4;
  static
  const int kV2Shift = 5;

  static
  const ymuint kRangeMask = 1U << kRangeShift;
  static
  const ymuint kTargetMask = 1U << kTargetShift;
  static
  const ymuint kFlowMask = 1U << kFlowShift;
  static
  const ymuint kPoMask = 1U << kPoShift;
  static
  const ymuint kV1Mask = 1U << kV1Shift;
  static
  const ymuint kV2Mask = 1U << kV2Shift;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最大レベル
  ymuint32 mLevel;

  // mark_tfi で用いるノードのリスト
  vector<const SbjNode*> mNodeList;

  // ノードごとの作業領域
  vector<ymuint32> mMarkArray;

  // ノードごとの深さ
  vector<ymuint32> mDepthArray;

  // 枝のフローの印を表す配列
  vector<bool> mFlowArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief range マークを調べる．
inline
bool
SbjMinDepth::check_rmark(const SbjNode* node)
{
  ymuint32& mark = this->mark(node);
  ymuint32 old_mark = mark;
  mark |= kRangeMask;
  return static_cast<bool>((old_mark >> kRangeShift) & 1U);
}

// @brief range マークを返す．
inline
bool
SbjMinDepth::rmark(const SbjNode* node)
{
  return static_cast<bool>((mark(node) >> kRangeShift) & 1U);
}

// @brief target マークを返す．
inline
bool
SbjMinDepth::tmark(const SbjNode* node)
{
  return static_cast<bool>((mark(node) >> kTargetShift) & 1U);
}

// @brief target マークを付ける．
inline
void
SbjMinDepth::set_tmark(const SbjNode* node)
{
  mark(node) |= kTargetMask;
}

// @brief flow 用のマークを返す．
inline
bool
SbjMinDepth::fmark(const SbjNode* node)
{
  return static_cast<bool>((mark(node) >> kFlowShift) & 1U);
}

// @brief flow 用のマークを付ける．
inline
void
SbjMinDepth::set_fmark(const SbjNode* node)
{
  mark(node) |= kFlowMask;
}

// @brief range/target/flow マークを消す．
inline
void
SbjMinDepth::clear_rtfmark(const SbjNode* node)
{
  mark(node) &= ~(kRangeMask | kTargetMask | kFlowMask);
}

// @brief 入力側の visit フラグを調べる．
inline
bool
SbjMinDepth::check_vmark1(const SbjNode* node)
{
  ymuint32& mark = this->mark(node);
  ymuint old_mark = mark;
  mark |= kV1Mask;
  return static_cast<bool>((old_mark >> kV1Shift) & 1U);
}

// @brief 出力側の visit フラグを調べる．
inline
bool
SbjMinDepth::check_vmark2(const SbjNode* node)
{
  ymuint32& mark = this->mark(node);
  ymuint old_mark = mark;
  mark |= kV2Mask;
  return static_cast<bool>((old_mark >> kV2Shift) & 1U);
}

// @brief visit フラグを消す．
inline
void
SbjMinDepth::clear_vmark(const SbjNode* node)
{
  mark(node) &= ~(kV1Mask | kV2Mask);
}

// @brief ノードのマークにアクセスする関数
inline
ymuint32&
SbjMinDepth::mark(const SbjNode* node)
{
  return mMarkArray[node->id()];
}

// @brief flow が流れている時 true を返す．
inline
bool
SbjMinDepth::flow(const SbjEdge* edge)
{
  return mFlowArray[edge->to()->id() * 2 + edge->pos()];
}

// @brief flow フラグをセットする．
inline
void
SbjMinDepth::set_flow(const SbjEdge* edge)
{
  mFlowArray[edge->to()->id() * 2 + edge->pos()] = true;
}

// @brief flow フラグをクリアする．
inline
void
SbjMinDepth::clear_flow(const SbjEdge* edge)
{
  mFlowArray[edge->to()->id() * 2 + edge->pos()] = false;
}

END_NAMESPACE_YM_LUTMAP

#endif // YM_LUTMAP_SBJMINDEPTH_H
