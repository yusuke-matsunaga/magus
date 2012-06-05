
/// @file GenAig.cc
/// @brief GenAig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenAig.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 3入力のNPN同値類代表関数
ymuint32 npn3rep[] = {
#include "npn3rep.h"
};

// 3入力のNPN変換を表す配列
ymuint8 npn3perm[][4] = {
#include "npn3perm.h"
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint8 npn4perm[][5] = {
#include "npn4perm.h"
};

#if 0
// 5入力のNPN同値類代表関数
ymuint32 npn5rep[] = {
#include "npn5rep.h"
};
#endif

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GenAig
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenAig::GenAig()
{
}

// @brief デストラクタ
GenAig::~GenAig()
{
}

// @brief ni 入力の全ての関数のパタンを求める．
void
GenAig::operator()(ymuint ni,
		   ymuint slack)
{

  mNi = ni;
  mNp = 1U << ni;
  mNf = 1U << mNp;
  mMask = mNf - 1U;

  if ( mNi == 3 ) {
    for (ymuint i = 0; npn3rep[i] != 0xFF; ++ i) {
      mNpnHash.insert(npn3rep[i]);
    }
  }
  else if ( mNi == 4 ) {
    for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
      mNpnHash.insert(npn4rep[i]);
    }
  }

  ff_mode();

  aig_mode(slack);
}

// @brief FF モードの関数レベルを計算する．
void
GenAig::ff_mode()
{
  mFuncTable.clear();
  mFuncListArray.clear();

  mFuncTable.resize(mNf, PatList());

  mRemainFunc = mNf - 2;

  // レベル0のパタンを作る．
  mFuncListArray.push_back(vector<ymuint32>());
  for (ymuint i = 0; i < mNi; ++ i) {
    ymuint32 fv;
    switch ( i ) {
    case 0: fv = 0xaaaaaaaa; break;
    case 1: fv = 0xcccccccc; break;
    case 2: fv = 0xf0f0f0f0; break;
    case 3: fv = 0xff00ff00; break;
    case 4: fv = 0xffff0000; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }

    fv &= mMask;
    add_pat(fv, 0, 0, fv, 0);
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 1; mRemainFunc > 0; ++ level) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl;
    max_level = level;
    mFuncListArray.push_back(vector<ymuint32>());
    ymuint h = (level + 1) / 2;
    for (ymuint llevel = 0; llevel < h; ++ llevel) {
      ymuint rlevel = level - llevel - 1;
      cout << "  l-level = " << llevel
	   << ", r-level = " << rlevel << endl;
      // llevel 左の子供のレベル
      // rlevel 右の子供のレベル
      if ( llevel == rlevel ) {
	const vector<ymuint32>& func_list = mFuncListArray[llevel];
	ymuint n = func_list.size();
	for (ymuint l = 0; l < n; ++ l) {
	  ymuint32 func1 = func_list[l];
	  for (ymuint r = l + 1; r < n; ++ r) {
	    ymuint32 func2 = func_list[r];
	    add_pat(func1 & func2, func1, func2, 0x08, level);
	  }
	}
      }
      else {
	const vector<ymuint32>& l_func_list = mFuncListArray[llevel];
	const vector<ymuint32>& r_func_list = mFuncListArray[rlevel];
	ymuint nl = l_func_list.size();
	ymuint nr = r_func_list.size();
	for (ymuint l = 0; l < nl; ++ l) {
	  ymuint32 func1 = l_func_list[l];
	  for (ymuint r = 0; r < nr; ++ r) {
	    ymuint32 func2 = r_func_list[r];
	    add_pat(func1 & func2, func1, func2, 0x08, level);
	  }
	}
      }
    }
  }

  ymuint total_cost = 0;
  for (ymuint i = 1; i < mNf - 1; ++ i) {
    if ( mFuncTable[i].mList.empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += mFuncTable[i].mMinLevel;
  }
  cout << "total cost = " << total_cost << endl
       << endl;
}

