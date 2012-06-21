/// @file GpMgr.cc
/// @brief GpMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GpMgr.h"
#include "GpNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GpMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GpMgr::GpMgr() :
  mAlloc(1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
GpMgr::~GpMgr()
{
  delete [] mHashTable;
  // ノードのメモリは mAlloc のデストラクタで無事開放される．
}

// @brief 定数0ノードを生成する．
GpHandle
GpMgr::make_const0()
{
  return GpHandle(NULL, false);
}

// @brief 定数1ノードを生成する．
GpHandle
GpMgr::make_const1()
{
  return GpHandle(NULL, true);
}

// @brief 入力ノードを生成する．
// @param[in] input_id 入力番号
// @note すでに同じ入力番号のノードが存在していたらそれを返す．
GpHandle
GpMgr::make_input(ymuint32 input_id)
{
  assert_cond( input_id < 4, __FILE__, __LINE__);

  while ( mInputList.size() <= input_id ) {
    GpNode* node = new_node();
    ymuint32 new_id = mInputList.size();
    node->mInputId = (new_id << 4);
    mInputList.push_back(node);
  }
  GpNode* node = mInputList[input_id];
  node->mLevel = 0;
  node->mSize = 0;
  switch ( input_id ) {
  case 0: node->mFunc = 0xaaaa; break;
  case 1: node->mFunc = 0xcccc; break;
  case 2: node->mFunc = 0xf0f0; break;
  case 3: node->mFunc = 0xff00; break;
  }

  return GpHandle(node, false);
}

// @brief AND ノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
GpHandle
GpMgr::make_and(GpHandle fanin0,
		GpHandle fanin1)
{
  // トリビアルな場合の処理
  if ( fanin0.is_const0() || fanin1.is_const0() ) {
    return make_const0();
  }
  if ( fanin0.is_const1() ) {
    return fanin1;
  }
  if ( fanin1.is_const1() ) {
    return fanin0;
  }
  if ( fanin0 == fanin1 ) {
    return fanin0;
  }
  if ( fanin0.node() == fanin1.node() ) {
    // 上でハンドルの一致は調べているのでここに来たら逆極性
    return make_const0();
  }

  // 順番の正規化
  if ( fanin0.node()->id() < fanin1.node()->id() ) {
    GpHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }

  // 同じ構造を持つノードがないか調べる．
  ymuint pos = hash_func(fanin0, fanin1, false);
  ymuint idx = pos % mHashSize;
  for (GpNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 &&
	 node->mFanin[1] == fanin1 &&
	 node->is_and() ) {
      return GpHandle(node, false);
    }
  }

  // 新しいノードを作る．
  GpNode* node = new_node();
  node->mInputId = 2U;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;
  node->mLevel = fanin0.node()->level() + fanin1.node()->level() + 1;
  node->mSize = count_size(node);
  node->mFunc = fanin0.func() & fanin1.func();

  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return GpHandle(node, false);
}

// @brief XOR ノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
GpHandle
GpMgr::make_xor(GpHandle fanin0,
		GpHandle fanin1)
{
  // トリビアルな場合の処理
  if ( fanin0.is_const0() ) {
    return fanin1;
  }
  if ( fanin0.is_const1() ) {
    return ~fanin1;
  }
  if ( fanin1.is_const0() ) {
    return fanin0;
  }
  if ( fanin1.is_const1() ) {
    return ~fanin0;
  }
  if ( fanin0 == fanin1 ) {
    return make_const0();
  }
  if ( fanin0 == ~fanin1 ) {
    return make_const1();
  }

  // 順番の正規化
  if ( fanin0.node()->id() < fanin1.node()->id() ) {
    GpHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }

  // 極性の正規化
  bool oinv = fanin0.inv() ^ fanin1.inv();
  fanin0.cannonicalize();
  fanin1.cannonicalize();

  // 同じ構造を持つノードがないか調べる．
  ymuint pos = hash_func(fanin0, fanin1, true);
  ymuint idx = pos % mHashSize;
  for (GpNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 &&
	 node->mFanin[1] == fanin1 &&
	 node->is_xor() ) {
      return GpHandle(node, oinv);
    }
  }

  // 新しいノードを作る．
  GpNode* node = new_node();
  node->mInputId = 3U;
  node->mFanin[0] = fanin0;
  node->mFanin[1] = fanin1;
  node->mLevel = fanin0.node()->level() + fanin1.node()->level() + 1;
  node->mSize = count_size(node);
  node->mFunc = fanin0.func() ^ fanin1.func();

  // ハッシュ表に登録する．
  if ( mAndList.size() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mSlink = mHashTable[idx];
  mHashTable[idx] = node;
  mAndList.push_back(node);

  return GpHandle(node, oinv);
}

