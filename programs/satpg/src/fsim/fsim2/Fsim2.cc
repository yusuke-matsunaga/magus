
/// @file Fsim2.cc
/// @brief Fsim2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim2.h"
#include "FsimOp.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "DetectOp.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

Fsim*
new_Fsim2()
{
  return new nsFsim2::Fsim2();
}

END_NAMESPACE_YM_SATPG


BEGIN_NAMESPACE_YM_SATPG_FSIM2

BEGIN_NONAMESPACE

void
clear_lobs(SimNode* node)
{
  while ( node->check_lobs() ) {
    node->clear_lobs();
    node = node->fanout(0);
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Fsim2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Fsim2::Fsim2()
{
}

// @brief デストラクタ
Fsim2::~Fsim2()
{
  clear();
}

// @brief ネットワークをセットする関数
// @param[in] network ネットワーク
void
Fsim2::set_network(const TpgNetwork& network)
{
  clear();

  mNetwork = &network;

  ymuint nn = mNetwork->node_num();
  ymuint ni = mNetwork->input_num2();
  ymuint no = mNetwork->output_num2();

  // SimNode の生成
  // 対応付けを行うマップの初期化
  mSimMap.resize(nn);
  mInputArray.resize(ni);
  mOutputArray.resize(no);

  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* tpgnode = mNetwork->node(i);
    SimNode* node = NULL;

    if ( tpgnode->is_input() ) {
      // 外部入力に対応する SimNode の生成
      node = make_input();
      mInputArray[tpgnode->input_id()] = node;
    }
    else if ( tpgnode->is_output() ) {
      // 外部出力に対応する SimNode の生成
      // 実際には外部出力のファンインに対応するノードに出力の印をつけるだけ．
      node = find_simnode(tpgnode->fanin(0));
      node->set_output();
      mOutputArray[tpgnode->output_id()] = node;
    }
    else if ( tpgnode->is_logic() ) {
      // 論理ノードに対する SimNode の作成
      ymuint ni = tpgnode->fanin_num();

      // ファンインに対応する SimNode を探す．
      vector<SimNode*> inputs(ni);
      for (ymuint i = 0; i < ni; ++ i) {
	const TpgNode* itpgnode = tpgnode->fanin(i);
	SimNode* inode = find_simnode(itpgnode);
	assert_cond(inode, __FILE__, __LINE__);
	inputs[i] = inode;
      }

      // 出力の論理を表す SimNode を作る．
      tTgGateType type = tpgnode->gate_type();
      node = make_node(type, inputs);
    }
    // 対応表に登録しておく．
    mSimMap[tpgnode->id()] = node;
  }

  // 各ノードのファンアウトリストの設定
  ymuint node_num = mNodeArray.size();
  {
    vector<vector<SimNode*> > fanout_lists(node_num);
    vector<ymuint> ipos(node_num);
    for (vector<SimNode*>::iterator p = mNodeArray.begin();
	 p != mNodeArray.end(); ++ p) {
      SimNode* node = *p;
      ymuint ni = node->nfi();
      for (ymuint i = 0; i < ni; ++ i) {
	SimNode* inode = node->fanin(i);
	fanout_lists[inode->id()].push_back(node);
	ipos[inode->id()] = i;
      }
    }
    for (ymuint i = 0; i < node_num; ++ i) {
      SimNode* node = mNodeArray[i];
      node->set_fanout_list(fanout_lists[i], ipos[i]);
    }
  }

  // FFR の設定
  ymuint ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      ++ ffr_num;
    }
  }
  mFFRArray.resize(ffr_num);
  ffr_num = 0;
  for (ymuint i = node_num; i > 0; ) {
    -- i;
    SimNode* node = mNodeArray[i];
    if ( node->is_output() || node->nfo() != 1 ) {
      SimFFR* ffr = &mFFRArray[ffr_num];
      node->set_ffr(ffr);
      ffr->set_root(node);
      ++ ffr_num;
    }
    else {
      SimFFR* ffr = node->fanout(0)->ffr();
      node->set_ffr(ffr);
    }
  }

  // 消去用の配列の大きさはノード数を越えない．
  mClearArray.reserve(mNodeArray.size());

  // 最大レベルを求め，イベントキューを初期化する．
  ymuint max_level = 0;
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);


  //////////////////////////////////////////////////////////////////////
  // 故障リストの設定
  //////////////////////////////////////////////////////////////////////
  const vector<TpgFault*>& rep_faults = network.rep_faults();
  ymuint nf = rep_faults.size();
  mSimFaults.resize(nf);
  mFaultArray.resize(network.max_fault_id());
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = rep_faults[i];
    const TpgNode* node = f->node();
    SimNode* simnode = find_simnode(node);
    ymuint ipos = 0;
    SimNode* isimnode = NULL;
    if ( f->is_input_fault() ) {
      ipos = f->pos();
      const TpgNode* inode = node->fanin(f->pos());
      isimnode = find_simnode(inode);
    }
    else {
      isimnode = simnode;
    }
    mSimFaults[i].set(f, simnode, ipos, isimnode);
    SimFault* ff = &mSimFaults[i];
    mFaultArray[f->id()] = ff;
  }
}

