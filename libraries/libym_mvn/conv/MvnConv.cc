
/// @file libym_mvn/conv/MvnConv.cc
/// @brief MvnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_YM_MVN

// @brief AND ノードを作る．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] sbjnode0, sbjnode1 ファンインのノード
// @param[in] inv0, inv1 ファンインの極性
// @param[out] oinv 出力の極性
// @return 生成したノードを返す．
// @note どちらかのファンインが定数の場合でも正しく動く．
SbjNode*
MvnConv::make_and(SbjGraph& sbjgraph,
		  SbjNode* sbjnode0,
		  SbjNode* sbjnode1,
		  bool inv0,
		  bool inv1,
		  bool& oinv)
{
  SbjNode* sbjnode = NULL;
  oinv = false;
  if ( sbjnode0 == NULL ) {
    if ( inv0 ) {
      // 入力0が1固定
      sbjnode = sbjnode1;
      oinv = inv1;
    }
    else {
      // 入力0が0固定
      sbjnode = NULL;
      oinv = false;
    }
  }
  else if ( sbjnode1 == NULL ) {
    if ( inv1 ) {
      // 入力1が1固定
      sbjnode = sbjnode0;
      oinv = inv0;
    }
    else {
      // 入力1が0固定
      sbjnode = NULL;
      oinv = false;
    }
  }
  else {
    sbjnode = sbjgraph.new_and(sbjnode0, sbjnode1, inv0, inv1);
    oinv = false;
  }
  return sbjnode;
}

// @brief OR ノードを作る．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] sbjnode0, sbjnode1 ファンインのノード
// @param[in] inv0, inv1 ファンインの極性
// @param[out] oinv 出力の極性
// @return 生成したノードを返す．
// @note どちらかのファンインが定数の場合でも正しく動く．
void
MvnConv::make_or(SbjGraph& sbjgraph,
		 SbjNode* sbjnode0,
		 SbjNode* sbjnode1,
		 bool inv0,
		 bool inv1,
		 bool& oinv)
{
  SbjNode* sbjnode = NULL;
  oinv = false;
  if ( sbjnode0 == NULL ) {
    if ( inv0 ) {
      // 入力0が1固定
      sbjnode = NULL;
      oinv = true;
    }
    else {
      // 入力0が0固定
      sbjnode = sbjnode1;
      oinv = inv1;
    }
  }
  else if ( sbjnode1 == NULL ) {
    if ( inv1 ) {
      // 入力1が1固定
      sbjnode = NULL;
      oinv = true;
    }
    else {
      // 入力1が0固定
      sbjnode = sbjnode0;
      oinv = inv0;
    }
  }
  else {
    sbjnode = sbjgraph.new_and(sbjnode0, sbjnode1, !inv0, !inv1);
    oinv = true;
  }
  return sbjnode;
}

// @brief XOR ノードを作る．
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] sbjnode0, sbjnode1 ファンインのノード
// @param[in] inv0, inv1 ファンインの極性
// @param[out] oinv 出力の極性
// @return 生成したノードを返す．
// @note どちらかのファンインが定数の場合でも正しく動く．
void
MvnConv::make_xor(SbjGraph& sbjgraph,
		  SbjNode* sbjnode0,
		  SbjNode* sbjnode1,
		  bool inv0,
		  bool inv1,
		  bool& oinv)
{
  SbjNode* sbjnode = NULL;
  oinv = inv0 ^ inv1;
  if ( sbjnode0 == NULL ) {
    sbjnode = sbjnode1;
  }
  else if ( sbjnode1 == NULL ) {
    sbjnode = sbjnode0;
  }
  else {
    sbjnode = sbjgraph.new_xor(sbjnode0, sbjnode1);
  }
  return sbjnode;
}

END_NAMESPACE_YM_MVN
