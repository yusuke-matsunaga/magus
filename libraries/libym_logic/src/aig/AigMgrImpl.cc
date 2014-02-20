
/// @file AigMgrImpl.cc
/// @brief AigMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AigMgrImpl.h"
#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AigMgrImpl::AigMgrImpl() :
  mAlloc(sizeof(AigNode) * 1024),
  mInputNum(0),
  mInputHashTable(NULL),
  mInputHashSize(0),
  mAndNum(0),
  mAndHashTable(NULL),
  mAndHashSize(0)
{
  // ハッシュ表の初期値は適当
  alloc_table(mInputHashTable, 128, mInputHashSize, mInputNextLimit);
  alloc_table(mAndHashTable, 1024, mAndHashSize, mAndNextLimit);
}

// @brief デストラクタ
AigMgrImpl::~AigMgrImpl()
{
  // ノードは mAlloc がすべて片付けてくれる．
  delete [] mInputHashTable;
  delete [] mAndHashTable;
}

// @brief 外部入力を作る．
// @param[in] id 入力番号
Aig
AigMgrImpl::make_input(VarId id)
{
  // すでに存在しているか調べる．
  AigNode* node = input_node(id);
  if ( node != NULL ) {
    return Aig(pack(node, false));
  }
  // なかったので新たに作る．
  if ( mInputNum >= mInputNextLimit ) {
    // ハッシュ表を拡大する．
    AigNode** old_table = mInputHashTable;
    ymuint old_size = mInputHashSize;

    alloc_table(mInputHashTable, mInputHashSize * 2, mInputHashSize, mInputNextLimit);

    for (ymuint i = 0; i < old_size; ++ i) {
      AigNode* next = NULL;
      for (AigNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = node->input_id().val();
	ymuint idx = pos % mInputHashSize;
	node->mLink = mInputHashTable[idx];
	mInputHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  node = new_node();
  node->set_input(id);
  ymuint pos = id.val() % mInputHashSize;
  node->mLink = mInputHashTable[pos];
  mInputHashTable[pos] = node;

  return Aig(pack(node, false));
}

// @brief 入力ノードを取り出す．
// @param[in] id 入力番号
AigNode*
AigMgrImpl::input_node(VarId id) const
{
  ymuint pos = id.val() % mInputHashSize;
  for (AigNode* node = mInputHashTable[pos]; node; node = node->mLink) {
    if ( node->input_id() == id ) {
      return node;
    }
  }
  return NULL;
}

// @brief AND ノードを作る．
Aig
AigMgrImpl::make_and(Aig handle1,
		     Aig handle2)
{
  // trivial な場合の処理
  if ( handle1.is_zero() || handle2.is_zero() ) {
    return make_zero();
  }
  if ( handle1.is_one() ) {
    return handle2;
  }
  if ( handle2.is_one() ) {
    return handle1;
  }
  if ( handle1 == handle2 ) {
    return handle1;
  }
  if ( handle1.normalize() == handle2.normalize() ) {
    // handle1.inv != handle2.inv() のはず
    return make_zero();
  }

  // 順番の正規化
  if ( handle1.node_id() < handle2.node_id() ) {
    Aig tmp = handle1;
    handle1 = handle2;
    handle2 = tmp;
  }

  // 同じ構造を持つノードが既にないか調べる．
  ymuint pos = hash_func(handle1, handle2);
  ymuint idx = pos % mAndHashSize;
  for (AigNode* node = mAndHashTable[idx]; node; node = node->mLink) {
    if ( node->fanin0() == handle1 &&
	 node->fanin1() == handle2 ) {
      // 同じノードがあった．
      return Aig(pack(node, false));
    }
  }

  // 新しいノードを作る．
  AigNode* node = new_node();
  node->set_and(handle1, handle2);

  // ハッシュ表に登録する．
  if ( mAndNum >= mAndNextLimit ) {
    AigNode** old_table = mAndHashTable;
    ymuint old_size = mAndHashSize;

    alloc_table(mAndHashTable, mAndHashSize * 2, mAndHashSize, mAndNextLimit);

    for (ymuint i = 0; i < old_size; ++ i) {
      AigNode* next = NULL;
      for (AigNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->fanin0(), node->fanin1());
	ymuint idx = pos % mAndHashSize;
	node->mLink = mAndHashTable[idx];
	mAndHashTable[idx] = node;
      }
    }
    delete [] old_table;

    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mAndHashSize;
  }
  node->mLink = mAndHashTable[idx];
  mAndHashTable[idx] = node;

  return Aig(pack(node, false));
}

// 新しいノードを作成する．
// 作成されたノードを返す．
AigNode*
AigMgrImpl::new_node()
{
  void* p = mAlloc.get_memory(sizeof(AigNode));
  ymuint id = mAllNodes.size();
  AigNode* node = new (p) AigNode(id);
  mAllNodes.push_back(node);
  return node;
}

// @brief ハッシュ表を拡大する．
// @note 現在のサイズが0のときは適当な大きさに初期化される．
void
AigMgrImpl::alloc_table(AigNode**& table,
			ymuint req_size,
			ymuint& size,
			ymuint& limit)
{
  if ( size == 0 ) {
    size = req_size;
  }
  else {
    while ( size < req_size ) {
      size <<= 1;
    }
  }
  table = new AigNode*[size];
  for (ymuint i = 0; i < size; ++ i) {
    table[i] = NULL;
  }
  limit = static_cast<ymuint32>(size * 1.8);
}

END_NAMESPACE_YM_AIG
