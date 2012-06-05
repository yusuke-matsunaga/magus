
/// @file GenAig.cc
/// @brief GenAig の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenAig.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

struct Npn3Cannon
{
  ymuint32 mFunc;
  ymuint8 mPerm[4];
};

struct Npn4Cannon
{
  ymuint32 mFunc;
  ymuint8 mPerm[5];
};

// 3入力のNPN同値類代表関数
ymuint32 npn3rep[] = {
#include "npn3rep.h"
};

// 3入力のNPN変換を表す配列
ymuint8 npn3perm[][4] = {
#include "npn3perm.h"
};

// 3入力のNPN同値類代表関数への変換を表す配列
Npn3Cannon npn3cannon[] = {
#include "npn3cannon.h"
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint8 npn4perm[][5] = {
#include "npn4perm.h"
};

// 4入力のNPN同値類代表関数への変換を表す配列
Npn4Cannon npn4cannon[] = {
#include "npn4cannon.h"
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

  aig_mode(slack);
}

// @brief AIG モード
void
GenAig::aig_mode(ymuint slack)
{
  mAigList.clear();
  mRepAigList.clear();
  mCandListArray.clear();
  mFuncArray.clear();
  mFuncLevel.clear();
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
    add_cand(iaig, fv, 0);
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 0; level < mCandListArray.size(); ++ level ) {
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

    if ( mRemainFunc == 0 ) {
      cout << "All functions has its patterns" << endl;
      break;
    }

    // level の代表パタンと他のパタンとの対を作る．
    const vector<AigPat>& src_list1 = mRepAigList[level];
    ymuint n1 = src_list1.size();
    for (ymuint i = 0; i < n1; ++ i) {
      const AigPat& aigpat1 = src_list1[i];

      mCountHash.clear();
      ymuint level_base = count1(aigpat1.mAig);

      for (ymuint l = 0; l < level; ++ l) {
	const vector<AigPat>& src_list2 = mAigList[l];
	ymuint n2 = src_list2.size();
	for (ymuint j = 0; j < n2; ++ j) {
	  const AigPat& aigpat2 = src_list2[j];
	  cout << "L#" << l << ": " << j << " / " << n2
	       << " | " << i << " / " << n1 << endl;
	  compose(aigpat1, aigpat2, level_base);
	}
      }
      const vector<AigPat>& src_list2 = mAigList[level];
      ymuint n2 = src_list2.size();
      for (ymuint j = 0; j < n2; ++ j) {
	cout << "L#" << level << ": " << j << " / " << n2
	     << " | " << i << " / " << n1 << endl;
	const AigPat& aigpat2 = src_list2[j];
	compose(aigpat1, aigpat2, level_base);
      }
    }
  }

  ymuint total_cost = 0;
  bool error = false;
  for (ymuint i = 1; i < mNf - 1; ++ i) {
    if ( mFuncArray[i].empty() ) {
      cout << "No patterns for function["
	   << setw(mNp / 4) << setfill('0') << hex << i << dec << "]" << endl;
      error = true;
    }
    else {
      total_cost += mFuncLevel[i];
    }
  }
  if ( error ) {
    cout << "Error occured" << endl;
  }
  else {
    cout << "total cost = " << total_cost << endl
	 << endl;
  }
}

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
  const Npn3Cannon& cannon = npn3cannon[func];
  ymuint32 cfunc = cannon.mFunc;
  cperm[0] = cannon.mPerm[0];
  cperm[1] = cannon.mPerm[1];
  cperm[2] = cannon.mPerm[2];
  cperm[3] = cannon.mPerm[3];
  return cfunc;
}

// @brief 関数ベクタを代表関数に変換する(4入力版)
ymuint32
GenAig::cannonical4(ymuint32 func,
		    ymuint8 cperm[])
{
  const Npn4Cannon& cannon = npn4cannon[func];
  ymuint32 cfunc = cannon.mFunc;
  cperm[0] = cannon.mPerm[0];
  cperm[1] = cannon.mPerm[1];
  cperm[2] = cannon.mPerm[2];
  cperm[3] = cannon.mPerm[3];
  cperm[4] = cannon.mPerm[4];
  return cfunc;
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
  }
  mFuncArray[fv].push_back(aigpat);

  while ( mAigList.size() <= level ) {
    mAigList.push_back(vector<AigPat>());
    mRepAigList.push_back(vector<AigPat>());
  }
  mAigList[level].push_back(aigpat);
  if ( mNpnHash.count(fv) > 0 ) {
    mRepAigList[level].push_back(aigpat);
  }
}

BEGIN_NONAMESPACE

