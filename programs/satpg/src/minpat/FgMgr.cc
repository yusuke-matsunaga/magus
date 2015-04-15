
/// @file FgMgr.cc
/// @brief FgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr.h"
#include "TvMgr.h"
#include "GvalCnf.h"
#include "FvalCnf.h"
#include "ModelValMap.h"
#include "Extractor.h"
#include "TpgCnf1.h"
#include "TpgFault.h"
#include "TpgNetwork.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値 + 1
FgMgr::FgMgr(ymuint max_node_id) :
  mMaxNodeId(max_node_id)
{
}

// @brief デストラクタ
FgMgr::~FgMgr()
{
  clear();
}

// @brief クリアする．
void
FgMgr::clear()
{
  for (ymuint gid = 0; gid < mGroupList.size(); ++ gid) {
    delete mGroupList[gid];
  }
  mGroupList.clear();
}

// @brief 現在のグループ数を返す．
ymuint
FgMgr::group_num() const
{
  return mGroupList.size();
}

// @brief 新しいグループを作る．
// @return グループ番号を返す．
ymuint
FgMgr::new_group()
{
  FaultGroup* fg = new FaultGroup;
  fg->mId = mGroupList.size();
  mGroupList.push_back(fg);
  return fg->mId;
}

// @brief 既存のグループに故障を追加する．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault 故障
void
FgMgr::add_fault(ymuint gid,
		 TpgFault* fault)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];

#if 0
  TpgCnf1 tpg_cnf(string(), string(), NULL);
  tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

  NodeValList suf_list;
  bool stat = tpg_cnf.get_suf_list(fg->mSufList, suf_list);
  ASSERT_COND( stat );
#else
  SatEngine engine(string(), string(), NULL);

  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);

  fval_cnf.make_cnf(engine, fault, kVal1);

  engine.assumption_begin();
  fval_cnf.add_assumption(engine, fg->mSufList);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  ASSERT_COND ( sat_ans == kB3True );

  ModelValMap val_map(fval_cnf.gvar_map(), fval_cnf.fvar_map(), sat_model);
  Extractor extract(val_map);
  NodeValList suf_list;
  extract(fault, suf_list);
#endif

  fg->add_fault(fault, suf_list);
}

// @brief 故障を取り除く
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
// @param[in] fault_list 故障リスト
void
FgMgr::delete_fault(ymuint gid,
		    const vector<TpgFault*>& fault_list)
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];

  HashSet<ymuint> fault_hash;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    TpgFault* fault = fault_list[i];
    fault_hash.add(fault->id());
  }

  fg->mSufList = NodeValList();
  ymuint nf = fg->mFaultList.size();
  ymuint wpos = 0;
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* fault = fg->mFaultList[i];
    if ( fault_hash.check(fault->id()) ) {
      continue;
    }
    if ( wpos != i ) {
      fg->mFaultList[wpos] = fault;
      fg->mFaultSufList[wpos] = fg->mFaultSufList[i];
    }
    ++ wpos;
    fg->mSufList.merge(fg->mFaultSufList[i]);
  }
  fg->mFaultList.erase(fg->mFaultList.begin() + wpos, fg->mFaultList.end());
  fg->mFaultSufList.erase(fg->mFaultSufList.begin() + wpos, fg->mFaultSufList.end());
}

// @brief 故障を追加することのできるグループを求める．
//
// 見つからない場合には group_num() を返す．
ymuint
FgMgr::find_group(TpgFault* fault)
{
#if 0
  TpgCnf1 tpg_cnf(string(), string(), NULL);
  tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

  ymuint ng = group_num();
  for (ymuint gid = 0; gid < ng; ++ gid) {
    const NodeValList& suf_list0 = mGroupList[gid]->mSufList;
    if ( tpg_cnf.check_intersect(suf_list0) ) {
      return gid;
    }
  }
  return ng;
#else
  GvalCnf gval_cnf(mMaxNodeId);
  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);

  SatEngine engine(string(), string(), NULL);
  fval_cnf.make_cnf(engine, fault, kVal1);

  ymuint ng = group_num();
  for (ymuint gid = 0; gid < ng; ++ gid) {
    const NodeValList& suf_list0 = mGroupList[gid]->mSufList;
    if ( engine.check_sat(gval_cnf, suf_list0) == kB3True ) {
      return gid;
    }
  }
  return ng;
