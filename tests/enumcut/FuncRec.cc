
/// @file FuncRec.cc
/// @brief FuncRec の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "FuncRec.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// クラス FuncRec
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncRec::FuncRec()
{
  mMinSize = 1;
}

// @brief 最小カットサイズを設定する．
void
FuncRec::set_min_size(ymuint size)
{
  if ( size > 0 ) {
    mMinSize = size;
  }
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
FuncRec::all_init(const BdnMgr& sbjgraph,
		  ymuint limit)
{
  mCurPos = 0;
  mNcAll = 0;
  mNf = 0;
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
FuncRec::node_init(const BdnNode* node)
{
  mNcCur = 0;
  mCurNode = node;
#if 1
  cout << "#" << mCurPos << ": Node#" << node->id() << endl;
#endif
}


BEGIN_NONAMESPACE

// cut_to_func の下請け関数
TvFunc
make_func(const BdnNode* node,
	  hash_map<ymuint, TvFunc>& f_map)
{
  hash_map<ymuint, TvFunc>::const_iterator p = f_map.find(node->id());
  if ( p != f_map.end() ) {
    return p->second;
  }

  assert_cond( node->type() == BdnNode::kLOGIC, __FILE__, __LINE__);

  TvFunc func0 = make_func(node->fanin0(), f_map);
  if ( node->fanin0_inv() ) {
    func0 = ~func0;
  }

  TvFunc func1 = make_func(node->fanin1(), f_map);
  if ( node->fanin1_inv() ) {
    func1 = ~func1;
  }

  TvFunc func;
  if ( node->is_and() ) {
    func = func0 & func1;
  }
  else {
    func = func0 ^ func1;
  }

  f_map.insert(make_pair(node->id(), func));

  return func;
}

// @brief 関数を作る．
TvFunc
cut_to_func(const BdnNode* root,
	    ymuint ni,
	    const BdnNode** inputs)
{
  hash_map<ymuint, TvFunc> f_map;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = inputs[i];
    TvFunc f = TvFunc::posi_literal(ni, VarId(i));
    f_map.insert(make_pair(node->id(), f));
  }
  TvFunc f = make_func(root, f_map);

  return f;
}

END_NONAMESPACE


void
FuncRec::found_cut(const BdnNode* root,
		   ymuint ni,
		   const BdnNode** inputs)
{
  if ( ni < mMinSize ) {
    return;
  }

  ++ mNcCur;

  TvFunc f = cut_to_func(root, ni, inputs);

  if ( mFuncHash.count(f) == 0 ) {
    mFuncHash.insert(f);
    ++ mNf;
  }

#if 0
  cout << "found_cut(" << root->id() << ", {";
  for (ymuint i = 0; i < ni; ++ i) {
    cout << " " << inputs[i]->id();
  }
  cout << "}" << endl;
#endif
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
void
FuncRec::node_end(const BdnNode* node)
{
  assert_cond( node == mCurNode, __FILE__, __LINE__);
  ++ mCurPos;
  mNcAll += mNcCur;

#if 1
  cout << "    " << mNcCur << " cuts" << endl
       << endl;
#endif
}

// @brief 処理の最後に呼ばれる関数
void
FuncRec::all_end(const BdnMgr& sbjgraph,
		 ymuint limit)
{
  cout << "Total " << setw(12) << mNcAll << " cuts" << endl
       << "      " << setw(12) << mNf << " functions" << endl;
}

END_NAMESPACE_YM_NETWORKS