#if 1
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
#endif

END_NONAMESPACE

// @brief 2つのAIGから新しいパタンを作る．
// @note 具体的には aig1 & aig2 と ~aig & aig
void
GenAig::compose(AigPat aigpat1,
		AigPat aigpat2,
		ymuint level_base)
{
  ymuint32 fv1 = aigpat1.mFunc;
  ymuint32 fv2 = aigpat2.mFunc;
  ymuint32 fv3 = fv1 & fv2;
  ymuint32 fv1_n = ~fv1 & mMask;
  ymuint32 fv4 = fv1_n & fv2;

  bool valid1 = false;
  if ( fv3 != 0U && fv3 != fv1 && fv3 != fv2 ) {
    valid1 = true;
  }
  bool valid2 = false;
  if ( fv4 != 0U && fv4 != fv1_n && fv4 != fv2 ) {
    valid2 = true;
  }

  if ( !valid1 && !valid2 ) {
    return;
  }

  Aig aig1 = aigpat1.mAig;
  Aig aig2 = aigpat2.mAig;

  ymuint level = count2(aig2) + level_base + 1;

  if ( valid1 ) {
    add_aigpair(aig1, aig2, fv3, level);
  }

  if ( valid2 ) {
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

  if ( mAigHash.count(caig) == 0 ) {
    mAigHash.insert(caig);
    add_cand(caig, cfunc, level);
  }
}

// @brief 候補のリストに追加する．
void
GenAig::add_cand(Aig aig,
		 ymuint32 func,
		 ymuint level)
{
  AigPat aigpat(aig, func, level);
#if 0
  bool stat = sanity_check(aigpat);
  assert_cond(stat, __FILE__, __LINE__);
#endif
  while ( mCandListArray.size() <= level ) {
    mCandListArray.push_back(vector<AigPat>());
  }
  mCandListArray[level].push_back(aigpat);
}

// @brief aig の子供に印をつけてノード数を数える．
ymuint
GenAig::count1(Aig aig)
{
  if ( aig.inv() ) {
    aig = ~aig;
  }

  if ( mCountHash.count(aig) > 0 ) {
    return 0;
  }

  mCountHash.insert(aig);
  ymuint ans = 0;
  if ( aig.is_and() ) {
    ans = 1;
    ans += count1(aig.fanin0());
    ans += count1(aig.fanin1());
  }
  return ans;
}

// @brief count1 で印のついていないノード数を数える．
ymuint
GenAig::count2(Aig aig)
{
  hash_set<Aig> hash1;
  return count2_sub(aig, hash1);
}

// @brief count2 の下請け関数
ymuint
GenAig::count2_sub(Aig aig,
		   hash_set<Aig>& hash)
{
  if ( aig.inv() ) {
    aig = ~aig;
  }

  if ( mCountHash.count(aig) > 0 ||
       hash.count(aig) > 0 ) {
    return 0;
  }

  hash.insert(aig);
  ymuint ans = 0;
  if ( aig.is_and() ) {
    ans = 1;
    ans += count2_sub(aig.fanin0(), hash);
    ans += count2_sub(aig.fanin1(), hash);
  }
  return ans;
}

BEGIN_NONAMESPACE

ymuint32
aig_to_func(Aig aig)
{
  ymuint32 func = 0U;
  if ( aig.is_input() ) {
    ymuint id = aig.input_id().val();
    switch ( id ) {
    case 0: func = 0xaaaa; break;
    case 1: func = 0xcccc; break;
    case 2: func = 0xf0f0; break;
    case 3: func = 0xff00; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }
  else if ( aig.is_and() ) {
    ymuint32 func0 = aig_to_func(aig.fanin0());
    ymuint32 func1 = aig_to_func(aig.fanin1());
    func = func0 & func1;
  }
  else if ( aig.is_const() ) {
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( aig.inv() ) {
    func = ~func;
  }
  return func;
}

END_NONAMESPACE

// @brief AigPat の内容が正しいか調べる．
bool
GenAig::sanity_check(AigPat aigpat)
{
  ymuint32 func = aig_to_func(aigpat.mAig) & mMask;
  if ( aigpat.mFunc != func ) {
    cout << "Error:  aigpat.mAig != aigpat.mFunc" << endl;
    return false;
  }
  hash_set<Aig> aig_hash;
  ymuint level = count_sub(aigpat.mAig, aig_hash);
  if ( aigpat.mLevel != level ) {
    cout << "Error: level of aigpat.mAig != aigpat.mLevel" << endl;
    return false;
  }
  return true;
}

END_NAMESPACE_YM
