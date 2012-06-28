
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
  mConstNode->mVolume = 0U;
  mConstNode->mFunc = 0x0000U;
  mConstNode->mSupport = 0U;
  mConstNode->mSlink = NULL;
  assert_cond( mConstNode->id() == 0, __FILE__, __LINE__);

  // 入力ノードの生成
  mInputNode = alloc_node();
  mInputNode->mId |= 1U;
  mInputNode->mVolume = 0U;
  mInputNode->mFunc = 0xaaaa;
  mInputNode->mSupport = 0x1U;
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

  // ただし出力に関しては AND になるように調整する．
  bool oinv = (perm >> 0) & 1U;
  perm &= ~1U;

  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);

  NpnHandle c_fanin0 = fanin0 * xf;
  NpnHandle c_fanin1 = fanin1 * xf;

  // 既存の等価なノードを探すか新しいノードを作る．
  NpnHandle ohandle = new_node(false, c_func, oinv, c_fanin0, c_fanin1);

  return ohandle * inv_xf;
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

  // func がNPN同値類の代表関数になるように正規化する．
  const Npn4Cannon& npn_cannon = npn4cannon[func];
  ymuint16 c_func = npn_cannon.mFunc;
  ymuint16 perm = npn_cannon.mPerm;

  // ただし出力は反転させない．
  bool oinv = (perm >> 0) & 1U;
  perm &= ~1U;

  NpnXform xf(perm);
  NpnXform inv_xf = inverse(xf);

  NpnHandle c_fanin0 = fanin0 * xf;
  NpnHandle c_fanin1 = fanin1 * xf;

  // 既存の等価なノードを探すか新しいノードを作る．
  NpnHandle ohandle = new_node(true, c_func, oinv, c_fanin0, c_fanin1);

  return ohandle * inv_xf;
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
support(ymuint16 func)
{
  // 数が少ないので個別にやる．
  ymuint ans = 0U;

  // 0 番めの変数
  ymuint16 c0_0 = func & 0x5555U;
  ymuint16 c0_1 = (func & 0xaaaaU) >> 1;
  if ( c0_0 != c0_1 ) {
    ans |= 1U;
  }

  // 1 番めの変数
  ymuint16 c1_0 = func & 0x3333U;
  ymuint16 c1_1 = (func & 0xccccU) >> 2;
  if ( c1_0 != c1_1 ) {
    ans |= 2U;
  }

  // 2 番めの変数
  ymuint16 c2_0 = func & 0x0f0fU;
  ymuint16 c2_1 = (func & 0xf0f0U) >> 4;
  if ( c2_0 != c2_1 ) {
    ans |= 4U;
  }

  // 3 番めの変数
  ymuint16 c3_0 = func & 0x00ffU;
  ymuint16 c3_1 = (func & 0xff00U) >> 8;
  if ( c3_0 != c3_1 ) {
    ans |= 8;
  }

  return ans;
}

void
mark1(NpnNodeMgr* mgr,
      ymuint id,
      hash_set<ymuint32>& node_hash)
{
  NpnNode* node = mgr->node(id);
  if ( true && !node->is_logic() ) {
    return;
  }

  if ( node_hash.count(id) > 0 ) {
    return;
  }
  node_hash.insert(id);

  if ( node->is_logic() ) {
    mark1(mgr, node->fanin0().node_id(), node_hash);
    mark1(mgr, node->fanin1().node_id(), node_hash);
  }
}

void
mark2(NpnNodeMgr* mgr,
      ymuint id,
      hash_set<ymuint32>& node_hash,
      hash_set<ymuint32>& node_hash2,
      vector<ymuint32>& node_list)
{
  NpnNode* node = mgr->node(id);
  if ( true && !node->is_logic() ) {
    return;
  }

  if ( node_hash2.count(id) > 0 ) {
    return;
  }
  node_hash2.insert(id);

  if ( node_hash.count(id) > 0 ) {
    node_list.push_back(id);
  }

  if ( node->is_logic() ) {
    mark2(mgr, node->fanin0().node_id(), node_hash, node_hash2, node_list);
    mark2(mgr, node->fanin1().node_id(), node_hash, node_hash2, node_list);
  }
}

