
/// @file RlImp.cc
/// @brief RlImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "RlImp.h"
#include "StrImp.h"
#include "ImpMgr.h"
#include "ImpNode.h"
#include "ImpInfo.h"
#include "ImpList.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス RlImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RlImp::RlImp()
{
  mLevel = 1;
}

// @brief デストラクタ
RlImp::~RlImp()
{
}

// @brief ラーニングのレベルを設定する．
void
RlImp::set_learning_level(ymuint level)
{
  mLevel = level;
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
RlImp::learning(ImpMgr& imp_mgr,
		const ImpInfo& direct_imp,
		ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    ImpNode* src_node = imp_mgr.node(src_id);

    // src_node に値を割り当てる．
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      vector<ImpDst> imp_list;
      bool ok = make_all_implication(imp_mgr, src_node, src_val, mLevel, imp_list);
      if ( ok ) {
	for (vector<ImpDst>::iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  ImpNode* dst_node = p->node();
	  ymuint dst_id = dst_node->id();
	  ymuint dst_val = p->val();
	  if ( !imp_info.check(src_id, src_val, dst_id, dst_val) ) {
	    imp_info.put(src_id, src_val, dst_id, dst_val);
	  }
	  if ( !imp_info.check(dst_id, dst_val ^ 1, src_id, src_id ^ 1) ) {
	    imp_info.put(dst_id, dst_val ^ 1, src_id, src_val ^ 1);
	  }
	}
      }
    }
  }

#if 0
  // 直接含意を求める．
  StrImp strimp;
  ImpInfo d_imp;
  strimp.learning(network, d_imp);
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      list<ImpVal>& imp_list = imp_info.get(src_id, src_val);
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ) {
	list<ImpVal>::iterator q = p;
	++ p;
	const ImpVal& imp = *q;
	ymuint dst_id = imp.id();
	ymuint dst_val = imp.val();
	if ( d_imp.check(src_id, src_val, dst_id, dst_val) ||
	     d_imp.check(dst_id, dst_val ^ 1, src_id, src_val ^ 1) ) {
	  imp_list.erase(q);
	}
      }
    }
  }
#endif

#if 0
  // 検証
  if ( 0 ) {
    SatSolver solver;
    for (ymuint i = 0; i < n; ++ i) {
      VarId vid = solver.new_var();
      assert_cond( vid.val() == i, __FILE__, __LINE__);
    }
    vector<BdnNode*> node_list;
    network.sort(node_list);
    for (vector<BdnNode*>::iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      const BdnNode* bnode = *p;
      ymuint id = bnode->id();
      Literal lit(VarId(id), kPolPosi);

      const BdnNode* bnode0 = bnode->fanin0();
      bool inv0 = bnode->fanin0_inv();
      Literal lit0(VarId(bnode0->id()), inv0 ? kPolNega : kPolPosi);

      const BdnNode* bnode1 = bnode->fanin1();
      bool inv1 = bnode->fanin1_inv();
      Literal lit1(VarId(bnode1->id()), inv1 ? kPolNega : kPolPosi);

      if ( bnode->is_and() ) {
	solver.add_clause(lit0, ~lit);
	solver.add_clause(lit1, ~lit);
	solver.add_clause(~lit0, ~lit1, lit);
      }
      else if ( bnode->is_xor() ) {
	solver.add_clause(lit0, ~lit1, lit);
	solver.add_clause(~lit0, lit1, lit);
	solver.add_clause(~lit0, ~lit1, ~lit);
	solver.add_clause(lit0, lit1, ~lit);
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
    }

    for (ymuint src_id = 0; src_id < n; ++ src_id) {
      for (ymuint src_val = 0; src_val < 2; ++ src_val) {
	Literal lit0(VarId(src_id), src_val == 0 ? kPolNega : kPolPosi);
	const ImpList& imp_list = imp_info.get(src_id, src_val);
	for (ImpList::iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpCell& imp = *p;
	  ymuint dst_id = imp.dst_id();
	  ymuint dst_val = imp.dst_val();
	  Literal lit1(VarId(dst_id), dst_val == 0 ? kPolNega : kPolPosi);
	  vector<Literal> tmp(2);
	  vector<Bool3> model;
	  tmp[0] = lit0;
	  tmp[1] = ~lit1;
	  if ( solver.solve(tmp, model) != kB3False ) {
	    cout << "Error: Node#" << src_id << ": " << src_val
		 << " ==> Node#" << dst_id << ": " << dst_val << endl;
	  }
	}
      }
    }
  }
#endif

#if 0
  cout << "RECURSIVE LERNING IMPLICATION" << endl;
  imp_info.print(cout);
#endif
}

