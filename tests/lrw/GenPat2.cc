
/// @file GenPat2.cc
/// @brief GenPat2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenPat2.h"
#include "NpnNode.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

struct Npn4Cannon
{
  ymuint16 mFunc;
  ymuint16 mPerm;
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint16 npn4perm[] = {
#include "npn4perm.h"
};

// 4入力のNPN同値類の代表関数への変換を表す配列
Npn4Cannon npn4cannon[] = {
#include "npn4cannon.h"
};

void
print_func(ostream& s,
	   ymuint16 func)
{
  s << hex << setw(4) << setfill('0') << func << dec;
}

ymuint32 n_compose;
ymuint32 level_over;
ymuint32 duplicate_aig;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GenPat2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenPat2::GenPat2()
{
}

// @brief デストラクタ
GenPat2::~GenPat2()
{
}

// @brief 4入力の全ての関数のパタンを求める．
void
GenPat2::operator()(ymuint slack)
{
  init_npn4rep();

  mNpnNodeList.clear();
  mRepList.clear();
  mCandListArray.clear();
  mFuncArray.clear();
  mFuncLevel.clear();
  mNpnHandleHash.clear();

  ymuint np = 1U << 4;
  ymuint nf = 1U << np;

  mFuncArray.resize(nf);
  mFuncLevel.resize(nf, 0xffffffff);

  calc_upper();

  n_compose = 0;
  level_over = 0;
  duplicate_aig = 0;

  mSlack = slack;

  // 定数0と定数1は除く
  mRemainFunc = nf - 2;

  // かなり怪しいコード
  mRemainRep = 221;

  // レベル0のパタンを作る．
  {
    NpnHandle ih = mMgr.make_input(0);
    add_cand(ih, 0);
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 0; level < mCandListArray.size(); ++ level ) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl
	 << "   remain_rep = " << mRemainRep << endl;
    max_level = level;

    while ( mNpnNodeList.size() <= level ) {
      mNpnNodeList.push_back(vector<NpnHandle>());
    }
    while ( mRepList.size() <= level ) {
      mRepList.push_back(vector<NpnHandle>());
    }

    ymuint n = mCandListArray[level].size();
    cout << "  " << n << " seed patterns" << endl;
    hash_map<ymuint16, vector<NpnHandle> > pat_list;
    vector<ymuint16> flist;
    for (ymuint i = 0; i < n; ++ i) {
      NpnHandle handle = mCandListArray[level][i];
      ymuint16 func = mMgr.func(handle);
      if ( mFuncLevel[func] + mSlack < level ) {
	continue;
      }
      mRepList[level].push_back(handle);
      if ( pat_list.count(func) == 0 ) {
	flist.push_back(func);
      }
      pat_list[func].push_back(handle);
    }

    sort(flist.begin(), flist.end());
    for (vector<ymuint16>::iterator p = flist.begin(); p != flist.end(); ++ p) {
      hash_map<ymuint16, vector<NpnHandle> >::iterator q = pat_list.find(*p);
      assert_cond( q != pat_list.end(), __FILE__, __LINE__);
      vector<NpnHandle>& handle_list = q->second;
      cout << "Function: " << setw(4) << setfill('0') << hex << *p << dec << endl;
      mMgr.dump_handle(cout, handle_list);
      cout << endl;
    }
    if ( level == 4 ) {
      exit(0);
    }

    const vector<NpnHandle>& src_list1 = mRepList[level];
    ymuint n1 = src_list1.size();
    cout << "  " << n1 << " true seed patterns" << endl;
    for (ymuint i = 0; i < n1; ++ i) {
      NpnHandle handle = src_list1[i];
      if ( 0 ) {
	mMgr.dump_handle(cout, handle);
	cout << endl;
      }
      npn_expand(handle, level);
    }
    cout << "  expand " << mNpnNodeList[level].size() << " patterns" << endl;

    if ( mRemainFunc == 0 ) {
      cout << "All functions has its patterns" << endl;
      break;
    }

    // level の代表パタンと他のパタンとの対を作る．
    for (ymuint i = 0; i < n1; ++ i) {
      NpnHandle handle1 = src_list1[i];

      mCountHash.clear();
      ymuint level_base = count1(handle1);

      for (ymuint l = 0; l < level; ++ l) {
	const vector<NpnHandle>& src_list2 = mNpnNodeList[l];
	ymuint n2 = src_list2.size();
	for (ymuint j = 0; j < n2; ++ j) {
	  NpnHandle handle2 = src_list2[j];
	  if ( 0 ) {
	    cout << "L#" << l << ": " << j << " / " << n2
		 << " | " << i << " / " << n1 << endl;
	  }
	  compose(handle1, handle2, level_base);
	}
      }

      const vector<NpnHandle>& src_list2 = mNpnNodeList[level];
      ymuint n2 = src_list2.size();
      for (ymuint j = 0; j < n2; ++ j) {
	if ( 0 ) {
	  cout << "L#" << level << ": " << j << " / " << n2
	       << " | " << i << " / " << n1 << endl;
	}
	NpnHandle handle2 = src_list2[j];
	compose(handle1, handle2, level_base);
      }
    }
  }

