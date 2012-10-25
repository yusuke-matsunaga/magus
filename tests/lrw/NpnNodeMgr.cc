
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

// 4入力の準正規形関数への変換を表す配列
Npn4Cannon npn4norm[] = {
#include "npn4norm.h"
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint16 npn4perm[] = {
#include "npn4perm.h"
};

void
print_func(ostream& s,
	   ymuint16 func)
{
  s << hex << setw(4) << setfill('0') << func << dec;
}

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

  make_ident_list();

  // 定数ノードの生成
  mConstNode = alloc_node();
  mConstNode->mFunc = 0x0000U;
  mConstNode->mSupport = 0U;
  mConstNode->mSlink = NULL;
  assert_cond( mConstNode->id() == 0, __FILE__, __LINE__);

  // 入力ノードの生成
  mInputNode = alloc_node();
  mInputNode->mId |= 1U;
  mInputNode->mFunc = 0xAAAAU;
  mInputNode->mSupport = 1U;
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
  return NpnHandle(0, NpnXform(0, 1U));
}

// @brief 入力ノードを生成する．
  /// @param[in] input_id ( 0 <= input_id < 4 )
// @note すでに同じ入力番号のノードが存在していたらそれを返す．
NpnHandle
NpnNodeMgr::make_input(ymuint input_id)
{
  switch ( input_id ) {
  case 0: return NpnHandle(1, NpnXform());
  case 1: return NpnHandle(1, NpnXform(6, 0U));
  case 2: return NpnHandle(1, NpnXform(12, 0U));
  case 3: return NpnHandle(1, NpnXform(18, 0U));
  }
  assert_not_reached(__FILE__, __LINE__);
  return NpnHandle();
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

  return new_node(false, func, false, fanin0, fanin1);
}

// @brief ORノードを生成する．
// @param[in] fanin0 ファンイン0
// @param[in] fanin1 ファンイン1
// @note もしも既に同じ構造のノードがあればそれを返す．
// @note 場合によってはファンインその物や定数ノードを返すこともある．
// @note 実際には AND ノード＋反転属性
NpnHandle
NpnNodeMgr::make_or(NpnHandle fanin0,
		    NpnHandle fanin1)
{
  // AND の双対
  return ~make_and(~fanin0, ~fanin1);
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

  bool inv0 = fanin0.oinv();
  bool inv1 = fanin1.oinv();
  bool oinv = inv0 ^ inv1;
  if ( inv0 ) {
    fanin0 = ~fanin0;
  }
  if ( inv1 ) {
    fanin1 = ~fanin1;
  }

  return new_node(true, func, oinv, fanin0, fanin1);
}

// @brief ハンドルの表す関数を返す．
ymuint16
NpnNodeMgr::func(NpnHandle handle) const
{
  NpnNode* node = mNodeList[handle.node_id()];
  return xform(node->func(), handle.npn_xform());
}

BEGIN_NONAMESPACE

// 関数のサポートを求める．
ymuint
support_vec(ymuint16 func)
{
  // 数が少ないので個別にやる．
  ymuint vec = 0U;

  // 0 番めの変数
  ymuint16 c0_0 = func & 0x5555U;
  ymuint16 c0_1 = (func & 0xaaaaU) >> 1;
  if ( c0_0 != c0_1 ) {
    vec |= 1U;
  }

  // 1 番めの変数
  ymuint16 c1_0 = func & 0x3333U;
  ymuint16 c1_1 = (func & 0xccccU) >> 2;
  if ( c1_0 != c1_1 ) {
    vec |= 2U;
  }

  // 2 番めの変数
  ymuint16 c2_0 = func & 0x0f0fU;
  ymuint16 c2_1 = (func & 0xf0f0U) >> 4;
  if ( c2_0 != c2_1 ) {
    vec |= 4U;
  }

  // 3 番めの変数
  ymuint16 c3_0 = func & 0x00ffU;
  ymuint16 c3_1 = (func & 0xff00U) >> 8;
  if ( c3_0 != c3_1 ) {
    vec |= 8;
  }

  return vec;
}