void
mark3(NpnNodeMgr* mgr,
      ymuint id0,
      NpnHandle handle,
      hash_set<ymuint32>& handle_hash,
      vector<NpnHandle>& handle_list)
{
  ymuint sig = handle.hash();
  if ( handle_hash.count(sig) > 0 ) {
    return;
  }
  handle_hash.insert(sig);

  ymuint id = handle.node_id();
  if ( id == id0 ) {
    handle_list.push_back(handle);
  }

  NpnXform xf = handle.npn_xform();
  NpnNode* node = mgr->node(id);
  mark3(mgr, id0, node->fanin0() * xf, handle_hash, handle_list);
  mark3(mgr, id0, node->fanin1() * xf, handle_hash, handle_list);
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
  NpnHandle orig_fanin0 = fanin0;
  NpnHandle orig_fanin1 = fanin1;
  NpnXform oxf;
  ymuint16 func = c_func;
  if ( c_oinv ) {
    func ^= 0xFFFFU;
    oxf.flip_oinv();
  }
  if ( 0 ) { // デバッグ用の表示
    cout << "=======<<new_node>>========================================" << endl;
    cout << "func: ";
    print_func(cout, func);
    cout << endl;
    if ( is_xor ) {
      if ( c_oinv ) {
	cout << "XNOR" << endl;
      }
      else {
	cout << "XOR" << endl;
      }
    }
    else {
      if ( c_oinv ) {
	cout << "NAND" << endl;
      }
      else {
	cout << "AND" << endl;
      }
    }

    cout << "  func0: ";
    print_func(cout, this->func(fanin0));
    cout << endl
	 << "  func1: ";
    print_func(cout, this->func(fanin1));
    cout << endl
	 << endl;
    vector<NpnHandle> tmp;
    tmp.push_back(fanin0);
    tmp.push_back(fanin1);
    dump_handle(cout, tmp);
    cout << endl;
  }

  // fanin0 と fanin1 に共通なノードを求める．
  vector<NpnHandle> eq_list;
  vector<pair<NpnHandle, NpnHandle> > neq_list;
  {
    hash_set<ymuint32> node_hash;
    vector<ymuint32> node_list;
    mark1(this, fanin0.node_id(), node_hash);
    hash_set<ymuint32> node_hash2;
    mark2(this, fanin1.node_id(), node_hash, node_hash2, node_list);
    for (vector<ymuint32>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      ymuint id = *p;
      // Node#(id) に関する fanin0 と fanin1 の組を求める．
      hash_set<ymuint32> handle_hash;
      vector<NpnHandle> handle_list0;
      mark3(this, id, fanin0, handle_hash, handle_list0);
      handle_hash.clear();
      vector<NpnHandle> handle_list1;
      mark3(this, id, fanin1, handle_hash, handle_list1);
      for (vector<NpnHandle>::iterator q = handle_list0.begin();
	   q != handle_list0.end(); ++ q) {
	NpnHandle h0 = cannonical(*q);
	if ( h0.oinv() ) {
	  h0 = ~h0;
	}
	for (vector<NpnHandle>::iterator r = handle_list1.begin();
	     r != handle_list1.end(); ++ r) {
	  NpnHandle h1 = cannonical(*r);
	  if ( h1.oinv() ) {
	    h1 = ~h1;
	  }
	  if ( h0 == h1 ) {
	    eq_list.push_back(h0);
	  }
	  else {
	    neq_list.push_back(make_pair(h0, h1));
	  }
	}
      }
    }
  }
  if ( 0 ) {
    cout << " eq_list:" << endl;
    for (vector<NpnHandle>::const_iterator p = eq_list.begin();
	 p != eq_list.end(); ++ p) {
      NpnHandle h = *p;
      dump_handle(cout, h);
    }
    cout << endl;
    cout << " neq_list:" << endl;
    for (vector<pair<NpnHandle, NpnHandle> >::const_iterator p = neq_list.begin();
	 p != neq_list.end(); ++ p) {
      NpnHandle h0 = p->first;
      NpnHandle h1 = p->second;
      vector<NpnHandle> tmp_handles;
      tmp_handles.push_back(h0);
      tmp_handles.push_back(h1);
      dump_handle(cout, tmp_handles);
    }
    cout << endl << endl;
  }

  // Phase-1: f = g0 o g1 ( o は AND/XOR ) の同位体変換のうち
  //          g0 が最小になるものを求める．
  // 結果として fanin0, fanin1 が書き換わる．
  // ??? 出力の極性はどうする．???
  if ( 0 ) {
    NpnXform xf1;
    bool oinv1 = false;
    bool xchg1 = false;
    ymuint16 func0 = this->func(fanin0);
    ymuint16 func1 = this->func(fanin1);
    ymuint16 min_f0 = func0;
    if ( min_f0 > func1 ) {
      min_f0 = func1;
      xchg1 = true;
    }
    assert_cond( mIdentList.count(c_func) > 0, __FILE__, __LINE__);
    const vector<NpnXform>& ident_list = mIdentList[c_func];
    for (vector<NpnXform>::const_iterator p = ident_list.begin();
	 p != ident_list.end(); ++ p) {
      NpnXform xf = *p;
      // 出力の反転はファンインへ推移しないので分離する．
      bool oinv = false;
      if ( xf.output_inv() ) {
	oinv = true;
	xf.flip_oinv();
      }
      ymuint16 tmp_func0 = xform(func0, xf);
      ymuint16 tmp_func1 = xform(func1, xf);
      if ( min_f0 > tmp_func0 ) {
	min_f0 = tmp_func0;
	xf1 = xf;
	oinv1 = oinv;
      }
      if ( min_f0 > tmp_func1 ) {
	min_f0 = tmp_func1;
	xf1 = xf;
	oinv1 = oinv;
	xchg1 = true;
      }
    }
    if ( xchg1 ) {
      fanin0 = orig_fanin1 * xf1;
      fanin1 = orig_fanin0 * xf1;
    }
    else {
      fanin0 = orig_fanin0 * xf1;
      fanin1 = orig_fanin1 * xf1;
    }
    if ( oinv1 ) {
      oxf.flip_oinv();
      func ^= 0xFFFF;
    }
  }
  if ( 0 ) { // デバッグ用の表示
    cout << "=======<<new_node: Phase-1>>===================================" << endl;
    cout << "func: ";
    print_func(cout, func);
    cout << endl;
    if ( is_xor ) {
      cout << "XOR" << endl;
    }
    else {
      cout << "AND" << endl;
    }

    cout << "  func0: ";
    print_func(cout, this->func(fanin0));
    cout << endl
	 << "  func1: ";
    print_func(cout, this->func(fanin1));
    cout << endl
	 << endl;
    vector<NpnHandle> tmp;
    tmp.push_back(fanin0);
    tmp.push_back(fanin1);
    dump_handle(cout, tmp);
    cout << endl;
  }

  // Phase-2: 各々のファンインの同位体変換のうちシグネチャが最小になるものを
  //          求める．
  {
    NpnHandle min_fanin0 = fanin0;
    NpnHandle min_fanin1 = fanin1;
    NpnXform min_xf0;
    NpnXform min_xf1;

    ymuint16 func0 = this->func(fanin0);
    ymuint16 func1 = this->func(fanin1);
    ymuint8 sup0 = support(func0);
    ymuint8 sup1 = support(func1);

    ymuint16 c_func0;
    NpnXform cxf0;
    NpnXform inv_cxf0;
    {
      const Npn4Cannon& npn_cannon = npn4cannon[func0];
      c_func0 = npn_cannon.mFunc;
      cxf0 = NpnXform(npn_cannon.mPerm);
      inv_cxf0 = inverse(cxf0);
      assert_cond( mIdentList.count(c_func0) > 0, __FILE__, __LINE__);
    }
    const vector<NpnXform>& ident_list0 = mIdentList[c_func0];

    ymuint16 c_func1;
    NpnXform cxf1;
    NpnXform inv_cxf1;
    {
      const Npn4Cannon& npn_cannon = npn4cannon[func1];
      c_func1 = npn_cannon.mFunc;
      cxf1 = NpnXform(npn_cannon.mPerm);
      inv_cxf1 = inverse(cxf1);
      assert_cond( mIdentList.count(c_func1) > 0, __FILE__, __LINE__);
    }
    const vector<NpnXform>& ident_list1 = mIdentList[c_func1];

    bool first0 = true;
    bool oinv0 = false;
    for (vector<NpnXform>::const_iterator p = ident_list0.begin();
	 p != ident_list0.end(); ++ p) {
      NpnXform xf0 = cxf0 * (*p) * inv_cxf0;
      xf0 = xf0.rep(sup0);
      // もとの構造と等価な構造を持つかチェックする．
      bool valid = false;
      for (vector<NpnXform>::const_iterator q = ident_list1.begin();
	   q != ident_list1.end(); ++ q) {
	NpnXform xf1 = cxf1 * (*q) * inv_cxf1;
	xf1 = xf1.rep(sup1);
	if ( check_compat(xf0, xf1, eq_list, neq_list) ) {
	  valid = true;
	  break;
	}
      }
      if ( !valid ) {
	continue;
      }

      NpnHandle xfanin0 = cannonical(fanin0 * xf0);
      bool oinv = false;
      if ( is_xor && xfanin0.oinv() ) {
	xfanin0 = ~xfanin0;
	oinv = !oinv;
      }
      if ( first0 || min_fanin0 > xfanin0 ) {
	first0 = false;
	min_fanin0 = xfanin0;
	min_xf0 = xf0;
	oinv0 = oinv;
      }
    }
    assert_cond( !first0, __FILE__, __LINE__);
    {
      ymuint tmp_func0 = func0;
      if ( oinv0 ) {
	tmp_func0 ^= 0xFFFFU;
      }
      assert_cond( this->func(min_fanin0) == tmp_func0, __FILE__, __LINE__);
    }
    if ( 0 ) {
      cout << "min_fanin0:" << endl;
      dump_handle(cout, min_fanin0);
    }

    bool first1 = true;
    bool oinv1 = false;
    for (vector<NpnXform>::const_iterator p = ident_list1.begin();
	 p != ident_list1.end(); ++ p) {
      NpnXform xf1 = cxf1 * (*p) * inv_cxf1;
      xf1 = xf1.rep(sup1);
      if ( !check_compat(min_xf0, xf1, eq_list, neq_list) ) {
	continue;
      }
      NpnHandle xfanin1 = cannonical(fanin1 * xf1);
      bool oinv = false;
      if ( is_xor && xfanin1.oinv() ) {
	xfanin1 = ~xfanin1;
	oinv = !oinv;
      }
      if ( first1 || min_fanin1 > xfanin1 ) {
	first1 = false;
	min_fanin1 = xfanin1;
	min_xf1 = xf1;
	oinv1 = oinv;
      }
    }
    assert_cond( !first1, __FILE__, __LINE__);
    {
      ymuint tmp_func1 = func1;
      if ( oinv1 ) {
	tmp_func1 ^= 0xFFFFU;
      }
      assert_cond( this->func(min_fanin1) == tmp_func1, __FILE__, __LINE__);
    }

    if ( min_fanin0 < min_fanin1 ) {
      fanin0 = min_fanin0;
      fanin1 = min_fanin1;
    }
    else {
      fanin0 = min_fanin1;
      fanin1 = min_fanin0;
    }
    if ( oinv0 ^ oinv1 ) {
      oxf.flip_oinv();
      func ^= 0xFFFF;
    }
  }
  if ( 0 ) { // デバッグ用の表示
    cout << "=======<<new_node: Phase-2>>===================================" << endl;
    cout << "func: ";
    print_func(cout, func);
    cout << endl;
    if ( is_xor ) {
      cout << "XOR" << endl;
    }
    else {
      cout << "AND" << endl;
    }

    cout << "  func0: ";
    print_func(cout, this->func(fanin0));
    cout << endl
	 << "  func1: ";
    print_func(cout, this->func(fanin1));
    cout << endl
	 << endl;
    vector<NpnHandle> tmp;
    tmp.push_back(fanin0);
    tmp.push_back(fanin1);
    dump_handle(cout, tmp);
    cout << endl;
  }

  if ( 0 ) {
    ymuint16 func0 = this->func(fanin0);
    ymuint16 func1 = this->func(fanin1);
    ymuint16 tmp_func;
    if ( is_xor ) {
      tmp_func = func0 ^ func1;
    }
    else {
      tmp_func = func0 & func1;
    }
    if ( func != tmp_func ) {
      if ( is_xor ) {
	cout << "XOR" << endl;
      }
      else {
	cout << "AND" << endl;
      }
      cout << "func: ";
      print_func(cout, func);
      cout << ", tmp_func: ";
      print_func(cout, tmp_func);
      cout << endl;
      cout << " func0: ";
      print_func(cout, func0);
      cout << endl;
      cout << " func1: ";
      print_func(cout, func1);
      cout << endl;
      vector<NpnHandle> tmp_fanins;
      tmp_fanins.push_back(fanin0);
      tmp_fanins.push_back(fanin1);
      dump_handle(cout, tmp_fanins);
      cout << endl
	   << "Original fanin" << endl;
      tmp_fanins.clear();
      tmp_fanins.push_back(orig_fanin0);
      tmp_fanins.push_back(orig_fanin1);
      dump_handle(cout, tmp_fanins);
    }
    assert_cond( func == tmp_func, __FILE__, __LINE__);
  }

  ymuint type_pat = is_xor ? 3U : 2U;
  ymuint pos = hash_func(fanin0, fanin1, is_xor);
  ymuint idx = pos % mHashSize;
  for (NpnNode* node = mHashTable[idx]; node; node = node->mSlink) {
    if ( node->mFanin[0] == fanin0 &&
	 node->mFanin[1] == fanin1 &&
	 (node->mId & 3U) == type_pat ) {
      if ( 0 ) {
	cout << "node->id() = " << node->id()
	     << ", xf = " << oxf << endl
	     << "=====<<new_node end>>======================================" << endl;
      }
      return NpnHandle(node->id(), oxf);
    }
  }
  NpnNode* node = alloc_node();
  node->mId |= type_pat;
  node->mFunc = func;
  node->mSupport = support(func);
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

  if ( 0 ) {
    cout << "node->id() = " << node->id()
	 << ", xf = " << oxf << endl;
    dump_handle(cout, NpnHandle(node->id(), oxf));
    cout << "=====<<new_node end>>======================================" << endl;
  }

  return NpnHandle(node->id(), oxf);
}