  ymuint total_cost = 0;
  bool error = false;
  for (ymuint i = 1; i < nf - 1; ++ i) {
    if ( mFuncArray[i].empty() ) {
      cerr << "No patterns for function["
	   << setw(4) << setfill('0') << hex << i << dec << "]" << endl;
      error = true;
    }
    else {
      total_cost += mFuncLevel[i];
    }
  }
  if ( error ) {
    cerr << "Error occured" << endl;
  }
  else {
    cout << "# total cost = " << total_cost << endl;

    vector<NpnHandle> handle_list;
    for (ymuint i = 1; i < nf - 1; ++ i) {
      if ( mNpnHash.count(i) == 0 ) {
	continue;
      }
      cout << "Function#" << hex << setw(4) << setfill('0')
	   << i << dec << ": "
	   << "Level " << mFuncLevel[i] << ": ";
      for (vector<NpnHandle>::iterator p = mFuncArray[i].begin();
	   p != mFuncArray[i].end(); ++ p) {
	NpnHandle handle = *p;
	cout << " " << handle.npn_xform();
	cout << "Node#" << handle.node_id() << endl;
	handle_list.push_back(handle);
      }
      cout << endl;
    }
    mMgr.dump_handle(cout, handle_list);
  }

  cout << "# # of compose = " << n_compose << endl
       << "# level over = " << level_over << endl
       << "# duplicate aig = " << duplicate_aig << endl;
}

// @brief NPN同値類を求める．
// @param[in] handle ハンドル
// @param[in] level レベル
void
GenPat2::npn_expand(NpnHandle handle,
		    ymuint32 level)
{
  hash_map<ymuint32, vector<FuncXform> >::const_iterator p;
  p = mNpnHash.find(mMgr.func(handle));
  assert_cond( p != mNpnHash.end(), __FILE__, __LINE__);
  const vector<FuncXform>& xf_list = p->second;
  for (vector<FuncXform>::const_iterator q = xf_list.begin();
       q != xf_list.end(); ++ q) {
    const FuncXform& xform = *q;
    NpnHandle handle1 = handle * xform.mXf;
    add_pat(handle1, level);
  }
}

