
/// @file MinPatSimple.cc
/// @brief MinPatSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple.h"
#include "TpgFault.h"
#include "FaultAnalyzer.h"
#include "EqChecker.h"
#include "DomChecker.h"
#include "ConflictChecker.h"

#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
MinPat*
new_MinPatSimple(bool group_dominance)
{
  return new MinPatSimple(group_dominance);
}

BEGIN_NONAMESPACE

struct FaultLt
{
  FaultLt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(const TpgFault* left,
	     const TpgFault* right)
  {
    return mChecker.det_count(left->id()) < mChecker.det_count(right->id());
  }

  DomChecker mChecker;

};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MinPatSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
MinPatSimple::MinPatSimple(bool group_dominance) :
  MinPatNaive(group_dominance)
{
}

// @brief デストラクタ
MinPatSimple::~MinPatSimple()
{
}

// @brief 初期化を行う．
// @param[in] fault_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatSimple::init(const vector<const TpgFault*>& fault_list,
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

    // 検出パタン数の少ない順に並べる．
    sort(dom_fault_list.begin(), dom_fault_list.end(), FaultLt(checker));
  }

  set_fault_list(dom_fault_list);
}

END_NAMESPACE_YM_SATPG