// 2つのNPN変換が両立するか調べる．
bool
NpnNodeMgr::check_compat(NpnXform xf0,
			 NpnXform xf1,
			 const vector<NpnHandle>& eq_list,
			 const vector<pair<NpnHandle, NpnHandle> >& neq_list)
{
  if ( 0 ) {
    cout << "check_compatible" << endl
	 << " xf0: " << xf0 << endl
	 << " xf1: " << xf1 << endl;
    cout << "eq_list" << endl;
  }
  for (vector<NpnHandle>::const_iterator p = eq_list.begin();
       p != eq_list.end(); ++ p) {
    NpnHandle h = *p;
    NpnHandle h0 = cannonical(h * xf0);
    if ( h0.oinv() ) {
      h0 = ~h0;
    }
    NpnHandle h1 = cannonical(h * xf1);
    if ( h1.oinv() ) {
      h1 = ~h1;
    }
    if ( 0 ) {
      cout << "h * xf0" << endl;
      dump_handle(cout, h0);
      cout << "h * xf1" << endl;
      dump_handle(cout, h1);
    }
    if ( h0 != h1 ) {
      if ( 0 ) {
	cout << "false" << endl;
      }
      return false;
    }
  }
  if ( 0 ) {
    cout << "neq_list" << endl;
  }
  for (vector<pair<NpnHandle, NpnHandle> >::const_iterator p = neq_list.begin();
       p != neq_list.end(); ++ p) {
    NpnHandle h0 = p->first;
    NpnHandle h1 = p->second;
    h0 = cannonical(h0 * xf0);
    h1 = cannonical(h1 * xf1);
    if ( h0.oinv() ) {
      h0 = ~h0;
    }
    if ( h1.oinv() ) {
      h1 = ~h1;
    }
    if ( 0 ) {
      cout << "h0 * xf0" << endl;
      dump_handle(cout, h0);
      cout << "h1 * xf1" << endl;
      dump_handle(cout, h1);
    }
    if ( h0 == h1 ) {
      if ( 0 ) {
	cout << "false" << endl;
      }
      return false;
    }
  }
  if ( 0 ) {
    cout << "true" << endl;
  }
  return true;
}

