
/// @file MinPatBase.cc
/// @brief MinPatBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"
#include "FgMgr.h"
#include "TpgNetwork.h"
#include "TvMgr.h"
#include "Verifier.h"

#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPatBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] group_dominance グループ支配を計算する．
// @param[in] fault_dominace 故障支配を計算する．
MinPatBase::MinPatBase(bool group_dominance,
		       bool fault_dominance)
{
  mVerbose = 0;
  mGroupDominance = group_dominance;
  mFaultDominance = fault_dominance;
}

// @brief デストラクタ
MinPatBase::~MinPatBase()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatBase::run(TpgNetwork& network,
		TvMgr& tvmgr,
		Fsim& fsim2,
		vector<TestVector*>& tv_list,
		USTime& time)
{
  StopWatch total_timer;
  total_timer.start();

  ymuint max_node_id = network.max_node_id();

  vector<TpgFault*> fault_list;
  init(network, tvmgr, fsim2, fault_list);
  ymuint nf = fault_list.size();

  StopWatch local_timer;
  local_timer.start();

  FgMgr fgmgr(max_node_id);

  { // 最初の故障を選ぶ
    TpgFault* fault = get_first_fault();

    // 最初のグループを作る．
    ymuint gid = fgmgr.new_group();

    // 故障を追加する．
    fgmgr.add_fault(gid, fault);
  }

  // 未処理の故障がある限り以下の処理を繰り返す．
  for (ymuint c = 0; ; ++ c) {

    if ( verbose() > 1 ) {
      cout << "\r                       ";
      cout << "\r   " << setw(6) << c << " / " << setw(6) << nf
	   << " : " << setw(4) << fgmgr.group_num();
      cout.flush();
    }

    // 故障を選ぶ．
    TpgFault* fault = get_next_fault(fgmgr);
    if ( fault == NULL ) {
      break;
    }

    // 故障を支配しているグループを見つける．
    if ( mGroupDominance ) {
      ymuint gid = fgmgr.find_dom_group(fault);
      if ( gid < fgmgr.group_num() ) {
	// 見つけた．
	if ( mFaultDominance ) {
	  if ( fgmgr.check_fault_dominance(gid, fault) ) {
	    // 個別の故障に支配されていたら fault は考慮しなくて良い．
	    continue;
	  }
	}
	fgmgr.add_fault(gid, fault);
	continue;
      }
    }

    // 故障を追加できるグループを見つける．
    ymuint gid = find_group(fgmgr, fault);
    if ( gid == fgmgr.group_num() ) {
      // 見つからなかった．
      // 新たなグループを作る．
      gid = fgmgr.new_group();
    }

    // 故障を追加する．
    fgmgr.add_fault(gid, fault);
  }

  local_timer.stop();
  if ( verbose() > 0 ) {
    if ( verbose() > 1 ) {
      cout << endl;
    }
    cout << " # of fault groups = " << fgmgr.group_num() << endl;
    cout << "CPU time (coloring)              " << local_timer.time() << endl;
  }

  // 後処理
  local_timer.reset();
  local_timer.start();

  vector<ymuint> group_list;
  fgmgr.compaction(group_list);

  local_timer.stop();
  if ( verbose() > 0 ) {
    cout << " # of fault groups = " << group_list.size() << endl;
    cout << "CPU time (compaction)              " << local_timer.time() << endl;
  }

  // テストパタンを作る．
  local_timer.reset();
  local_timer.start();

  ymuint new_ng = group_list.size();
  tv_list.clear();
  tv_list.reserve(new_ng);
  for (ymuint i = 0; i < new_ng; ++ i) {
    ymuint gid = group_list[i];
    TestVector* tv = tvmgr.new_vector();
    fgmgr.make_testvector(gid, network, tv);
    tv_list.push_back(tv);
  }

  local_timer.stop();
  if ( verbose() > 0 ) {
    cout << "CPU time (testvector generation) " << local_timer.time() << endl;
  }

  {
    // 検証しておく．
    Verifier ver;
    if ( ver.check(fsim2, fault_list, tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  time = total_timer.time();
}

// @brief verbose フラグをセットする．
void
MinPatBase::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief verbose フラグを得る．
int
MinPatBase::verbose() const
{
  return mVerbose;
}

END_NAMESPACE_YM_SATPG
