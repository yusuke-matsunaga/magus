
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
// @param[in] fid_list 検出された故障のリスト
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
void
MinPatNaive::init(const vector<ymuint>& fid_list,
		  TvMgr& tvmgr,
		  Fsim& fsim2)
{
  set_fid_list(fid_list);
}

// @brief 故障リストを設定する．
void
MinPatNaive::set_fid_list(const vector<ymuint>& src_list)
{
  ymuint nf = src_list.size();
  mFidList.clear();
  mFidList.resize(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    ymuint fid = src_list[i];
    mFidList[i] = fid;
  }
}

// @brief 対象の全故障数を返す．
ymuint
MinPatNaive::fault_num()
{
  return mFidList.size();
}

// @brief 故障番号のリストを返す．
const vector<ymuint>&
MinPatNaive::fid_list()
{
  return mFidList;
}

// @brief 最初の故障を選ぶ．
ymuint
MinPatNaive::get_first_fault()
{
  mNextPos = 1;
  return mFidList[0];
}

// @brief 次に処理すべき故障を選ぶ．
// @param[in] fgmgr 故障グループを管理するオブジェクト
// @param[in] group_list 現在のグループリスト
ymuint
MinPatNaive::get_next_fault(FgMgr& fgmgr,
			    const vector<ymuint>& group_list)
{
  if ( mNextPos < mFidList.size() ) {
    ymuint fid = mFidList[mNextPos];
    ++ mNextPos;
    return fid;
  }
  else {
    return 0;
  }
}

END_NAMESPACE_YM_SATPG
