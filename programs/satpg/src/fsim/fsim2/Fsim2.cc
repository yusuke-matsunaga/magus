
/// @file Fsim2.cc
/// @brief Fsim2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim2.h"
#include "FsimOp1.h"
#include "FsimOp2.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TestVector.h"
#include "DetectOp.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "utils/StopWatch.h"


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
// @param[in] fault_mgr 故障マネージャ
void
Fsim2::set_network(const TpgNetwork& network,
		   FaultMgr& fault_mgr)
{
  clear();

  mNetwork = &network;

  ymuint nn = mNetwork->node_num();
  ymuint ni = mNetwork->input_num2();
  ymuint no = mNetwork->output_num2();

  // SimNode の生成
  // 対応付けを行うマップの初期化
  mSimMap.resize(nn);
  mEdgeMap.resize(nn);
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
      mEdgeMap[tpgnode->id()].resize(ni);
      if ( tpgnode->is_cplx_logic() ) {
	// 複数のプリミティブで構成されるノードの場合
	// 基本的にはプリミティブを SimNode に対応させればよいが，
	// ノードとしてのファンインの枝に対応するプリミティブのファンインが
	// 複数存在する時に対応付けができないので，ダミーのバッファノードを
	// 挿入する．
	ymuint np = tpgnode->primitive_num();
	vector<ymuint> input_count(ni * 2, 0);
	for (ymuint pid = 0; pid < np; ++ pid) {
	  const TpgPrimitive* prim = tpgnode->primitive(pid);
	  if ( prim->is_input() ) {
	    // 入力プリミティブの場合
	    ymuint iid = prim->input_id();
	    ++ input_count[iid * 2 + 0];
	  }
	  else if ( prim->is_not_input() ) {
	    // 否定付き入力プリミティブの場合
	    ymuint iid = prim->input_id();
	    ++ input_count[iid * 2 + 1];
	  }
	}

	// 各入力の使われ方をチェック
	vector<SimNode*> inputs2(ni * 2, NULL);
	vector<EdgeMap*> emap(ni, NULL);
	for (ymuint i = 0; i < ni; ++ i) {
	  ymuint np = input_count[i * 2 + 0];
	  ymuint nn = input_count[i * 2 + 1];
	  EdgeMap& edge_map = mEdgeMap[tpgnode->id()][i];
	  if ( np == 1 && nn == 0 ) {
	    // - A) 肯定リテラルが 1 つ．
	    inputs2[i * 2 + 0] = inputs[i];
	    emap[i] = &edge_map;
	  }
	  else if ( np > 1 && nn == 0 ) {
	    // - B) 肯定リテラルが 2 つ以上
	    // ダミーのバッファノードを作る．
	    vector<SimNode*> tmp(1, inputs[i]);
	    SimNode* buf = make_node(kTgGateBuff, tmp);
	    inputs2[i * 2 + 0] = buf;
	    edge_map.mNode = buf;
	    edge_map.mPos = 0;
	  }
	  else if ( np == 0 && nn > 0 ) {
	    // - C) 否定リテラルのみ．数は問わない．
	    // NOTノードを作る．
	    vector<SimNode*> tmp(1, inputs[i]);
	    SimNode* inv = make_node(kTgGateNot, tmp);
	    inputs2[i * 2 + 1] = inv;
	    edge_map.mNode = inv;
	    edge_map.mPos = 0;
	  }
	  else if ( np > 0 && nn > 0 ) {
	    // - D) 肯定と否定リテラルが各々 1 つ以上
	    // ダミーのバッファとNOTノードを作る．
	    vector<SimNode*> tmp(1, inputs[i]);
	    SimNode* buf = make_node(kTgGateBuff, tmp);
	    tmp[0] = buf;
	    SimNode* inv = make_node(kTgGateNot, tmp);
	    inputs2[i * 2 + 0] = buf;
	    inputs2[i * 2 + 1] = inv;
	    edge_map.mNode = buf;
	    edge_map.mPos = 0;
	  }
	  else {
	    cout << "np = " << np << endl
		 << "nn = " << nn << endl;
	    assert_not_reached(__FILE__, __LINE__);
	  }
	}

	// 各プリミティブに対応したノードを作る．
	node = make_primitive(tpgnode->primitive(np - 1), inputs2, emap);
      }
      else {
	tTgGateType type = tpgnode->gate_type();
	node = make_node(type, inputs);
	for (ymuint i = 0; i < ni; ++ i) {
	  EdgeMap& edge_map = mEdgeMap[tpgnode->id()][i];
	  edge_map.mNode = node;
	  edge_map.mPos = i;
	}
      }
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

  clear_faults();

  ymuint nf = fault_mgr.rep_num();
  mFsimFaults.resize(nf);
  mFaultArray.resize(fault_mgr.all_num());
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = fault_mgr.rep_fault(i);
    const TpgNode* node = f->node();
    SimNode* simnode = NULL;
    ymuint ipos = 0;
    SimNode* isimnode = NULL;
    if ( f->is_input_fault() ) {
      find_simedge(node, f->pos(), simnode, ipos);
      const TpgNode* inode = node->fanin(f->pos());
      isimnode = find_simnode(inode);
    }
    else {
      simnode = find_simnode(node);
      isimnode = simnode;
    }
    mFsimFaults[i].set(f, simnode, ipos, isimnode);
    SimFFR* ffr = simnode->ffr();
    FsimFault* ff = &mFsimFaults[i];
    ffr->fault_list().push_back(ff);
    mFaultArray[f->id()] = ff;
  }
}