// @brief パタンを登録する．
// @param[in] fv 関数ベクタ
// @param[in] aig AIG
// @param[in] level レベル
void
GenPat2::add_pat(NpnHandle handle,
		 ymuint32 level)
{
  ymuint16 fv = mMgr.func(handle);
  if ( mFuncArray[fv].empty() ) {
    mFuncLevel[fv] = level;
    -- mRemainFunc;
    if ( mNpnHash.count(fv) > 0 ) {
      -- mRemainRep;
    }
  }
  else if ( mFuncLevel[fv] > level ) {
    mFuncLevel[fv] = level;
  }

  mFuncArray[fv].push_back(handle);

  mNpnNodeList[level].push_back(handle);
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

END_NONAMESPACE

// @brief 2つのノードから新しいパタンを作る．
// @note 具体的には aig1 & aig2 と ~aig & aig
void
GenPat2::compose(NpnHandle handle1,
		 NpnHandle handle2,
		 ymuint level_base)
{
  ++ n_compose;

  ymuint32 fv1 = mMgr.func(handle1);
  ymuint32 fv2 = mMgr.func(handle2);
  ymuint32 fv3 = fv1 & fv2;
  ymuint32 fv4 = fv1 | fv2;
  ymuint32 fv5 = fv1 ^ fv2;

  ymuint sup0 = support(fv1) | support(fv2);
  ymuint sup3 = support(fv3);
  bool valid1 = true;
  if ( sup3 != sup0 ) {
    valid1 = false;
  }
  ymuint sup4 = support(fv4);
  bool valid2 = true;
  if ( sup4 != sup0 ) {
    valid2 = false;
  }
  ymuint sup5 = support(fv5);
  bool valid3 = true;
  if ( sup5 != sup0 ) {
    valid3 = false;
  }
#if 0
  bool valid1 = false;
  if ( fv3 != 0U && fv3 != fv1 && fv3 != fv2 ) {
    if ( (mFuncLevel[fv3] + mSlack) >= level_base + 1 ) {
      valid1 = true;
    }
  }
  bool valid2 = false;
  if ( fv4 != 0xFFFFU && fv4 != fv1 && fv4 != fv2 ) {
    if ( (mFuncLevel[fv4] + mSlack) >= level_base + 1 ) {
      valid2 = true;
    }
  }
  bool valid3 = false;
  if ( fv5 != 0U && fv5 != 0xFFFF && fv5 != fv1 && fv5 != fv2 ) {
    if ( (mFuncLevel[fv5] + mSlack) >= level_base + 1 ) {
      valid3 = true;
    }
  }

  if ( !valid1 && !valid2 && !valid3 ) {
    return;
  }

  ymuint level = count2(handle2) + level_base + 1;

  if ( 0 ) {
    NpnHandle tmp = mMgr.make_xor(handle1, handle2);
    level = mMgr.count(tmp);
  }

  if ( valid1 && (mFuncLevel[fv3] + mSlack) >= level ) {
    NpnHandle handle = mMgr.make_and(handle1, handle2);
    add_pair(handle, level);
  }

  if ( valid2 && (mFuncLevel[fv4] + mSlack) >= level ) {
    NpnHandle handle = mMgr.make_or(handle1, handle2);
    add_pair(handle, level);
  }

  if ( valid3 && (mFuncLevel[fv5] + mSlack) >= level ) {
    NpnHandle handle = mMgr.make_xor(handle1, handle2);
    add_pair(handle, level);
  }
#else
  ymuint level = count2(handle2) + level_base + 1;
  if ( valid1 && fv3 != fv1 && fv3 != fv2 ) {
    NpnHandle handle = mMgr.make_and(handle1, handle2);
    ymuint level1 = mMgr.count(handle);
    if ( level != level1 ) {
      cout << "error: level = " << level << ", level1 = " << level1 << endl;
      cout << "func:" << endl;
      print_func(cout, fv3);
      cout << endl;
      mMgr.dump_handle(cout, handle1);
      cout << endl;
      mMgr.dump_handle(cout, handle2);
      cout << endl;
      mMgr.dump_handle(cout, handle);
      cout << endl;
      cout << "sup0 = " << sup0 << endl
	   << "sup3 = " << sup3 << endl;
      mMgr.dump_handle2(cout, handle1);
      cout << endl;
      mMgr.dump_handle2(cout, handle2);
      cout << endl;
      mMgr.dump_handle2(cout, handle);
      cout << endl;
      if ( level > level1 ) {
	level = level1;
      }
    }
    //assert_cond( level == level1, __FILE__, __LINE__);
    if ( (mFuncLevel[fv3] + mSlack) >= level ) {
      add_pair(handle, level);
    }
  }
  if ( valid2 && fv4 != fv1 && fv4 != fv2 ) {
    NpnHandle handle = mMgr.make_or(handle1, handle2);
    ymuint level1 = mMgr.count(handle);
    if ( level != level1 ) {
      cout << "error: level = " << level << ", level1 = " << level1 << endl;
      cout << "func:" << endl;
      print_func(cout, fv4);
      cout << endl;
      mMgr.dump_handle(cout, handle1);
      cout << endl;
      mMgr.dump_handle(cout, handle2);
      cout << endl;
      mMgr.dump_handle(cout, handle);
      cout << endl;
      cout << "sup0 = " << sup0 << endl
	   << "sup4 = " << sup4 << endl;
      mMgr.dump_handle2(cout, handle1);
      cout << endl;
      mMgr.dump_handle2(cout, handle2);
      cout << endl;
      mMgr.dump_handle2(cout, handle);
      cout << endl;
      if ( level > level1 ) {
	level = level1;
      }
    }
    //assert_cond( level == level1, __FILE__, __LINE__);
    if ( (mFuncLevel[fv4] + mSlack) >= level ) {
      add_pair(handle, level);
    }
  }
  if ( valid3 && fv5 != 0xFFFF && fv5 != fv1 && fv5 != fv2 ) {
    NpnHandle handle = mMgr.make_xor(handle1, handle2);
    ymuint level1 = mMgr.count(handle);
    if ( level != level1 ) {
      cout << "error: level = " << level << ", level1 = " << level1 << endl;
      cout << "func:" << endl;
      print_func(cout, fv5);
      cout << endl;
      mMgr.dump_handle(cout, handle1);
      cout << endl;
      mMgr.dump_handle(cout, handle2);
      cout << endl;
      mMgr.dump_handle(cout, handle);
      cout << endl;
      cout << "sup0 = " << sup0 << endl
	   << "sup5 = " << sup5 << endl;
      mMgr.dump_handle2(cout, handle1);
      cout << endl;
      mMgr.dump_handle2(cout, handle2);
      cout << endl;
      mMgr.dump_handle2(cout, handle);
      cout << endl;
      if ( level > level1 ) {
	level = level1;
      }
    }
    //assert_cond( level == level1, __FILE__, __LINE__);
    if ( (mFuncLevel[fv5] + mSlack) >= level ) {
      add_pair(handle, level);
    }
  }
#endif
}

// @brief NpnHandle を登録する．
// @note 結果は mCandListArray に追加される．
void
GenPat2::add_pair(NpnHandle handle,
		  ymuint level)
{
  NpnXform xf(npn4cannon[mMgr.func(handle)].mPerm);
  NpnHandle chandle = handle * xf;
  ymuint id = chandle.node_id();
  if ( mNpnHandleHash.count(id) == 0 ) {
    mNpnHandleHash.insert(id);
    add_cand(chandle, level);
  }
  else {
    ++ duplicate_aig;
  }
}

// @brief 候補のリストに追加する．
void
GenPat2::add_cand(NpnHandle handle,
		  ymuint level)
{
  while ( mCandListArray.size() <= level ) {
    mCandListArray.push_back(vector<NpnHandle>());
  }
  mCandListArray[level].push_back(handle);
}

// @brief handle の子供に印をつけてノード数を数える．
ymuint
GenPat2::count1(NpnHandle handle)
{
  NpnNode* node = mMgr.node(handle.node_id());
  if ( !node->is_logic() ) {
    return 0;
  }

  NpnXform xf = handle.npn_xform();
  NpnXform xf0 = xf.rep(node->support());
  if ( xf0.output_inv() ) {
    xf0.flip_oinv();
  }
  ymuint sig = (node->id() << 10) | xf0.data();
  if ( mCountHash.count(sig) > 0 ) {
    return 0;
  }
  mCountHash.insert(sig);

  ymuint ans = 1;
  ans += count1(node->fanin0() * xf0);
  ans += count1(node->fanin1() * xf0);
  return ans;
}

// @brief count1 で印のついていないノード数を数える．
ymuint
GenPat2::count2(NpnHandle handle)
{
  hash_set<ymuint32> hash1;
  return count2_sub(handle, hash1);
}

// @brief count2 の下請け関数
ymuint
GenPat2::count2_sub(NpnHandle handle,
		    hash_set<ymuint32>& hash)
{
  NpnNode* node = mMgr.node(handle.node_id());
  if ( !node->is_logic() ) {
    return 0;
  }

  NpnXform xf = handle.npn_xform();
  NpnXform xf0 = xf.rep(node->support());
  if ( xf0.output_inv() ) {
    xf0.flip_oinv();
  }
  ymuint sig = (node->id() << 10) | xf0.data();
  if ( mCountHash.count(sig) > 0 || hash.count(sig) > 0 ) {
    return 0;
  }
  hash.insert(sig);

  ymuint ans = 1;
  ans += count2_sub(node->fanin0() * xf0, hash);
  ans += count2_sub(node->fanin1() * xf0, hash);
  return ans;
}

BEGIN_NONAMESPACE

// @brief 関数ベクタを変換する(4入力版)
ymuint16
xform_func4(ymuint16 fv,
	    NpnXform xf)
{
  ymuint16 ans = 0U;
  for (ymuint p = 0; p < 16; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      ymuint v3 = (p >> 3) & 1U;
      if ( xf.input_inv(0) ) {
	v0 ^= 1U;
      }
      if ( xf.input_inv(1) ) {
	v1 ^= 1U;
      }
      if ( xf.input_inv(2) ) {
	v2 ^= 1U;
      }
      if ( xf.input_inv(3) ) {
	v3 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << xf.input_perm(0));
      }
      if ( v1 ) {
	q |= (1U << xf.input_perm(1));
      }
      if ( v2 ) {
	q |= (1U << xf.input_perm(2));
      }
      if ( v3 ) {
	q |= (1U << xf.input_perm(3));
      }
      ans |= (1U << q);
    }
  }
  if ( xf.output_inv() ) {
    ans ^= 0xFFFF;
  }
  return ans;
}

