
/// @file MinPatSimple2.cc
/// @brief MinPatSimple2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple2.h"
#include "TpgFault.h"
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
  operator()(const TpgFault* left,
	     const TpgFault* right)
  {
    return mConfNumArray[left->id()] > mConfNumArray[right->id()];
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
// @param[in] fault_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatSimple2::init(const vector<const TpgFault*>& fault_list,
		    TvMgr& tvmgr,
		    Fsim& fsim2)
{
  // 代表故障のリスト
  vector<const TpgFault*> rep_fault_list;
  {
    EqChecker checker1(analyzer(), tvmgr, fsim2);
    checker1.get_rep_faults(fault_list, rep_fault_list);
  }

  // 支配故障のリスト
  vector<const TpgFault*> dom_fault_list;
  {
    DomChecker checker(analyzer(), tvmgr, fsim2);
    checker.get_dom_faults(rep_fault_list, dom_fault_list);
  }

  ymuint nf = dom_fault_list.size();

  // 故障を衝突数の多い順に並べる．
  vector<ymuint> conf_num_array;
  ConflictChecker checker2(analyzer(), tvmgr, fsim2);
  checker2.estimate_conflict(dom_fault_list, conf_num_array);

  sort(dom_fault_list.begin(), dom_fault_list.end(), FaultGt(conf_num_array));

  ymuint nd = dom_fault_list.size();
  vector<ymuint> dom_fid_list;
  dom_fid_list.reserve(nd);
  for (ymuint i = 0; i < nd; ++ i) {
    dom_fid_list.push_back(dom_fault_list[i]->id());
  }
  set_fid_list(dom_fid_list);

}

END_NAMESPACE_YM_SATPG
