
/// @file Fsim3.cc
/// @brief Fsim3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim3.h"
#include "FsimOp.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "YmUtils/HashSet.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

Fsim*
new_Fsim3()
{
  return new nsFsim3::Fsim3();
}

END_NAMESPACE_YM_SATPG

BEGIN_NAMESPACE_YM_SATPG_FSIM

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
// Fsim
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Fsim3::Fsim3()
{
}

// @brief デストラクタ
Fsim3::~Fsim3()
{
  clear();
}

// @brief ネットワークをセットする．
// @param[in] network ネットワーク
void
Fsim3::set_network(const TpgNetwork& network)
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
      // 論理ノードに対応する SimNode の生成
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
    // 対応表に登録しておく
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
  mGvalClearArray.reserve(mNodeArray.size());
  mFvalClearArray.reserve(mNodeArray.size());

  // 最大レベルを求め，イベントキューを初期化する．
  ymuint max_level = 0;
  for (ymuint i = 0; i < no; ++ i) {
    SimNode* inode = mOutputArray[i];
    if ( max_level < inode->level() ) {
      max_level = inode->level();
    }
  }
  mEventQ.init(max_level);

  // 値を X に初期化しておく
  for (vector<SimNode*>::iterator p = mNodeArray.begin();
       p != mNodeArray.end(); ++ p) {
    SimNode* simnode = *p;
    simnode->set_gval(kPvAll0, kPvAll0);
  }


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
Fsim3::set_skip(TpgFault* f)
{
  mFaultArray[f->id()]->mSkip = true;
}

// @brief 故障リストを設定する．
// @param[in] fault_list 対象の故障リスト
//
// スキップマークは消される．
void
Fsim3::set_faults(const vector<TpgFault*>& fault_list)
{
  HashSet<ymuint> fault_set;
  for (ymuint i = 0; i < fault_list.size(); ++ i) {
    fault_set.add(fault_list[i]->id());
  }

  // 同時に各 SimFFR 内の fault_list() も再構築する．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    p->fault_list().clear();
  }
  ymuint nf = mSimFaults.size();
  for (ymuint i = 0; i < nf; ++ i) {
    SimFault* ff = &mSimFaults[i];
    if ( fault_set.check(ff->mOrigF->id()) ) {
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

// @brief SPSFP故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] f 対象の故障
// @retval true 故障の検出が行えた．
// @retval false 故障の検出が行えなかった．
bool
Fsim3::spsfp(TestVector* tv,
	     TpgFault* f)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  mGvalClearArray.clear();
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    switch ( tv->val3(i) ) {
    case kVal0: simnode->set_gval(kPvAll1, kPvAll0); break;
    case kVal1: simnode->set_gval(kPvAll0, kPvAll1); break;
    case kValX: continue;
    }
    update_gval(simnode);
  }

  // 正常値の計算を行う．
  calc_gval();

  // FFR 内の故障伝搬を行う．
  SimNode* simnode = find_simnode(f->node());
  PackedVal lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = f->pos();
    lobs = simnode->calc_lobs() & simnode->calc_gobs3(ipos);
  }
  else {
    lobs = simnode->calc_lobs();
  }
  clear_lobs(simnode);

  SimNode* isimnode = find_simnode(f->source_node());
  PackedVal valdiff;
  if ( f->val() == 1 ) {
    valdiff = isimnode->gval_0();
  }
  else {
    valdiff = isimnode->gval_1();
  }
  lobs &= valdiff;

  bool ans = false;

  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs != kPvAll0 ) {
    SimNode* root = simnode->ffr()->root();
    if ( root->is_output() ) {
      ans = (lobs != kPvAll0);
    }
    else {
      PackedVal gval0 = root->gval_0();
      PackedVal gval1 = root->gval_1();
      PackedVal fval0 = (gval0 & ~lobs) | (gval1 & lobs);
      PackedVal fval1 = (gval1 & ~lobs) | (gval0 & lobs);
      root->set_fval(fval0, fval1);
      update_fval(root);

      PackedVal obs = calc_fval() & lobs;
      ans = (obs != kPvAll0);
    }
  }

  // 値をクリアする．
  clear_gval();

  return ans;
}