// @brief ノードを生成する下請け関数
GpNode*
GpMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(GpNode));
  ymuint id = mNodeList.size();
  GpNode* node = new (p) GpNode(id);
  mNodeList.push_back(node);
  return node;
}

// @brief ハンドルとその子供の内容を出力する．
void
GpMgr::dump_handle(ostream& s,
		   GpHandle handle) const
{
  vector<GpHandle> tmp_list(1, handle);
  dump_handle(s, tmp_list);
}

BEGIN_NONAMESPACE

void
print_handle(ostream& s,
	     GpHandle handle)
{
  if ( handle.is_const0() ) {
    s << "CONST0";
  }
  else if ( handle.is_const1() ) {
    s << "CONST1";
  }
  else {
    if ( handle.inv() ) {
      s << "~";
    }
    s << handle.node()->id();
  }
}

void
dh_sub(ostream& s,
       GpNode* node,
       hash_set<ymuint32>& h_hash)
{
  if ( h_hash.count(node->id()) > 0 ) {
    return;
  }
  h_hash.insert(node->id());

  if ( node->is_logic() ) {
    dh_sub(s, node->fanin0_node(), h_hash);
    dh_sub(s, node->fanin1_node(), h_hash);
  }

  s << "Node#" << node->id() << ": ";
  if ( node->is_input() ) {
    s << "INPUT(" << node->input_id() << ")" << endl;
  }
  else { // node->is_logic()
    if ( node->is_and() ) {
      s << "AND";
    }
    else {
      s << "XOR";
    }
    s << "(";
    print_handle(s, node->fanin0());
    s << ", ";
    print_handle(s, node->fanin1());
    s << ")" << endl;
  }
}

END_NONAMESPACE

// @brief 複数のハンドルとその子供の内容を出力する．
void
GpMgr::dump_handle(ostream& s,
		   const vector<GpHandle>& handle_list) const
{
  s << "----------------------------------------" << endl;
  ymuint i = 0;
  for (vector<GpHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p, ++ i) {
    GpHandle handle = *p;
    s << "Handle#" << i << ": ";
    print_handle(s, handle);
    cout << endl;
  }
  s << "----------------------------------------" << endl;

  hash_set<ymuint32> tmp_hash;
  for (vector<GpHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p) {
    GpHandle handle = *p;
    if ( handle.is_const0() || handle.is_const1() ) {
      continue;
    }
    dh_sub(s, handle.node(), tmp_hash);
  }
  s << "----------------------------------------" << endl;
}

BEGIN_NONAMESPACE

// count_size() の下請け関数
ymuint32
cs_sub(GpNode* node,
       vector<bool>& marks)
{
  if ( node->is_input() ) {
    return 0;
  }
  if ( marks[node->id()] ) {
    // すでにカウント済み
    return 0;
  }
  marks[node->id()] = true;
  ymuint ans = 1;
  ans += cs_sub(node->fanin0_node(), marks);
  ans += cs_sub(node->fanin1_node(), marks);
  return ans;
}

END_NONAMESPACE

// @brief node を根とするパタンのノード数を数える．
ymuint32
GpMgr::count_size(GpNode* node)
{
  vector<bool> marks(mNodeList.size(), false);
  return cs_sub(node, marks);
}

// @brief ハッシュ関数
ymuint32
GpMgr::hash_func(GpHandle fanin0,
		 GpHandle fanin1,
		 bool xor_flag)
{
  return fanin0.mNodePol + fanin1.mNodePol + xor_flag;
}

// @brief ハッシュテーブルを拡大する．
void
GpMgr::alloc_table(ymuint req_size)
{
  GpNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }

  mHashTable = new GpNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      GpNode* next = NULL;
      for (GpNode* node = old_table[i]; node; node = next) {
	next = node->mSlink;
	ymuint pos = hash_func(node->mFanin[0], node->mFanin[1], node->is_xor());
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
