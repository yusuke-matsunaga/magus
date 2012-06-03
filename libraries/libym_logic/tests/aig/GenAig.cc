
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

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 5入力のNPN同値類代表関数
ymuint32 npn5rep[] = {
#if 0
#include "npn5rep.h"
#else
  0xacb00e68U,
  0xFFFFFFFFU
#endif
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GenAig
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenAig::GenAig()
{
  for (ymuint i = 0; npn3rep[i] != 0xFF; ++ i) {
    mNpn3Hash.insert(npn3rep[i]);
  }
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    mNpn4Hash.insert(npn4rep[i]);
  }
}

// @brief デストラクタ
GenAig::~GenAig()
{
}

// @brief ni 入力の全ての関数のパタンを求める．
void
GenAig::operator()(ymuint ni)
{
  mFuncTable.clear();
  mAigPairListArray.clear();

  mNi = ni;
  mNp = 1U << ni;
  ymuint nf = 1U << mNp;
  mMask = nf - 1U;
  mFuncTable.resize(nf);

  mRemainFunc = nf - 2;

  // レベル0のパタンを作る．
  for (ymuint i = 0; i < ni; ++ i) {
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
    Aig iaig = mMgr.make_input(VarId(i));
    add_pat(fv, iaig, 0);
  }

  // レベル1以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 1; mRemainFunc > 0; ++ level) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl;
    max_level = level;
    ymuint n = mAigPairListArray[level].size();
    for (ymuint i = 0; i < n; ++ i) {
      const AigPair& aigpair = mAigPairListArray[level][i];
      Aig aig1 = aigpair.mAig1;
      Aig aig2 = aigpair.mAig2;
      Aig aig = mMgr.make_and(aig1, aig2);
      add_pat(aigpair.mFunc, aig, level);
    }
  }

#if 0
  for (ymuint level = 2; level <= max_level; ++ level) {
    const vector<ymuint32>& func_list = mFuncListArray[level];
    for (vector<ymuint32>::const_iterator p = func_list.begin();
	 p != func_list.end(); ++ p) {
      ymuint32 func = *p;
      cout << "func = [" << hex << func << dec << "]" << endl;
      const vector<Pat>& pat_list = mFuncTable[func].mList;
      for (vector<Pat>::const_iterator q = pat_list.begin();
	   q != pat_list.end(); ++ q) {
	const Pat& pat = *q;
	ymuint32 f0 = pat.mF0;
	ymuint32 f1 = pat.mF1;
	const vector<LogExpr>& ff_list0 = mLogExprTable[f0];
	const vector<LogExpr>& ff_list1 = mLogExprTable[f1];
	vector<LogExpr>& dst_list = mLogExprTable[func];
	for (vector<LogExpr>::const_iterator r0 = ff_list0.begin();
	     r0 != ff_list0.end(); ++ r0) {
	  LogExpr ff0 = *r0;
	  for (vector<LogExpr>::const_iterator r1 = ff_list1.begin();
	       r1 != ff_list1.end(); ++ r1) {
	    LogExpr ff1 = *r1;
	    LogExpr ff;
	    switch ( pat.mTv ) {
	    case 0x06: ff = ff0 ^ ff1; break;
	    case 0x07: ff = ~(ff0 & ff1); break;
	    case 0x08: ff = ff0 & ff1; break;
	    case 0x09: ff = ~ff0 ^ ff1; break;
	    case 0x00:
	    case 0x01:
	    case 0x02:
	    case 0x03:
	    case 0x04:
	    case 0x05:
	    case 0x0A:
	    case 0x0B:
	    case 0x0C:
	    case 0x0D:
	    case 0x0E:
	    case 0x0F: assert_not_reached(__FILE__, __LINE__);
	    }
	    dst_list.push_back(ff);
#if 0
	    cout << "  " << ff << endl;
	    cout << "    ff0 = " << ff0 << " [" << hex << f0 << dec << "]" << endl
		 << "    ff1 = " << ff1 << " [" << hex << f1 << dec << "]" << endl;
#endif
	  }
	}
#if 0
	cout << endl;
#endif
      }
    }
  }
#endif

  ymuint total_cost = 0;
  for (ymuint i = 1; i < nf - 1; ++ i) {
    if ( mFuncTable[i].mList.empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += mFuncTable[i].mMinLevel;
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

// @brief 2つの関数から新しいパタンを合成する．
void
GenAig::compose(ymuint32 func1,
		ymuint32 func2,
		ymuint level)
{
  ymuint32 func = func1 & func2;
  if ( mFuncTable[func].mMinLevel < level ) {
    return;
  }

  const AigList& aig_list1 = mFuncTable[func1];
  const AigList& aig_list2 = mFuncTable[func2];
  for (vector<Aig>::const_iterator p1 = aig_list1.mList.begin();
       p1 != aig_list1.mList.end(); ++ p1) {
    Aig aig1 = *p1;
    for (vector<Aig>::const_iterator p2 = aig_list2.mList.begin();
	 p2 != aig_list2.mList.end(); ++ p2) {
      Aig aig2 = *p2;
      ymuint level1 = count(aig1, aig2) + 1;
      if ( level1 != level ) {
	continue;
      }
      Aig aig = mMgr.make_and(aig1, aig2);
      add_pat(func, aig, level);
    }
  }
}

/// @brief パタンを登録する．
/// @param[in] fv 関数ベクタ
/// @param[in] aig AIG
/// @param[in] level レベル
void
GenAig::add_pat(ymuint32 fv,
		Aig aig,
		ymuint32 level)
{
  if ( mFuncTable[fv].mMinLevel < level ) {
    return;
  }
  add_pat1(fv, aig, level);
  ymuint32 fv_n = ~fv & mMask;
  add_pat1(fv_n, ~aig, level);
}

// @brief パタンを登録する．
// @param[in] fv 関数ベクタ
// @param[in] aig AIG
// @param[in] level レベル
void
GenAig::add_pat1(ymuint32 fv,
		 Aig aig,
		 ymuint32 level)
{
  if ( mFuncTable[fv].push_back(level, aig) ) {
    -- mRemainFunc;
  }

  ymuint nf = 1U << mNp;
  for (ymuint32 v = 0; v < nf; ++ v) {
    ymuint32 fv1 = fv & v;
    if ( fv1 == fv || fv1 == 0U ) {
      continue;
    }
    for (vector<Aig>::iterator p = mFuncTable[v].mList.begin();
	 p != mFuncTable[v].mList.end(); ++ p) {
      Aig aig1 = *p;
      add_aigpair(aig1, aig, fv1);
    }
  }
}

// @brief AIG の対を登録する．
// @note 結果は mAigPairListArray に追加される．
void
GenAig::add_aigpair(Aig aig1,
		    Aig aig2,
		    ymuint32 func)
{
  ymuint level = count(aig1, aig2) + 1;
  while ( mAigPairListArray.size() <= level ) {
    mAigPairListArray.push_back(vector<AigPair>());
  }
  Aig aig = mMgr.make_and(aig1, aig2);
  mAigPairListArray[level].push_back(AigPair(aig1, aig2, func, level));
}

END_NAMESPACE_YM
