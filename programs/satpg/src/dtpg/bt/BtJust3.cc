
/// @file BtJust3.cc
/// @brief BtJust3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJust3.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Just3' タイプの生成を行なう．
BackTracer*
new_BtJust3()
{
  return new BtJust3();
}


//////////////////////////////////////////////////////////////////////
// クラス BtJust3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtJust3::BtJust3()
{
}

// @brief デストラクタ
BtJust3::~BtJust3()
{
}

BEGIN_NONAMESPACE

END_NONAMESPACE

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
// @param[out] assign_list 値の割当リスト
void
BtJust3::operator()(TpgNode* fnode,
		    const vector<Bool3>& model,
		    const vector<TpgNode*>& input_list,
		    const vector<TpgNode*>& output_list,
		    NodeValList& assign_list)
{
  // 故障差の伝搬している経路を探す．
  HashMap<ymuint, bool> mark1;
  vector<TpgNode*> tmp_list;
  dfs(fnode, model, mark1, tmp_list);

  if ( false ) {
    cout << " propagation path =";
    for (vector<TpgNode*>::iterator p = tmp_list.begin();
	 p != tmp_list.end(); ++ p) {
      TpgNode* node = *p;
      cout << " Node#" << node->id();
    }
    cout << endl;
  }

  // その経路の side input の値を記録する．
  HashSet<ymuint> mark;
  assign_list.clear();
  for (vector<TpgNode*>::iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    TpgNode* node = *p;
    ymuint ni = node->fanin_num();
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_recur(inode, model, mark, assign_list);
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

bool
BtJust3::dfs(TpgNode* node,
	     const vector<Bool3>& model,
	     HashMap<ymuint, bool>& mark,
	     vector<TpgNode*>& node_list)
{
  bool ans = false;
  if ( mark.find(node->id(), ans) ) {
    return ans;
  }

  bool reached = node->is_output();
  if ( node_gval(node, model) != node_fval(node, model) ) {
    ymuint nfo = node->active_fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      TpgNode* onode = node->active_fanout(i);
      if ( dfs(onode, model, mark, node_list) ) {
	reached = true;
      }
    }
    if ( reached ) {
      node_list.push_back(node);
    }
  }
  mark.add(node->id(), reached);
  return reached;
}

void
BtJust3::record_recur(TpgNode* node,
		      const vector<Bool3>& model,
		      HashSet<ymuint>& mark,
		      NodeValList& assign_list)
{
  if ( mark.check(node->id()) ) {
    return;
  }

  mark.add(node->id());

  record_value(node, model, assign_list);

  if ( !node->has_fvar() ) {
    return;
  }

  ymuint ni = node->fanin_num();

  if ( node_gval(node, model) == node_fval(node, model) ) {
    // ファンインには sensitized node があって
    // side input がある場合．
    bool has_cval = false;
    bool has_snode = false;
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      if ( !inode->has_fvar() && inode->cval() == node_gval(inode, model) ) {
	record_recur(inode, model, mark, assign_list);
	return;
      }
    }
    // ファンインに再帰する．
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_recur2(inode, model, mark, assign_list);
    }
  }
  else {
    // ファンインに再帰する．
    for (ymuint i = 0; i < ni; ++ i) {
      TpgNode* inode = node->fanin(i);
      record_recur(inode, model, mark, assign_list);
    }
  }
}

void
BtJust3::record_recur2(TpgNode* node,
		       const vector<Bool3>& model,
		       HashSet<ymuint>& mark,
		       NodeValList& assign_list)
{
  if ( mark.check(node->id()) ) {
    return;
  }

  mark.add(node->id());

  if ( !node->has_fvar() ) {
    record_value(node, model, assign_list);
    return;
  }

  ASSERT_COND ( node_gval(node, model) == node_fval(node, model) );

  ymuint ni = node->fanin_num();
  // ファンインには sensitized node があって
  // side input がある場合．
  bool has_cval = false;
  bool has_snode = false;
  TpgNode* cnode = NULL;
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() ) {
      if ( node_gval(inode, model) != node_fval(inode, model) ) {
	has_snode = true;
      }
    }
    else {
      if ( inode->cval() == node_gval(inode, model) ) {
	has_cval = true;
	cnode = inode;
	break;
      }
    }
    if ( has_snode && has_cval ) {
      record_recur(cnode, model, mark, assign_list);
      return;
    }
  }

  // ファンインに再帰する．
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    if ( inode->has_fvar() && node_gval(inode, model) != node_fval(inode, model) ) {
      record_recur(inode, model, mark, assign_list);
    }
    else {
      record_recur2(inode, model, mark, assign_list);
    }
  }
}

END_NAMESPACE_YM_SATPG
