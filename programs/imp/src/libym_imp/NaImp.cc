
/// @file NaImp.cc
/// @brief NaImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpNode.h"
#include "ImpListRec3.h"
#include "ImpValList.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

#if defined(YM_DEBUG)
bool debug = false;
#else
bool debug = false;
#endif


// 対象のベクタを整列して単一化する．
inline
void
sort_unique(vector<ImpVal>& imp_list)
{
  sort(imp_list.begin(), imp_list.end());
  vector<ImpVal>::iterator ep = unique(imp_list.begin(), imp_list.end());
  imp_list.erase(ep, imp_list.end());
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス NaImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp::NaImp()
{
  mUseDI = true;
  mUseContra = true;
  mUseCapMerge2 = true;
}

// @brief デストラクタ
NaImp::~NaImp()
{
}

// @brief 直接含意を用いるかどうかのフラグをセットする．
void
NaImp::use_di(bool use)
{
  mUseDI = use;
}

// @brief 対偶の関係を用いるかどうかのフラグをセットする．
void
NaImp::use_contra(bool use)
{
  mUseContra = use;
}

// @brief cap_merge2 を用いるかどうかのフラグをセットする．
void
NaImp::use_cap_merge2(bool use)
{
  mUseCapMerge2 = use;
}


// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
NaImp::learning(ImpMgr& imp_mgr,
		ImpInfo& imp_info)
{
  cerr << "NaImp start" << endl;

  StopWatch timer;
  timer.start();

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<ImpValList> imp_lists(n * 2);

  // direct_imp の情報を imp_lists にコピーする．
  {
    vector<vector<ImpVal> > imp_lists_array(n * 2);
    ImpListRec3 rec(imp_lists_array);
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      ImpNode* node = imp_mgr.node(src_id);
      if ( node == NULL ) {
	continue;
      }

      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	// src_val の反対の値
	ymuint src_val1 = src_val ^ 1;

	// 自分自身を追加する．
	imp_lists_array[src_id * 2 + src_val].push_back(ImpVal(src_id, src_val));

	if ( mUseDI ) {
	  // node に src_val を割り当てる．
	  bool ok = imp_mgr.assert(node, src_val, rec);
	  imp_mgr.backtrack();
	  if ( !ok ) {
	    // 単一の割り当てで矛盾が起こった．
	    // node は src_val1 固定
	    cout << "Node#" << src_id << " is const-" << src_val1 << endl;
	    imp_mgr.set_const(src_id, src_val1);
	    break;
	  }
	}
      }
    }
    for (ymuint i = 0; i < n; ++ i) {
      if ( imp_mgr.is_const(i) ) {
	continue;
      }
      for (ymuint val = 0; val < 2; ++ val) {
	vector<ImpVal>& imp_list = imp_lists_array[i * 2 + val];
	sort_unique(imp_list);
	ImpValList& dst_list = imp_lists[i * 2 + val];
	dst_list.set(imp_mgr, imp_list);
	dst_list.set_change1();
	dst_list.set_change2();
      }
    }
  }

  if ( debug ) {
    cout << "Phase0 end" << endl;
    for (ymuint i = 0; i < n; ++ i) {
      for (ymint val = 0; val < 2; ++ val) {
	ImpValList& imp_list = imp_lists[i * 2 + val];
	cout << "Node#" << i << ":" << val << endl;
	imp_list.print(cout);
      }
    }
  }
#if 1
  timer.stop();
  USTime time = timer.time();
  cerr << "phase0:" << time << endl;
  timer.reset();
  timer.start();
