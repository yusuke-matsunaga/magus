
/// @file MinPatNaive.cc
/// @brief MinPatNaive の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatNaive.h"
#include "FaultAnalyzer.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
MinPat*
new_MinPat(bool group_dominance)
{
  return new MinPatNaive(group_dominance);
}


//////////////////////////////////////////////////////////////////////
// クラス MinPatNaive
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
MinPatNaive::MinPatNaive(bool group_dominance) :
  MinPatBase(group_dominance)
{
}

// @brief デストラクタ
MinPatNaive::~MinPatNaive()
{
}

// @brief 初期化を行う．
// @param[in] fault_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatNaive::init(const vector<TpgFault*>& fault_list,
		  TvMgr& tvmgr,
		  Fsim& fsim2)
{
  set_fault_list(fault_list);
}

// @brief 故障リストを設定する．
void
MinPatNaive::set_fault_list(const vector<TpgFault*>& src_list)
{
  ymuint nf = src_list.size();
  mFaultList.clear();
  mFaultList.resize(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    mFaultList[i] = src_list[i];
  }
}

// @brief 対象の全故障数を返す．
ymuint
MinPatNaive::fault_num()
{
  return mFaultList.size();
}

// @brief 最初の故障を選ぶ．
TpgFault*
MinPatNaive::get_first_fault()
{
  mNextPos = 1;
  return mFaultList[0];
}

// @brief 次に処理すべき故障を選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] group_list 現在のグループリスト
//
// 故障が残っていなければ NULL を返す．
TpgFault*
MinPatNaive::get_next_fault(FgMgr& fgmgr,
			    const vector<ymuint>& group_list)
{
  if ( mNextPos < mFaultList.size() ) {
    TpgFault* fault = mFaultList[mNextPos];
    ++ mNextPos;
    return fault;
  }
  else {
    return NULL;
  }
}

END_NAMESPACE_YM_SATPG