END_NONAMESPACE

// @brief 4入力関数の情報をセットアップする．
void
GenPat2::init_npn4rep()
{
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    ymuint32 fv = npn4rep[i];

    vector<FuncXform> tmp_list;
    hash_set<ymuint32> func_hash;
    for (ymuint p = 0; p < 768; ++ p) {
      NpnXform xf(npn4perm[p]);
      ymuint16 fv1 = xform_func4(fv, xf);
      if ( func_hash.count(fv1) == 0 ) {
	func_hash.insert(fv1);
	tmp_list.push_back(FuncXform(fv1, xf.data()));
      }
    }

    mNpnHash.insert(make_pair(fv, tmp_list));
  }
}

#if 0
// @brief NpnHandle を変換する(4入力版)
NpnHandle
GenPat2::xform4(NpnHandle handle,
		NpnXform xf)
{
  if ( handle.is_const1() || handle.is_const0() ) {
    if ( xf.output_inv() ) {
      handle = ~handle;
    }
    return handle;
  }

  NpnHandle handle1 = xf4_sub(handle, xf);
  if ( xf.output_inv() ) {
    handle1 = ~handle1;
  }
  return handle1;
}

// @brief xform4 の下請け関数
NpnHandle
GenPat2::xf4_sub(NpnHandle handle,
		NpnXform xf)
{
  NpnNode* node = handle.node();
  bool inv = handle.inv();

  if (node->is_input() ) {
    ymuint id = node->input_id();
    ymuint new_id = xf.input_perm(id);
    NpnHandle handle1 = mMgr.make_input(new_id);
    if ( inv ) {
      handle1 = ~handle1;
    }
    if ( xf.input_inv(id) ) {
      handle1 = ~handle1;
    }
    return handle1;
  }

  NpnHandle handle1 = xf4_sub(node->fanin0(), xf);
  NpnHandle handle2 = xf4_sub(node->fanin1(), xf);
  NpnHandle ans;
  if ( node->is_and() ) {
    ans = mMgr.make_and(handle1, handle2);
  }
  else {
    ans = mMgr.make_xor(handle1, handle2);
  }
  if ( inv ) {
    ans = ~ans;
  }
  return ans;
}
#endif

