
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
  mConstNode = alloc_node();
  mConstNode->mVolume = 0;
  mConstNode->mFunc = 0x0000;
  mConstNode->mSlink = NULL;
  assert_cond( mConstNode->id() == 0, __FILE__, __LINE__);

  // 入力ノードの生成
  mInputNode = alloc_node();
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
  // 回りくどいけど正規化のための手順を行なう．
  ymuint16 func = 0U;
  switch ( input_id ) {
  case 0: func = 0xaaaa; break;
  case 1: func = 0xcccc; break;
  case 2: func = 0xf0f0; break;
  case 3: func = 0xff00; break;
  }
  const Npn4Cannon& npn_cannon = npn4cannon[func];
  ymuint16 c_func = npn_cannon.mFunc;
  assert_cond( c_func == 0xaaaa, __FILE__, __LINE__);
  NpnXform xf(npn_cannon.mPerm);
  return NpnHandle(1, inverse(xf));
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
  ymuint16 c_func = npn_cannon.mFunc;
  ymuint16 perm = npn_cannon.mPerm;
  bool oinv = (perm >> 4) & 1U;
  if ( oinv ) {
    c_func ^= 0xFFFF;
  }
  perm &= ~16U;
  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);

  NpnHandle c_fanin0 = cannonical(fanin0 * xf);
  NpnHandle c_fanin1 = cannonical(fanin1 * xf);

  // 既存の等価なノードを探すか新しいノードを作る．
  NpnNode* node = new_node(false, c_func, c_fanin0, c_fanin1);

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
  ymuint16 c_func = npn_cannon.mFunc;
  ymuint16 perm = npn_cannon.mPerm;
  bool oinv = (perm >> 4) & 1U;
  if ( oinv ) {
    c_func ^= 0xFFFF;
  }
  perm &= ~16U;
  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);
  if ( iinv ) {
    inv_xf.flip_oinv();
  }
  NpnHandle c_fanin0 = cannonical(fanin0 * xf);
  NpnHandle c_fanin1 = cannonical(fanin1 * xf);

  // 既存の等価なノードを探すか新しいノードを作る．
  NpnNode* node = new_node(true, c_func, c_fanin0, c_fanin1);

  return NpnHandle(node->id(), inv_xf);
}

// @brief ハンドルの表す関数を返す．
ymuint16
NpnNodeMgr::func(NpnHandle handle) const
{
  NpnNode* node = mNodeList[handle.node_id()];
  return xform(node->func(), handle.npn_xform());
}

// @brief 枝を正規化する．
NpnHandle
NpnNodeMgr::cannonical(NpnHandle src)
{
  // まどろっこしいけど，いったん，関数の正規形を求めて
  // それへの変換の正規形を求める．
  ymuint16 f = func(src);
  const Npn4Cannon& npn_cannon = npn4cannon[f];
  ymuint id = src.node_id();
  NpnXform xf(npn_cannon.mPerm);
  ymuint16 cf = xform(f, xf);
  NpnNode* node = mNodeList[id];
  if ( node->func() == (cf ^ 0xFFFF) ) {
    xf.flip_oinv();
  }
  else {
    if ( node->func() != cf ) {
      cout << "node->func() = " << hex << setw(4) << setfill('0') << node->func()
	   << ", cf = " << setw(4) << setfill('0') << f << dec << endl;
    }
    assert_cond( node->func() == cf, __FILE__, __LINE__);
  }
  return NpnHandle(id, inverse(xf));
}

// @brief 新しいノードを登録する関数
// @param[in] is_xor XOR ノードの時 true にするフラグ
// @param[in] func 関数ベクタ
// @param[in] fanin0, fanin1 ファンインのハンドル
// @note おなじノードが既に存在していたらそのノードを返す．
NpnNode*
NpnNodeMgr::new_node(bool is_xor,
		     ymuint16 func,
		     NpnHandle fanin0,
		     NpnHandle fanin1)
{
  // 順番の正規化
  if ( fanin0 > fanin1 ) {
    NpnHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }

  ymuint type_pat = is_xor ? 3U : 2U;
  ymuint pos = hash_func(fanin0, fanin1, is_xor);
  ymuint idx = pos % mHashSize;
  for (NpnNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 &&
	 node->mFanin[1] == fanin1 &&
	 (node->mId & 3U) == type_pat ) {
      return node;
    }
  }
  NpnNode* node = alloc_node();
  node->mId |= type_pat;
  node->mFunc = func;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;

  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return node;
}

// @brief ノードを生成する関数
NpnNode*
NpnNodeMgr::alloc_node()
{
  void* p = mAlloc.get_memory(sizeof(NpnNode));
  ymuint id = mNodeList.size();
  NpnNode* node = new (p) NpnNode(id);
  mNodeList.push_back(node);
  return node;
}

BEGIN_NONAMESPACE

void
print_handle(ostream& s,
	     NpnHandle handle)
{
  ymuint id = handle.node_id();
  NpnXform xf = handle.npn_xform();
  s << "NODE#" << id << " : " << xf;
}

END_NONAMESPACE

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
  s << "dump_handle" << endl
    << "----------------------------------------" << endl;
  ymuint i = 0;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p, ++ i) {
    NpnHandle handle = *p;
    s << "Handle#" << i << ": ";
    print_handle(s, handle);
    cout << endl;
  }
  s << "----------------------------------------" << endl;

  hash_set<ymuint32> node_hash;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p) {
    NpnHandle handle = *p;
    dh_sub(s, handle.node_id(), node_hash);
  }
  s << "----------------------------------------" << endl;
}

// @brief dump_handle() の下請け関数
void
NpnNodeMgr::dh_sub(ostream& s,
		   ymuint id,
		   hash_set<ymuint32>& node_hash) const
{
  if ( node_hash.count(id) > 0 ) {
    return;
  }
  node_hash.insert(id);

  NpnNode* node = mNodeList[id];

  if ( node->is_logic() ) {
    dh_sub(s, node->fanin0().node_id(), node_hash);
    dh_sub(s, node->fanin1().node_id(), node_hash);
  }

  s << "NODE#" << id << ": ";
  if ( node->is_const() ) {
    s << "CONST0" << endl;
  }
  else if ( node->is_input() ) {
    s << "INPUT" << endl;
  }
  else { // node->is_logic()
    if ( node->is_and() ) {
      s << "AND";
    }
    else {
      s << "XOR";
    }
    s << "( ";
    print_handle(s, node->fanin0());
    s << " , ";
    print_handle(s, node->fanin1());
    s << " )" << endl;
  }
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
