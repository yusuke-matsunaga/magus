
/// @file MinPatSimple2.cc
/// @brief MinPatSimple2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple2.h"
#include "FaultAnalyzer.h"
#include "EqChecker.h"
#include "DomChecker.h"
#include "ConflictChecker.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
MinPat*
new_MinPatSimple2(bool group_dominance)
{
  return new MinPatSimple2(group_dominance);
}

BEGIN_NONAMESPACE

struct FaultGt
{
  FaultGt(const vector<ymuint>& conf_num_array) :
    mConfNumArray(conf_num_array)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mConfNumArray[left] > mConfNumArray[right];
  }

  const vector<ymuint>& mConfNumArray;

};

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス MinPatSimple2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
MinPatSimple2::MinPatSimple2(bool group_dominance) :
  MinPatNaive(group_dominance)
{
}

// @brief デストラクタ
MinPatSimple2::~MinPatSimple2()
{
}

// @brief 初期化を行う．
// @param[in] fid_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatSimple2::init(const vector<ymuint>& fid_list,
		    TvMgr& tvmgr,
		    Fsim& fsim2)
{
  // 代表故障のリスト
  vector<ymuint> rep_fid_list;
  {
    EqChecker checker1(analyzer(), tvmgr, fsim2);
    checker1.get_rep_faults(fid_list, rep_fid_list);
  }

  // 支配故障のリスト
  vector<ymuint> dom_fid_list;
  {
    DomChecker checker(analyzer(), tvmgr, fsim2);
    checker.get_dom_faults(rep_fid_list, dom_fid_list);
  }

  ymuint nf = dom_fid_list.size();

  // 故障を衝突数の多い順に並べる．
  vector<ymuint> conf_num_array;
  ConflictChecker checker2(analyzer(), tvmgr, fsim2);
  checker2.estimate_conflict(dom_fid_list, conf_num_array);

  sort(dom_fid_list.begin(), dom_fid_list.end(), FaultGt(conf_num_array));

  set_fid_list(dom_fid_list);

}

END_NAMESPACE_YM_SATPG
