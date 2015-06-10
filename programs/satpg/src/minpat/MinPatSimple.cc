
/// @file MinPatSimple.cc
/// @brief MinPatSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatSimple.h"
#include "FaultAnalyzer.h"
#include "EqChecker.h"
#include "DomChecker.h"
#include "ConflictChecker.h"

#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
// @param[in] rep_faults 等価故障の検出を行う．
MinPat*
new_MinPatSimple(bool group_dominance,
		 bool rep_faults)
{
  return new MinPatSimple(group_dominance, rep_faults);
}

BEGIN_NONAMESPACE

struct FaultLt
{
  FaultLt(DomChecker& checker) :
    mChecker(checker)
  {
  }

  bool
  operator()(ymuint left,
	     ymuint right)
  {
    return mChecker.det_count(left) < mChecker.det_count(right);
  }

  DomChecker mChecker;

};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MinPatSimple
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
// @param[in] rep_faults 等価故障の検出を行う．
MinPatSimple::MinPatSimple(bool group_dominance,
			   bool rep_faults) :
  MinPatNaive(group_dominance),
  mRepFaults(rep_faults)
{
}

// @brief デストラクタ
MinPatSimple::~MinPatSimple()
{
}

// @brief 初期化を行う．
// @param[in] fid_list 検出された故障番号のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatSimple::init(const vector<ymuint>& fid_list,
		   TvMgr& tvmgr,
		   Fsim& fsim2)
{
  // 支配故障のリスト
  vector<ymuint> dom_fid_list;

  if ( mRepFaults ) {
    // 代表故障のリスト
    vector<ymuint> rep_fid_list;
    {
      EqChecker checker(analyzer(), tvmgr, fsim2);
      checker.get_rep_faults(fid_list, rep_fid_list);
    }
    {
      DomChecker checker(analyzer(), tvmgr, fsim2);
      checker.get_dom_faults(rep_fid_list, dom_fid_list);

      // 検出パタン数の少ない順に並べる．
      sort(dom_fid_list.begin(), dom_fid_list.end(), FaultLt(checker));
    }
  }
  else {
    {
      DomChecker checker(analyzer(), tvmgr, fsim2);
      checker.get_dom_faults(fid_list, dom_fid_list);

      // 検出パタン数の少ない順に並べる．
      sort(dom_fid_list.begin(), dom_fid_list.end(), FaultLt(checker));
    }
  }
  set_fid_list(dom_fid_list);
}

END_NAMESPACE_YM_SATPG
