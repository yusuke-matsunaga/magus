
/// @file MaxCompat.cc
/// @brief MaxCompat の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MaxCompat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MaxCompat
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] tf_array 故障検出関数のリスト
MaxCompat::MaxCompat(const vector<Bdd>& tf_array) :
  mTfArray(tf_array),
  mCurVector(tf_array.size(), false)
{
}

// @brief デストラクタ
MaxCompat::~MaxCompat()
{
}

// @brief maximal compatible set を求める．
void
MaxCompat::operator()(ymuint f0,
		      const vector<bool>& covered,
		      vector<ymuint>& f_set)
{
  vector<ymuint> f_list;
  Bdd c0 = mTfArray[f0];
  for (ymuint i = 0; i < mTfArray.size(); ++ i) {
    if ( i == f0 ) {
      continue;
    }
    if ( covered[i] ) {
      continue;
    }
    Bdd tf = mTfArray[i];
    if ( tf.is_zero() ) {
      continue;
    }
    if ( ! (c0 && tf) ) {
      continue;
    }
    f_list.push_back(i);
  }
  mMaxUpdates = 0;
  mCurMax = 0;
  mCurMaxSet.clear();
  vector<ymuint> cur_set;
  cur_set.push_back(f0);
  mCurVector[f0] = true;
  cout << "  " << f0 << " ON" << endl;
  kernel(c0, f_list, 0, cur_set);
  cout << "  " << f0 << " OFF" << endl;
  mCurVector[f0] = false;

  f_set = mCurMaxSet;
}

void
MaxCompat::kernel(const Bdd& c0,
		  const vector<ymuint>& f_list,
		  ymuint pos,
		  const vector<ymuint>& cur_set)
{
  cout << "kernel(f_list = " << f_list.size() << ", pos = " << pos << ")" << endl;
  ymuint nf = f_list.size();
  vector<ymuint> new_f_list;
  new_f_list.reserve(nf);
  vector<ymuint> cur_set2(cur_set);
  vector<ymuint> new_set;
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint f = f_list[i];
    if ( c0 < mTfArray[f] ) {
      if ( !mCurVector[f] ) {
	if ( i < pos ) {
	  // すでに調べているはず．
	  goto end;
	}
	else {
	  // f を加える．
	  cur_set2.push_back(f);
	  new_set.push_back(f);
	  mCurVector[f] = true;
	  cout << "  " << f << " ON" << endl;
	}
      }
    }
    else {
      new_f_list.push_back(f);
    }
  }

  {
    ymuint nf2 = new_f_list.size();
    bool done = false;
    for (ymuint i = pos; i < nf2; ++ i) {
      ymuint f = new_f_list[i];
      Bdd c = c0 & mTfArray[f];
      if ( c.is_zero() ) {
	continue;
      }
      done = true;
      cur_set2.push_back(f);
      mCurVector[f] = true;
      cout << "  " << f << " ON" << endl;
      kernel(c, new_f_list, i + 1, cur_set2);
      cur_set2.pop_back();
      mCurVector[f] = false;
      cout << "  " << f << " OFF" << endl;
      if ( mMaxUpdates > 1 ) {
	break;
      }
    }
    if ( !done ) {
      ymuint n = cur_set2.size();
      if ( mCurMax < n ) {
	mCurMax = n;
	mCurMaxSet = cur_set2;
	cout << " FOUND, MAX = " << n << endl;
	++ mMaxUpdates;
      }
    }
  }

 end:
  for (ymuint i = 0; i < new_set.size(); ++ i) {
    ymuint f = new_set[i];
    mCurVector[f] = false;
    cout << "  " << f << " OFF" << endl;
  }
}

END_NAMESPACE_YM_SATPG
