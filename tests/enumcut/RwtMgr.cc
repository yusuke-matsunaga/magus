
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"
#include "RwtPat.h"
#include "RwtNode.h"
#include "RwtPatGen.h"
#include "RwtPatList.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RwtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RwtMgr::RwtMgr() :
  mAlloc(4096)
{
  mInputNum = 0;
  mNodeNum = 0;
  mNodeArray = NULL;
  mTableSize = 0;
  mTable = NULL;
}

// @brief デストラクタ
RwtMgr::~RwtMgr()
{
}

// @brief 関数に対応したパタンのリストを得る．
// @param[in] f 関数
// @return f に対応するパタンリストを返す．
// @note 見つからなければ NULL を返す．
const RwtPatList*
RwtMgr::find_patlist(const TvFunc& f) const
{
  ymuint idx = f.hash() % mTableSize;
  for (RwtPatList* patlist = mTable[idx]; patlist; patlist = patlist->mLink) {
    if ( patlist->mFunc == f ) {
      return patlist;
    }
  }
  return NULL;
}

#if 0
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
  NpnMap map1;
  mNpnMgr.cannonical(f, map1);
  TvFunc f1 = f.xform(map1);
  hash_map<TvFunc, RwtPat*>::iterator p = mPatMap.find(f1);
  if ( p == mPatMap.end() ) {
    return NULL;
  }
  else {
    RwtPat* pat = p->second;
    cmap = inverse(map1);
    return pat;
  }
}
#endif

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
RwtMgr::print(ostream& s) const
{
  s << "InputNum: " << input_num() << endl
    << "NodeNum : " << mNodeNum << endl;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
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
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (RwtPatList* patlist = mTable[i]; patlist; patlist = patlist->mLink) {
      s << "Function: " << patlist->mFunc << endl;
      ++ nf;
      for (ymuint j = 0; j < patlist->pat_num(); ++ j) {
	++ np;
	const RwtPat& pat = patlist->pat(j);
	s << "  Pat#" << np << ": "
	  << "#inputs = " << pat.input_num()
	  << ", node_list = {";
	for (ymuint i = 0; i < pat.node_num(); ++ i) {
	  s << " " << pat.node(i)->id();
	}
	s << "}" << endl;
      }
    }
  }
  s << "Total " << nf << " functions" << endl
    << "Total " << np << " patterns" << endl;
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

// @brief 内容をバイナリダンプする．
void
RwtMgr::dump(BinO& s) const
{
}

// @brief 内容をコピーする．
void
RwtMgr::copy(const RwtPatGen& pg)
{
  // 諸元を設定する．
  init(pg.input_num(), pg.node_num(), pg.func_num());

  // ノードの情報をコピーする．
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const RwtNode* src_node = pg.node(i);
    ymuint32 id = src_node->mId;
    ymuint32 type = src_node->mType;
    ymuint32 i0 = (src_node->fanin0_node()->id() << 1) | src_node->fanin0_inv();
    ymuint32 i1 = (src_node->fanin1_node()->id() << 1) | src_node->fanin1_inv();
    set_node(id, type, i0, i1);
  }

  // 関数ごとのパタンリストの情報をコピーする．
  for (ymuint i = 0; i < mTableSize; ++ i) {
    TvFunc f = pg.func(i);
    const vector<RwtPat*>& src_patlist = pg.pat_list(i);
    ymuint np = src_patlist.size();
    RwtPatList* dst_patlist = new_patlist(f, np);
    for (ymuint i = 0; i < np; ++ i) {
      const RwtPat* src_pat = src_patlist[i];
      RwtPat* dst_pat = &dst_patlist->mPatList[i];
      ymuint nn = src_pat->mNodeNum;
      set_pat(dst_pat, nn, src_pat->mInputNum);
      for (ymuint k = 0; k < nn; ++ k) {
	set_pat_node(dst_pat, k, src_pat->node(k)->id());
      }
    }
  }
}