// @brief 故障にスキップマークをつける．
void
Fsim2::set_skip(TpgFault* f)
{
  mFaultArray[f->id()]->mSkip = true;
}

// @brief 故障リストを設定する．
// @param[in] fault_list 対象の故障リスト
//
// スキップマークは消される．
void
Fsim2::set_faults(const vector<TpgFault*>& fault_list)
{
  unordered_set<ymuint> fault_set;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    fault_set.insert(fault_list[i]->id());
  }

  // 同時に各 SimFFR 内の fault_list() も再構築する．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    p->fault_list().clear();
  }
  ymuint nf = mSimFaults.size();
  for (ymuint i = 0; i < nf; ++ i) {
    SimFault* ff = &mSimFaults[i];
    if ( fault_set.count(ff->mOrigF->id()) > 0 ) {
      ff->mSkip = false;
      SimNode* simnode = ff->mNode;
      SimFFR* ffr = simnode->ffr();
      ffr->fault_list().push_back(ff);
    }
    else {
      ff->mSkip = true;
    }
  }
}

// @brief ひとつのパタンで故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] op 検出した時に起動されるファンクタオブジェクト
void
Fsim2::sppfp(TestVector* tv,
	     FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval(val);
  }

  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  ymuint bitpos = 0;
  SimFFR* ffr_buff[kPvBitLen];
  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は SimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    SimNode* root = ffr->root();
    if ( root->is_output() ) {
      // 常に観測可能
      fault_sweep(ffr, op);
      continue;
    }

    // キューに積んでおく
    PackedVal bitmask = 1UL << bitpos;
    PackedVal pat = root->gval() ^ bitmask;
    root->set_fval(pat);
    root->set_fmask(~bitmask);

    mClearArray.push_back(root);
    ymuint no = root->nfo();
    for (ymuint i = 0; i < no; ++ i) {
      mEventQ.put(root->fanout(i));
    }
    ffr_buff[bitpos] = ffr;

    ++ bitpos;
    if ( bitpos == kPvBitLen ) {
      PackedVal obs = eventq_simulate();
      for (ymuint i = 0; i < kPvBitLen; ++ i, obs >>= 1) {
	if ( obs & 1UL ) {
	  fault_sweep(ffr_buff[i], op);
	}
      }
      bitpos = 0;
    }
  }
  if ( bitpos > 0 ) {
    PackedVal obs = eventq_simulate();
    for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
      if ( obs & 1UL ) {
	fault_sweep(ffr_buff[i], op);
      }
    }
  }
}

// @brief 複数のパタンで故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
void
Fsim2::ppsfp(const vector<TestVector*>& tv_array,
	     FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();
  ymuint nb = tv_array.size();

  // tv_array を入力ごとに固めてセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    PackedVal val = kPvAll0;
    PackedVal bit = 1UL;
    for (ymuint j = 0; j < nb; ++ j, bit <<= 1) {
      if ( tv_array[j]->val3(i) == kVal1 ) {
	val |= bit;
      }
    }
    // 残ったビットには 0 番めのパタンを詰めておく．
    if ( tv_array[0]->val3(i) == kVal1 ) {
      for (ymuint j = nb; j < kPvBitLen; ++ j, bit <<= 1) {
	val |= bit;
      }
    }
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(val);
  }

  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);
    if ( ffr->fault_list().empty() ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は SimFault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    // FFR の出力の故障伝搬を行う．
    SimNode* root = ffr->root();
    PackedVal obs = kPvAll0;
    if ( root->is_output() ) {
      obs = kPvAll1;
    }
    else {
      PackedVal pat = root->gval() ^ ffr_req;
      root->set_fval(pat);
      mClearArray.clear();
      mClearArray.push_back(root);
      ymuint no = root->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(root->fanout(i));
      }
      obs = eventq_simulate();
    }

    // obs と各々の故障の mObsMask との AND が 0 でなければ故障検出
    // できたということ．対応するテストベクタを記録する．
    vector<SimFault*>& flist = ffr->fault_list();
    for (vector<SimFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      SimFault* ff = *p;
      if ( ff->mSkip ) {
	continue;
      }
      PackedVal dpat = obs & ff->mObsMask;
      if ( dpat ) {
	TpgFault* f = ff->mOrigF;
	op(f, dpat);
      }
    }
  }
}

