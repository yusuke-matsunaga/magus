
/// @file src/base/FaultMgr.cc
/// @brief FaultMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FaultMgr.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "FaultMgr.h"
#include "SaFault.h"
#include "TestVector.h"
#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
FaultMgr::FaultMgr() :
  mFaultAlloc(sizeof(SaFault), 1024),
  mFinfoAlloc(sizeof(Finfo), 1024),
  mIfaultsAlloc(4096),
  mNetwork(NULL),
  mChanged(false)
{
}

// @brief デストラクタ
FaultMgr::~FaultMgr()
{
}

// @brief クリアする．
void
FaultMgr::clear()
{
  mFaultAlloc.destroy();
  mFinfoAlloc.destroy();
  mIfaultsAlloc.destroy();
  mNetwork = NULL;
  mFnodeArray.clear();
  mAllList.clear();
  mAllRepList.clear();
  mDetList.clear();
  mRemainList.clear();
  mUntestList.clear();
  mChanged = true;
}


BEGIN_NONAMESPACE

// node が FOS の時 true を返す．
inline
bool
check_fos(const TgNode* node)
{
  ymuint nfo = node->fanout_num();
  if ( nfo == 0 ) {
    return false;
  }
  if ( nfo > 1 ) {
    return true;
  }
  if ( node->fanout(0)->is_output() ) {
    return true;
  }
  return false;
}

// 同一 FFR のノードを DFS の preorder で廻る．
void
ord_ffr(const TgNode* node,
	list<const TgNode*>& node_list)
{
  if ( !check_fos(node) && !node->is_input() ) {
    node_list.push_back(node);
    ymuint ni = node->ni();
    for (ymuint i = 0; i < ni; ++ i) {
      ord_ffr(node->fanin(i), node_list);
    }
  }
}

END_NONAMESPACE


// @brief FaultMgr に全ての単一縮退故障を設定する．
void
FaultMgr::set_ssa_fault(const TgNetwork& network)
{
  clear();

  mNetwork = &network;

  ymuint ni = network.input_num2();
  ymuint nl = network.logic_num();
  ymuint nn = network.node_num();

  mFnodeArray.resize(nn);

  // 全部の故障を生成する．
  for (ymuint i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    Fnode& fnode = mFnodeArray[i];
    fnode.mOfault[0] = new_fault(node, true, 0, 0);
    fnode.mOfault[1] = new_fault(node, true, 0, 1);
    ymuint ni = node->ni();
    void* p = mIfaultsAlloc.get_memory(sizeof(SaFault*) * ni * 2);
    fnode.mIfaults = new (p) SaFault*[ni * 2];
    for (ymuint j = 0; j < ni; ++ j) {
      fnode.mIfaults[j * 2 + 0] = new_fault(node, false, j, 0);
      fnode.mIfaults[j * 2 + 1] = new_fault(node, false, j, 1);
    }
  }

  // 代表故障を記録していく．
#if 0
  // トポロジカル順
  for (ymuint i = nl; i > 0; ) {
    -- i;
    const TgNode* node = network.sorted_logic(i);
    reg_faults(node);
  }
#else
  // FFR を固めた順
  for (ymuint i = 0; i < nl; ++ i) {
    const TgNode* node = network.sorted_logic(i);
    if ( !check_fos(node) ) {
      continue;
    }
    list<const TgNode*> node_list;
    node_list.push_back(node);
    ymuint ni = node->ni();
    for (ymuint j = 0; j < ni; ++ j) {
      ord_ffr(node->fanin(j), node_list);
    }
    for (list<const TgNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const TgNode* node = *p;
      reg_faults(node);
    }
  }
#endif

  for (ymuint i = 0; i < ni; ++ i) {
    const TgNode* node = network.input(i);
    SaFault* rep0 = NULL;
    SaFault* rep1 = NULL;
    if ( node->fanout_num() == 1 ) {
      const TgEdge* e = node->fanout_edge(0);
      if ( !e->to_node()->is_output() ) {
	rep0 = find_input_fault(e->to_node(), e->to_ipos(), 0);
	rep1 = find_input_fault(e->to_node(), e->to_ipos(), 1);
      }
    }
    add_ofault(node, 0, rep0);
    add_ofault(node, 1, rep1);
  }
}

