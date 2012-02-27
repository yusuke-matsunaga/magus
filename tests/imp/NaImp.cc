
/// @file NaImp.cc
/// @brief NaImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NaImp.h"
#include "ImpInfo.h"
#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

void
print_list(const list<ImpVal>& imp_list)
{
  for (list<ImpVal>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    cout << " ";
    if ( p->val() == 0 ) {
      cout << "~";
    }
    cout << p->id();
  }
  cout << endl;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス NaImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
NaImp::NaImp()
{
}

// @brief デストラクタ
NaImp::~NaImp()
{
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] network 対象のネットワーク
// @param[in] imp_info 間接含意のリスト
void
NaImp::learning(const BdnMgr& network,
		ImpInfo& imp_info)
{
  ymuint n = network.max_node_id();

  imp_info.set_size(n);

  vector<list<ImpVal> > imp_lists(n * 2);

  // node_list に network のノードをトポロジカル順に並べる．
  vector<BdnNode*> node_list;
  network.sort(node_list);

  // 外部入力ノードの割り当て情報を作る．
  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();
    // 入力は自分自身だけ
    imp_lists[id * 2 + 0].push_back(ImpVal(id, 0));
    imp_lists[id * 2 + 1].push_back(ImpVal(id, 1));
  }

  // 論理ノードの割り当て情報を作る．
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* bnode = *p;
    ymuint id = bnode->id();

    const BdnNode* bnode0 = bnode->fanin0();
    ymuint id0 = bnode0->id();
    bool inv0 = bnode->fanin0_inv();
    ymuint idx0 = id0 * 2 + (inv0 ? 1 : 0);

    const BdnNode* bnode1 = bnode->fanin1();
    ymuint id1 = bnode1->id();
    bool inv1 = bnode->fanin1_inv();
    ymuint idx1 = id1 * 2 + (inv1 ? 1: 0);

    // 出力が0になる割り当ては入力が0になる割り当ての共通部分
    {
      const list<ImpVal>& imp_list0 = imp_lists[idx0];
      const list<ImpVal>& imp_list1 = imp_lists[idx1];
      {
	cout << "Node#" << id << ": 0" << endl;
	cout << " fanin0: ";
	if ( inv0 ) {
	  cout << "~";
	}
	cout << "Node#" << id0 << endl;
	cout << "   ";
	print_list(imp_list0);
	cout << " fanin1: ";
	if ( inv1 ) {
	  cout << "~";
	}
	cout << "Node#" << id1 << endl;
	cout << "   ";
	print_list(imp_list1);
	cout << endl;
      }
      list<ImpVal>& imp_list = imp_lists[id * 2 + 0];
      list<ImpVal>::const_iterator p0 = imp_list0.begin();
      list<ImpVal>::const_iterator p1 = imp_list1.begin();
      while ( p0 != imp_list0.end() && p1 != imp_list1.end() ) {
	ymuint id0 = p0->id();
	ymuint id1 = p1->id();
	if ( id0 < id1 ) {
	  ++ p0;
	}
	else if ( id0 > id1 ) {
	  ++ p1;
	}
	else {
	  if ( p0->val() == p1->val() ) {
	    imp_list.push_back(*p0);
	    imp_info.put(id, 0, id0, p0->val());
	  }
	  ++ p0;
	  ++ p1;
	}
      }
      // 最後に自分自身を足しておく
      bool done = false;
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( p->id() > id ) {
	  imp_list.insert(p, ImpVal(id, 0));
	  done = true;
	  break;
	}
      }
      if ( !done ) {
	imp_list.push_back(ImpVal(id, 0));
      }
      cout << "  result" << endl
	   << "   ";
      print_list(imp_list);
      cout << endl
	   << endl;
    }
    // 出力が1になる割り当ては入力が1になる割り当ての和
    {
      const list<ImpVal>& imp_list0 = imp_lists[idx0 ^ 1];
      const list<ImpVal>& imp_list1 = imp_lists[idx1 ^ 1];
      {
	cout << "Node#" << id << ": 1" << endl;
	cout << " fanin0: ";
	if ( inv0 ) {
	  cout << "~";
	}
	cout << "Node#" << id0 << endl;
	cout << "   ";
	print_list(imp_list0);
	cout << " fanin1: ";
	if ( inv1 ) {
	  cout << "~";
	}
	cout << "Node#" << id1 << endl;
	cout << "   ";
	print_list(imp_list1);
	cout << endl;
      }
      list<ImpVal>& imp_list = imp_lists[id * 2 + 1];
      list<ImpVal>::const_iterator p0 = imp_list0.begin();
      list<ImpVal>::const_iterator p1 = imp_list1.begin();
      list<ImpVal>::const_iterator e0 = imp_list0.end();
      list<ImpVal>::const_iterator e1 = imp_list1.end();
      while ( p0 != e0 && p1 != e1 ) {
	ymuint id0 = p0->id();
	ymuint id1 = p1->id();
	if ( id0 < id1 ) {
	  imp_list.push_back(*p0);
	  imp_info.put(id, 0, id0, p0->val());
	  ++ p0;
	}
	else if ( id0 > id1 ) {
	  imp_list.push_back(*p1);
	  imp_info.put(id, 0, id1, p1->val());
	  ++ p1;
	}
	else {
	  assert_cond( p0->val() == p1->val(), __FILE__, __LINE__);
	  imp_list.push_back(*p0);
	  imp_info.put(id, 0, id0, p0->val());
	  ++ p0;
	  ++ p1;
	}
      }
      for ( ; p0 != e0; ++ p0 ) {
	imp_list.push_back(*p0);
      }
      for ( ; p1 != e1; ++ p1 ) {
	imp_list.push_back(*p1);
      }
      // 最後に自分自身を足しておく
      bool done = false;
      for (list<ImpVal>::iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	if ( p->id() > id ) {
	  imp_list.insert(p, ImpVal(id, 1));
	  done = true;
	  break;
	}
      }
      if ( !done ) {
	imp_list.push_back(ImpVal(id, 1));
      }
      cout << "  result" << endl
	   << "   ";
      print_list(imp_list);
      cout << endl
	   << endl;
    }
  }
}

END_NAMESPACE_YM_NETWORKS
