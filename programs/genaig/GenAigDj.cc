
/// @file GenAigDj.cc
/// @brief GenAigDj の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GenAigDj.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenAigDj
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GenAigDj::GenAigDj()
{
}

// @brief デストラクタ
GenAigDj::~GenAigDj()
{
}

// @brief ni 入力の全ての関数のパタンを求める．
void
GenAigDj::operator()(ymuint ni)
{

  mNi = ni;
  mNp = 1U << ni;
  mNf = 1U << mNp;
  mMask = mNf - 1U;

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
  bool error = false;
  for (ymuint i = 1; i < mNf - 1; ++ i) {
    if ( mFuncTable[i].mList.empty() ) {
      cout << "No patterns for function["
	   << setw(mNp / 4) << setfill('0') << hex << i << dec << "]" << endl;
      error = true;
    }
    else {
      total_cost += mFuncTable[i].mMinLevel;
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

/// @brief パタンを登録する．
/// @param[in] fv 関数ベクタ
/// @param[in] f0 左の子供の関数ベクタ
/// @param[in] f1 右の子供の関数ベクタ
/// @param[in] tv 自分自身の関数ベクタ
/// @param[in] level レベル
void
GenAigDj::add_pat(ymuint32 fv,
		  ymuint32 f0,
		  ymuint32 f1,
		  ymuint32 tv,
		  ymuint32 level)
{
  ymuint32 fv_n = ~fv & mMask;
  if ( fv == 0U || fv_n == 0U ) {
    return;
  }

  if ( mFuncTable[fv].push_back(level, f0, f1, tv) ) {
    mFuncListArray[level].push_back(fv);
    -- mRemainFunc;
  }
  if ( mFuncTable[fv_n].push_back(level, f0, f1, 15 - tv) ) {
    mFuncListArray[level].push_back(fv_n);
    -- mRemainFunc;
  }
}

// @brief 関数ベクタに対応するレベルを返す．
ymuint
GenAigDj::level(ymuint32 func) const
{
  return mFuncTable[func].mMinLevel;
}

END_NAMESPACE_YM