// @brief node に関する故障を登録する．
void
FaultMgr::reg_faults(const TgNode* node)
{
  SaFault* rep0 = NULL;
  SaFault* rep1 = NULL;
  if ( node->fanout_num() == 1 ) {
    const TgEdge* e = node->fanout_edge(0);
    if ( !e->to_node()->is_output() ) {
      rep0 = find_input_fault(e->to_node(), e->to_ipos(), 0);
      rep1 = find_input_fault(e->to_node(), e->to_ipos(), 1);
    }
  }
  SaFault* f0 = add_ofault(node, 0, rep0);
  SaFault* f1 = add_ofault(node, 1, rep1);

  tTgNodeType type = node->type();
  ymuint ni = node->ni();
  switch ( type ) {
  case kTgBuff:
    add_ifault(node, 0, 0, f0);
    add_ifault(node, 0, 1, f1);
    break;

  case kTgNot:
    add_ifault(node, 0, 0, f1);
    add_ifault(node, 0, 1, f0);
    break;

  case kTgXor:
    for (ymuint j = 0; j < ni; ++ j) {
      add_ifault(node, j, 0);
      add_ifault(node, j, 1);
    }
    break;

  case kTgAnd:
    for (ymuint j = 0; j < ni; ++ j) {
      add_ifault(node, j, 0, f0);
      add_ifault(node, j, 1);
    }
    break;

  case kTgNand:
    for (ymuint j = 0; j < ni; ++ j) {
      add_ifault(node, j, 0, f1);
      add_ifault(node, j, 1);
    }
    break;

  case kTgOr:
    for (ymuint j = 0; j < ni; ++ j) {
      add_ifault(node, j, 0);
      add_ifault(node, j, 1, f1);
    }
    break;

  case kTgNor:
    for (ymuint j = 0; j < ni; ++ j) {
      add_ifault(node, j, 0);
      add_ifault(node, j, 1, f0);
    }
    break;

  case kTgCplx:
    {
      LogExpr ofunc = mNetwork->get_lexp(node);
      for (ymuint j = 0; j < ni; ++ j) {
	LogExpr tmp = ofunc.compose(VarId(j), LogExpr::make_zero());
	SaFault* rep = NULL;
	if ( tmp.is_zero() ) {
	  rep = f0;
	}
	else if ( tmp.is_one() ) {
	  rep = f1;
	}
	add_ifault(node, j, 0, rep);
	tmp = ofunc.compose(VarId(j), LogExpr::make_one());
	rep = NULL;
	if ( tmp.is_zero() ) {
	  rep = f0;
	}
	else if ( tmp.is_one() ) {
	  rep = f1;
	}
	add_ifault(node, j, 1, rep);
      }
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
}

// @brief 故障を生成する．
// @param[in] node 対象のノード
// @param[in] is_output 出力の故障のときに true とするフラグ
// @param[in] pos 入力の故障の時に入力番号を表す
// @param[in] val 縮退している値
SaFault*
FaultMgr::new_fault(const TgNode* node,
		    bool is_output,
		    ymuint pos,
		    int val)
{
  void* p = mFaultAlloc.get_memory(sizeof(SaFault));
  SaFault* f = new (p) SaFault(node, is_output, pos, val);
  return f;
}

// @brief 代表故障を返す．
SaFault*
FaultMgr::find_rep_fault(SaFault* f) const
{
  return f->mFinfo->rep();
}

// @brief 出力の故障を取り出す．
// @param[in] node 対象のノード
// @param[in] val 縮退している値
SaFault*
FaultMgr::find_output_fault(const TgNode* node,
			    int val)
{
  Fnode& fnode = mFnodeArray[node->gid()];
  SaFault* f = fnode.mOfault[val];
  return f;
}

// @brief 入力の故障を取り出す．
// @param[in] node 対象のノード
// @param[in] pos 入力の故障の時に入力番号を表す
// @param[in] val 縮退している値
SaFault*
FaultMgr::find_input_fault(const TgNode* node,
			   ymuint pos,
			   int val)
{
  Fnode& fnode = mFnodeArray[node->gid()];
  SaFault* f = fnode.mIfaults[pos * 2 + val];
  return f;
}

// @brief 同じ箇所で反対の故障値を持つ故障を返す．
SaFault*
FaultMgr::find_alternative_fault(SaFault* f)
{
  const TgNode* node = f->node();
  int fval = f->val() ^ 1;
  Fnode& fnode = mFnodeArray[node->gid()];
  if ( f->is_input_fault() ) {
    ymuint pos = f->pos();
    return fnode.mIfaults[pos * 2 + fval];
  }
  else {
    return fnode.mOfault[fval];
  }
}

// @brief 故障を追加する．
SaFault*
FaultMgr::add_fault(const TgNode* node,
		    bool is_output,
		    ymuint pos,
		    int val,
		    SaFault* rep)
{
  SaFault* f = NULL;
  if ( is_output ) {
    f = find_output_fault(node, val);
  }
  else {
    f = find_input_fault(node, pos, val);
  }
  f->mId = mAllList.size();
  mAllList.push_back(f);
  mChanged = true;

  if ( rep ) {
    f->mFinfo = rep->mFinfo;

  }
  else {
    void* p = mFinfoAlloc.get_memory(sizeof(Finfo));
    f->mFinfo = new (p) Finfo;
    mAllRepList.push_back(f);
    mRemainList.push_back(f);
  }
  f->mFinfo->mEqFaults.push_back(f);

  return f;
}

// @brief fault の状態を変更する．
void
FaultMgr::set_status(SaFault* fault,
		     FaultStatus stat)
{
  Finfo* finfo = fault->mFinfo;
  finfo->mStatus = stat;
  mChanged = true;
}

// @brief 故障リストをスキャンして未検出リストを更新する．
void
FaultMgr::update()
{
  if ( mChanged ) {
    ymuint n = mRemainList.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < n; ++ rpos) {
      SaFault* f = mRemainList[rpos];
      switch ( f->status() ) {
      case kFsUndetected:
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;

      case kFsDetected:
	mDetList.push_back(f);
	break;

      case kFsUntestable:
	mUntestList.push_back(f);
	break;

      case kFsAborted:
	set_status(f, kFsUndetected);
	if ( wpos != rpos ) {
	  mRemainList[wpos] = f;
	}
	++ wpos;
	break;
      }
    }
    if ( wpos < n ) {
      mRemainList.erase(mRemainList.begin() + wpos, mRemainList.end());
    }
    mChanged = false;
  }
}

END_NAMESPACE_YM_SATPG
