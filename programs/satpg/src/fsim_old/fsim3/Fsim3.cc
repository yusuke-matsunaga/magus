
/// @file src/fsim3/Fsim3.cc
/// @brief Fsim3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "Fsim3.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgPrimitive.h"
#include "TpgFault.h"
#include "FaultMgr.h"
#include "TestVector.h"
#include "SimNode.h"
#include "SimFFR.h"
#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

FsimOld*
new_FsimOld3()
{
  return new nsFsimOld3::Fsim3();
}

END_NAMESPACE_YM_SATPG

BEGIN_NAMESPACE_YM_SATPG_FSIM3

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
// @param[in] fault_mgr 故障マネージャ
void
Fsim3::set_network(const TpgNetwork& network,
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

	// 各変数の使われ方をチェック
	vector<SimNode*> inputs2(ni * 2);
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
    simnode->set_gval(kValX);
  }


  //////////////////////////////////////////////////////////////////////
  // 故障リストの設定
  //////////////////////////////////////////////////////////////////////

  clear_faults();

  ymuint nf = fault_mgr.rep_num();
  mFsimFaults.resize(nf);
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
    ffr->fault_list().push_back(&mFsimFaults[i]);
  }
}

// @brief SPPFP故障シミュレーションを行う．
// @param[in] tv テストベクタ
// @param[in] det_faults 検出された故障を格納するリスト
void
Fsim3::sppfp(TestVector* tv,
	     vector<TpgFault*>& det_faults)
{
  det_faults.clear();

  ymuint npi = mNetwork->input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(tv->val3(i));
  }

  // 正常値の計算を行う．
  calc_gval();

  // FFR ごとに処理を行う．
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    SimFFR& ffr = *p;
    if ( ffr.fault_list().empty() ) continue;

    SimNode* root = ffr.root();
    Val3 gval = root->gval();

    // FFR の根の値が X なら故障の検出はできない．
    if ( gval == kValX ) continue;

    // FFR 内の故障伝搬を行う．
    // 結果は Fsim3Fault.mObsMask に保存される．
    // FFR 内の全ての obs マスクを ffr_req に入れる．
    // 検出済みの故障は ffr->fault_list() から取り除かれる．
    bool ffr_req = ffr_simulate(ffr);

    // ffr_req が 0 ならその後のシミュレーションを行う必要はない．
    if ( ffr_req == false ) {
      continue;
    }

    if ( !root->is_output() ) {
      // ffr から外部出力までの故障シミュレーションを行う．
      bool obs = calc_fval(root);
      if ( !obs ) {
	continue;
      }
    }

    for (vector<FsimFault*>::iterator p = mDetFaults.begin();
	 p != mDetFaults.end(); ++ p) {
      FsimFault* ff = *p;
      TpgFault* f = ff->mOrigF;
      det_faults.push_back(f);
    }
  }

  // 値をクリアする．
  clear_gval();
}

// @brief PPSFP故障シミュレーションを行う．
// @param[in] tv_array テストベクタの配列
// @param[in] det_faults 検出された故障を格納するリストの配列
void
Fsim3::ppsfp(const vector<TestVector*>& tv_array,
	     vector<vector<TpgFault*> >& det_faults)
{
  ymuint nb = tv_array.size();
  assert_cond(det_faults.size() >= nb, __FILE__, __LINE__);

  // このクラスは複数パタンを扱えない．
  for (ymuint i = 0; i < nb; ++ i) {
    sppfp(tv_array[i], det_faults[i]);
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

  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* simnode = mInputArray[i];
    simnode->set_gval(tv->val3(i));
  }

  // 正常値の計算を行う．
  calc_gval();

  // FFR 内の故障伝搬を行う．
  SimNode* simnode = NULL;
  bool lobs;
  if ( f->is_input_fault() ) {
    ymuint ipos = 0;
    find_simedge(f->node(), f->pos(), simnode, ipos);
    lobs = simnode->calc_lobs() & simnode->calc_gobs3(ipos);
  }
  else {
    simnode = find_simnode(f->node());
    lobs = simnode->calc_lobs();
  }
  clear_lobs(simnode);

  SimNode* isimnode = find_simnode(f->source_node());
  Val3 gval = isimnode->gval();
  if ( gval == kValX ) {
    return false;
  }

  bool valdiff = (gval != f->val3());
  lobs &= valdiff;

  bool ans = false;

  // lobs が 0 ならその後のシミュレーションを行う必要はない．
  if ( lobs ) {
    SimNode* root = simnode->ffr()->root();
    Val3 gval = root->gval();
    if ( gval == kValX ) {
      ans = false;
    }
    else if ( root->is_output() ) {
      ans = true;
    }
    else {
      ans = calc_fval(root);
    }
  }

  // 値をクリアする．
  clear_gval();

  return ans;
}

// @brief 現在保持している SimNode のネットワークを破棄する．
void
Fsim3::clear()
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

  mGvalClearArray.clear();
  mFvalClearArray.clear();


  // 念のため
  mNetwork = NULL;
}

// @brief FsimFault を破棄する．
void
Fsim3::clear_faults()
{
  for (vector<SimFFR>::iterator p = mFFRArray.begin();
       p != mFFRArray.end(); ++ p) {
    (*p).fault_list().clear();
  }

  mFsimFaults.clear();
}

