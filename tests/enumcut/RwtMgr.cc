
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"
#include "RwtPat.h"
#include "RwtNode.h"
#include "ym_logic/NpnMgr.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RwtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RwtMgr::RwtMgr()
{
  mInputNum = 0;
  mNodeNum = 0;
  mNodeArray = NULL;
}

// @brief デストラクタ
RwtMgr::~RwtMgr()
{
  clear();
}

// @brief クリアする．
void
RwtMgr::clear()
{
  delete [] mNodeArray;
  mInputNum = 0;
  mNodeNum = 0;
  mNodeArray = 0;
  mPatMap.clear();
}

// @brief 初期化する．
// @param[in] input_num 入力数
// @param[in] node_num 内部ノード数
// @note 全ノード数は input_num + node_num + 1(定数0) となる．
void
RwtMgr::init(ymuint input_num,
	     ymuint node_num)
{
  clear();
  mInputNum = input_num;
  mNodeNum = input_num + node_num + 1;
  mNodeArray = new RwtNode[mNodeNum];

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeArray[i].mId = i;
  }

  // 定数0ノードの設定
  {
    RwtNode* node = &mNodeArray[0];
    node->mType = 0UL;
    set_func(node, TvFunc::const_zero(input_num));
  }

  // 入力ノードの設定
  for (ymuint i = 0; i < mInputNum; ++ i) {
    RwtNode* node = &mNodeArray[i + 1];
    node->mType = (i << 2) | 1UL;
    set_func(node, TvFunc::posi_literal(input_num, i));
  }
}

BEGIN_NONAMESPACE

// ポインタと極性の情報をマージする．
ympuint
encode(const RwtNode* node,
       bool inv)
{
  return reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv);
}

END_NONAMESPACE

// @brief ノードの情報を設定する．
// @param[in] id ノードの ID番号
// @param[in] is_xor XOR ノードの時に true にするフラグ
// @param[in] fanin0 ファンイン0のノード番号
// @param[in] fanin1 ファンイン1のノード番号
// @param[in] inv0 ファンイン0の極性
// @param[in] inv1 ファンイン1の極性
void
RwtMgr::set_node(ymuint id,
		 bool is_xor,
		 ymuint fanin0,
		 ymuint fanin1,
		 bool inv0,
		 bool inv1)
{
  assert_cond( id < mNodeNum, __FILE__, __LINE__);

  RwtNode* node = &mNodeArray[id];
  RwtNode* node0 = &mNodeArray[fanin0];
  RwtNode* node1 = &mNodeArray[fanin1];

  TvFunc f0 = node0->mFunc;
  if ( inv0 ) {
    f0.negate();
  }

  TvFunc f1 = node1->mFunc;
  if ( inv1 ) {
    f1.negate();
  }

  TvFunc f;
  if ( is_xor ) {
    node->mType = 3UL;
    f = f0 ^ f1;
  }
  else {
    node->mType = 2UL;
    f = f0 & f1;
  }

  node->mFanin0 = encode(node0, inv0);
  node->mFanin1 = encode(node1, inv1);

  set_func(node, f);
}

BEGIN_NONAMESPACE

ymuint
traverse(const RwtNode* node,
	 vector<bool>& mark,
	 vector<const RwtNode*>& inputs)
{
  if ( mark[node->id()] ) {
    return 0;
  }
  mark[node->id()] = true;

  ymuint n = 1;
  if ( node->is_input() ) {
    inputs.push_back(node);
  }
  else if ( !node->is_const0() ) {
    n += traverse(node->fanin0_node(), mark, inputs);
    n += traverse(node->fanin1_node(), mark, inputs);
  }
  return n;
}

void
traverse2(const RwtNode* node,
	  vector<bool>& mark,
	  const RwtNode** node_list,
	  ymuint& last)
{
  if ( !mark[node->id()] ) {
    return;
  }

  if ( !node->is_const0() ) {
    assert_cond( !node->is_input(), __FILE__, __LINE__);

    traverse2(node->fanin0_node(), mark, node_list, last);
    traverse2(node->fanin1_node(), mark, node_list, last);
  }
  node_list[last] = node;
  ++ last;
  mark[node->id()] = false;
}