// @brief recursive learning を行なう．
// @param[in] imp_mgr ImpMgr
// @param[in] node ノード
// @param[in] val 値
// @param[in] level レベル
// @param[in] imp_list 含意のリスト
bool
RlImp::make_all_implication(ImpMgr& imp_mgr,
			    ImpNode* node,
			    ymuint val,
			    ymuint level,
			    vector<ImpDst>& imp_list)
{
  if ( debug ) {
    cout << "make_all_implication(Node#" << node->id()
	 << ": " << val << " @level#" << level << ")" << endl;
  }

  ymuint n2 = imp_mgr.node_num();

  imp_list.clear();
  bool ok = imp_mgr.assert(node, val, imp_list);

  if ( debug ) {
    cout << "direct implications {" << endl;
    for (vector<ImpDst>::iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ImpNode* dst_node = p->node();
      ymuint dst_val = p->val();
      cout << "  ==> Node#" << dst_node->id() << ": " << dst_val << endl;
    }
    cout << "}" << endl;
  }

  if ( ok ) {
    if ( level > 0 ) {
      vector<ImpNode*> unode_list;
      imp_mgr.get_unodelist(unode_list);
      // 共通の割り当てを調べるための配列
      vector<bool> vmark(n2, false);
      for (vector<ImpNode*>::iterator p = unode_list.begin();
	   p != unode_list.end(); ++ p) {
	ImpNode* unode = *p;

	if ( debug ) {
	  cout << "Unode: Node#" << unode->id() << endl;
	}

	ymuint np = unode->justification_num();
	bool first = true;
	vector<ymuint> common_val(n2, 2);
	vector<ImpNode*> common_list;
	for (ymuint i = 0; i < np; ++ i) {
	  ImpDst imp = unode->get_justification(i);
	  ImpNode* inode = imp.node();
	  ymuint ival = imp.val();

	  if ( debug ) {
	    cout << "  Inode: Node#" << inode->id()
		 << ": " << ival << endl;
	  }

	  vector<ImpDst> imp_list1;
	  bool ok1 = make_all_implication(imp_mgr, inode, ival, level - 1,
					  imp_list1);
	  if ( ok1 ) {
	    if ( first ) {
	      first = false;
	      for (vector<ImpDst>::iterator p = imp_list1.begin();
		   p != imp_list1.end(); ++ p) {
		const ImpDst& imp = *p;
		ImpNode* dst_node = imp.node();
		ymuint dst_id = dst_node->id();
		ymuint val = imp.val();
		if ( vmark[dst_id] ) {
		  common_val[dst_id] = 2;
		}
		else {
		  common_val[dst_id] = val;
		  common_list.push_back(dst_node);
		}
	      }
	    }
	    else {
	      vector<bool> vmark1(n2, false);
	      for (vector<ImpDst>::iterator p = imp_list1.begin();
		   p != imp_list1.end(); ++ p) {
		const ImpDst& imp = *p;
		ImpNode* dst_node = imp.node();;
		ymuint dst_id = dst_node->id();
		ymuint val = imp.val();
		if ( common_val[dst_id] != val ) {
		  common_val[dst_id] = 2;
		}
		vmark1[dst_id] = true;
	      }
	      for (vector<ImpNode*>::iterator p = common_list.begin();
		   p != common_list.end(); ++ p) {
		ImpNode* dst_node = *p;
		ymuint dst_id = dst_node->id();
		if ( !vmark1[dst_id] ) {
		  common_val[dst_id] = 2;
		}
	      }
	    }
	  }
	}
	ymuint nc = common_list.size();
	for (ymuint i = 0; i < nc; ++ i) {
	  ImpNode* dst_node = common_list[i];
	  ymuint dst_id = dst_node->id();
	  if ( common_val[dst_id] == 2 ) continue;
	  imp_list.push_back(ImpDst(dst_node, common_val[dst_id]));

	  if ( debug ) {
	    cout << "  Common Implication: Node#" << dst_node->id()
		 << ": " << common_val[dst_id] << endl;
	  }

	  vmark[dst_id] = true;
	}
	if ( debug ) {
	  cout << "Unode: Node#" << unode->id() << " end" << endl;
	}
      }
    }
  }
  else {
    // 単一の割り当てで矛盾が起こった．
    // node は !val 固定
  }
  imp_mgr.backtrack();

  return ok;
}

END_NAMESPACE_YM_NETWORKS
