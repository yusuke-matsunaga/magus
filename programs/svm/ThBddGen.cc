
/// @file ThBddGen.cc
/// @brief ThBddGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ThBddGen.h"


BEGIN_NAMESPACE_YM

static
bool debug = true;

//////////////////////////////////////////////////////////////////////
// クラス ThBddGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] bddmgr BDDマネージャ
ThBddGen::ThBddGen(BddMgr& bddmgr) :
  mBddMgr(bddmgr)
{
}

// @brief デストラクタ
ThBddGen::~ThBddGen()
{
}

// @brief しきい値関数を表す BDD を生成する．
// @param[in] weight_array 係数の配列
// @param[in] threshold しきい値
Bdd
ThBddGen::operator()(const vector<double>& weight_array,
		     double threshold)
{
  mWeightArray.clear();
  mNv = weight_array.size();
  mWeightArray.resize(mNv);
  mItvlList.clear();
  mItvlList.resize(mNv);
  for (ymuint i = 0; i < mNv; ++ i) {
    Weight& w = mWeightArray[i];
    w.mIdx = i;
    w.mWeight = weight_array[i];
  }
  sort(mWeightArray.begin(), mWeightArray.end(), WeightGT());
  double acc = 0.0;
  for (ymuint i = 0; i < mNv; ++ i) {
    Weight& w = mWeightArray[mNv - i - 1];
    acc += w.mWeight;
    w.mAcc = acc;
  }

  mThreshold = threshold;

  for (ymuint i = 0; i < mNv; ++ i) {
    Weight& w = mWeightArray[i];
    cout << "POS: " << i
	 << "\tIDX: " << w.mIdx
	 << "\tWEIGHT: " << setw(10) << w.mWeight
	 << "\tACC: " << setw(10) << w.mAcc
	 << endl;
  }

  double lb;
  double ub;
  vector<bool> curvals(mNv, false);
  vector<bool> lbvals(mNv, false);
  vector<bool> ubvals(mNv, false);
  Bdd f = gen_bdd(0, threshold, curvals, lb, ub, lbvals, ubvals);

  return f;
}

// BDD を作る再帰関数
Bdd
ThBddGen::gen_bdd(ymuint pos,
		  double slack,
		  const vector<bool>& curvals,
		  double& lb,
		  double& ub,
		  vector<bool>& lbvals,
		  vector<bool>& ubvals)
{
  Weight& w = mWeightArray[pos];

  if ( slack < 0.0 ) {
    if ( debug ) {
      cout << " ==> one" << endl;
    }
    lb = DBL_MIN;
    ub = 0.0;
    return mBddMgr.make_one();
  }

  if ( slack >= w.mAcc ) {
    if ( debug ) {
      cout << " ==> zero" << endl;
    }
    lb = w.mAcc;
    ub = DBL_MAX;
    for (ymuint i = pos; i < mNv; ++ i) {
      lbvals[i] = true;
    }
    return mBddMgr.make_zero();
  }

  // 区間検索
  ItvlList& itvl_list = mItvlList[pos];
  Bdd f;
  bool found = itvl_list.find(slack, lb, ub, lbvals, ubvals, f);
  if ( found ) {
    if ( debug ) {
      cout << " ==> found ("
	   << lb << ", " << ub << ")"
	   << endl;
    }
    return f;
  }

  double weight = w.mWeight;

  if ( debug ) {
    cout << "gen_bdd0("
	 << pos + 1
	 << ", "
	 << slack << ")"
	 << endl;
  }
  double lb0;
  double ub0;
  vector<bool> cur0vals = curvals;
  vector<bool> lb0vals(mNv);
  vector<bool> ub0vals(mNv);
  Bdd f0 = gen_bdd(pos + 1, slack, cur0vals, lb0, ub0, lb0vals, ub0vals);
  if ( debug ) {
    cout << "lb0 = " << lb0
	 << ", ub0 = " << ub0
	 << endl
	 << endl;
  }

  if ( debug ) {
    cout << "gen_bdd1("
	 << pos + 1
	 << ", "
	 << slack - weight
	 << ")" << endl;
  }
  double lb1;
  double ub1;
  cur0vals[pos] = true;
  vector<bool> lb1vals(mNv);
  vector<bool> ub1vals(mNv);
  Bdd f1 = gen_bdd(pos + 1, slack - weight, cur0vals, lb1, ub1, lb1vals, ub1vals);
  if ( lb1 != DBL_MIN ) {
    lb1vals[pos] = true;
    lb1 += weight;
  }
  if ( ub1 != DBL_MAX ) {
    ub1vals[pos] = true;
    ub1 += weight;
  }
  if ( debug ) {
    cout << "lb1 = " << lb1
	 << ", ub1 = " << ub1
	 << endl
	 << endl;
  }

  lb = lb0;
  lbvals = lb0vals;
  if ( lb1 != DBL_MIN && lb < lb1 ) {
    lb = lb1;
    lbvals = lb1vals;
  }
  ub = ub0;
  ubvals = ub0vals;
  if ( ub1 != DBL_MAX && ub > ub1 ) {
    ub = ub1;
    ubvals = ub1vals;
  }

  if ( debug ) {
    double delta = ub - lb;
    cout << "pos = " << pos << ", slack = " << slack << endl;
    cout << "lb = " << lb << endl
	 << "ub = " << ub << endl
	 << "delta = " << delta << endl
	 << endl;
    if ( delta < 1e-5 ) {
      double v = 0.0;
      cout << "lbvals = ";
      for (ymuint i = 0; i < mNv; ++ i) {
	if ( curvals[i] || lbvals[i] ) {
	  cout << " " << i
	       << "(" << mWeightArray[i].mWeight << ")";
	  v += mWeightArray[i].mWeight;
	}
      }
      cout << endl;
      cout << "lbtotal = " << v - mThreshold << endl;
      cout << "ubvals = ";
      v = 0.0;
      for (ymuint i = 0; i < mNv; ++ i) {
	if ( curvals[i] || ubvals[i] ) {
	  cout << " " << i
	       << "(" << mWeightArray[i].mWeight << ")";
	  v += mWeightArray[i].mWeight;
	}
      }
      cout << endl;
      cout << "ubtotal = " << v - mThreshold << endl;
      abort();
    }
  }
#if 0
  f = mBddMgr.make_bdd(VarId(w.mIdx), f0, f1);
#else
  f = mBddMgr.make_bdd(VarId(pos), f0, f1);
#endif

  itvl_list.add(lb, ub, lbvals, ubvals, f);

#if 0
  cout << "Level#" << pos << ": " << itvl_list.size() << endl
       << "  [" << lb << ", " << ub << ")" << endl;
  //f.display(cout);
  cout << endl;
#endif

  return f;
}

END_NAMESPACE_YM
