
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

// 2つのNPN変換が両立するか調べる．
bool
check_compatible(NpnXform xf0,
		 NpnXform xf1,
		 ymuint8 sup,
		 ymuint8 sup0,
		 ymuint8 sup1)
{
  for (ymuint i = 0; i < 4; ++ i) {
    if ( sup & (1 << i) ) {
      if ( xf0.input_inv(i) != xf1.input_inv(i) ) {
	return false;
      }
      if ( xf0.input_perm(i) != xf1.input_perm(i) ) {
	ymuint x0 = xf0.input_perm(i);
	ymuint x1 = xf1.input_perm(i);
	if ( sup0 & (1 << x1) ||
	     sup1 & (1 << x0) ) {
	  return false;
	}
      }
    }
  }
  return true;
}

END_NONAMESPACE

// @brief 新しいノードを登録する関数
// @param[in] is_xor XOR ノードの時 true にするフラグ
// @param[in] func 関数ベクタ
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
  { // デバッグ用の表示
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

  ymuint sup = support(c_func);

  // fanin0, fanin1 に対して恒等変換を行い，
  // もっとも「小さい」ものを求める．
  if ( fanin0 > fanin1 ) {
    NpnHandle tmp = fanin0;
    fanin0 = fanin1;
    fanin1 = tmp;
  }
  NpnHandle min_fanin0 = fanin0; // ダミー
  NpnHandle min_fanin1 = fanin1; // ダミー
  bool first = true;
  bool min_oinv = false;

  assert_cond( mIdentList.count(c_func) > 0, __FILE__, __LINE__);
  const vector<NpnXform>& ident_list = mIdentList[c_func];
  for (vector<NpnXform>::const_iterator p = ident_list.begin();
       p != ident_list.end(); ++ p) {
    NpnXform xf = *p;
    bool tmp_oinv = false;
    if ( xf.output_inv() ) {
      xf.flip_oinv();
      tmp_oinv = true;
    }

    NpnHandle tmp_fanin0 = orig_fanin0 * xf;
    NpnHandle tmp_fanin1 = orig_fanin1 * xf;

    if ( tmp_fanin0 > tmp_fanin1 ) {
      NpnHandle tmp = tmp_fanin0;
      tmp_fanin0 = tmp_fanin1;
      tmp_fanin1 = tmp;
    }

    NpnHandle tmp_min_fanin0 = tmp_fanin0; // ダミー
    NpnHandle tmp_min_fanin1 = tmp_fanin1; // ダミー
    bool tmp_oinv0 = false;
    bool tmp_oinv1 = false;
    bool first0 = true;
    bool first1 = true;

    NpnXform tmp_min_xf0;

    ymuint16 func0 = this->func(tmp_fanin0);
    ymuint8 sup0 = support(func0);
    ymuint16 func1 = this->func(tmp_fanin1);
    ymuint8 sup1 = support(func1);
    ymuint8 sup01 = sup0 & sup1;

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
    const vector<NpnXform>& ident_list0 = mIdentList[c_func0];
    const vector<NpnXform>& ident_list1 = mIdentList[c_func1];
    {
      for (vector<NpnXform>::const_iterator p = ident_list0.begin();
	   p != ident_list0.end(); ++ p) {
	NpnXform xf0 = cxf0 * (*p) * inv_cxf0;
	xf0 = xf0.rep(sup0);
	bool valid = false;
	for (vector<NpnXform>::const_iterator q = ident_list1.begin();
	     q != ident_list1.end(); ++ q) {
	  NpnXform xf1 = cxf1 * (*q) * inv_cxf1;
	  xf1 = xf1.rep(sup1);
	  if ( check_compatible(xf0, xf1, sup01, sup0, sup1) ) {
	    valid = true;
	    break;
	  }
	}
	if ( !valid ) {
	  if ( c_func == 0x9898U ) {
	    cout << "xf0: " << xf0 << " is invalid" << endl;
	  }
	  continue;
	}
	NpnHandle xfanin0 = cannonical(tmp_fanin0 * xf0);
	bool oinv = false;
	if ( is_xor && xfanin0.oinv() ) {
	  xfanin0 = ~xfanin0;
	  oinv = !oinv;
	}
	if ( first0 || tmp_min_fanin0 > xfanin0 ) {
	  first0 = false;
	  tmp_min_fanin0 = xfanin0;
	  tmp_min_xf0 = xf0;
	  tmp_oinv0 = oinv;
	}
      }
      assert_cond( !first0, __FILE__, __LINE__);
      if ( c_func == 0x9898U ) {
	cout << "func0" << endl;
	print_func(cout, func0);
	cout << endl;
	cout << "fanin0" << endl;
	dump_handle(cout, tmp_fanin0);
	cout << endl
	     << "func(min_fanin0)" << endl;
	print_func(cout, this->func(tmp_min_fanin0));
	cout << endl;
	cout << "min_fanin0" << endl;
	dump_handle(cout, tmp_min_fanin0);
      }
      ymuint tmp_func0 = func0;
      if ( tmp_oinv0 ) {
	tmp_func0 ^= 0xFFFFU;
      }
      assert_cond( this->func(tmp_min_fanin0) == tmp_func0, __FILE__, __LINE__);
    }

    NpnXform tmp_min_xf1;
    {
      for (vector<NpnXform>::const_iterator p = ident_list1.begin();
	   p != ident_list1.end(); ++ p) {
	NpnXform xf1 = cxf1 * (*p) * inv_cxf1;
	xf1 = xf1.rep(sup1);
	if ( !check_compatible(tmp_min_xf0, xf1, sup01, sup0, sup1) ) {
	  if ( c_func == 0x9898U ) {
	    cout << "xf0: " << tmp_min_xf0
		 << " and xf1: " << xf1
		 << " are incompatible" << endl;
	  }
	  continue;
	}
	NpnHandle xfanin1 = cannonical(tmp_fanin1 * xf1);
	bool oinv = false;
	if ( is_xor && xfanin1.oinv() ) {
	  xfanin1 = ~xfanin1;
	  oinv = !oinv;
	}
	if ( first1 || tmp_min_fanin1 > xfanin1 ) {
	  first1 = false;
	  tmp_min_fanin1 = xfanin1;
	  tmp_min_xf1 = xf1;
	  tmp_oinv1 = oinv;
	}
      }
      assert_cond( !first1, __FILE__, __LINE__);

      if ( c_func == 0x9898U ) {
	cout << "func1" << endl;
	print_func(cout, func1);
	cout << endl;
	cout << "fanin1" << endl;
	dump_handle(cout, tmp_fanin1);
	cout << endl
	     << "func(min_fanin1)" << endl;
	print_func(cout, this->func(tmp_min_fanin1));
	cout << endl;
	cout << "min_fanin1" << endl;
	dump_handle(cout, tmp_min_fanin1);
      }
      ymuint tmp_func1 = func1;
      if ( tmp_oinv1 ) {
	tmp_func1 ^= 0xFFFFU;
      }
      assert_cond( this->func(tmp_min_fanin1) == tmp_func1, __FILE__, __LINE__);
    }

    tmp_oinv ^= tmp_oinv0;
    tmp_oinv ^= tmp_oinv1;
    if ( 0 ) {
      cout << "tmp_min_fanins" << endl;
      vector<NpnHandle> tmp_fanins;
      tmp_fanins.push_back(tmp_min_fanin0);
      tmp_fanins.push_back(tmp_min_fanin1);
      dump_handle(cout, tmp_fanins);
    }

    if ( first ) {
      first = false;
      min_fanin0 = tmp_min_fanin0;
      min_fanin1 = tmp_min_fanin1;
      min_oinv = tmp_oinv;
    }
    else if ( min_fanin0 > tmp_min_fanin0 ||
	      min_fanin0 == tmp_min_fanin0 && min_fanin1 > tmp_min_fanin1 ) {
      min_fanin0 = tmp_min_fanin0;
      min_fanin1 = tmp_min_fanin1;
      min_oinv = tmp_oinv;
    }
  }

  fanin0 = min_fanin0;
  fanin1 = min_fanin1;
  if ( min_oinv ) {
    oxf.flip_oinv();
    func ^= 0xFFFF;
  }
  if ( 1 ) {
    cout << "min_oinv = " << min_oinv << endl;
    vector<NpnHandle> tmp;
    tmp.push_back(fanin0);
    tmp.push_back(fanin1);
    dump_handle(cout, tmp);
    cout << "=================================================" << endl;
  }

  {
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
      cout << "node->id() = " << node->id()
	   << ", xf = " << oxf << endl
	   << "=====<<new_node end>>======================================" << endl;
      return NpnHandle(node->id(), oxf);
    }
  }
  NpnNode* node = alloc_node();
  node->mId |= type_pat;
  node->mFunc = func;
  node->mSupport = sup;
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

  cout << "node->id() = " << node->id()
       << ", xf = " << oxf << endl
       << "=====<<new_node end>>======================================" << endl;

  dump_handle(cout, NpnHandle(node->id(), oxf));

  return NpnHandle(node->id(), oxf);
}

// @brief 枝を正規化する．
NpnHandle
NpnNodeMgr::cannonical(NpnHandle src)
{
  ymuint id = src.node_id();
  NpnNode* node = mNodeList[id];
  NpnXform xf = src.npn_xform();
  NpnXform cxf = xf.rep(node->support());
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