END_NONAMESPACE

// @brief ノードと論理関数を結びつける．
// @param[in] node ノード
// @param[in] f 論理関数
void
RwtMgr::set_func(RwtNode* node,
		 const TvFunc& f)
{
  node->mFunc = f;

  RwtPat* pat = new RwtPat;

  vector<bool> mark(mNodeNum, false);
  vector<const RwtNode*> inputs;
  inputs.reserve(mInputNum);
  ymuint n = traverse(node, mark, inputs);
  pat->mNodeNum = n;
  pat->mInputNum = inputs.size();
  pat->mNodeList = new const RwtNode*[n];
  for (ymuint i = 0; i < pat->mInputNum; ++ i) {
    const RwtNode* node = inputs[i];
    pat->mNodeList[i] = node;
    mark[node->id()] = false;
  }
  ymuint last = pat->mInputNum;
  traverse2(node, mark, pat->mNodeList, last);
  assert_cond( last == n, __FILE__, __LINE__);
  assert_cond( pat->mNodeList[n - 1] == node, __FILE__, __LINE__);

  mNpnMgr.cannonical(f, pat->mCmap);
  TvFunc f1 = f.xform(pat->mCmap);
  hash_map<TvFunc, RwtPat*>::iterator p = mPatMap.find(f1);
  if ( p == mPatMap.end() ) {
    mPatMap.insert(make_pair(f1, pat));
  }
  else {
    RwtPat* pat1 = p->second;
    for ( ; pat1->mLink; pat1 = pat1->mLink) ;
    pat1->mLink = pat;
  }
}

// @brief 論理関数に対するパタンを返す．
// @param[in] f 論理関数
// @param[out] cmap 変換マップ
const RwtPat*
RwtMgr::pat(const TvFunc& f,
	    NpnMap& cmap)
{
  mNpnMgr.cannonical(f, cmap);
  TvFunc f1 = f.xform(cmap);
  hash_map<TvFunc, RwtPat*>::iterator p = mPatMap.find(f1);
  if ( p == mPatMap.end() ) {
    return NULL;
  }
  else {
    return p->second;
  }
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
RwtMgr::print(ostream& s) const
{
  s << "InputNum: " << input_num() << endl
    << "NodeNum : " << node_num() << endl;
  for (ymuint i = 0; i < node_num(); ++ i) {
    const RwtNode* node = &mNodeArray[i];

    s << "Node#" << node->id() << ": ";
    if ( node->is_const0() ) {
      s << "CONST0";
    }
    else if ( node->is_input() ) {
      s << "INPUT[" << node->input_id() << "]";
    }
    else {
      if ( node->is_and() ) {
	s << "AND";
      }
      else {
	s << "XOR";
      }
      s << "(";
      if ( node->fanin0_inv() ) {
	s << "~";
      }
      s << node->fanin0_node()->id()
	<< ", ";
      if ( node->fanin1_inv() ) {
	s << "~";
      }
      s << node->fanin1_node()->id()
	<< ")";
    }
    s << endl;
  }

  ymuint nf = 0;
  ymuint np = 0;
  for (hash_map<TvFunc, RwtPat*>::const_iterator p = mPatMap.begin();
       p != mPatMap.end(); ++ p) {
    const TvFunc& f = p->first;
    s << "Function: " << f << endl;
    for (RwtPat* pat = p->second; pat; pat = pat->mLink) {
      s << "  Pat#" << np << ": "
	<< "#inputs = " << pat->input_num()
	<< ", node_list = {";
      for (ymuint i = 0; i < pat->node_num(); ++ i) {
	s << " " << pat->node(i)->id();
      }
      s << "}" << endl;
      ++ np;
    }
    s << endl;
    ++ nf;
  }
  s << "Total " << nf << " functions" << endl;
}

END_NAMESPACE_YM_NETWORKS