// @brief ひとつのパタンで故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] op 検出した時に起動されるファンクタオブジェクト
void
Fsim3::sppfp(TestVector* tv,
	     FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();

  // tv を全ビットにセットしていく．
  mGvalClearArray.clear();
  {
    for (ymuint i = 0; i < npi; ++ i) {
      SimNode* simnode = mInputArray[i];
      switch ( tv->val3(i) ) {
      case kVal0: simnode->set_gval(kPvAll1, kPvAll0); break;
      case kVal1: simnode->set_gval(kPvAll0, kPvAll1); break;
      case kValX: continue; // 直前の clear_gval() で X になっているはず．
      }
      update_gval(simnode);
    }
  }

  // 正常値の計算を行う．
  calc_gval();

  ymuint bitpos = 0;
  SimFFR* ffr_buff[kPvBitLen];
  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);

    SimNode* root = ffr->root();
    PackedVal gval0 = root->gval_0();
    PackedVal gval1 = root->gval_1();
    // FFR の根の値が X なら故障の検出はできない．
    if ( (gval0 | gval1) == kPvAll0 ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は Fsim3Fault.mObsMask に保存される．
    // FFR 内の全ての obs マスクの OR を ffr_req に入れる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    if ( root->is_output() ) {
      // 常に観測可能
      fault_sweep(ffr, op);
      continue;
    }

    // この FFR の root の故障伝搬をシミュレートする必要があるので
    // キューに積んでおく
    PackedVal bitmask = 1UL << bitpos;
    PackedVal fval0 = (gval0 & ~bitmask) | (gval1 & bitmask);
    PackedVal fval1 = (gval1 & ~bitmask) | (gval0 & bitmask);
    root->set_fval(fval0, fval1);
    root->set_fmask(~bitmask);

    update_fval(root);
    ffr_buff[bitpos] = ffr;

    ++ bitpos;
    if ( bitpos == kPvBitLen ) {
      // kPvBitLen だけ故障がたまったのでシミュレートする．
      PackedVal obs = calc_fval();
      for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
	if ( obs & 1UL ) {
	  fault_sweep(ffr_buff[i], op);
	}
      }
      bitpos = 0;
    }
  }
  if ( bitpos > 0 ) {
    // キューに残っている故障をシミュレートする．
    PackedVal obs = calc_fval();
    for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
      if ( obs & 1UL ) {
	fault_sweep(ffr_buff[i], op);
      }
    }
  }

  // 値をクリアする．
  clear_gval();
}

// @brief 複数のパタンで故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
void
Fsim3::ppsfp(const vector<TestVector*>& tv_array,
	     FsimOp& op)
{
  ymuint npi = mNetwork->input_num2();
  ymuint nb = tv_array.size();

  // tv_array を入力ごとに固めてセットしていく．
  mGvalClearArray.clear();
  for (ymuint i = 0; i < npi; ++ i) {
    PackedVal val_0 = kPvAll0;
    PackedVal val_1 = kPvAll0;
    PackedVal bit = 1UL;
    for (ymuint j = 0; j < nb; ++ j, bit <<= 1) {
      switch ( tv_array[j]->val3(i) ) {
      case kVal0:
	val_0 |= bit;
	break;

      case kVal1:
	val_1 |= bit;
	break;

      default:
	break;
      }
    }

    // 残ったビットには 0 番めのパタンを詰めておく．
    // これは無駄なイベントを発生させないため．
    switch ( tv_array[0]->val3(i) ) {
    case kVal0:
      for (ymuint j = nb; j < kPvBitLen; ++ j, bit <<= 1) {
	val_0 |= bit;
      }
      break;

    case kVal1:
      for (ymuint j = nb; j < kPvBitLen; ++ j, bit <<= 1) {
	val_1 |= bit;
      }
      break;

    default:
      break;

    }
    SimNode* simnode = mInputArray[i];
    if ( (val_0 | val_1) != kPvAll0 ) {
      simnode->set_gval(val_0, val_1);
      update_gval(simnode);
    }
  }

  // 正常値の計算を行う．
  calc_gval();

  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR* ffr = &(*p);

    SimNode* root = ffr->root();
    PackedVal gval0 = root->gval_0();
    PackedVal gval1 = root->gval_1();
    // FFR の根の値が X なら故障の検出はできない．
    if ( (gval0 | gval1) == kPvAll0 ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は Fsim3Fault.mObsMask に保存される．
    // FFR 内の全ての obs マスクの OR を ffr_req に入れる．
    PackedVal ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == kPvAll0 ) {
      continue;
    }

    // FFR の出力の故障伝搬を行う．
    PackedVal obs = kPvAll0;
    if ( root->is_output() ) {
      obs = kPvAll1;
    }
    else {
      mFvalClearArray.clear();
      // FFR 内で必要とされているビットだけ反転させる．
      PackedVal gval0 = root->gval_0();
      PackedVal gval1 = root->gval_1();
      PackedVal fval0 = (ffr_req & gval1) | (~ffr_req & gval0);
      PackedVal fval1 = (ffr_req & gval0) | (~ffr_req & gval1);
      root->set_fval(fval0, fval1);
      update_fval(root);
      obs = calc_fval();
    }

    // obs と各々の故障の mObsMask との AND が 0 でなければ故障検出
    // できたということ．対応するテストベクタを記録する．
    const vector<SimFault*>& flist = ffr->fault_list();
    for (vector<SimFault*>::const_iterator p_ff = flist.begin();
	 p_ff != flist.end(); ++ p_ff) {
      SimFault* ff = *p_ff;
      PackedVal dbits = obs & ff->mObsMask;
      if ( dbits ) {
	TpgFault* f = ff->mOrigF;
	op(f, dbits);
      }
    }
  }

  // 値をクリアする．
  clear_gval();
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
Fsim3::clear()
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

  mGvalClearArray.clear();
  mFvalClearArray.clear();

  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    p->fault_list().clear();
  }

  mSimFaults.clear();
  mFaultArray.clear();


  // 念のため
  mNetwork = NULL;
}