// 関数のサポートを求める．
ymuint
support(ymuint16 func)
{
  ymuint vec = support_vec(func);

  if ( vec == 0U ) {
    return 0;
  }
  if ( vec == 1U ) {
    return 1;
  }
  if ( vec == 3U ) {
    return 2;
  }
  if ( vec == 7U ) {
    return 3;
  }
  if ( vec == 15U ) {
    return 4;
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// 関数のコファクターを求める．
ymuint
cofactor0(ymuint func,
	  ymuint pos)
{
  switch ( pos ) {
  case 0:
    {
      ymuint tmp = func & 0x5555U;
      return tmp | (tmp << 1);
    }

  case 1:
    {
      ymuint tmp = func & 0x3333U;
      return tmp | (tmp << 2);
    }

  case 2:
    {
      ymuint tmp = func & 0x0F0FU;
      return tmp | (tmp << 4);
    }

  case 3:
    {
      ymuint tmp = func & 0x00FFU;
      return tmp | (tmp << 8);
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0U;
}

// 関数のコファクターを求める．
ymuint
cofactor1(ymuint func,
	  ymuint pos)
{
  switch ( pos ) {
  case 0:
    {
      ymuint tmp = func & 0xAAAAU;
      return tmp | (tmp >> 1);
    }

  case 1:
    {
      ymuint tmp = func & 0xCCCCU;
      return tmp | (tmp >> 2);
    }

  case 2:
    {
      ymuint tmp = func & 0xF0F0U;
      return tmp | (tmp >> 4);
    }

  case 3:
    {
      ymuint tmp = func & 0xFF00U;
      return tmp | (tmp >> 8);
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0U;
}

END_NONAMESPACE


// @brief 新しいノードを登録する関数
// @param[in] is_xor XOR ノードの時 true にするフラグ
// @param[in] func 関数ベクタ
// @param[in] oinv 出力の極性
// @param[in] fanin0, fanin1 ファンインのハンドル
// @note おなじノードが既に存在していたらそのノードを返す．
NpnHandle
NpnNodeMgr::new_node(bool is_xor,
		     ymuint16 c_func,
		     bool c_oinv,
		     NpnHandle fanin0,
		     NpnHandle fanin1)
{
  bool debug = true;

  if ( debug ) {
    cout << endl;
    cout << "new_node(";
    if ( is_xor ) {
      cout << "XOR";
    }
    else {
      cout << "AND";
    }
    cout << ")" << endl;
    vector<NpnHandle> handle_list;
    handle_list.push_back(fanin0);
    handle_list.push_back(fanin1);
    dump_handle(cout, handle_list);
    cout << endl;
    cout << "func0 = ";
    print_func(cout, this->func(fanin0));
    cout << endl;
    cout << "func1 = ";
    print_func(cout, this->func(fanin1));
    cout << endl;
    if ( is_xor ) {
      cout << "func0 ^ fnc1 = ";
      print_func(cout, this->func(fanin0) ^ this->func(fanin1));
      cout << endl;
    }
    else {
      cout << "func0 & fnc1 = ";
      print_func(cout, this->func(fanin0) & this->func(fanin1));
      cout << endl;
    }
  }

  // Phase1: 準正規形にする．
  const Npn4Cannon& npn_normal = npn4norm[c_func];
  ymuint nfunc = npn_normal.mFunc;
  NpnXform xf(npn_normal.mPerm);
  fanin0 = xform_handle(fanin0, xf);
  fanin1 = xform_handle(fanin1, xf);

  // fanin0 の同位体変換のうちシグネチャが最小のものを求める．
  if ( 1 ) {
    ymuint16 func0 = func(fanin0);
    const Npn4Cannon& npn_cannon = npn4cannon[func0];
    ymuint16 c_func0 = npn_cannon.mFunc;
    assert_cond( mIdentList.count(c_func0) > 0, __FILE__, __LINE__);
    NpnXform cxf0 = NpnXform(npn_cannon.mPerm);
    NpnXform inv_cxf0 = inverse(cxf0);
    const vector<NpnXform>& ident_list0 = mIdentList[c_func0];
    NpnHandle min_fanin0 = fanin0;
    for (vector<NpnXform>::const_iterator p = ident_list0.begin();
	 p != ident_list0.end(); ++ p) {
      // ident_list0 は c_func0 のものなので cxf0 と inv_cxf0 でサンドイッチにする．
      NpnXform xf = cxf0 * (*p) * inv_cxf0;
      NpnHandle new_fanin0 = xform_handle(fanin0, xf);
      if ( min_fanin0 > new_fanin0 ) {
	min_fanin0 = new_fanin0;
      }
    }
    fanin0 = min_fanin0;
  }

  // fanin1 の同位体変換のうちシグネチャが最小のものを求める．
  if ( 1 ) {
    ymuint16 func1 = func(fanin1);
    const Npn4Cannon& npn_cannon = npn4cannon[func1];
    ymuint16 c_func1 = npn_cannon.mFunc;
    assert_cond( mIdentList.count(c_func1) > 0, __FILE__, __LINE__);
    NpnXform cxf1 = NpnXform(npn_cannon.mPerm);
    NpnXform inv_cxf1 = inverse(cxf1);
    const vector<NpnXform>& ident_list1 = mIdentList[c_func1];
    NpnHandle min_fanin1 = fanin1;
    for (vector<NpnXform>::const_iterator p = ident_list1.begin();
	 p != ident_list1.end(); ++ p) {
      // ident_list1 は c_func1 のものなので cxf1 と inv_cxf1 でサンドイッチにする．
      NpnXform xf = cxf1 * (*p) * inv_cxf1;
      NpnHandle new_fanin1 = xform_handle(fanin1, xf);
      if ( min_fanin1 > new_fanin1 ) {
	min_fanin1 = new_fanin1;
      }
    }
    fanin1 = min_fanin1;
  }

  if ( debug ) {
    cout << endl;
    cout << "normalize" << endl;
    cout << "original func   = ";
    print_func(cout, c_func);
    cout << endl
	 << "xform           = " << xf << endl
	 << "normalized func = ";
    print_func(cout, nfunc);
    cout << endl
	 << "fanin0          = " << fanin0 << endl
	 << "fanin1          = " << fanin1 << endl
	 << endl;
  }

  if ( debug ) {
    ymuint func2 = 0U;
    ymuint func0 = this->func(fanin0);
    ymuint func1 = this->func(fanin1);
    if ( is_xor ) {
      func2 = func0 ^ func1;
    }
    else {
      func2 = func0 & func1;
    }
    if ( c_oinv ) {
      func2 ^= 0xFFFFU;
    }
    ymuint new_func2 = xform(func2, inverse(xf));
    if ( new_func2 != c_func ) {
      cout << "func0 = ";
      print_func(cout, func0);
      cout << endl;
      cout << "func1 = ";
      print_func(cout, func1);
      cout << endl;
      cout << "original func2 = ";
      print_func(cout, func2);
      cout << endl;
      cout << "new func2 = ";
      print_func(cout, new_func2);
      cout << endl;
      cout << "c_func = ";
      print_func(cout, c_func);
      cout << endl;
    }
    assert_cond( new_func2 == c_func, __FILE__, __LINE__);
  }

  // Phase2: 左右の子供の順序を正規化する．
  ymuint node0 = fanin0.node_id();
  ymuint node1 = fanin1.node_id();
  if ( node0 > node1 ) {
    NpnHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }
  else if ( node0 == node1 ) {
    NpnXform xf0 = fanin0.npn_xform();
    NpnXform xf1 = fanin1.npn_xform();
    NpnXform ixf0 = inverse(xf0);
    NpnXform ixf1 = inverse(xf1);
    NpnXform a = xf1 * ixf0;
    NpnXform b = xf0 * ixf1;
    if ( a > b ) {
      NpnHandle tmp = fanin0;
      fanin0 = fanin1;
      fanin1 = tmp;
    }
  }
  NpnXform xf0 = fanin0.npn_xform();
  if ( xf0.output_inv() ) {
    xf0.flip_oinv();
  }
  NpnXform ixf0 = inverse(xf0);
  fanin0 = xform_handle(fanin0, ixf0);
  fanin1 = xform_handle(fanin1, ixf0);

  NpnXform oxf = xf0 * inverse(xf);
  if ( c_oinv ) {
    oxf.flip_oinv();
  }

  if ( debug ) {
    cout << endl;
    cout << "after normalize2" << endl;
    cout << "fanin0 = " << fanin0 << endl
	 << "fanin1 = " << fanin1 << endl
	 << "oxf    = " << oxf << endl
	 << "xf0    = " << xf0 << endl
	 << "ixf0   = " << ixf0 << endl;
  }

  // Phase3: XOR にまつわる極性の正規化
  {
    ymuint func0 = func(fanin0);
    ymuint func1 = func(fanin1);
    ymuint sup0 = support_vec(func0);
    ymuint sup1 = support_vec(func1);
    // - fanin0 の出力に否定がついていて
    // - fanin0 のみに現れる変数があって，
    // - その変数のみで XOR 分解できるとき，
    // - 出力の否定をその変数の否定に変換することができる．
    if ( fanin0.npn_xform().output_inv() ) {
      ymuint diff0 = sup0 & (0xFU ^ sup1);
      for (ymuint i = 0; i < 4; ++ i) {
	if ( (1U << i) & diff0 ) {
	  ymuint c0 = cofactor0(func0, i);
	  ymuint c1 = cofactor1(func0, i);
	  if ( (c0 ^ 0xFFFFU) == c1 ) {
	    fanin0 = ~fanin0;
	    oxf.flip_iinv(i);
	    break;
	  }
	}
      }
    }
    // fanin1 も同様
    if ( fanin1.npn_xform().output_inv() ) {
      ymuint diff1 = sup1 & (0xFU ^ sup0);
      for (ymuint i = 0; i < 4; ++ i) {
	if ( (1U << i) & diff1 ) {
	  ymuint c0 = cofactor0(func1, i);
	  ymuint c1 = cofactor1(func1, i);
	  if ( (c0 ^ 0xFFFFU) == c1 ) {
	    fanin1 = ~fanin1;
	    oxf.flip_iinv(i);
	    break;
	  }
	}
      }
    }
  }

  ymuint type_pat = is_xor ? 3U : 2U;
  ymuint pos = hash_func(fanin0, fanin1, is_xor);
  ymuint idx = pos % mHashSize;
  NpnNode* node = NULL;
  for (node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 &&
	 node->mFanin[1] == fanin1 &&
	 (node->mId & 3U) == type_pat ) {
      break;
    }
  }
  if ( node == NULL ) {
    ymuint func0 = func(fanin0);
    ymuint func1 = func(fanin1);
    ymuint node_func = (is_xor) ? func0 ^ func1 : func0 & func1;
    node = alloc_node();
    node->mId |= type_pat;
    node->mFunc = node_func;
    node->mSupport = support(node_func);
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
  }

  if ( debug ) {
    NpnHandle handle(node->id(), oxf);
    ymuint func1 = this->func(handle);
    cout << "func1 = ";
    print_func(cout, func1);
    cout << endl;
    cout << "c_func = ";
    print_func(cout, c_func);
    cout << endl;
    assert_cond( c_func == func1, __FILE__, __LINE__);
  }

  if ( debug ) {
    cout << "node->id() = " << node->id()
	 << ", xf = " << oxf << endl
	 << "support = " << static_cast<ymuint>(node->mSupport) << endl;
    dump_handle(cout, NpnHandle(node->id(), oxf));
    cout << "=====<<new_node end>>======================================" << endl;
  }

  return NpnHandle(node->id(), oxf);
}

// @brief NpnHandle に NPN 変換を施す．
NpnHandle
NpnNodeMgr::xform_handle(NpnHandle handle,
			 NpnXform xf) const
{
  ymuint id = handle.node_id();
  NpnXform new_xf = handle.npn_xform();
  new_xf *= xf;
  NpnNode* node = this->node(id);
  ymuint sup = node->support();
  new_xf.normalize(sup);
  return NpnHandle(id, new_xf);
}

// @brief mIdentList を設定する．
void
NpnNodeMgr::make_ident_list()
{
  vector<NpnXform> perm_list;
  for (ymuint p = 0; p < 768; ++ p) {
    perm_list.push_back(NpnXform(npn4perm[p]));
  }

  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    ymuint16 func = npn4rep[i];
    ymuint8 sup = support(func);
    for (vector<NpnXform>::iterator p = perm_list.begin();
	 p != perm_list.end(); ++ p) {
      NpnXform xf = *p;
      NpnXform xcf = xf.rep(sup);
      if ( xcf != xf ) {
	continue;
      }
      ymuint16 func1 = xform(func, xf);
      if ( func1 == func ) {
	mIdentList[func].push_back(xf);
      }
    }
  }
}

// @brief 展開したノード数を仮想的に返す．
ymuint
NpnNodeMgr::count(NpnHandle handle) const
{
  hash_set<ymuint32> hash1;
  return count_sub(handle, hash1);
}

// @brief count() の下請け関数
ymuint
NpnNodeMgr::count_sub(NpnHandle handle,
		      hash_set<ymuint32>& hash1) const
{
  NpnNode* node = mNodeList[handle.node_id()];
  if ( !node->is_logic() ) {
    return 0;
  }

  NpnXform xf = handle.npn_xform();
  if ( xf.output_inv() ) {
    xf.flip_oinv();
  }
  NpnHandle rep_handle(node->id(), xf);
  ymuint sig = rep_handle.hash();
  if ( hash1.count(sig) > 0 ) {
    return 0;
  }
  hash1.insert(sig);

  ymuint ans = 1;
  ans += count_sub(xform_handle(node->fanin0(), xf), hash1);
  ans += count_sub(xform_handle(node->fanin1(), xf), hash1);
  return ans;
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
  s << "----------------------------------------" << endl;
  ymuint i = 0;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p, ++ i) {
    NpnHandle handle = *p;
    s << "Handle#" << i << ": " << handle << endl;
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
    s << "( " << node->fanin0()
      << ", " << node->fanin1()
      << " )" << endl;
  }
}

// @brief ハンドルとその子供の内容を出力する．
void
NpnNodeMgr::dump_handle2(ostream& s,
			 NpnHandle handle) const
{
  vector<NpnHandle> tmp_list(1, handle);
  dump_handle2(s, tmp_list);
}

// @brief 複数のハンドルとその子供の内容を出力する．
void
NpnNodeMgr::dump_handle2(ostream& s,
			 const vector<NpnHandle>& handle_list) const
{
  s << "----------------------------------------" << endl;
  ymuint i = 0;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p, ++ i) {
    NpnHandle handle = *p;
    s << "Handle#" << i << ": " << handle << endl;
  }
  s << "----------------------------------------" << endl;

  hash_set<ymuint32> node_hash;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p) {
    NpnHandle handle = *p;
    dh2_sub(s, handle, node_hash);
  }
  s << "----------------------------------------" << endl;
}

// @brief dump_handle2() の下請け関数
void
NpnNodeMgr::dh2_sub(ostream& s,
		    NpnHandle handle,
		    hash_set<ymuint32>& node_hash) const
{
  ymuint id = handle.node_id();
  NpnNode* node = mNodeList[id];
  NpnXform xf = handle.npn_xform();
  NpnXform xf0 = xf.rep(node->support());
  NpnHandle rep_handle(id, xf0);
  ymuint sig = rep_handle.hash();
  if ( node_hash.count(sig) > 0 ) {
    return;
  }
  node_hash.insert(sig);

  if ( node->is_logic() ) {
    dh2_sub(s, xform_handle(node->fanin0(), xf0), node_hash);
    dh2_sub(s, xform_handle(node->fanin1(), xf0), node_hash);
  }

  s << rep_handle << ": ";
  if ( node->is_const() ) {
    if ( xf0.output_inv() ) {
      s << "CONST1" << endl;
    }
    else {
      s << "CONST0" << endl;
    }
  }
  else if ( node->is_input() ) {
    ymuint iid = xf0.input_perm(0);
    if ( xf0.input_inv(0) ) {
      s << "~";
    }
    s << "INPUT" << iid << endl;
  }
  else { // node->is_logic()
    if ( node->is_and() ) {
      s << "AND";
    }
    else {
      s << "XOR";
    }
    s << "( "
      << xform_handle(node->fanin0(), xf0)
      << " , "
      << xform_handle(node->fanin1(), xf0)
      << " )" << endl;
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
