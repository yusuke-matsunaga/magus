
/// @file NpnNodeMgr.cc
/// @brief NpnNodeMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NpnNodeMgr.h"
#include "NpnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

struct Npn4Cannon
{
  ymuint16 mFunc;
  ymuint16 mPerm;
};

// 4入力のNPN同値類の代表関数への変換を表す配列
Npn4Cannon npn4cannon[] = {
#include "npn4cannon.h"
};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス NpnNodeMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NpnNodeMgr::NpnNodeMgr() :
  mAlloc(1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);

  // 定数ノードの生成
  mConstNode = new_node();
  mConstNode->mVolume = 0;
  mConstNode->mFunc = 0x0000;
  mConstNode->mSlink = NULL;
  assert_cond( mConstNode->id() == 0, __FILE__, __LINE__);

  // 入力ノードの生成
  mInputNode = new_node();
  mInputNode->mId |= 1U;
  mInputNode->mVolume = 0;
  mInputNode->mFunc = 0xaaaa;
  mInputNode->mSlink = NULL;
  assert_cond( mInputNode->id() == 1, __FILE__, __LINE__);
}

// @brief デストラクタ
NpnNodeMgr::~NpnNodeMgr()
{
  delete [] mHashTable;
  // ノードのメモリは mAlloc のデストラクタで無事開放される．
}

// @brief 定数0ノードを生成する．
NpnHandle
NpnNodeMgr::make_const0()
{
  return NpnHandle(0, NpnXform());
}

// @brief 定数1ノードを生成する．
NpnHandle
NpnNodeMgr::make_const1()
{
  // ちょっとトリッキーなコード
  return NpnHandle(0, NpnXform(0, 16U));
}

// @brief 入力ノードを生成する．
// @param[in] input_id ( 0 <= input_id < 4 )
// @note すでに同じ入力番号のノードが存在していたらそれを返す．
NpnHandle
NpnNodeMgr::make_input(ymuint input_id)
{
  ymuint perm[4];
  perm[0] = input_id;
  ymuint rpos = 0;
  for (ymuint i = 1; i < 4; ++ i, ++ rpos) {
    if ( rpos == input_id ) {
      ++ rpos;
    }
    perm[i] = rpos;
  }
  ymuint pid = NpnXform::perm_id(perm);
  return NpnHandle(1, NpnXform(pid, 0U));
}

// @brief ANDノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
NpnHandle
NpnNodeMgr::make_and(NpnHandle fanin0,
		     NpnHandle fanin1)
{
  ymuint16 func0 = func(fanin0);
  ymuint16 func1 = func(fanin1);
  ymuint16 func = func0 & func1;
  // トリビアルな場合の処理
  if ( func == 0U ) {
    return make_const0();
  }
  if ( func == func0 ) {
    return fanin0;
  }
  if ( func == func1 ) {
    return fanin1;
  }

  // func がNPN同値類の代表関数になるように正規化する．
  const Npn4Cannon& npn_cannon = npn4cannon[func];
  ymuint16 cfunc = npn_cannon.mFunc;
  ymuint16 perm = npn_cannon.mPerm;
  bool oinv = (perm >> 4) & 1U;
  if ( oinv ) {
    cfunc ^= 0xFFFF;
  }
  perm &= ~16U;
  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);

  NpnHandle c_fanin0 = fanin0 * xf;
  NpnHandle c_fanin1 = fanin1 * xf;

  // 順番の正規化
  if ( c_fanin0 > c_fanin1 ) {
    NpnHandle tmp = c_fanin0;
    c_fanin0 = c_fanin1;
    c_fanin1 = tmp;
  }

  // 同じ構造を持つノードがないか調べる．
  ymuint pos = hash_func(c_fanin0, c_fanin1, false);
  ymuint idx = pos % mHashSize;
  for (NpnNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == c_fanin0 &&
	 node->mFanin[1] == c_fanin1 &&
	 node->is_and() ) {
      return NpnHandle(node->id(), inv_xf);
    }
  }

  // 新しいノードを作る．
  NpnNode* node = new_node();
  node->mId |= 2U;
  node->mFunc = cfunc;
  node->mFanin[0] = c_fanin0;
  node->mFanin[1] = c_fanin1;

  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return NpnHandle(node->id(), inv_xf);
}

