
/// @file Compactor.cc
/// @brief Compactor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Compactor.h"
#include "FgMgr.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス Compactor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Compactor::Compactor()
{
}

// @brief デストラクタ
Compactor::~Compactor()
{
}

// @brief 故障グループを圧縮する．
// @param[in] fmgr 故障グループマネージャ
// @param[in] max_node_id ノード番号の最大値
// @param[in] group_list もとのグループ番号リスト
// @param[in] new_group_list 圧縮後のグループ番号リスト
void
Compactor::run(FgMgr& fgmgr,
	       ymuint max_node_id,
	       const vector<ymuint>& group_list,
	       vector<ymuint>& new_group_list)
{
  mMaxNodeId = max_node_id;

  new_group_list = group_list;

  for ( ; ; ) {
    ymuint ng0 = new_group_list.size();
    phase1(fgmgr, new_group_list);
    if ( new_group_list.size() == ng0 ) {
      break;
    }

    phase2(fgmgr, new_group_list);
  }
}

// @brief phase-1
// @param[inout] group_list 選択されたグループ番号のリスト
//
// 他のグループに移動させることでグループを削除する．
void
Compactor::phase1(FgMgr& fgmgr,
		  vector<ymuint>& group_list)
{
  ymuint max_group_id = fgmgr.group_num();

  vector<bool> locked(max_group_id, false);
  for ( ; ; ) {
    ymuint min_gid = max_group_id;
    ymuint min_size = 0;
    ymuint ng = group_list.size();

    // 要素数が最小のグループを求める．
    for (ymuint i = 0; i < ng; ++ i) {
      ymuint gid = group_list[i];
      if ( locked[gid] ) {
	continue;
      }

      ymuint size = fgmgr.fault_list(gid).size();
      if ( min_size == 0 || min_size > size ) {
	min_size = size;
	min_gid = gid;
      }
    }
    if ( min_gid == max_group_id ) {
      // すべてのグループが調査済みだった．
      break;
    }

    // 現在の情報を tmp_group_list にコピーしておく
    vector<ymuint> tmp_group_list = group_list;

    // min_gid のグループの故障を他のグループへ移動できるか調べる．
    bool red = true;
    const vector<TpgFault*>& fault_list = fgmgr.fault_list(min_gid);
    for (ymuint i = 0; i < fault_list.size(); ++ i) {
      TpgFault* fault = fault_list[i];

      SatEngine engine(string(), string(), NULL);
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);

      // fault を検出するための CNF を作る．
      fval_cnf.make_cnf(engine, fault, kVal1);

      // fault がマージできる他のグループを探す．
      bool found = false;
      for (ymuint j = 0; j < ng; ++ j) {
	ymuint gid = tmp_group_list[j];
	if ( gid == min_gid ) {
	  continue;
	}

	const NodeValList& suf_list0 = fgmgr.sufficient_assignment(gid);
	if ( engine.check_sat(gval_cnf, suf_list0) == kB3True ) {
	  ymuint gid2 = gid;
	  if ( tmp_group_list[j] == group_list[j] ) {
	    gid2 = fgmgr.duplicate_group(gid);
	    tmp_group_list[j] = gid2;
	  }
	  fgmgr.add_fault(gid2, fault);
	  found = true;
	  break;
	}
      }
      if ( !found ) {
	// 見つからなかった．
	red = false;
	break;
      }
    }
    if ( red ) {
      // 変更を反映させる．
      for (ymuint i = 0; i < ng; ++ i) {
	ymuint gid1 = group_list[i];
	ymuint gid2 = tmp_group_list[i];
	if ( gid1 != gid2 ) {
	  fgmgr.replace_group(gid1, gid2);
	}
      }
      // group_list から min_group を除く．
      ymuint wpos = 0;
      for (ymuint rpos = 0; rpos < ng; ++ rpos) {
	ymuint gid = group_list[rpos];
	if ( gid != min_gid ) {
	  if ( wpos != rpos ) {
	    group_list[wpos] = gid;
	  }
	  ++ wpos;
	}
      }
      group_list.erase(group_list.begin() + wpos, group_list.end());
    }
    else {
      // 変更を破棄する．
      for (ymuint i = 0; i < ng; ++ i) {
	ymuint gid1 = group_list[i];
	ymuint gid2 = tmp_group_list[i];
	if ( gid1 != gid2 ) {
	  fgmgr.delete_group(gid2);
	}
      }
    }
    locked[min_gid] = true;
  }
}

// @brief phase-2
// @param[inout] group_list 選択されたグループ番号のリスト
//
// 他のグループに移動させることでグループを削除する．
void
Compactor::phase2(FgMgr& fgmgr,
		  vector<ymuint>& group_list)
{
  ymuint max_group_id = fgmgr.group_num();
  ymuint ng = group_list.size();

  vector<bool> locked(max_group_id, false);
  for ( ; ; ) {
    ymuint min_pos = 0;
    ymuint min_gid = max_group_id;
    ymuint min_size = 0;

    // 要素数が最小のグループを求める．
    for (ymuint i = 0; i < ng; ++ i) {
      ymuint gid = group_list[i];
      if ( locked[gid] ) {
	continue;
      }

      ymuint size = fgmgr.fault_list(gid).size();
      if ( min_size == 0 || min_size > size ) {
	min_size = size;
	min_pos = i;
	min_gid = gid;
      }
    }
    if ( min_gid == max_group_id ) {
      // すべてのグループが調査済みだった．
      break;
    }

    // 可能な限り故障を他のグループに移動する．
    const vector<TpgFault*>& fault_list = fgmgr.fault_list(min_gid);
    vector<TpgFault*> del_fault_list;
    del_fault_list.reserve(fault_list.size());
    for (ymuint i = 0; i < fault_list.size(); ++ i) {
      TpgFault* fault = fault_list[i];

      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      SatEngine engine(string(), string(), NULL);

      // fault を検出する CNF を作る．
      fval_cnf.make_cnf(engine, fault, kVal1);

      // fault がマージできる他のグループを探す．
      for (ymuint j = 0; j < ng; ++ j) {
	ymuint gid = group_list[j];
	if ( gid == min_gid ) {
	  continue;
	}
	if ( locked[gid] ) {
	  // 処理済みのグループには移動しない．
	  continue;
	}
	const NodeValList& suf_list0 = fgmgr.sufficient_assignment(gid);
	if ( engine.check_sat(gval_cnf, suf_list0) == kB3True ) {
	  fgmgr.add_fault(gid, fault);
	  del_fault_list.push_back(fault);
	  break;
	}
      }
    }
    if ( !del_fault_list.empty() ) {
      fgmgr.delete_fault(min_gid, del_fault_list);
    }
    locked[min_gid] = true;
  }
}

END_NAMESPACE_YM_SATPG