#endif
}

// @brief 故障グループの圧縮を行う．
// @param[out] group_list グループ番号のリスト
void
FgMgr::compaction(vector<ymuint>& group_list)
{
  ymuint ng = mGroupList.size();
  group_list.clear();
  group_list.reserve(ng);
  for (ymuint i = 0; i < ng; ++ i) {
    group_list.push_back(i);
  }

  for ( ; ; ) {
    ymuint ng0 = group_list.size();
    phase1(group_list);
    if ( group_list.size() == ng0 ) {
      break;
    }

    phase2(group_list);
  }
}

// @brief 故障リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const vector<TpgFault*>&
FgMgr::fault_list(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mFaultList;
}

// @brief 十分割当リストを返す．
// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
const NodeValList&
FgMgr::suf_list(ymuint gid) const
{
  ASSERT_COND( gid < group_num() );
  FaultGroup* fg = mGroupList[gid];
  return fg->mSufList;
}

// @brief テストパタンを作る．
// @param[in] gid グループ番号
// @param[in] network ネットワーク
// @param[in] tv テストベクタ
void
FgMgr::make_testvector(ymuint gid,
		       TpgNetwork& network,
		       TestVector* tv)
{
  ASSERT_COND( gid < group_num() );
  const NodeValList& suf_list = mGroupList[gid]->mSufList;

  ymuint max_node_id = network.max_node_id();

  GvalCnf gval_cnf(max_node_id);

  SatEngine engine(string(), string(), NULL);

  engine.assumption_begin();
  gval_cnf.add_assumption(engine, suf_list);

  vector<Bool3> sat_model;
  SatStats sat_stats;
  USTime sat_time;
  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
  ASSERT_COND ( sat_ans == kB3True );

  const VidMap& var_map = gval_cnf.var_map();
  ModelValMap val_map(var_map, var_map, sat_model);
  ymuint ni = network.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* node = network.input(i);
    ymuint input_id = node->input_id();
    Val3 val;
    if ( var_map(node) == kVarIdIllegal ) {
      val = kVal0;
    }
    else {
      val = val_map.gval(node);
    }
    tv->set_val(input_id, val);
  }
}

