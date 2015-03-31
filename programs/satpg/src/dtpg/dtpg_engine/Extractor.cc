
/// @file Extractor.cc
/// @brief Extractor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Extractor.h"
#include "TpgNode.h"
#include "NodeValList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Extractor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val_map ノードの値割当を保持するクラス
Extractor::Extractor(const ValMap& val_map) :
  mValMap(val_map)
{
}

// @brief デストラクタ
Extractor::~Extractor()
{
}

// @brief 値割当を求める．
// @param[in] fnode 故障のあるノード
// @param[out] assign_list 値の割当リスト
void
Extractor::operator()(TpgNode* fnode,
		      NodeValList& assign_list)
{
  // 故障差の伝搬している経路を探す．
  mNodeList.clear();
  mReachabilityMap.clear();
  bool stat = get_sensitized_node(fnode);
  ASSERT_COND( stat );

  if ( false ) {
    cout << " propagation path =";
    for (vector<TpgNode*>::iterator p = mNodeList.begin();
	 p != mNodeList.end(); ++ p) {
      TpgNode* node = *p;
      cout << " Node#" << node->id();
    }
    cout << endl;
  }

  // その経路の side input の値を記録する．
  mRecorded.clear();
  assign_list.clear();
  for (vector<TpgNode*>::iterator p = mNodeList.begin();
       p != mNodeList.end(); ++ p) {
    TpgNode* node = *p;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_side_inputs(inode, assign_list);
    }
  }

  if ( false ) {
    ymuint n = assign_list.size();
    for (ymuint i = 0; i < n; ++ i) {
      NodeVal nv = assign_list[i];
      TpgNode* node = nv.node();
      cout << " Node#" << node->id() << ":";
      if ( nv.val() ) {
	cout << "1";
      }
      else {
	cout << "0";
      }
    }
    cout << endl;
  }
}

// @brief 故障の影響を伝搬するノードを求める．
// @param[in] node 対象のノード
bool
Extractor::get_sensitized_node(TpgNode* node)
{
  // まずすでに計算済みかどうか調べる．
  bool reached = false;
  if ( !mReachabilityMap.find(node->id(), reached) ) {
    reached = node->is_output();
    if ( mValMap.gval(node) != mValMap.fval(node) ) {
      // node までは sensitized されていた．
      ymuint nfo = node->active_fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	TpgNode* onode = node->active_fanout(i);
	if ( get_sensitized_node(onode) ) {
	  reached = true;
	}
      }
      if ( reached ) {
	mNodeList.push_back(node);
      }
    }
    mReachabilityMap.add(node->id(), reached);
  }
  return reached;
}

// @brief side inputs の値を記録する．
// @param[in] node 対象のノード
// @param[out] assign_list 値割当を記録するリスト
void
Extractor::record_side_inputs(TpgNode* node,
			      NodeValList& assign_list)
{
  if ( mRecorded.check(node->id()) ) {
    return;
  }
  mRecorded.add(node->id());

  // node の値を記録する．
  record_node(node, assign_list);

  if ( !node->has_fvar() ) {
    return;
  }

  ymuint ni = node->fanin_num();

  if ( mValMap.gval(node) == mValMap.fval(node) ) {
    // ファンインには sensitized node があって
    // side input がある場合．
    bool has_cval = false;
    bool has_snode = false;
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      if ( !inode->has_fvar() && inode->cval() == mValMap.gval(inode) ) {
	// inode が side input でかつ制御値を持っている．
	record_side_inputs(inode, assign_list);
	return;
      }
    }
    // ファンインに再帰する．
    // ただし，故障の影響の伝搬を止める値の割当のみを記録する．
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_masking_node(inode, assign_list);
    }
  }
  else {
    // ファンインに再帰する．
    // 実は side inputs ではない sensitized node にも再起しているが
    // もんだいはない．
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_side_inputs(inode, assign_list);
    }
  }
}

// @brief 故障の影響の伝搬を阻害する値割当を記録する．
// @param[in] node 対象のノード
// @param[out] assign_list 値割当を記録するリスト
void
Extractor::record_masking_node(TpgNode* node,
			       NodeValList& assign_list)
{
  if ( mRecorded.check(node->id()) ) {
    return;
  }
  mRecorded.add(node->id());

  if ( !node->has_fvar() ) {
    record_node(node, assign_list);
    return;
  }

  ASSERT_COND ( mValMap.gval(node) == mValMap.fval(node) );

  ymuint ni = node->fanin_num();
  // ファンインには sensitized node があって
  // side input がある場合．
  bool has_cval = false;
  bool has_snode = false;
  TpgNode* cnode = NULL;
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() ) {
      if ( mValMap.gval(inode) != mValMap.fval(inode) ) {
	has_snode = true;
      }
    }
    else {
      if ( inode->cval() == mValMap.gval(inode) ) {
	has_cval = true;
	cnode = inode;
	break;
      }
    }
    if ( has_snode && has_cval ) {
      record_side_inputs(cnode, assign_list);
      return;
    }
  }

  // ファンインに再帰する．
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() && mValMap.gval(inode) != mValMap.fval(inode) ) {
      record_side_inputs(inode, assign_list);
    }
    else {
      record_masking_node(inode, assign_list);
    }
  }
}

// @brief ノードの割当を記録する．
// @param[in] node 対象のノード
// @param[out] assign_list 値割当を記録するリスト
void
Extractor::record_node(TpgNode* node,
		       NodeValList& assign_list)
{
  bool val = (mValMap.gval(node) == kVal1);
  assign_list.add(node, val);
}

END_NAMESPACE_YM_SATPG
