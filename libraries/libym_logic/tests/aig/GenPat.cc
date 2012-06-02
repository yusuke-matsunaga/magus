
/// @file GenPat.cc
/// @brief GenPat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenPat.h"


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
// クラス GenPat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenPat::GenPat()
{
  for (ymuint i = 0; npn3rep[i] != 0xFF; ++ i) {
    mNpn3Hash.insert(npn3rep[i]);
  }
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    mNpn4Hash.insert(npn4rep[i]);
  }
}

// @brief デストラクタ
GenPat::~GenPat()
{
}

// @brief ni 入力の全ての関数のパタンを求める．
void
GenPat::operator()(ymuint ni,
		   bool allow_xor)
{
  mFuncTable.clear();
  mFuncListArray.clear();
  mLogExprTable.clear();

  mAllowXor = allow_xor;

  mNi = ni;
  mNp = 1U << ni;
  ymuint nf = 1U << mNp;
  mMask = nf - 1U;
  mFuncTable.resize(nf, PatList());
  mLogExprTable.resize(nf);

  mRemainFunc = nf;
  // レベル0のパタンを作る．
  {
    mFuncListArray.push_back(vector<ymuint32>());
    ymuint32 fv0 = 0U;
    add_pat(fv0, 0, 0, fv0, 0);
    mLogExprTable[fv0].push_back(LogExpr::make_zero());
    ymuint32 fv1 = ~fv0 & mMask;
    mLogExprTable[fv1].push_back(LogExpr::make_one());
  }

  // レベル1のパタンを作る．
  mFuncListArray.push_back(vector<ymuint32>());
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
    add_pat(fv, 0, 0, fv, 1);
    mLogExprTable[fv].push_back(LogExpr::make_posiliteral(VarId(i)));
    ymuint32 fv_n = ~fv & mMask;
    mLogExprTable[fv_n].push_back(LogExpr::make_negaliteral(VarId(i)));
  }

  // レベル2以上のパタンを作る．
  ymuint max_level = 0;
  for (ymuint level = 2; mRemainFunc > 0; ++ level) {
    cout << endl
	 << "level = " << level << ", remain_func = " << mRemainFunc << endl;
    max_level = level;
    mFuncListArray.push_back(vector<ymuint32>());
    ymuint h = level / 2;
    for (ymuint llevel = 1; llevel <= h; ++ llevel) {
      ymuint rlevel = level - llevel;
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
	    compose(func1, func2, level);
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
	    compose(func1, func2, level);
	  }
	}
      }
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
  for (ymuint i = 0; i < nf; ++ i) {
    if ( mFuncTable[i].mList.empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += mFuncTable[i].mMinLevel;
  }
  cout << "total cost = " << total_cost << endl
       << endl;
}

// @brief 2つの関数から新しいパタンを合成する．
void
GenPat::compose(ymuint32 func1,
		ymuint32 func2,
		ymuint level)
{
  // AND 合成
  add_pat(func1 & func2, func1, func2, 0x08, level);

  if ( mAllowXor ) {
    // XOR 合成
    add_pat(func1 ^ func2, func1, func2, 0x06, level);
  }
}

/// @brief パタンを登録する．
/// @param[in] fv 関数ベクタ
/// @param[in] f0 左の子供の関数ベクタ
/// @param[in] f1 右の子供の関数ベクタ
/// @param[in] tv 自分自身の関数ベクタ
/// @param[in] level レベル
void
GenPat::add_pat(ymuint32 fv,
		ymuint32 f0,
		ymuint32 f1,
		ymuint32 tv,
		ymuint32 level)
{
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

END_NAMESPACE_YM