// @brief FFR 内の故障シミュレーションを行う．
// @param[in] ffr 対象のFFR
//
// ffr 内の対象故障に対して故障が伝搬するかを調べる．
// 結果は各故障の mObsMask に設定される．
// また，すべての mObsMask の bitwise-OR を返す．
//
// 故障は SimFFR::fault_list() に格納されているが，
// スキップフラグが立った故障はリストから取り除かれる．
PackedVal
Fsim3::ffr_simulate(SimFFR* ffr)
{
  PackedVal ffr_req = kPvAll0;
  vector<SimFault*>& flist = ffr->fault_list();
  ymuint fnum = flist.size();
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    SimFault* ff = flist[rpos];
    TpgFault* f = ff->mOrigF;
    if ( ff->mSkip ) {
      continue;
    }
    if ( wpos != rpos ) {
      flist[wpos] = ff;
    }
    ++ wpos;

    // ff の故障伝搬を行う．
    SimNode* simnode = ff->mNode;
    PackedVal lobs = simnode->calc_lobs();
    SimNode* isimnode = ff->mInode;
    if ( f->is_input_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      lobs &= simnode->calc_gobs3(ipos);
    }

    PackedVal valdiff;
    if ( f->val() == 1 ) {
      valdiff = isimnode->gval_0();
    }
    else {
      valdiff = isimnode->gval_1();
    }
    lobs &= valdiff;

    ff->mObsMask = lobs;
    ffr_req |= lobs;
  }

  if ( wpos < fnum ) {
    // flist を切り詰める．
    flist.erase(flist.begin() + wpos, flist.end());
    fnum = wpos;
  }

  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    SimFault* ff = flist[rpos];
    SimNode* node = ff->mNode;
    clear_lobs(node);
  }

  return ffr_req;
}

// @brief 正常値の計算を行う．
// @note 値の変わったノードは mGvalClearArray に積まれる．
void
Fsim3::calc_gval()
{
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    if ( node->calc_gval3() ) {
      update_gval(node);
    }
  }
}

// @brief 正常値をクリアする．
// @note mGvalClearArray を使う．
void
Fsim3::clear_gval()
{
  for (vector<SimNode*>::iterator p = mGvalClearArray.begin();
       p != mGvalClearArray.end(); ++ p) {
    SimNode* node = *p;
    node->set_gval(kPvAll0, kPvAll0);
  }
}

// @brief イベントキューを用いてシミュレーションを行う．
PackedVal
Fsim3::calc_fval()
{
  PackedVal obs = kPvAll0;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    PackedVal diff = node->calc_fval3(~obs);
    if ( diff != kPvAll0 ) {
      mFvalClearArray.push_back(node);
      if ( node->is_output() ) {
	PackedVal gval0 = node->gval_0();
	PackedVal gval1 = node->gval_1();
	PackedVal fval0 = node->fval_0();
	PackedVal fval1 = node->fval_1();
	obs |= (gval0 ^ fval0) & (gval1 ^ fval1);
	if ( obs == kPvAll1 ) {
	  break;
	}
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
  for (vector<SimNode*>::iterator p = mFvalClearArray.begin();
       p != mFvalClearArray.end(); ++ p) {
    (*p)->clear_fval();
  }
  mFvalClearArray.clear();
  return obs;
}

// @brief ffr 内の故障が検出可能か調べる．
// @param[in] ffr 対象の FFR
// @param[in] op 検出した時に起動されるファンクタオブジェクト
//
// ここでは各FFR の fault_list() は変化しない．
void
Fsim3::fault_sweep(SimFFR* ffr,
		   FsimOp& op)
{
  const vector<SimFault*>& flist = ffr->fault_list();
  ymuint fnum = flist.size();
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    SimFault* ff = flist[rpos];
    if ( ff->mObsMask ) {
      TpgFault* f = ff->mOrigF;
      op(f, kPvAll1);
    }
  }
}

// @brief 外部入力ノードを作る．
SimNode*
Fsim3::make_input()
{
  ymuint32 id = mNodeArray.size();
  SimNode* node = SimNode::new_input(id);
  mNodeArray.push_back(node);
  return node;
}

// @brief 単純な logic ノードを作る．
SimNode*
Fsim3::make_node(tTgGateType type,
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
Fsim3::find_simnode(const TpgNode* node) const
{
  return mSimMap[node->id()];
}

END_NAMESPACE_YM_SATPG_FSIM
