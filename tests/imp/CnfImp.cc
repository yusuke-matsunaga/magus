
/// @file CnfImp.cc
/// @brief CnfImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CnfImp.h"
#include "CnfClause.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

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
// @param[in] network 対象のネットワーク
// @param[in] imp_list 間接含意のリスト
void
CnfImp::learning(const BdnMgr& network,
		 vector<ImpInfo>& imp_list)
{
  // BDN から CNF を作る．
  vector<BdnNode*> node_list;
  network.sort(node_list);

  ymuint c = 0;
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    if ( bnode->is_and() ) {
      c += 3;
    }
    else if ( bnode->is_xor() ) {
      c += 4;
    }
  }

  vector<CnfClause*> cnf_list;
  cnf_list.reserve(c);
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
      CnfClause* c0 = new CnfClause;
      c0->set_literal_num(2);
      c0->set_literal(0, lit0);
      c0->set_literal(1, ~lit);
      cnf_list.push_back(c0);

      CnfClause* c1 = new CnfClause;
      c1->set_literal_num(2);
      c1->set_literal(0, lit1);
      c1->set_literal(1, ~lit);
      cnf_list.push_back(c1);

      CnfClause* c2 = new CnfClause;
      c2->set_literal_num(3);
      c2->set_literal(0, ~lit0);
      c2->set_literal(1, ~lit1);
      c2->set_literal(2, lit);
      cnf_list.push_back(c2);
    }
    else if ( bnode->is_xor() ) {
      CnfClause* c0 = new CnfClause;
      c0->set_literal_num(3);
      c0->set_literal(0, ~lit0);
      c0->set_literal(1, lit1);
      c0->set_literal(2, lit);
      cnf_list.push_back(c0);

      CnfClause* c1 = new CnfClause;
      c1->set_literal_num(3);
      c1->set_literal(0, lit0);
      c1->set_literal(1, ~lit1);
      c1->set_literal(2, lit);
      cnf_list.push_back(c1);

      CnfClause* c2 = new CnfClause;
      c2->set_literal_num(3);
      c2->set_literal(0, ~lit0);
      c2->set_literal(1, ~lit1);
      c2->set_literal(2, ~lit);
      cnf_list.push_back(c2);

      CnfClause* c3 = new CnfClause;
      c3->set_literal_num(3);
      c3->set_literal(0, lit0);
      c3->set_literal(1, lit1);
      c3->set_literal(2, ~lit);
      cnf_list.push_back(c3);
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }
  for (ymuint j = 0; j < cnf_list.size(); ++ j) {
    CnfClause* c = cnf_list[j];
    c->print(cout);
  }
  cout << "Total " << cnf_list.size() << " clauses" << endl;

  vector<CnfClause*> p_list;
  vector<CnfClause*> n_list;
  ymuint n = network.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    // i の肯定と否定のリテラルを含む節を求める．
    p_list.clear();
    n_list.clear();
    p_list.reserve(cnf_list.size());
    n_list.reserve(cnf_list.size());
    Literal p_lit(VarId(i), kPolPosi);
    Literal n_lit(VarId(i), kPolNega);
    for (ymuint j = 0; j < cnf_list.size(); ++ j) {
      CnfClause* c = cnf_list[j];
      if ( c->is_in(p_lit) ) {
	p_list.push_back(c);
      }
      else if ( c->is_in(n_lit) ) {
	n_list.push_back(c);
      }
    }
    cout << "Var#" << i << ": " << p_list.size() << " x " << n_list.size() << endl;
    ymuint nimp = 0;
#if 0
    cout << "p_list" << endl;
    for (vector<CnfClause*>::iterator p = p_list.begin();
	 p != p_list.end(); ++ p) {
      CnfClause* c = *p;
      c->print(cout);
    }
    cout << "n_list" << endl;
    for (vector<CnfClause*>::iterator p = n_list.begin();
	 p != n_list.end(); ++ p) {
      CnfClause* c = *p;
      c->print(cout);
    }
#endif
    for (vector<CnfClause*>::iterator p = p_list.begin();
	 p != p_list.end(); ++ p) {
      CnfClause* pc = *p;
      vector<Literal> tmp;
      for (ymuint i = 0; i < pc->literal_num(); ++ i) {
	Literal l = pc->literal(i);
	if ( l != p_lit ) {
	  tmp.push_back(l);
	}
      }
      for (vector<CnfClause*>::iterator p = n_list.begin();
	   p != n_list.end(); ++ p) {
	CnfClause* nc = *p;
	vector<Literal> tmp2(tmp);
	for (ymuint i = 0; i < nc->literal_num(); ++ i) {
	  Literal l = nc->literal(i);
	  if ( l != n_lit ) {
	    tmp2.push_back(l);
	  }
	}
	sort(tmp2.begin(), tmp2.end());
	ymuint nn = tmp2.size();
	Literal prev = tmp2[0];
	bool tautology = false;
	vector<Literal> tmp3;
	tmp3.reserve(nn);
	tmp3.push_back(prev);
	for (ymuint i = 1; i < nn; ++ i) {
	  Literal cur = tmp2[i];
	  if ( prev.varid() == cur.varid() ) {
	    if ( prev.pol() != cur.pol() ) {
	      tautology = true;
	      break;
	    }
	  }
	  else {
	    tmp3.push_back(cur);
	    prev = cur;
	  }
	}
	if ( tautology ) {
	  continue;
	}
	nn = tmp3.size();
	bool redundant = false;
	for (vector<CnfClause*>::iterator p = cnf_list.begin();
	     p != cnf_list.end(); ++ p) {
	  CnfClause* c = *p;
	  ymuint nc = c->literal_num();
	  if ( nc > nn ) continue;
	  bool contain = true;
	  for (ymuint i = 0; i < nc; ++ i) {
	    Literal l = c->literal(i);
	    bool found = false;
	    for (ymuint j = 0; j < nn; ++ j) {
	      if ( tmp3[j] == l ) {
		found = true;
		break;
	      }
	    }
	    if ( !found ) {
	      contain = false;
	      break;
	    }
	  }
	  if ( contain ) {
	    redundant = true;
	    break;
	  }
	}
	if ( redundant ) {
	  continue;
	}
	CnfClause* rc = new CnfClause;
	rc->set_literal_num(nn);
	for (ymuint i = 0; i < nn; ++ i) {
	  rc->set_literal(i, tmp3[i]);
	}
	cnf_list.push_back(rc);
	++ nimp;
#if 0
	cout << "Resolution";
	for (vector<Literal>::iterator p = tmp3.begin();
	     p != tmp3.end(); ++ p) {
	  cout << " " << *p;
	}
	cout << endl;
#endif
      }
    }
    cout << "  " << nimp << " learned implications" << endl;
  }
}

END_NAMESPACE_YM_NETWORKS