// @brief 故障にスキップマークをつける．
void
Fsim2::set_skip(TpgFault* f)
{
  mFaultArray[f->id()]->mSkip = true;
}

// @brief すべての故障のスキップマークを消す．
void
Fsim2::clear_skip()
{
  ymuint n = mFaultArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( mFaultArray[i] ) {
      mFaultArray[i]->mSkip = false;
    }
  }
}

// @brief ひとつのパタンで故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] det_faults このパタンで検出された故障のリスト
void
Fsim2::sppfp(TestVector* tv,
	     vector<TpgFault*>& det_faults)
{
  det_faults.clear();

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
    // 結果は FsimFault.mObsMask に保存される．
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
      fault_sweep(ffr, det_faults);
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
	  fault_sweep(ffr_buff[i], det_faults);
	}
      }
      bitpos = 0;
    }
  }
  if ( bitpos > 0 ) {
    PackedVal obs = eventq_simulate();
    for (ymuint i = 0; i < bitpos; ++ i, obs >>= 1) {
      if ( obs & 1UL ) {
	fault_sweep(ffr_buff[i], det_faults);
      }
    }
  }
}

// @brief 複数のパタンで故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
void
Fsim2::ppsfp(const vector<TestVector*>& tv_array,
	     FsimOp2& op)
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
    // 結果は FsimFault.mObsMask に保存される．
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
    vector<FsimFault*>& flist = ffr->fault_list();
    for (vector<FsimFault*>::const_iterator p = flist.begin();
	 p != flist.end(); ++ p) {
      FsimFault* ff = *p;
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
  SimNode* simnode = NULL;
  PackedVal lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = 0;
    find_simedge(f->node(), f->pos(), simnode, ipos);
    lobs = simnode->calc_lobs() & simnode->calc_gobs2(ipos);
  }
  else {
    simnode = find_simnode(f->node());
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
  clear_faults();

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

  // 念のため
  mNetwork = NULL;
}

// @brief FsimFault を破棄する．
void
Fsim2::clear_faults()
{
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }

  mFsimFaults.clear();
  mFaultArray.clear();
}

// @brief FFR 内の故障シミュレーションを行う．
PackedVal
Fsim2::ffr_simulate(SimFFR* ffr)
{
  PackedVal ffr_req = kPvAll0;
  const vector<FsimFault*>& flist = ffr->fault_list();
  for (vector<FsimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    FsimFault* ff = *p;
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

  for (vector<FsimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    FsimFault* ff = *p;
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
		   vector<TpgFault*>& det_faults)
{
  vector<FsimFault*>& flist = ffr->fault_list();
  for (vector<FsimFault*>::const_iterator p = flist.begin();
       p != flist.end(); ++ p) {
    FsimFault* ff = *p;
    if ( !ff->mSkip && ff->mObsMask != kPvAll0 ) {
      TpgFault* f = ff->mOrigF;
      det_faults.push_back(f);
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

// @brief プリミティブに対応したノードを作る．
// @param[in] prim プリミティブ
// @param[in] inputs もとのノードの入力の SimNode
// @param[in] emap もとのノードの枝の対応関係を記録する配列
// @note inputs のサイズはノードの入力数 x 2
SimNode*
Fsim2::make_primitive(const TpgPrimitive* prim,
		      const vector<SimNode*>& inputs,
		      const vector<EdgeMap*>& emap)
{
  if ( prim->is_input() ) {
    // 入力の場合 inputs に登録されているノードを返す．
    ymuint iid = prim->input_id();
    return inputs[iid * 2 + 0];
  }

  if ( prim->is_not_input() ) {
    // 否定付き入力の場合 inputs に登録されているノードを返す．
    ymuint iid = prim->input_id();
    return inputs[iid * 2 + 1];
  }

  assert_cond( prim->is_logic(), __FILE__, __LINE__);

  // ファンインのノードを作る．
  ymuint ni = prim->fanin_num();
  vector<SimNode*> tmp_inputs(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgPrimitive* iprim = prim->fanin(i);
    SimNode* inode = make_primitive(iprim, inputs, emap);
    tmp_inputs[i] = inode;
  }
  SimNode* node = make_node(prim->gate_type(), tmp_inputs);

  // ファンインが入力プリミティブかつ対応する emap が NULL でなければ
  // EdgeMap の設定を行う．
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgPrimitive* iprim = prim->fanin(i);
    if ( iprim->is_input() ) {
      ymuint iid = iprim->input_id();
      if ( emap[iid] != NULL ) {
	emap[iid]->mNode = node;
	emap[iid]->mPos = i;
      }
    }
  }

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

// @brief node の pos 番めの入力に対応する枝を得る．
void
Fsim2::find_simedge(const TpgNode* node,
		    ymuint pos,
		    SimNode*& simnode,
		    ymuint& ipos) const
{
  const EdgeMap& edge_map = mEdgeMap[node->id()][pos];
  simnode = edge_map.mNode;
  ipos = edge_map.mPos;
}

END_NAMESPACE_YM_SATPG_FSIM2