/// @brief パタンを登録する．
/// @param[in] fv 関数ベクタ
/// @param[in] f0 左の子供の関数ベクタ
/// @param[in] f1 右の子供の関数ベクタ
/// @param[in] tv 自分自身の関数ベクタ
/// @param[in] level レベル
void
GenAig::add_pat(ymuint32 fv,
		ymuint32 f0,
		ymuint32 f1,
		ymuint32 tv,
		ymuint32 level)
{
  if ( fv == 0U || (~fv & mMask) == 0U ) {
    return;
  }

  if ( mFuncTable[fv].push_back(level, f0, f1, tv) ) {
    mFuncListArray[level].push_back(fv);
    -- mRemainFunc;
#if 0
    cout << "[" << hex << fv << dec << "]: "
	 << "fv1 = " << hex << f0 << dec
	 << ", fv2 = " << hex << f1 << dec
	 << ", tv  = " << tv << endl;
#endif
  }
  ymuint32 fv_n = ~fv & mMask;
  if ( mFuncTable[fv_n].push_back(level, f0, f1, 15 - tv) ) {
    mFuncListArray[level].push_back(fv_n);
    -- mRemainFunc;
#if 0
    cout << "[" << hex << fv_n << dec << "]: "
	 << "fv1 = " << hex << f0 << dec
	 << ", fv2 = " << hex << f1 << dec
	 << ", tv  = " << (15 - tv) << endl;
#endif
  }
}

// @brief AIG モード
void
GenAig::aig_mode(ymuint slack)
{
  mFuncArray.clear();
  mFuncLevel.clear();
  mAigList.clear();
  mAigHash.clear();

  mFuncArray.resize(mNf);
  mFuncLevel.resize(mNf);

  mSlack = slack;

  // 定数0と定数1は除く
  mRemainFunc = mNf - 2;

  // レベル0のパタンを作る．
  {
    ymuint32 fv = 0xaaaaaaaa & mMask;
    Aig iaig = mMgr.make_input(VarId(0));
    mCandListArray.push_back(vector<AigPat>());
    mCandListArray[0].push_back(AigPat(iaig, fv, 0));
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 0;
       level < mCandListArray.size() && mRemainFunc > 0;
       ++ level) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl;
    max_level = level;
    ymuint n = mCandListArray[level].size();
    cout << "  " << n << " seed patterns" << endl;
    for (ymuint i = 0; i < n; ++ i) {
      const AigPat& aigpat = mCandListArray[level][i];
      npn_expand(aigpat.mFunc, aigpat.mAig, level);
    }
    cout << "  expand " << mAigList[level].size() << " patterns" << endl;

    // level の代表パタンと他のパタンとの対を作る．
    const vector<AigPat>& src_list1 = mAigList[level];
    for (vector<AigPat>::const_iterator p = src_list1.begin();
	 p != src_list1.end(); ++ p) {
      const AigPat& aigpat1 = *p;
      ymuint32 func1 = aigpat1.mFunc;
      if ( mNpnHash.count(func1) == 0 ) {
	// 代表関数以外はスキップ
	continue;
      }
      for (ymuint l = 0; l <= level; ++ l) {
	const vector<AigPat>& src_list2 = mAigList[l];
	for (vector<AigPat>::const_iterator q = src_list2.begin();
	     q != src_list2.end(); ++ q) {
	  const AigPat& aigpat2 = *q;
	  compose(aigpat1, aigpat2);
	}
      }
    }
  }

  ymuint total_cost = 0;
  for (ymuint i = 1; i < mNf - 1; ++ i) {
    if ( mFuncArray[i].empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += mFuncLevel[i];
  }
  cout << "total cost = " << total_cost << endl
       << endl;
}

BEGIN_NONAMESPACE

