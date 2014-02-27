
/// @file RwtPatGen.cc
/// @brief RwtPatGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "RwtPatGen.h"
#include "PgNode.h"
#include "PgPat.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス RwtPatGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RwtPatGen::RwtPatGen() :
  mAlloc(4096),
  mNodeHashTable(NULL),
  mNodeHashSize(0)
{
  mInputNum = 0;
  alloc_table(1024);
}

// @brief デストラクタ
RwtPatGen::~RwtPatGen()
{
}

// @brief 初期化する．
// @param[in] input_num
// @note 定数0ノードおよび 0 〜 (input_num - 1) までの入力ノードを作る．
void
RwtPatGen::init(ymuint input_num)
{
  mAlloc.destroy();
  mNodeArray.clear();
  mFuncList.clear();

  mInputNum = input_num;
  mNodeHashTable = NULL;
  mNodeHashSize = 0;
  alloc_table(1024);

  // 定数0ノードを作る．
  PgNode* node = alloc_node();
  node->mType = 0U;
  node->mFunc = TvFunc::const_zero(mInputNum);

  // 定数0のパタンを登録しておく．
  new_pat(node, false);

  // 入力ノードを作る．
  for (ymuint i = 0; i < mInputNum; ++ i) {
    PgNode* node = alloc_node();
    node->mType = 1U | (i << 2);
    node->mFunc = TvFunc::posi_literal(mInputNum, VarId(i));
  }
}

BEGIN_NONAMESPACE

ymuint32
encode(const PgNode* node,
       bool inv)
{
  return (node->id() << 1) | static_cast<ymuint32>(inv);
}

END_NONAMESPACE

// @brief AND ノードを作る．
// @param[in] fanin0 ファンイン0
// @param[in] inv0 ファンイン0の極性
// @param[in] fanin1 ファンイン1
// @param[in] inv1 ファンイン1の極性
const PgNode*
RwtPatGen::new_and(const PgNode* fanin0,
		   bool inv0,
		   const PgNode* fanin1,
		   bool inv1)
{
  if ( fanin0->is_const0() ) {
    if ( inv0 ) {
      return fanin1;
    }
    else {
      return fanin0;
    }
  }
  if ( fanin1->is_const0() ) {
    if ( inv1 ) {
      return fanin0;
    }
    else {
      return fanin1;
    }
  }
  if ( fanin0 == fanin1 ) {
    if ( inv0 == inv1 ) {
      return fanin0;
    }
    else {
      // 定数ノード
      return mNodeArray[0];
    }
  }
  ymuint32 id0 = encode(fanin0, inv0);
  ymuint32 id1 = encode(fanin1, inv1);
  if ( id0 > id1 ) {
    ymuint32 tmp = id0;
    id0 = id1;
    id1 = tmp;
  }
  PgNode* node = new_node(2U, id0, id1);

  TvFunc f0 = fanin0->mFunc;
  if ( inv0 ) {
    f0.negate();
  }

  TvFunc f1 = fanin1->mFunc;
  if ( inv1 ) {
    f1.negate();
  }

  node->mFunc = f0 & f1;

  return node;
}

// @brief XOR ノードを作る．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
const PgNode*
RwtPatGen::new_xor(const PgNode* fanin0,
		   const PgNode* fanin1)
{
  if ( fanin0->is_const0() ) {
    return fanin1;
  }
  if ( fanin1->is_const0() ) {
    return fanin0;
  }
  if ( fanin0 == fanin1 ) {
    return mNodeArray[0];
  }
  ymuint32 id0 = encode(fanin0, false);
  ymuint32 id1 = encode(fanin1, false);
  if ( id0 > id1 ) {
    ymuint32 tmp = id0;
    id0 = id1;
    id1 = tmp;
  }
  PgNode* node = new_node(3U, id0, id1);

  TvFunc f0 = fanin0->mFunc;
  TvFunc f1 = fanin1->mFunc;

  node->mFunc = f0 ^ f1;

  return node;
}

