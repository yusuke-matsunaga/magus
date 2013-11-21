
/// @file GenAigNaive.cc
/// @brief GenAigNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenAigNaive.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenAigNaive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenAigNaive::GenAigNaive()
{
}

// @brief デストラクタ
GenAigNaive::~GenAigNaive()
{
}

// @brief ni 入力の全ての関数のパタンを求める．
void
GenAigNaive::operator()(ymuint ni,
			ymuint slack)
{

  mNi = ni;
  mNp = 1U << ni;
  mNf = 1U << mNp;
  mMask = mNf - 1U;

  aig_mode(slack);
}

// @brief AIG モード
void
GenAigNaive::aig_mode(ymuint slack)
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
  for (ymuint i = 0; i < mNi; ++ i) {
    ymuint32 fv = 0U;
    switch ( i ) {
    case 0: fv = 0xaaaa; break;
    case 1: fv = 0xcccc; break;
    case 2: fv = 0xf0f0; break;
    case 3: fv = 0xff00; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    fv &= mMask;
    Aig iaig = mMgr.make_input(VarId(i));
    add_cand(iaig, fv, 0);

    ymuint32 fv_n = ~fv & mMask;
    add_cand(~iaig, fv_n, 0);
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 0; level < mCandListArray.size(); ++ level ) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl;
    max_level = level;
    {
      const vector<AigPat>& aig_list = mCandListArray[level];
      ymuint n = aig_list.size();
      cout << "  " << n << " candidate patterns" << endl;
      for (ymuint i = 0; i < n; ++ i) {
	const AigPat& aigpat = aig_list[i];
	add_pat(aigpat.mAig, aigpat.mFunc, aigpat.mLevel);
      }
    }

    if ( mRemainFunc == 0 ) {
      cout << "All functions has its patterns" << endl;
      break;
    }

    // level の代表パタンと他のパタンとの対を作る．
    const vector<AigPat>& src_list1 = mAigList[level];
    ymuint n1 = src_list1.size();
    for (ymuint i = 0; i < n1; ++ i) {
      const AigPat& aigpat1 = src_list1[i];
      for (ymuint l = 0; l < level; ++ l) {
	const vector<AigPat>& src_list2 = mAigList[l];
	for (vector<AigPat>::const_iterator q = src_list2.begin();
	     q != src_list2.end(); ++ q) {
	  const AigPat& aigpat2 = *q;
	  compose(aigpat1, aigpat2);
	}
      }
      for (ymuint j = 0; j < i; ++ j) {
	const AigPat& aigpat2 = src_list1[j];
	compose(aigpat1, aigpat2);
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


// @brief パタンを登録する．
// @param[in] fv 関数ベクタ
// @param[in] aig AIG
// @param[in] level レベル
void
GenAigNaive::add_pat(Aig aig,
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
  }
  mAigList[level].push_back(aigpat);
}

// @brief 2つのAIGから新しいパタンを作る．
// @note 具体的には aig1 & aig2 と ~aig & aig
void
GenAigNaive::compose(AigPat aigpat1,
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
}

// @brief AIG の対を登録する．
// @note 結果は mCandListArray に追加される．
void
GenAigNaive::add_aigpair(Aig aig1,
		    Aig aig2,
		    ymuint32 func,
		    ymuint level)
{
  if ( !mFuncArray[func].empty() && (mFuncLevel[func] + mSlack) < level ) {
    return;
  }

  Aig aig = mMgr.make_and(aig1, aig2);

  add_cand(aig, func, level);
  add_cand(~aig, ~func & mMask, level);
}

// @brief 候補のリストに追加する．
void
GenAigNaive::add_cand(Aig aig,
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
GenAigNaive::sanity_check(AigPat aigpat)
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
