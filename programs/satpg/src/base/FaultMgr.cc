
/// @file FaultMgr.cc
/// @brief FaultMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2012-2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FaultMgr.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "TpgNetwork.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

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
    if ( !node->is_output() ) {
      mAllNum += node->fanin_num() * 2 + 2;
    }
  }

  // 故障を生成する．
  mFaultChunk = new TpgFault[mAllNum];
  ymuint fid = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    // 代表故障を等価故障のなかでもっとも出力よりの故障と定めるので
    // 出力側からのトポロジカル順に処理する．
    TpgNode* node = network.node(nn - i - 1);
    if ( node->is_output() ) {
      continue;
    }

    TpgFault* rep0 = NULL;
    TpgFault* rep1 = NULL;

    if ( node->fanout_num() == 1 && !node->fanout(0)->is_output() ) {
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

    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* rep0 = NULL;
      Bool3 oval0 = node->c_val(j, kB3False);
      if ( oval0 == kB3False ) {
	rep0 = f0;
      }
      else if ( oval0 == kB3True ) {
	rep0 = f1;
      }
      node->set_input_fault(0, j, new_ifault(node, j, 0, rep0, fid));

      TpgFault* rep1 = NULL;
      Bool3 oval1 = node->c_val(j, kB3True);
      if ( oval1 == kB3False ) {
	rep1 = f0;
      }
      else if ( oval1 == kB3True ) {
	rep1 = f1;
      }
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
    // ただし，外部出力に到達できない故障は検出不能となる．
    if ( f->node()->is_active() ) {
      mRemainList.push_back(f);
    }
    else {
      mUntestList.push_back(f);
    }
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