// @brief SPSFP故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
// @retval true 故障の検出が行えた．
// @retval false 故障の検出が行えなかった．
bool
Fsim2::spsfp(TestVector* tv,
	     TpgFault* f)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    PackedVal val = (tv->val3(i) == kVal1) ? kPvAll1 : kPvAll0;
    simnode->set_gval(val);
  }

  // 正常値の計算を行う．
  for (vector<SimNode*>::iterator q = mLogicArray.begin();
       q != mLogicArray.end(); ++ q) {
    SimNode* node = *q;
    node->calc_gval2();
  }

  // FFR 内の故障伝搬を行う．
  SimNode* simnode = find_simnode(f->node());
  PackedVal lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    lobs = simnode->calc_lobs() & simnode->calc_gobs2(ipos);
  }
  else {
    lobs = simnode->calc_lobs();
  }
  clear_lobs(simnode);

  SimNode* isimnode = find_simnode(f->source_node());
  PackedVal valdiff = isimnode->gval();
  if ( f->val() == 1 ) {
    valdiff = ~valdiff;
  }
  lobs &= valdiff;

  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs == kPvAll0 ) {
    return false;
  }

  SimNode* root = simnode->ffr()->root();
  if ( root->is_output() ) {
    return (lobs != kPvAll0);
  }

  root->set_fval(~root->gval());

  mClearArray.push_back(root);
  ymuint no = root->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(root->fanout(i));
  }

  PackedVal obs = eventq_simulate() & lobs;
  return (obs != kPvAll0);
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
Fsim2::clear()
{
  mSimMap.clear();

  // mNodeArray が全てのノードを持っている
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    delete *p;
  }
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();

  mFFRArray.clear();

  mClearArray.clear();

  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }

  mSimFaults.clear();
  mFaultArray.clear();

  // 念のため
  mNetwork = NULL;
}

// @brief FFR 内の故障シミュレーションを行う．
PackedVal
Fsim2::ffr_simulate(SimFFR* ffr)
{
  PackedVal ffr_req = kPvAll0;
  const vector<SimFault*>& flist = ffr->fault_list();
  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( ff->mSkip ) {
      continue;
    }

    // ff の故障伝搬を行う．
    SimNode* simnode = ff->mNode;
    PackedVal lobs = simnode->calc_lobs();
    PackedVal valdiff = ff->mInode->gval();
    TpgFault* f = ff->mOrigF;
    if ( f->is_input_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->calc_gobs2(ipos);
    }
    if ( f->val() == 1 ) {
      valdiff = ~valdiff;
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
  }

  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( !ff->mSkip ) {
      SimNode* node = ff->mNode;
      clear_lobs(node);
    }
  }

  return ffr_req;
}

// @brief イベントキューを用いてシミュレーションを行う．
PackedVal
Fsim2::eventq_simulate()
{
  PackedVal obs = kPvAll0;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    PackedVal diff = node->calc_fval2(~obs);
    if ( diff != kPvAll0 ) {
      mClearArray.push_back(node);
      if ( node->is_output() ) {
	obs |= diff;
      }
      else {
	ymuint no = node->nfo();
	for (ymuint i = 0; i < no; ++ i) {
	  mEventQ.put(node->fanout(i));
	}
      }
    }
  }
  // 今の故障シミュレーションで値の変わったノードを元にもどしておく
  for (vector<SimNode*>::iterator p = mClearArray.begin();
       p != mClearArray.end(); ++ p) {
    (*p)->clear_fval();
  }
  mClearArray.clear();
  return obs;
}

// @brief ffr 内の故障が検出可能か調べる．
void
Fsim2::fault_sweep(SimFFR* ffr,
		   FsimOp& op)
{
  vector<SimFault*>& flist = ffr->fault_list();
  for (vector<SimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    SimFault* ff = *p;
    if ( !ff->mSkip && ff->mObsMask != kPvAll0 ) {
      TpgFault* f = ff->mOrigF;
      op(f, kPvAll1);
    }
  }
}

// @brief 外部入力ノードを作る．
SimNode*
Fsim2::make_input()
{
  ymuint32 id = mNodeArray.size();
  SimNode* node = SimNode::new_input(id);
  mNodeArray.push_back(node);
  return node;
}

// @brief 単純な logic ノードを作る．
SimNode*
Fsim2::make_node(tTgGateType type,
		 const vector<SimNode*>& inputs)
{
  ymuint32 id = mNodeArray.size();
  SimNode* node = SimNode::new_node(id, type, inputs);
  mNodeArray.push_back(node);
  mLogicArray.push_back(node);
  return node;
}

// @brief node に対応する SimNode* を得る．
SimNode*
Fsim2::find_simnode(const TpgNode* node) const
{
  return mSimMap[node->id()];
}

END_NAMESPACE_YM_SATPG_FSIM2