// @brief バイナリファイルを読み込んで内容を設定する．
void
RwtMgr::restore(BinI& s)
{
  // 諸元を設定する．
  ymuint32 input_num;
  ymuint32 node_num;
  ymuint32 func_num;
  s >> input_num
    >> node_num
    >> func_num;
  init(input_num, node_num, func_num);

  // ノードの情報をコピーする．
  for (ymuint i = 0; i < node_num; ++ i) {
    ymuint32 id;
    ymuint32 type;
    ymuint32 i0;
    ymuint32 i1;
    s >> id
      >> type
      >> i0
      >> i1;
    set_node(id, type, i0, i1);
  }

  // 関数ごとのパタンリストの情報をコピーする．
  for (ymuint i = 0; i < func_num; ++ i) {
    TvFunc f;
    ymuint32 np;
    s >> f
      >> np;
    RwtPatList* dst_patlist = new_patlist(f, np);
    for (ymuint i = 0; i < np; ++ i) {
      RwtPat* dst_pat = &dst_patlist->mPatList[i];
      ymuint32 nn;
      ymuint32 in;
      s >> nn
	>> in;
      set_pat(dst_pat, nn, in);
      for (ymuint k = 0; k < nn; ++ k) {
	ymuint32 id;
	s >> id;
	set_pat_node(dst_pat, k, id);
      }
    }
  }
}

// @brief 入力数，ノード数，関数の数を設定する．
void
RwtMgr::init(ymuint input_num,
	     ymuint node_num,
	     ymuint func_num)
{
  mAlloc.destroy();

  mInputNum = input_num;
  mNodeNum = node_num;
  {
    void* p = mAlloc.get_memory(sizeof(RwtNode) * mNodeNum);
    mNodeArray = new (p) RwtNode[mNodeNum];
  }
  mTableSize = func_num;
  {
    void* p = mAlloc.get_memory(sizeof(RwtPatList*) * mTableSize);
    mTable = new (p) RwtPatList*[mTableSize];
  }
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
}

// @brief ノードの情報を設定する．
// @param[in] id ノード番号
// @param[in] type ノードの型
// @param[in] i0 ファンイン0のノード番号 + 極性
// @param[in] i1 ファンイン1のノード番号 + 極性
void
RwtMgr::set_node(ymuint32 id,
		 ymuint32 type,
		 ymuint32 i0,
		 ymuint32 i1)
{
  RwtNode* node = &mNodeArray[id];
  node->mId = id;
  node->mType = type;
  ymuint i0_id = i0 >> 1;
  RwtNode* inode0 = &mNodeArray[i0_id];
  bool i0_inv = static_cast<bool>(i0 & 1U);
  ymuint i1_id = i1 >> 1;
  RwtNode* inode1 = &mNodeArray[i1_id];
  bool i1_inv = static_cast<bool>(i1 & 1U);
  node->mFanin0 = encode(inode0, i0_inv);
  node->mFanin1 = encode(inode1, i1_inv);
}

// @brief RwtPatList を生成する．
// @param[in] f 関数
// @param[in] np パタンの数
RwtPatList*
RwtMgr::new_patlist(const TvFunc& f,
		    ymuint np)
{
  void* p = mAlloc.get_memory(sizeof(RwtPatList));
  RwtPatList* patlist = new (p) RwtPatList;

  patlist->mFunc = f;

  patlist->mPatNum = np;
  void* q = mAlloc.get_memory(sizeof(RwtPat) * np);
  patlist->mPatList = new (q) RwtPat[np];

  patlist->mLink = NULL;

  // ハッシュ表に登録する．
  ymuint idx = f.hash() % mTableSize;
  // 効率が悪いけど現れた順にリストに並べる．
  if ( mTable[idx] == NULL ) {
    mTable[idx] = patlist;
  }
  else {
    RwtPatList* tail;
    for (tail = mTable[idx]; tail->mLink; tail = tail->mLink) ;
    tail->mLink = patlist;
  }

  return patlist;
}

// @brief パタンの情報を設定する．
// @param[in] pat パタン
// @param[in] node_num ノード数
// @param[in] input_num 入力数＋根の極性情報
void
RwtMgr::set_pat(RwtPat* pat,
		ymuint node_num,
		ymuint input_num)
{
  pat->mNodeNum = node_num;
  pat->mInputNum = input_num;

  void* p = mAlloc.get_memory(sizeof(RwtNode*) * node_num);
  pat->mNodeList = new (p) RwtNode*[node_num];
}

// @brief パタンのノードリストの情報を設定する．
// @param[in] pat パタン
// @param[in] pos ノードリスト中の位置
// @param[in] id ノード番号
void
RwtMgr::set_pat_node(RwtPat* pat,
		     ymuint pos,
		     ymuint id)
{
  pat->mNodeList[pos] = &mNodeArray[id];
}

END_NAMESPACE_YM_NETWORKS
