
/// @file CnfImp.cc
/// @brief CnfImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CnfImp.h"
#include "ImpMgr.h"
#include "ImpInfo.h"
#include "ImpListRec2.h"
#include "YmLogic/CNFdd.h"
#include "YmLogic/CNFddMgr.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

#if defined(YM_DEBUG)
bool debug = true;
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

void
merge(CNFdd& dst,
      const CNFdd& src1,
      const CNFdd& src2)
{
  CNFdd tmp = src1 * src2;
  tmp = tmp.cut_off(2);
  dst |= tmp;
  dst.make_minimal();
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CnfImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CnfImp::CnfImp()
{
}

// @brief デストラクタ
CnfImp::~CnfImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] imp_info 間接含意のリスト
void
CnfImp::learning(ImpMgr& imp_mgr,
		 ImpInfo& imp_info)
{
  cerr << "CnfImp start" << endl;

  StopWatch timer;
  timer.start();

  CNFddMgr cnfddmgr("", "cnf_imp");

  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<CNFdd> imp_lists(n * 2);
  {
    vector<vector<ImpVal> > imp_lists_array(n * 2);
    ImpListRec2 rec(imp_lists_array);
    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      ImpNode* node = imp_mgr.node(src_id);
      if ( node == NULL ) {
	continue;
      }

      for (ymuint src_val = 0; src_val < 2; ++ src_val ) {
	ymuint src_val1 = src_val ^ 1;

	// 自分自身を追加する．
	imp_lists_array[src_id * 2 + src_val].push_back(ImpVal(src_id, src_val));

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
    for (ymuint i = 0; i < n; ++ i) {
      if ( imp_mgr.is_const(i) ) {
	continue;
      }
      for (ymuint val = 0; val < 2; ++ val) {
	vector<ImpVal>& imp_list = imp_lists_array[i * 2 + val];
	sort_unique(imp_list);
	CNFdd& dst = imp_lists[i * 2 + val];
	dst = cnfddmgr.make_empty();
	for (vector<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  ymuint id = p->id();
	  VarId v(id);
	  ymuint val = p->val();
	  Literal lit(v, (val == 0));
	  CNFdd elem = cnfddmgr.make_base();
	  elem.add_literal(lit);
	  dst |= elem;
	}
      }
    }
  }

  if ( debug ) {
    for (ymuint i = 0; i < n; ++ i) {
      for (ymuint val = 0; val < 2; ++ val) {
	cout << "Node#" << i << ":" << val << endl;
	imp_lists[i * 2 + val].print(cout);
      }
    }
  }

  timer.stop();
  USTime time = timer.time();
  cerr << "phase0: " << time << endl;
  timer.reset();
  timer.start();


  // 論理ノードの割り当て情報を作る．
  vector<ImpNode*> node_list;
  imp_mgr.get_node_list(node_list);

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

  for ( ; ; ) {
    for ( ; ; ) {
      ymuint delta = 0;
      for (vector<ImpNode*>::iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	ImpNode* node = *p;
	ymuint id = node->id();
	if ( imp_mgr.is_const(id) ) {
	  continue;
	}
	ymuint idx_0 = id * 2 + 0;
	ymuint idx_1 = id * 2 + 1;

	const ImpEdge& e0 = node->fanin0();
	ImpNode* node0 = e0.src_node();
	ymuint id0 = node0->id();
	bool inv0 = e0.src_inv();
	ymuint idx0_0 = id0 * 2 + (inv0 ? 1 : 0);
	ymuint idx0_1 = idx0_0 ^ 1;

	const ImpEdge& e1 = node->fanin1();
	ImpNode* node1 = e1.src_node();
	ymuint id1 = node1->id();
	bool inv1 = e1.src_inv();
	ymuint idx1_0 = id1 * 2 + (inv1 ? 1: 0);
	ymuint idx1_1 = idx1_0 ^ 1;

	// 出力が0になる条件は入力が0になる条件のユニオン
	if ( imp_mgr.is_const(id0) ) {
	  // ファンイン0が定数だった．
	  assert_cond( !imp_mgr.is_const(id1), __FILE__, __LINE__);
	  // ファンイン1の条件をそのままコピー
	  imp_lists[idx_0] |= imp_lists[idx1_0];
	  imp_lists[idx_1] |= imp_lists[idx1_1];
	}
	else if ( imp_mgr.is_const(id1) ) {
	  // ファンイン1が定数だった．
	  assert_cond( !imp_mgr.is_const(id0), __FILE__, __LINE__);
	  // ファンイン0の条件をそのままコピー
	  imp_lists[idx_0] |= imp_lists[idx0_0];
	  imp_lists[idx_1] |= imp_lists[idx0_1];
	}
	else {
	  {
	    if ( debug ) {
	      cout << "Node#" << id << ":0" << endl;
	    }
	    const CNFdd& src1_list = imp_lists[idx0_0];
	    const CNFdd& src2_list = imp_lists[idx1_0];
	    CNFdd& dst_list = imp_lists[idx_0];

	    ymuint old_num = dst_list.node_count();
	    dst_list |= src1_list;
	    dst_list |= src2_list;
	    ymuint delta1 = dst_list.node_count() - old_num;
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
	  // 出力が1になる条件は入力が1になる条件のインターセクション
	  {
	    if ( debug ) {
	      cout << "Node#" << id << ":1" << endl;
	    }
	    const CNFdd& src1_list = imp_lists[idx0_1];
	    const CNFdd& src2_list = imp_lists[idx1_1];
	    CNFdd& dst_list = imp_lists[idx_1];
	    ymuint old_num = dst_list.node_count();
	    merge(dst_list, src1_list, src2_list);
	    ymuint delta1 = dst_list.node_count() - old_num;
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

      for (vector<ImpNode*>::iterator p = rnode_list.begin();
	   p != rnode_list.end(); ++ p) {
	ImpNode* node = *p;
	ymuint id = node->id();
	if ( imp_mgr.is_const(id) ) {
	  continue;
	}

	ymuint idx_0 = id * 2 + 0;
	ymuint idx_1 = id * 2 + 1;

	CNFdd& dst0_list = imp_lists[idx_0];
	CNFdd& dst1_list = imp_lists[idx_1];
	ymuint nfo = node->fanout_num();
	for (ymuint i = 0; i < nfo; ++ i) {
	  const ImpEdge& edge = node->fanout(i);
	  ImpNode* onode = edge.dst_node();
	  ymuint oid = onode->id();
	  ymuint opos = edge.dst_pos();
	  bool inv = edge.src_inv();
	  ymuint oidx_0 = oid * 2 + 0;
	  ymuint oidx_1 = oid * 2 + 1;

	  const ImpEdge& other_edge = (opos == 0) ? onode->fanin1() : onode->fanin0();
	  ImpNode* snode = other_edge.src_node();
	  ymuint sid = snode->id();
	  bool sinv = other_edge.src_inv();
	  ymuint sidx_1 = sid * 2 + (sinv ? 0 : 1);

	  if ( imp_mgr.is_const(oid) ) {
	    // 出力が定数だった．
	    continue;
	  }
	  if ( imp_mgr.is_const1(sid) ) {
	    // 他方のファンインが定数1だった
	    if ( inv ) {
	      dst0_list |= imp_lists[oidx_1];
	      dst1_list |= imp_lists[oidx_0];
	    }
	    else {
	      dst0_list |= imp_lists[oidx_0];
	      dst1_list |= imp_lists[oidx_1];
	    }
	    continue;
	  }
	  // 出力の0の条件と他方のファンインの1の条件の共通部分が
	  // 0の条件となる．
	  {
	    if ( debug ) {
	      cout << "Node#" << id << ":" << (inv ? 1 : 0) << endl;
	    }
	    const CNFdd& src1_list = imp_lists[oidx_0];
	    const CNFdd& src2_list = imp_lists[sidx_1];
	    CNFdd& dst_list = inv ? dst1_list : dst0_list;
	    ymuint old_num = dst_list.node_count();
	    merge(dst_list, src1_list, src2_list);
	    ymuint delta1 = dst_list.node_count() - old_num;
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
	    const CNFdd& src_list = imp_lists[oidx_1];
	    CNFdd& dst_list = inv ? dst0_list : dst1_list;
	    ymuint old_num = dst_list.node_count();
	    dst_list |= src_list;
	    ymuint delta1 = dst_list.node_count() - old_num;
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
      if ( delta == 0 ) {
	break;
      }
    }

#if 0
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
      }
      cerr << "phase3: delta = " << delta << endl;
      if ( delta == 0 ) {
	break;
      }
    }
    else {
      break;
    }
#else
    break;
#endif
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
	const CNFdd& imp_list = imp_lists[dst_id * 2 + dst_val];
	CNFdd tmp = imp_list.cut_off(1);
	for ( ; ; ) {
	  assert_cond( !tmp.is_invalid(), __FILE__, __LINE__);
	  CNFdd f_0;
	  CNFdd f_p;
	  CNFdd f_n;
	  VarId vid = tmp.root_decomp(f_0, f_p, f_n);
	  ymuint src_id = dst_id;
	  ymuint src_val;
	  if ( f_p.is_one() ) {
	    src_id = vid.val();
	    src_val = 1;
	    tmp = f_0;
	  }
	  else if ( f_n.is_one() ) {
	    src_id = vid.val();
	    src_val = 0;
	    tmp = f_0;
	  }
	  if ( src_id != dst_id ) {
	    imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	    imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
	  }
	  if ( tmp.is_zero() ) {
	    break;
	  }
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