// @brief パタンを登録する．
void
RwtPatGen::new_pat(const PgNode* root,
		   bool inv)
{
  vector<bool> mark(mNodeArray.size());
  vector<const PgNode*> inputs;
  inputs.reserve(mInputNum);
  vector<const PgNode*> node_list;
  node_list.reserve(mNodeArray.size());
  dfs(root, mark, inputs, node_list);

  ymuint ni = inputs.size();
  ymuint nn = node_list.size();

  void* p = mAlloc.get_memory(sizeof(PgPat));
  PgPat* pat = new (p) PgPat;
  pat->mNodeNum = ni + nn;
  pat->mInputNum = (ni << 1);
  if ( inv ) {
    pat->mInputNum |= 1U;
  }
  void* q = mAlloc.get_memory(sizeof(ymuint32) * pat->mNodeNum);
  pat->mNodeList = new (q) ymuint32[pat->mNodeNum];
  for (ymuint i = 0; i < ni; ++ i) {
    const PgNode* node = inputs[i];
    pat->mNodeList[i] = node->id();
  }
  for (ymuint i = 0; i < nn; ++ i) {
    const PgNode* node = node_list[i];
    pat->mNodeList[i + ni] = node->id();
  }

  TvFunc f = root->mFunc;
  if ( inv ) {
    f.negate();
  }

  FuncMap::iterator r = mFuncHash.find(f);
  if ( r == mFuncHash.end() ) {
    ymuint id = mFuncList.size();
    mFuncList.push_back(f);
    mPatList.push_back(vector<PgPat*>(1, pat));
    mFuncHash.insert(make_pair(f, id));
  }
  else {
    ymuint id = r->second;
    mPatList[id].push_back(pat);
  }
}

// @brief パタンのノードをDFSで訪れる．
void
RwtPatGen::dfs(const PgNode* node,
	       vector<bool>& mark,
	       vector<const PgNode*>& inputs,
	       vector<const PgNode*>& node_list)
{
  if ( mark[node->id()] ) {
    return;
  }
  mark[node->id()] = true;

  if ( node->is_input() ) {
    inputs.push_back(node);
  }
  else {
    const PgNode* node0 = mNodeArray[node->fanin0_id()];
    dfs(node0, mark, inputs, node_list);
    const PgNode* node1 = mNodeArray[node->fanin1_id()];
    dfs(node1, mark, inputs, node_list);
    node_list.push_back(node);
  }
}

// @brief ノードを返す．
// @param[in] type タイプ
// @param[in] fanin0 ファンイン0のノード番号＋極性
// @param[in] fanin1 ファンイン1のノード番号＋極性
// @note 既に同じノードが存在していたらそれを返す．
// @note なければ新しく作ってそれを返す．
PgNode*
RwtPatGen::new_node(ymuint32 type,
		    ymuint32 fanin0,
		    ymuint32 fanin1)
{
  ymuint pos = hash_func(type, fanin0, fanin1);
  ymuint idx = pos % mNodeHashSize;
  for (PgNode* node = mNodeHashTable[idx]; node; node = node->mLink) {
    if ( node->mType == type &&
	 node->mFanin0 == fanin0 &&
	 node->mFanin1 == fanin1 ) {
      return node;
    }
  }

  if ( node_num() >= mNodeHashLimit ) {
    alloc_table(mNodeHashSize * 2);
    idx = pos % mNodeHashSize;
  }

  PgNode* node = alloc_node();
  node->mType = type;
  node->mFanin0 = fanin0;
  node->mFanin1 = fanin1;
  node->mLink = mNodeHashTable[idx];
  mNodeHashTable[idx] = node;

  return node;
}

// @brief ノードを確保する．
PgNode*
RwtPatGen::alloc_node()
{
  void* p = mAlloc.get_memory(sizeof(PgNode));
  PgNode* node = new (p) PgNode;
  node->mId = mNodeArray.size();
  mNodeArray.push_back(node);
  node->mFanin0 = 0U;
  node->mFanin1 = 0U;

  return node;
}

// @brief ハッシュ表を確保する．
void
RwtPatGen::alloc_table(ymuint req_size)
{
  PgNode** old_table = mNodeHashTable;
  ymuint old_size = mNodeHashSize;

  if ( mNodeHashSize == 0 ) {
    mNodeHashSize = 1024;
  }
  while ( mNodeHashSize < req_size ) {
    mNodeHashSize <<= 1;
  }
  mNodeHashTable = new PgNode*[mNodeHashSize];
  for (ymuint i = 0; i < mNodeHashSize; ++ i) {
    mNodeHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      PgNode* next = NULL;
      for (PgNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->mType, node->mFanin0, node->mFanin1);
	ymuint idx = pos % mNodeHashSize;
	node->mLink = mNodeHashTable[idx];
	mNodeHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNodeHashLimit = static_cast<ymuint32>(mNodeHashSize * 1.8);
}

END_NAMESPACE_YM_NETWORKS
