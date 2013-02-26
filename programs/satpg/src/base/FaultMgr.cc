
/// @file src/base/FaultMgr.cc
/// @brief FaultMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "FaultMgr.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "TpgNetwork.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// ゲートタイプから代表故障を求める．
void
get_rep_faults(TpgNode* node,
	       TpgFault* f0,
	       TpgFault* f1,
	       TpgFault*& rep0,
	       TpgFault*& rep1)
{
  if ( node->is_input() ) {
    // どうでもいい
    return;
  }
  if ( node->is_output() ) {
    // バッファと同じ
    rep0 = f0;
    rep1 = f1;
    return;
  }

  tTgGateType type = node->gate_type();
  switch ( type ) {
  case kTgGateBuff:
    rep0 = f0;
    rep1 = f1;
    break;

  case kTgGateNot:
    rep0 = f1;
    rep1 = f0;
    break;

  case kTgGateAnd:
    rep0 = f0;
    rep1 = NULL;
    break;

  case kTgGateNand:
    rep0 = f1;
    rep1 = NULL;
    break;

  case kTgGateOr:
    rep0 = NULL;
    rep1 = f1;
    break;

  case kTgGateNor:
    rep0 = NULL;
    rep1 = f0;
    break;

  case kTgGateXor:
    rep0 = NULL;
    rep1 = NULL;
    break;

  case kTgGateCplx:
    // 面倒くさいので
    rep0 = NULL;
    rep1 = NULL;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
}

END_NONAMESPACE


// @brief コンストラクタ
FaultMgr::FaultMgr()
{
  mNetwork = NULL;

  mAllNum = 0;
  mFaultChunk = NULL;

  mRepNum = 0;
  mRepArray = NULL;

  mChanged = false;
}

// @brief デストラクタ
FaultMgr::~FaultMgr()
{
}

// @brief クリアする．
void
FaultMgr::clear()
{
  mNetwork = NULL;

  delete [] mFaultChunk;
  delete [] mRepArray;

  mDetList.clear();
  mRemainList.clear();
  mUntestList.clear();

  mChanged = true;
}

// @brief FaultMgr に全ての単一縮退故障を設定する．
void
FaultMgr::set_ssa_fault(TpgNetwork& network)
{
  clear();

  mNetwork = &network;

  ymuint nn = network.node_num();

  // 全故障数を数える．
  mAllNum = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = network.node(i);
    mAllNum += node->fanin_num() * 2 + 2;
  }

  // 故障を生成する．
  mFaultChunk = new TpgFault[mAllNum];
  ymuint fid = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    // 代表故障を等価故障のなかでもっとも出力よりの故障と定めるので
    // 出力側からのトポロジカル順に処理する．
    TpgNode* node = network.node(nn - i - 1);
    TpgFault* rep0 = NULL;
    TpgFault* rep1 = NULL;

    if ( node->fanout_num() == 1 ) {
      TpgNode* onode = node->fanout(0);
      // ちょっとかっこ悪い探し方
      ymuint ni = onode->fanin_num();
      ymuint ipos = ni;
      for (ymuint j = 0; j < ni; ++ j) {
	if ( onode->fanin(j) == node ) {
	  ipos = j;
	}
      }
      assert_cond( ipos < ni, __FILE__, __LINE__);
      rep0 = onode->input_fault(0, ipos);
      rep1 = onode->input_fault(1, ipos);
    }

    TpgFault* f0 = new_ofault(node, 0, rep0, fid);
    TpgFault* f1 = new_ofault(node, 1, rep1, fid);
    node->set_output_fault(0, f0);
    node->set_output_fault(1, f1);

    // ノードタイプから代表故障(r0, r1)をもとめる．
    get_rep_faults(node, f0, f1, rep0, rep1);

    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      node->set_input_fault(0, j, new_ifault(node, j, 0, rep0, fid));
      node->set_input_fault(1, j, new_ifault(node, j, 1, rep1, fid));
    }
  }
  assert_cond( fid == mAllNum, __FILE__, __LINE__);

  // 代表故障を記録していく．
  mRepNum = 0;
  for (ymuint i = 0; i < mAllNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      ++ mRepNum;
    }
  }
  mRepArray = new TpgFault*[mRepNum];
  ymuint pos = 0;
  for (ymuint i = 0; i < mAllNum; ++ i) {
    TpgFault* f = &mFaultChunk[i];
    if ( f->is_rep() ) {
      mRepArray[pos] = f;
      ++ pos;
    }
  }
  assert_cond( pos == mRepNum, __FILE__, __LINE__);

  // 最初はすべての故障が mRemainList に属する．
  for (ymuint i = 0; i < mRepNum; ++ i) {
    TpgFault* f = mRepArray[i];
    mRemainList.push_back(f);
  }
}

// @brief 故障を生成する．
// @param[in] node 対象のノード
// @param[in] is_output 出力の故障のときに true とするフラグ
// @param[in] pos 入力の故障の時に入力番号を表す
// @param[in] val 縮退している値
// @param[in] rep 代表故障
// @param[inout] fid 故障のID番号
// @note fid はこの関数呼び出し後に1つインクリメントされる．
TpgFault*
FaultMgr::new_fault(TpgNode* node,
		    bool is_output,
		    ymuint pos,
		    int val,
		    TpgFault* rep,
		    ymuint& fid)
{
  TpgFault* f = &mFaultChunk[fid];
  f->set(fid, node, is_output, pos, val, rep);
  ++ fid;
  return f;
}

// @brief fault の状態を変更する．
void
FaultMgr::set_status(TpgFault* fault,
		     FaultStatus stat)
{
  fault->mStatus = stat;
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
      TpgFault* f = mRemainList[rpos];
      switch ( f->status() ) {
      case kFsUndetected:
      case kFsSkipped:
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
