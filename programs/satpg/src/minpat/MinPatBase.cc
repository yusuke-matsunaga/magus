
/// @file MinPatBase.cc
/// @brief MinPatBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"
#include "MinPatStats.h"
#include "FgMgr.h"
#include "TpgNetwork.h"
#include "Verifier.h"

#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス MinPatBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinPatBase::MinPatBase()
{
  mVerbose = false;
}

// @brief デストラクタ
MinPatBase::~MinPatBase()
{
}

// @brief テストベクタの最小化を行なう．
// @param[in] network 対象のネットワーク
// @param[in] tvmgr テストベクタマネージャ
// @param[in] fmgr 故障マネージャ
// @param[in] fsim2 2値の故障シミュレータ(検証用)
// @param[out] tv_list テストベクタのリスト
// @param[out] stats 実行結果の情報を格納する変数
void
MinPatBase::run(TpgNetwork& network,
		TvMgr& tvmgr,
		FaultMgr& fmgr,
		Fsim& fsim2,
		vector<TestVector*>& tv_list,
		MinPatStats& stats)
{
  StopWatch total_timer;
  total_timer.start();

  ymuint max_node_id = network.max_node_id();

  ymuint nf = init(network, tvmgr, fmgr, fsim2, tv_list);

  StopWatch local_timer;
  local_timer.start();
  if ( verbose() ) {
    cout << "grouping start" << endl;
  }

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

    if ( verbose() ) {
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
  if ( verbose() ) {
    cout << endl;
    cout << " # of fault groups = " << fgmgr.group_num() << endl;
    cout << "CPU time (coloring)              " << local_timer.time() << endl;
  }

  // テストパタンを作る．
  local_timer.reset();
  local_timer.start();

  vector<TestVector*> new_tv_list;
  fgmgr.make_testvector(network, tvmgr, new_tv_list);

  local_timer.stop();
  if ( verbose() ) {
    cout << "CPU time (testvector generation) " << local_timer.time() << endl;
  }

  ymuint orig_num = tv_list.size();

  tv_list = new_tv_list;

  {
    // 検証しておく．
    Verifier ver(fmgr, fsim2);
    if ( ver.check(tv_list) ) {
      cout << "No Errors" << endl;
    }
  }

  total_timer.stop();
  USTime time = total_timer.time();

  stats.set(orig_num, tv_list.size(), time);
}

// @brief verbose フラグをセットする．
void
MinPatBase::set_verbose(bool verbose)
{
  mVerbose = verbose;
}

// @brief verbose フラグを得る．
bool
MinPatBase::verbose() const
{
  return mVerbose;
}

END_NAMESPACE_YM_SATPG