// @brief 正常値の計算を行う．
// @note 値の変わったノードは mGvalClearArray に積まれる．
void
Fsim3::calc_gval()
{
  mGvalClearArray.clear();
  ymuint npi = mNetwork->input_num2();
  for (ymuint i = 0; i < npi; ++ i) {
    SimNode* node = mInputArray[i];
    Val3 gval = node->gval();
    if ( gval != kValX ) {
      mGvalClearArray.push_back(node);
      ymuint no = node->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(node->fanout(i));
      }
    }
  }
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    if ( node->calc_gval3() ) {
      mGvalClearArray.push_back(node);
      ymuint no = node->nfo();
      for (ymuint i = 0; i < no; ++ i) {
	mEventQ.put(node->fanout(i));
      }
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
    node->set_gval(kValX);
  }
}

// @brief 内部の故障リストの更新を行なう．
void
Fsim3::update_faults()
{
  ymuint nffr = mFFRArray.size();
  for (ymuint i = 0; i < nffr; ++ i) {
    SimFFR& ffr = mFFRArray[i];
    vector<FsimFault*>& flist = ffr.fault_list();
    ymuint fnum = flist.size();
    ymuint wpos = 0;
    for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
      FsimFault* ff = flist[rpos];
      TpgFault* f = ff->mOrigF;
      if ( f->status() != kFsDetected ) {
	if ( wpos != rpos ) {
	  flist[wpos] = ff;
	}
	++ wpos;
      }
    }
    if ( wpos < fnum ) {
      flist.erase(flist.begin() + wpos, flist.end());
    }
  }
}

// @brief FFR 内の故障シミュレーションを行う．
bool
Fsim3::ffr_simulate(SimFFR& ffr)
{
  vector<FsimFault*>& flist = ffr.fault_list();
  ymuint fnum = flist.size();
  ymuint wpos = 0;
  mDetFaults.clear();
  mDetFaults.reserve(fnum);
  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    FsimFault* ff = flist[rpos];
    TpgFault* f = ff->mOrigF;
    FaultStatus fs = f->status();
    if ( fs == kFsDetected ) {
      continue;
    }

    if ( wpos != rpos ) {
      flist[wpos] = ff;
    }
    ++ wpos;

    // ff の故障伝搬を行う．
    SimNode* simnode = ff->mNode;
    Val3 igval = ff->mInode->gval();
    if ( igval != ~f->val3() ) {
      continue;
    }

    bool lobs = simnode->calc_lobs();
    if ( !lobs ) {
      continue;
    }

    if ( f->is_input_fault() ) {
      // 入力の故障
      ymuint ipos = ff->mIpos;
      if ( !simnode->calc_gobs3(ipos) ) {
	continue;
      }
    }
    mDetFaults.push_back(ff);
  }

  if ( wpos < fnum ) {
    // flist を切り詰める．
    flist.erase(flist.begin() + wpos, flist.end());
    fnum = wpos;
  }

  for (ymuint rpos = 0; rpos < fnum; ++ rpos) {
    FsimFault* ff = flist[rpos];
    SimNode* node = ff->mNode;
    clear_lobs(node);
  }

  return !mDetFaults.empty();
}

// @brief イベントキューを用いてシミュレーションを行う．
bool
Fsim3::calc_fval(SimNode* root)
{
  root->set_fval(~root->gval());

  mEventQ.clear();
  mFvalClearArray.clear();
  mFvalClearArray.push_back(root);
  ymuint no = root->nfo();
  for (ymuint i = 0; i < no; ++ i) {
    mEventQ.put(root->fanout(i));
  }

  bool obs = false;
  for ( ; ; ) {
    SimNode* node = mEventQ.get();
    if ( node == NULL ) break;
    bool diff = node->calc_fval3();
    if ( diff ) {
      mFvalClearArray.push_back(node);
      if ( node->is_output() ) {
	if ( (node->gval() ^ node->fval()) == kVal1 ) {
	  obs = true;
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
  return obs;
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

// @brief プリミティブに対応したノードを作る．
// @param[in] prim プリミティブ
// @param[in] inputs もとのノードの入力の SimNode
// @param[in] emap もとのノードの枝の対応関係を記録する配列
// @note inputs のサイズはノードの入力数 x 2
SimNode*
Fsim3::make_primitive(const TpgPrimitive* prim,
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
Fsim3::make_node(tTgGateType type,
		 const vector<SimNode*>& inputs)
{
  ymuint32 id = mNodeArray.size();
  SimNode* node = SimNode::new_node(id, type, inputs);
  mNodeArray.push_back(node);
  return node;
}

// @brief node に対応する SimNode* を得る．
SimNode*
Fsim3::find_simnode(const TpgNode* node) const
{
  return mSimMap[node->id()];
}

// @brief node の pos 番めの入力に対応する枝を得る．
void
Fsim3::find_simedge(const TpgNode* node,
		    ymuint pos,
		    SimNode*& simnode,
		    ymuint& ipos) const
{
  const EdgeMap& edge_map = mEdgeMap[node->id()][pos];
  simnode = edge_map.mNode;
  ipos = edge_map.mPos;
}

END_NAMESPACE_YM_SATPG_FSIM3