// @brief phase-1
// @param[inout] group_list 選択されたグループ番号のリスト
//
// 他のグループに移動させることでグループを削除する．
void
FgMgr::phase1(vector<ymuint>& group_list)
{
  ymuint max_group_id = mGroupList.size();

  vector<bool> locked(max_group_id, false);
  for ( ; ; ) {
    ymuint min_gid = max_group_id;
    ymuint min_size = 0;
    ymuint ng = group_list.size();

    // 現在の情報を tmp_group にコピーしておく
    vector<FaultGroup> tmp_group(max_group_id);
    for (ymuint i = 0; i < ng; ++ i) {
      ymuint gid = group_list[i];
      tmp_group[gid] = *mGroupList[gid];
    }

    // 要素数が最小のグループを求める．
    for (ymuint i = 0; i < ng; ++ i) {
      ymuint gid = group_list[i];
      if ( locked[gid] ) {
	continue;
      }
      const vector<TpgFault*>& fault_list = tmp_group[gid].mFaultList;
      ymuint size = fault_list.size();
      if ( min_size == 0 || min_size > size ) {
	min_size = size;
	min_gid = gid;
      }
    }
    if ( min_gid == max_group_id ) {
      // すべてのグループが調査済みだった．
      break;
    }

    // min_gid のグループの故障を他のグループへ移動できるか調べる．
    bool red = true;
    const vector<TpgFault*>& fault_list = tmp_group[min_gid].mFaultList;
    for (ymuint i = 0; i < fault_list.size(); ++ i) {
      TpgFault* fault = fault_list[i];

#if 0
      TpgCnf1 tpg_cnf(string(), string(), NULL);
      tpg_cnf.make_fval_cnf(fault, mMaxNodeId);
#else
      SatEngine engine(string(), string(), NULL);
      GvalCnf gval_cnf(mMaxNodeId);
      FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
      fval_cnf.make_cnf(engine, fault, kVal1);
#endif

      // fault がマージできる他のグループを探す．
      bool found = false;
      for (ymuint j = 0; j < ng; ++ j) {
	ymuint gid = group_list[j];
	if ( gid == min_gid ) {
	  continue;
	}
	const NodeValList& suf_list0 = tmp_group[gid].mSufList;
#if 0
	if ( tpg_cnf.check_intersect(suf_list0) ) {
	  TpgCnf1 tpg_cnf(string(), string(), NULL);
	  tpg_cnf.make_fval_cnf(fault, mMaxNodeId);
	  NodeValList suf_list;
	  bool stat = tpg_cnf.get_suf_list(suf_list0, suf_list);
	  ASSERT_COND( stat );
	  tmp_group[gid].add_fault(fault, suf_list);
	  found = true;
	  break;
	}
#else
	engine.assumption_begin();
	fval_cnf.add_assumption(engine, suf_list0);
	if ( engine.check_sat() == kB3True ) {
	  SatEngine engine(string(), string(), NULL);
	  GvalCnf gval_cnf(mMaxNodeId);
	  FvalCnf fval_cnf(mMaxNodeId, gval_cnf);
	  fval_cnf.make_cnf(engine, fault, kVal1);
	  engine.assumption_begin();
	  fval_cnf.add_assumption(engine, suf_list0);

	  vector<Bool3> sat_model;
	  SatStats sat_stats;
	  USTime sat_time;
	  Bool3 sat_ans = engine.solve(sat_model, sat_stats, sat_time);
	  ASSERT_COND ( sat_ans == kB3True );

	  ModelValMap val_map(fval_cnf.gvar_map(), fval_cnf.fvar_map(), sat_model);
	  Extractor extract(val_map);
	  NodeValList suf_list;
	  extract(fault, suf_list);
	}
#endif
      }
      if ( !found ) {
	// 見つからなかった．
	red = false;
	break;
      }
    }
    if ( red ) {
      // group_list から min_group を除く．
      ymuint wpos = 0;
      for (ymuint rpos = 0; rpos < group_list.size(); ++ rpos) {
	ymuint gid = group_list[rpos];
	if ( gid != min_gid ) {
	  if ( wpos != rpos ) {
	    group_list[wpos] = gid;
	  }
	  ++ wpos;
	}
      }
      group_list.erase(group_list.begin() + wpos, group_list.end());

      // tmp_group の内容を mGroupList に書き戻す．
      for (ymuint i = 0; i < group_list.size(); ++ i) {
	ymuint gid = group_list[i];
	const FaultGroup* src_fg = &tmp_group[gid];
	FaultGroup* fg = mGroupList[gid];
	if ( fg->mFaultList.size() != src_fg->mFaultList.size() ) {
	  *fg = *src_fg;
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
FgMgr::phase2(vector<ymuint>& group_list)
{
  ymuint max_group_id = mGroupList.size();
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
      const vector<TpgFault*>& fault_list = mGroupList[gid]->mFaultList;
      ymuint size = fault_list.size();
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
    const vector<TpgFault*>& fault_list = mGroupList[min_gid]->mFaultList;
    vector<TpgFault*> del_fault_list;
    del_fault_list.reserve(fault_list.size());
    for (ymuint i = 0; i < fault_list.size(); ++ i) {
      TpgFault* fault = fault_list[i];

      TpgCnf1 tpg_cnf(string(), string(), NULL);
      tpg_cnf.make_fval_cnf(fault, mMaxNodeId);

      // fault がマージできる他のグループを探す．
      bool found = false;
      for (ymuint j = 0; j < ng; ++ j) {
	ymuint gid = group_list[j];
	if ( gid == min_gid ) {
	  continue;
	}
	if ( locked[gid] ) {
	  // 処理済みのグループには移動しない．
	  continue;
	}
	const NodeValList& suf_list0 = mGroupList[gid]->mSufList;
	if ( tpg_cnf.check_intersect(suf_list0) ) {
	  add_fault(gid, fault);
	  del_fault_list.push_back(fault);
	  break;
	}
      }
    }
    if ( !del_fault_list.empty() ) {
      delete_fault(min_gid, del_fault_list);
    }
    locked[min_gid] = true;
  }
}

END_NAMESPACE_YM_SATPG