// @brief 関数ごとのレベルの上限をセットする．
void
GenPat2::calc_upper()
{
  ymuint np = 1U << 4;
  ymuint nf = 1U << np;

  mUpperLevel.resize(nf, 0xffff);

  mRemainFunc = nf - 2;

  // レベル0の関数を求める．
  mFuncListArray.push_back(vector<ymuint16>());
  for (ymuint i = 0; i < 4; ++ i) {
    ymuint16 fv;
    switch ( i ) {
    case 0: fv = 0xaaaa; break;
    case 1: fv = 0xcccc; break;
    case 2: fv = 0xf0f0; break;
    case 3: fv = 0xff00; break;
    }
    add_func(fv, 0);
  }

  // レベル1以上の関数を求める．
  ymuint max_level = 0;
  for (ymuint level = 1; mRemainFunc > 0; ++ level) {
    cout << endl
	 << "level = " << level << ", remain func = " << mRemainFunc << endl;
    max_level = level;
    mFuncListArray.push_back(vector<ymuint16>());
    ymuint h = (level + 1) / 2;
    for (ymuint llevel = 0; llevel < h; ++ llevel) {
      ymuint rlevel = level - llevel - 1;
      cout << "  l-level = " << llevel
	   << ", r-level = " << rlevel << endl;
      if ( llevel == rlevel ) {
	const vector<ymuint16>& func_list = mFuncListArray[llevel];
	ymuint n = func_list.size();
	for (ymuint l = 0; l < n; ++ l) {
	  ymuint16 func1 = func_list[l];
	  for (ymuint r = l + 1; r < n; ++ r) {
	    ymuint16 func2 = func_list[r];
	    add_func(func1 & func2, level);
	    add_func(func1 ^ func2, level);
	  }
	}
      }
      else {
	const vector<ymuint16>& l_func_list = mFuncListArray[llevel];
	const vector<ymuint16>& r_func_list = mFuncListArray[rlevel];
	ymuint nl = l_func_list.size();
	ymuint nr = r_func_list.size();
	for (ymuint l = 0; l < nl; ++ l) {
	  ymuint16 func1 = l_func_list[l];
	  for (ymuint r = 0; r < nr; ++ r) {
	    ymuint16 func2 = r_func_list[r];
	    add_func(func1 & func2, level);
	    add_func(func1 ^ func2, level);
	  }
	}
      }
    }
  }

  bool error = false;
  for (ymuint p = 1; p < nf - 1; ++ p) {
    if ( mUpperLevel[p] == 0xFFFF ) {
      cout << "No patterns for function["
	   << setw(4) << setfill('0') << hex << p << dec << "]" << endl;
      error = true;
    }
    else {
      mFuncLevel[p] = mUpperLevel[p];
    }
  }
}

void
GenPat2::add_func(ymuint16 func,
		  ymuint level)
{
  if ( func == 0U || func == 0xFFFF ) {
    return;
  }

  if ( mUpperLevel[func] == 0xFFFF ) {
    mUpperLevel[func] = level;
    mFuncListArray[level].push_back(func);
    -- mRemainFunc;
  }
  ymuint16 func_n = func ^ 0xFFFF;
  if ( mUpperLevel[func_n] == 0xFFFF ) {
    mUpperLevel[func_n] = level;
    mFuncListArray[level].push_back(func_n);
    -- mRemainFunc;
  }
}

END_NAMESPACE_YM