#endif


  // 順方向のトポロジカル順のノードリストを用意する．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);

  // 逆方向のトポロジカル順のノードリストを用意する．
  // ただし，こちらは入力ノードも含める．
  vector<ImpNode*> rnode_list;
  ymuint ni = imp_mgr.input_num();
  rnode_list.reserve(node_list.size() + ni);
  for (vector<ImpNode*>::reverse_iterator p = node_list.rbegin();
       p != node_list.rend(); ++ p) {
    ImpNode* node = *p;
    if ( node->fanout_num() > 0 ) {
      rnode_list.push_back(node);
    }
  }
  for (ymuint i = 0; i < imp_mgr.input_num(); ++ i) {
    rnode_list.push_back(imp_mgr.input_node(i));
  }

  // 変化がなくなるまでループを繰り返す．
  for ( ; ; ) {
    for ( ; ; ) {
      ymuint delta = 0;

      // 順方向の処理
      for (vector<ImpNode*>::iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	ImpNode* node = *p;
	if ( node->is_const() ) {
	  continue;
	}
	ymuint id = node->id();
	ymuint idx_0 = id * 2 + 0;
	ymuint idx_1 = id * 2 + 1;
	ImpValList& dst0_list = imp_lists[idx_0];
	ImpValList& dst1_list = imp_lists[idx_1];

	// ファンイン0の情報
	const ImpEdge& e0 = node->fanin0();
	ImpNode* node0 = e0.src_node();
	ymuint id0 = node0->id();
	bool inv0 = e0.src_inv();
	ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
	ymuint idx0_1 = idx0_0 ^ 1;

	// ファンイン1の情報
	const ImpEdge& e1 = node->fanin1();
	ImpNode* node1 = e1.src_node();
	ymuint id1 = node1->id();
	bool inv1 = e1.src_inv();
	ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
	ymuint idx1_1 = idx1_0 ^ 1;

	if ( node0->is_const() ) {
	  // ファンイン0が定数だった．
	  assert_cond( !node1->is_const(), __FILE__, __LINE__);
	  // ファンイン1の条件をそのままコピー
	  dst0_list.merge(imp_lists[idx1_0]);
	  dst0_list.set_change1();
	  dst1_list.merge(imp_lists[idx1_1]);
	  dst1_list.set_change1();
	}
	else if ( node1->is_const() ) {
	  // ファンイン1が定数だった．
	  // assert_cond( !imp_mgr.is_const(id0), __FILE__, __LINE__); 不要
	  // ファンイン0の条件をそのままコピー
	  dst0_list.merge(imp_lists[idx0_0]);
	  dst0_list.set_change1();
	  dst1_list.merge(imp_lists[idx0_1]);
	  dst1_list.set_change1();
	}
	else {
	  { // 出力が0になる条件は入力が0になる条件のユニオン
	    if ( debug ) {
	      cout << "Node#" << id << ":0" << endl;
	    }
	    const ImpValList& src1_list = imp_lists[idx0_0];
	    const ImpValList& src2_list = imp_lists[idx1_0];
	    ImpValList& dst_list = imp_lists[idx_0];
	    ymuint old_num = dst_list.num();
	    if ( src1_list.changed() ) {
	      dst_list.merge(src1_list);
	    }
	    if ( src2_list.changed() ) {
	      dst_list.merge(src2_list);
	    }
	    ymuint delta1 = dst_list.num() - old_num;
	    if ( delta1 > 0 ) {
	      dst_list.set_change1();
	    }
	    else {
	      dst_list.reset_change1();
	    }
	    delta += delta1;
	    if ( debug ) {
	      if ( delta1 > 0 ) {
		cout << " Node#" << id << ":0 changed" << endl;
	      }
	      dst_list.print(cout);
	      cout << endl
		   << endl;
	    }
	  }
	  { // 出力が1になる条件は入力が1になる条件のインターセクション
	    if ( debug ) {
	      cout << "Node#" << id << ":1" << endl;
	    }
	    const ImpValList& src1_list = imp_lists[idx0_1];
	    const ImpValList& src2_list = imp_lists[idx1_1];
	    ImpValList& dst_list = imp_lists[idx_1];
	    ymuint old_num = dst_list.num();
	    if ( src1_list.changed() ||
		 src2_list.changed() ) {
	      if ( mUseCapMerge2 ) {
		dst_list.cap_merge2(src1_list, src2_list);
	      }
	      else {
		dst_list.cap_merge(src1_list, src2_list);
	      }
	    }
	    ymuint delta1 = dst_list.num() - old_num;
	    if ( delta1 > 0 ) {
	      dst_list.set_change1();
	    }
	    else {
	      dst_list.reset_change1();
	    }
	    delta += delta1;
	    if ( debug ) {
	      if ( delta1 > 0 ) {
		cout << " Node#" << id << ":1 changed" << endl;
	      }
	      dst_list.print(cout);
	      cout << endl << endl;
	    }
	  }
	}
      }
      cerr << "phase1: delta = " << delta << endl;
      if ( debug ) {
	cout << "Phase1 end" << endl;
	for (ymuint i = 0; i < n; ++ i) {
	  for (ymint val = 0; val < 2; ++ val) {
	    ImpValList& imp_list = imp_lists[i * 2 + val];
	    cout << "Node#" << i << ":" << val << endl;
	    imp_list.print(cout);
	  }
	}
      }

      // 逆方向の処理
      for (vector<ImpNode*>::iterator p = rnode_list.begin();
	   p != rnode_list.end(); ++ p) {
	ImpNode* node = *p;
	if ( node->is_const() ) {
	  continue;
	}

	ymuint id = node->id();
	ymuint idx_0 = id * 2 + 0;
	ymuint idx_1 = id * 2 + 1;

	ImpValList& dst0_list = imp_lists[idx_0];
	ImpValList& dst1_list = imp_lists[idx_1];
	dst0_list.reset_change2();
	dst1_list.reset_change2();
	ymuint nfo = node->fanout_num();
	for (ymuint i = 0; i < nfo; ++ i) {
	  const ImpEdge& edge = node->fanout(i);

	  // 出力の情報
	  ImpNode* onode = edge.dst_node();
	  ymuint oid = onode->id();
	  ymuint opos = edge.dst_pos();
	  bool inv = edge.src_inv();
	  ymuint oidx_0 = oid * 2 + 0;
	  ymuint oidx_1 = oid * 2 + 1;

	  // 他方のファンインの情報
	  const ImpEdge& other_edge = (opos == 0) ? onode->fanin1() : onode->fanin0();
	  ImpNode* snode = other_edge.src_node();
	  ymuint sid = snode->id();
	  bool sinv = other_edge.src_inv();
	  ymuint sidx_1 = sid * 2 + (sinv ? 0 : 1);

	  if ( onode->is_const() ) {
	    // 出力が定数だった．
	    continue;
	  }

	  if ( snode->is_const1() ) {
	    // 他方のファンインが定数1だった
	    // 出力の条件をマージする．
	    if ( inv ) {
	      dst0_list.merge(imp_lists[oidx_1]);
	      dst0_list.set_change2();
	      dst1_list.merge(imp_lists[oidx_0]);
	      dst1_list.set_change2();
	    }
	    else {
	      dst0_list.merge(imp_lists[oidx_0]);
	      dst0_list.set_change2();
	      dst1_list.merge(imp_lists[oidx_1]);
	      dst1_list.set_change2();
	    }
	    continue;
	  }
	  // 他方のファンインが定数0なら出力が定数になっているはず．

	  // 出力の0の条件と他方のファンインの1の条件の共通部分が
	  // 0の条件となる．
	  {
	    if ( debug ) {
	      cout << "Node#" << id << ":" << (inv ? 1 : 0) << endl;
	    }
	    const ImpValList& src1_list = imp_lists[oidx_0];
	    const ImpValList& src2_list = imp_lists[sidx_1];
	    ImpValList& dst_list = inv ? dst1_list : dst0_list;
	    ymuint old_num = dst_list.num();
	    if ( src1_list.changed() ||
		 src2_list.changed() ) {
	      if ( mUseCapMerge2 ) {
		dst_list.cap_merge2(src1_list, src2_list);
	      }
	      else {
		dst_list.cap_merge(src1_list, src2_list);
	      }
	    }
	    ymuint delta1 = dst_list.num() - old_num;
	    if ( delta1 > 0 ) {
	      dst_list.set_change2();
	    }
	    delta += delta1;
	    if ( debug ) {
	      if ( delta1 > 0 ) {
		cout << " Node#" << id << ":" << (inv ? 1 : 0) << " changed" << endl;
	      }
	      dst_list.print(cout);
	      cout << endl << endl;
	    }
	  }

	  // 出力の1の条件がファンイン0の1の条件となる．
	  {
	    if ( debug ) {
	      cout << "Node#" << id << ":" << (inv ? 0 : 1) << endl;
	    }
	    const ImpValList& src_list = imp_lists[oidx_1];
	    ImpValList& dst_list = inv ? dst0_list : dst1_list;
	    ymuint old_num = dst_list.num();
	    if ( src_list.changed() ) {
	      dst_list.merge(src_list);
	    }
	    ymuint delta1 = dst_list.num() - old_num;
	    if ( delta1 > 0 ) {
	      dst_list.set_change2();
	    }
	    delta += delta1;
	    if ( debug ) {
	      if ( delta1 > 0 ) {
		cout << " Node#" << id << ":" << (inv ? 0 : 1) << " changed" << endl;
	      }
	      dst_list.print(cout);
	      cout << endl;
	    }
	  }
	}
      }

      cerr << "phase2: delta = " << delta << endl;
      if ( debug ) {
	cout << "Phase2 end" << endl;
	for (ymuint i = 0; i < n; ++ i) {
	  for (ymint val = 0; val < 2; ++ val) {
	    ImpValList& imp_list = imp_lists[i * 2 + val];
	    cout << "Node#" << i << ":" << val << endl;
	    imp_list.print(cout);
	  }
	}
      }
      if ( delta == 0 ) {
	break;
      }

    }

    if ( mUseContra ) { // 対偶を加えておく
      for (ymuint i = 0; i < n * 2; ++ i) {
	imp_lists[i].reset_change1();
	imp_lists[i].reset_change2();
      }
      ymuint delta = 0;
      vector<vector<ImpVal> > tmp_list_array(n * 2);
      for (ymuint id = 0; id < n; ++ id) {
	if ( imp_mgr.is_const(id) ) {
	  continue;
	}
	for (ymuint val = 0; val < 2; ++ val) {
	  ImpValList& imp_list = imp_lists[id * 2 + val];
	  if ( imp_list.changed3() ) {
	    if ( debug ) {
	      cout << " + Node#" << id << ": " << val << endl;
	    }
	    for (ImpValListIter p = imp_list.begin();
		 p != imp_list.end(); ++ p) {
	      const ImpVal& impval = *p;
	      ymuint src_id = impval.id();
	      ymuint src_val = impval.val();
	      if ( src_id == id ) {
		continue;
	      }
	      ymuint val1 = val ^ 1;
	      ymuint src_val1 = src_val ^ 1;
	      tmp_list_array[src_id * 2 + src_val1].push_back(ImpVal(id, val1));
	    }
	  }
	  imp_list.reset_change3();
	}
      }
      ymuint n2 = n * 2;
      for (ymuint i = 0; i < n2; ++ i) {
	vector<ImpVal>& tmp_list = tmp_list_array[i];
	sort(tmp_list.begin(), tmp_list.end());
	ImpValList& dst_list = imp_lists[i];
	ymuint old_num = dst_list.num();
	dst_list.merge(tmp_list);
	ymuint delta1 = dst_list.num() - old_num;
	delta += delta1;
	if ( delta1 > 0 ) {
	  dst_list.set_change2();
	}
	if ( debug ) {
	  ymuint id = i / 2;
	  ymuint val = i % 2;
	  cout << "Node#" << id << ": " << val << endl;
	  if ( delta1 > 0 ) {
	    cout << " Node#" << id << ":" << val << " changed" << endl;
	  }
	  dst_list.print(cout);
	  cout << endl << endl;
	}
      }
      cerr << "phase3: delta = " << delta << endl;
      if ( delta == 0 ) {
	break;
      }
    }
    else {
      break;
    }
  }

  USTime time2 = timer.time();
  cerr << "phase4: " << time2 << endl;

  // imp_lists の情報から imp_info を作る．
  {
    vector<vector<ImpVal> > imp_list_array(n * 2);
    for (ymuint dst_id = 0; dst_id < n; ++ dst_id) {
      if ( imp_mgr.is_const(dst_id) ) {
	continue;
      }
      ImpNode* node = imp_mgr.node(dst_id);
      if ( node == NULL ) {
	continue;
      }
      for (ymuint dst_val = 0; dst_val < 2; ++ dst_val) {
	const ImpValList& imp_list = imp_lists[dst_id * 2 + dst_val];
	for (ImpValListIter p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpVal& val = *p;
	  ymuint src_id = val.id();
	  ymuint src_val = val.val();
	  if ( src_id == dst_id ) {
	    continue;
	  }
	  imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	  imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
	}
      }
    }
#if 1
    // imp_list_array の内容を imp_info にコピーする．
    imp_info.set(imp_list_array);

    for (ymuint id = 0; id < n; ++ id) {
      ImpNode* node = imp_mgr.node(id);
      for (ymuint val = 0; val < 2; ++ val) {
	const vector<ImpVal>& imp_list = imp_info.get(id, val);
	imp_mgr.set_ind_imp(node, val, imp_list);
      }
    }
#else
    for (ymuint id = 0; id < n; ++ id) {
      ImpNode* node = imp_mgr.node(id);
      for (ymuint val = 0; val < 2; ++ val) {
	vector<ImpVal>& imp_list = imp_list_array[id * 2 + val];
	sort_unique(imp_list);
	imp_mgr.set_ind_imp(node, val, imp_list);
      }
    }
#endif
  }

  for (ymuint id = 0; id < n; ++ id) {
    if ( imp_mgr.is_const(id) ) {
      continue;
    }
    ImpNode* node = imp_mgr.node(id);

    for (ymuint val = 0; val < 2; ++ val) {
      // node に val を割り当てる．
      bool ok = imp_mgr.assert(node, val);
      imp_mgr.backtrack();
      if ( !ok ) {
	// node は (val ^ 1)固定
	cout << "Node#" << id << " is const-" << (val ^ 1) << endl;
	imp_mgr.set_const(id, val ^ 1);
	break;
      }
    }
  }
#if 0
  check_const(imp_mgr, imp_info);
#endif
#if 0
  verify(imp_mgr, imp_info);
#endif

  cerr << "NaImp end" << endl;
}

END_NAMESPACE_YM_NETWORKS