// @brief XORノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
NpnHandle
NpnNodeMgr::make_xor(NpnHandle fanin0,
		     NpnHandle fanin1)
{
  ymuint16 func0 = func(fanin0);
  ymuint16 func1 = func(fanin1);
  ymuint16 func = func0 ^ func1;
  // トリビアルな場合の処理
  if ( func == 0U ) {
    return make_const0();
  }
  if ( func == 0xFFFFU ) {
    return make_const1();
  }
  if ( func == func0 ) {
    return fanin0;
  }
  if ( func == func1 ) {
    return fanin1;
  }

  // XOR ノードの入力には出力反転属性をつけない．
  bool iinv = false;
  if ( fanin0.oinv() ) {
    fanin0 = ~fanin0;
    iinv = !iinv;
  }
  if ( fanin1.oinv() ) {
    fanin1 = ~fanin1;
    iinv = !iinv;
  }

  // func がNPN同値類の代表関数になるように正規化する．
  const Npn4Cannon& npn_cannon = npn4cannon[func];
  ymuint16 cfunc = npn_cannon.mFunc;
  ymuint16 perm = npn_cannon.mPerm;
  bool oinv = (perm >> 4) & 1U;
  if ( oinv ) {
    cfunc ^= 0xFFFF;
  }
  perm &= ~16U;
  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);
  if ( iinv ) {
    inv_xf.flip_oinv();
  }
  NpnHandle c_fanin0 = fanin0 * xf;
  NpnHandle c_fanin1 = fanin1 * xf;

  // 順番の正規化
  if ( c_fanin0 > c_fanin1 ) {
    NpnHandle tmp = c_fanin0;
    c_fanin0 = c_fanin1;
    c_fanin1 = tmp;
  }

  // 同じ構造を持つノードがないか調べる．
  ymuint pos = hash_func(c_fanin0, c_fanin1, true);
  ymuint idx = pos % mHashSize;
  for (NpnNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == c_fanin0 &&
	 node->mFanin[1] == c_fanin1 &&
	 node->is_xor() ) {
      return NpnHandle(node->id(), inv_xf);
    }
  }

  // 新しいノードを作る．
  NpnNode* node = new_node();
  node->mId |= 3U;
  node->mFunc = cfunc;
  node->mFanin[0] = c_fanin0;
  node->mFanin[1] = c_fanin1;

  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return NpnHandle(node->id(), inv_xf);
}

// @brief ハンドルの表す関数を返す．
ymuint16
NpnNodeMgr::func(NpnHandle handle) const
{
  NpnNode* node = mNodeList[handle.node_id()];
  return xform(node->func(), handle.npn_xform());
}

// @brief ノードを生成する関数
NpnNode*
NpnNodeMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(NpnNode));
  ymuint id = mNodeList.size();
  NpnNode* node = new (p) NpnNode(id);
  mNodeList.push_back(node);
  return node;
}

// @brief ハンドルとその子供の内容を出力する．
void
NpnNodeMgr::dump_handle(ostream& s,
			NpnHandle handle) const
{
  vector<NpnHandle> tmp_list(1, handle);
  dump_handle(s, tmp_list);
}

// @brief 複数のハンドルとその子供の内容を出力する．
void
NpnNodeMgr::dump_handle(ostream& s,
			const vector<NpnHandle>& handle_list) const
{
}

// @brief ハッシュ関数
ymuint32
NpnNodeMgr::hash_func(NpnHandle fanin0,
		      NpnHandle fanin1,
		      bool xor_flag)
{
  return fanin0.mData + fanin1.mData + xor_flag;
}

// @brief ハッシュテーブルを拡大する．
void
NpnNodeMgr::alloc_table(ymuint req_size)
{
  NpnNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }

  mHashTable = new NpnNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      NpnNode* next = NULL;
      for (NpnNode* node = old_table[i]; node; node = next) {
	next = node->mSlink;
	ymuint pos = hash_func(node->fanin0(), node->fanin1(), node->is_xor());
	ymuint idx = pos % mHashSize;
	node->mSlink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

END_NAMESPACE_YM
