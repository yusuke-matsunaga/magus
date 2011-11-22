
/// @file ThBddGen.cc
/// @brief ThBddGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ThBddGen.h"


BEGIN_NAMESPACE_YM

static
bool debug = false;

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
  ymuint n = weight_array.size();
  mWeightArray.resize(n);
  mItvlList.clear();
  mItvlList.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    Weight& w = mWeightArray[i];
    w.mIdx = i;
    w.mWeight = weight_array[i];
  }
  sort(mWeightArray.begin(), mWeightArray.end(), WeightGT());
  double acc = 0.0;
  for (ymuint i = 0; i < n; ++ i) {
    Weight& w = mWeightArray[n - i - 1];
    acc += w.mWeight;
    w.mAcc = acc;
  }

  for (ymuint i = 0; i < n; ++ i) {
    Weight& w = mWeightArray[i];
    cout << "IDX: " << w.mIdx
	 << "\tWEIGHT: " << setw(10) << w.mWeight
	 << "\tACC: " << setw(10) << w.mAcc
	 << endl;
  }

  double lb;
  double ub;
  Bdd f = gen_bdd(0, threshold, lb, ub);

  return f;
}

// BDD を作る再帰関数
Bdd
ThBddGen::gen_bdd(ymuint pos,
		  double slack,
		  double& lb,
		  double& ub)
{
  if ( slack < 0.0 ) {
    if ( debug ) {
      cout << " ==> one" << endl;
    }
    lb = DBL_MIN;
    ub = 0.0;
    return mBddMgr.make_one();
  }

  Weight& w = mWeightArray[pos];
  if ( slack >= w.mAcc ) {
    if ( debug ) {
      cout << " ==> zero" << endl;
    }
    lb = 0.0;
    ub = DBL_MAX;
    return mBddMgr.make_zero();
  }

  // 区間検索
  vector<Cell>& itvl_list = mItvlList[pos];
  for (vector<Cell>::iterator p = itvl_list.begin();
       p != itvl_list.end(); ++ p) {
    Cell& cell = *p;
    if ( cell.mLb <= slack && cell.mUb >= slack ) {
      lb = cell.mLb;
      ub = cell.mUb;
      if ( debug ) {
	cout << " ==> found(" << lb << ", " << ub << ")" << endl;
      }
      return cell.mBdd;
    }
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
  Bdd f0 = gen_bdd(pos + 1, slack, lb0, ub0);
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
  Bdd f1 = gen_bdd(pos + 1, slack - weight, lb1, ub1);
  if ( lb1 != DBL_MIN ) {
    lb1 += weight;
  }
  if ( ub1 != DBL_MAX ) {
    ub1 += weight;
  }
  if ( debug ) {
    cout << "lb1 = " << lb1
	 << ", ub1 = " << ub1
	 << endl
	 << endl;
  }

  lb = lb0;
  if ( lb1 != DBL_MIN && lb < lb1 ) {
    lb = lb1;
  }
  ub = ub0;
  if ( ub1 != DBL_MAX && ub > ub1 ) {
    ub = ub1;
  }

  if ( debug ) {
    cout << "pos = " << pos << ", slack = " << slack << endl;
    cout << "lb = " << lb << endl
	 << "ub = " << ub << endl
	 << endl;
  }
  Bdd f = mBddMgr.make_bdd(VarId(w.mIdx), f0, f1);

  itvl_list.push_back(Cell(lb, ub, f));
  cout << "Level#" << pos << ": " << itvl_list.size() << endl;

  return f;
}

END_NAMESPACE_YM