ymuint
count_sub(Aig aig,
	  hash_set<Aig>& aig_hash)
{
  if ( aig.inv() ) {
    aig = ~aig;
  }

  ymuint ans = 0;
  if ( aig_hash.count(aig) == 0 ) {
    aig_hash.insert(aig);
    if ( aig.is_and() ) {
      ans = 1;
      ans += count_sub(aig.fanin0(), aig_hash);
      ans += count_sub(aig.fanin1(), aig_hash);
    }
  }
  return ans;
}

ymuint
count(Aig aig1,
      Aig aig2)
{
  hash_set<Aig> aig_hash;
  ymuint ans;
  ans = count_sub(aig1, aig_hash);
  ans += count_sub(aig2, aig_hash);
  return ans;
}

END_NONAMESPACE

// @brief NPN同値類を求める．
// @param[in] fv 関数ベクタ
// @param[in] aig AIG
// @param[in] level レベル
void
GenAig::npn_expand(ymuint32 fv,
		   Aig aig,
		   ymuint32 level)
{
  hash_set<ymuint32> func_hash;
  if ( mNi == 3 ) {
    for (ymuint p = 0; p < 96; ++ p) {
      ymuint8* perm = npn3perm[p];
      ymuint32 fv1 = xform_func3(fv, perm);
      if ( func_hash.count(fv1) == 0 ) {
	func_hash.insert(fv1);
	Aig aig1 = xform3(aig, perm);
	if ( 0 ) {
	  cout << endl
	       << "perm[0] = " << static_cast<ymuint>(perm[0]) << endl
	       << "perm[1] = " << static_cast<ymuint>(perm[1]) << endl
	       << "perm[2] = " << static_cast<ymuint>(perm[2]) << endl
	       << "perm[3] = " << static_cast<ymuint>(perm[3]) << endl;
	  cout << "func(" << hex << fv << dec << ") = "
	       << hex << fv1 << dec << endl;
	  cout << "xform3(" << aig << ") = " << aig1 << endl;
	  list<Aig> tmp_list;
	  tmp_list.push_back(aig);
	  tmp_list.push_back(aig1);
	  mMgr.print_handles(cout, tmp_list);
	  cout << endl;
	}
	add_pat(aig1, fv1, level);
      }
    }
  }
  else if ( mNi == 4 ) {
    for (ymuint p = 0; p < 768; ++ p) {
      ymuint8* perm = npn4perm[p];
      ymuint32 fv1 = xform_func4(fv, perm);
      if ( func_hash.count(fv1) == 0 ) {
	func_hash.insert(fv1);
	Aig aig1 = xform4(aig, perm);
	add_pat(aig1, fv1, level);
      }
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief 関数ベクタを代表関数に変換する(3入力版)
ymuint32
GenAig::cannonical3(ymuint32 func,
		    ymuint8 cperm[])
{
  // ベタなやり方
  for (ymuint p = 0; p < 96; ++ p) {
    ymuint8* perm = npn3perm[p];
    ymuint32 fv1 = xform_func3(func, perm);
    if ( mNpnHash.count(fv1) > 0 ) {
      cperm[0] = perm[0];
      cperm[1] = perm[1];
      cperm[2] = perm[2];
      cperm[3] = perm[3];
      return fv1;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 関数ベクタを代表関数に変換する(4入力版)
ymuint32
GenAig::cannonical4(ymuint32 func,
		    ymuint8 cperm[])
{
  // ベタなやり方
  for (ymuint p = 0; p < 768; ++ p) {
    ymuint8* perm = npn4perm[p];
    ymuint32 fv1 = xform_func4(func, perm);
    if ( mNpnHash.count(fv1) > 0 ) {
      cperm[0] = perm[0];
      cperm[1] = perm[1];
      cperm[2] = perm[2];
      cperm[3] = perm[3];
      cperm[4] = perm[4];
      return fv1;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 関数ベクタを変換する(3入力版)
ymuint32
GenAig::xform_func3(ymuint32 fv,
		    const ymuint8 perm[])
{
  ymuint32 ans = 0U;
  ymuint8 pols = perm[3];
  for (ymuint p = 0; p < 8; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      if ( pols & 1U ) {
	v0 ^= 1U;
      }
      if ( pols & 2U ) {
	v1 ^= 1U;
      }
      if ( pols & 4U ) {
	v2 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << perm[0]);
      }
      if ( v1 ) {
	q |= (1U << perm[1]);
      }
      if ( v2 ) {
	q |= (1U << perm[2]);
      }
      ans |= (1U << q);
    }
  }
  if ( pols & 8U ) {
    ans = ~ans & mMask;
  }
  return ans;
}

// @brief 関数ベクタを変換する(4入力版)
ymuint32
GenAig::xform_func4(ymuint32 fv,
		    const ymuint8 perm[])
{
  ymuint32 ans = 0U;
  ymuint8 pols = perm[4];
  for (ymuint p = 0; p < 16; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      ymuint v3 = (p >> 3) & 1U;
      if ( pols & 1U ) {
	v0 ^= 1U;
      }
      if ( pols & 2U ) {
	v1 ^= 1U;
      }
      if ( pols & 4U ) {
	v2 ^= 1U;
      }
      if ( pols & 8U ) {
	v3 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << perm[0]);
      }
      if ( v1 ) {
	q |= (1U << perm[1]);
      }
      if ( v2 ) {
	q |= (1U << perm[2]);
      }
      if ( v3 ) {
	q |= (1U << perm[3]);
      }
      ans |= (1U << q);
    }
  }
  if ( pols & 16U ) {
    ans = ~ans & mMask;
  }
  return ans;
}

// @brief AIG を変換する(3入力版)
Aig
GenAig::xform3(Aig aig,
	       const ymuint8 perm[])
{
  Aig aig1 = xf3_sub(aig, perm);
  if ( perm[3] & 8U ) {
    aig1 = ~aig1;
  }
  return aig1;
}

// @brief xform3 の下請け関数
Aig
GenAig::xf3_sub(Aig aig,
		const ymuint8 perm[])
{
  if ( aig.is_input() ) {
    VarId vid = aig.input_id();
    ymuint id = vid.val();
    VarId new_vid(perm[id]);
    Aig aig1 = mMgr.make_input(new_vid);
    if ( aig.inv() ) {
      aig1 = ~aig1;
    }
    if ( perm[3] & (1U << id) ) {
      aig1 = ~aig1;
    }
    return aig1;
  }
  if ( aig.is_and() ) {
    Aig aig1 = xf3_sub(aig.fanin0(), perm);
    Aig aig2 = xf3_sub(aig.fanin1(), perm);
    Aig ans = mMgr.make_and(aig1, aig2);
    if ( aig.inv() ) {
      ans = ~ans;
    }
    return ans;
  }
  cout << "Error : aig is constant." << endl;
  assert_not_reached(__FILE__, __LINE__);
  return Aig();
}

// @brief AIG を変換する(4入力版)
Aig
GenAig::xform4(Aig aig,
	       const ymuint8 perm[])
{
  Aig aig1 = xf4_sub(aig, perm);
  if ( perm[4] & 16U ) {
    aig1 = ~aig1;
  }
  return aig1;
}

// @brief xform4 の下請け関数
Aig
GenAig::xf4_sub(Aig aig,
		const ymuint8 perm[])
{
  if ( aig.is_input() ) {
    VarId vid = aig.input_id();
    ymuint id = vid.val();
    VarId new_vid(perm[id]);
    Aig aig1 = mMgr.make_input(new_vid);
    if ( aig.inv() ) {
      aig1 = ~aig1;
    }
    if ( perm[4] & (1U << id) ) {
      aig1 = ~aig1;
    }
    return aig1;
  }
  if ( aig.is_and() ) {
    Aig aig1 = xf4_sub(aig.fanin0(), perm);
    Aig aig2 = xf4_sub(aig.fanin1(), perm);
    Aig ans = mMgr.make_and(aig1, aig2);
    if ( aig.inv() ) {
      ans = ~ans;
    }
    return ans;
  }
  cout << "Error : aig is constant." << endl;
  assert_not_reached(__FILE__, __LINE__);
  return Aig();
}

// @brief パタンを登録する．
// @param[in] fv 関数ベクタ
// @param[in] aig AIG
// @param[in] level レベル
void
GenAig::add_pat(Aig aig,
		ymuint32 fv,
		ymuint32 level)
{
  AigPat aigpat(aig, fv, level);

  if ( mFuncArray[fv].empty() ) {
    mFuncLevel[fv] = level;
    -- mRemainFunc;
    cout << "add_pat(" << hex << aigpat.mFunc
	 << dec << ", Aig(" << aigpat.mAig
	 << "), Level#" << level
	 << ")" << endl;
  }
  mFuncArray[fv].push_back(aigpat);

  while ( mAigList.size() <= level ) {
    mAigList.push_back(vector<AigPat>());
  }
  mAigList[level].push_back(aigpat);
  if ( 0 ) {
    cout << "add_pat(" << hex << aigpat.mFunc
	 << dec << ", Aig(" << aigpat.mAig
	 << "), Level#" << level
	 << ")" << endl;
  }
}

// @brief 2つのAIGから新しいパタンを作る．
// @note 具体的には aig1 & aig2 と ~aig & aig
void
GenAig::compose(AigPat aigpat1,
		AigPat aigpat2)
{
  ymuint32 fv1 = aigpat1.mFunc;
  ymuint32 fv2 = aigpat2.mFunc;

  Aig aig1 = aigpat1.mAig;
  Aig aig2 = aigpat2.mAig;

  ymuint level = count(aig1, aig2) + 1;

  ymuint32 fv3 = fv1 & fv2;
  if ( fv3 != 0U && fv3 != fv1 && fv3 != fv2 ) {
    add_aigpair(aig1, aig2, fv3, level);
  }

  ymuint32 fv1_n = ~fv1 & mMask;
  ymuint32 fv4 = fv1_n & fv2;
  if ( fv4 != 0U && fv4 != fv1_n && fv4 != fv2 ) {
    add_aigpair(~aig1, aig2, fv4, level);
  }
}

// @brief AIG の対を登録する．
// @note 結果は mCandListArray に追加される．
void
GenAig::add_aigpair(Aig aig1,
		    Aig aig2,
		    ymuint32 func,
		    ymuint level)
{
  if ( !mFuncArray[func].empty() && (mFuncLevel[func] + mSlack) < level ) {
    return;
  }
  while ( mCandListArray.size() <= level ) {
    mCandListArray.push_back(vector<AigPat>());
  }

  Aig aig = mMgr.make_and(aig1, aig2);

  ymuint32 cfunc;
  Aig caig;
  if ( mNi == 3 ) {
    ymuint8 cperm[4];
    cfunc = cannonical3(func, cperm);
    caig = xform3(aig, cperm);
  }
  else {
    ymuint8 cperm[5];
    cfunc = cannonical4(func, cperm);
    caig = xform4(aig, cperm);
  }

  if ( mAigHash.count(caig) > 0 ) {
    return;
  }
  mAigHash.insert(caig);
  mCandListArray[level].push_back(AigPat(caig, cfunc, level));
}


//////////////////////////////////////////////////////////////////////
// クラス AigPat
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
AigPat::AigPat()
{
}

// コンストラクタ
AigPat::AigPat(Aig aig,
	       ymuint32 func,
	       ymuint level) :
  mAig(aig),
  mFunc(func),
  mLevel(level)
{

}

END_NAMESPACE_YM