// @brief 枝を正規化する．
NpnHandle
NpnNodeMgr::cannonical(NpnHandle src)
{
  ymuint id = src.node_id();
  NpnNode* node = mNodeList[id];
  NpnXform xf = src.npn_xform();
  NpnXform cxf = xf.rep(node->support());

  // 入力ノード同士の AND/XOR ノードの場合
  if ( node->is_logic() &&
       node->fanin0().node_id() == 0 &&
       node->fanin1().node_id() == 0 ) {
    assert_cond( node->support() == 3U, __FILE__, __LINE__);
    ymuint p0 = cxf.input_perm(0);
    ymuint p1 = cxf.input_perm(1);
    if ( p0 > p1 ) {
      cxf.xchg2();
    }
  }

  NpnHandle ans(id, cxf);

  if ( 0 ) {
    cout << "Cannonical" << endl
	 << "Node#" << id << ", support = ";
    for (ymuint i = 0; i < 4; ++ i) {
      cout << ((node->support() >> i) & 1U);
    }
    cout << endl;
    vector<NpnHandle> tmp;
    tmp.push_back(src);
    tmp.push_back(ans);
    dump_handle(cout, tmp);
  }
  {
    ymuint16 f0 = func(src);
    ymuint16 f1 = func(ans);
    if ( f0 != f1 ) {
      cout << "Error in cannonical" << endl;
      vector<NpnHandle> tmp;
      tmp.push_back(src);
      tmp.push_back(ans);
      dump_handle(cout, tmp);
    }
  }
  return ans;
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
  NpnXform xf0 = xf.rep(node->support());
  if ( xf0.output_inv() ) {
    xf0.flip_oinv();
  }
  NpnHandle rep_handle(node->id(), xf0);
  ymuint sig = rep_handle.hash();
  if ( hash1.count(sig) > 0 ) {
    return 0;
  }
  hash1.insert(sig);

  ymuint ans = 1;
  ans += count_sub(node->fanin0() * xf0, hash1);
  ans += count_sub(node->fanin1() * xf0, hash1);
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
  s << "----------------------------------------" << endl;
  ymuint i = 0;
  for (vector<NpnHandle>::const_iterator p = handle_list.begin();
       p != handle_list.end(); ++ p, ++ i) {
    NpnHandle handle = *p;
    s << "Handle#" << i << ": ";
    print_handle(s, handle);
    s << endl;
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
    s << "Handle#" << i << ": ";
    print_handle(s, handle);
    s << endl;
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
    dh2_sub(s, node->fanin0() * xf0, node_hash);
    dh2_sub(s, node->fanin1() * xf0, node_hash);
  }

  print_handle(s, rep_handle);
  s << ": ";
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
    s << "( ";
    print_handle(s, node->fanin0() * xf0);
    s << " , ";
    print_handle(s, node->fanin1() * xf0);
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
