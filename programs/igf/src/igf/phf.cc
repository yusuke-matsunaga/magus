
/// @file igf.cc
/// @brief igf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandPermGen.h"


BEGIN_NAMESPACE_YM_IGF

bool
dfs2(PhfNode* node2,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list);

bool
dfs1(PhfNode* node1,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list)
{
  ymuint ne = node1->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = node1->edge(i);
    PhfNode* node2 = edge->node2();
    if ( node2 == from ) {
      continue;
    }
    edge_list.push_back(edge);
    edge->set_dir(0);
    if ( v_mark[node2->id()] ) {
      return false;
    }
    v_mark[node2->id()] = true;
    bool stat = dfs2(node2, node1, v_mark, edge_list);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

bool
dfs2(PhfNode* node2,
     PhfNode* from,
     vector<bool>& v_mark,
     vector<PhfEdge*>& edge_list)
{
  ymuint ne = node2->edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = node2->edge(i);
    PhfNode* node1 = edge->node1();
    if ( node1 == from ) {
      continue;
    }
    if ( v_mark[node1->id()] ) {
      return false;
    }
    edge_list.push_back(edge);
    edge->set_dir(1);
    v_mark[node1->id()] = true;
    bool stat = dfs1(node1, node2, v_mark, edge_list);
    if ( !stat ) {
      return false;
    }
  }
  return true;
}

int
phf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // p オプション
  PoptUint popt_p(NULL, 'p',
		  "specify the number of inputs", "<INT>");
  app.add_option(&popt_p);

  // count オプション
  PoptUint popt_count("count", 'c',
		      "specify loop limit", "<INT>");
  app.add_option(&popt_count);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 p = 0;
  if ( popt_p.is_specified() ) {
    p = popt_p.val();
  }

  ymuint32 count_limit = 10000;
  if ( popt_count.is_specified() ) {
    count_limit = popt_count.val();
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);

  if ( n_args != 1 ) {
    app.usage(2);
  }

  const char* f_str = args[0].c_str();
  ifstream ifs(f_str);
  if ( !ifs ) {
    cerr << f_str << ": No such file" << endl;
    return 2;
  }

  RvMgr rvmgr;

  bool rstat = rvmgr.read_data(ifs);
  if ( !rstat ) {
    cerr << "read error" << endl;
    return 3;
  }

  ymuint n = rvmgr.vect_size();

  if ( (1U << (p * 2)) < n ) {
    cerr << "p is too small" << endl;
    return 4;
  }
  if ( p > 31 ) {
    cerr << "p is too big" << endl;
    return 4;
  }

  RandGen rg;
  for (ymuint count = 0; count < count_limit; ++ count) {
    // f1, f2 をランダムに作る．
    RandPermGen rpg(n);

    rpg.generate(rg);
    vector<ymuint> f1_vect(p);
    for (ymuint i = 0; i < p; ++ i) {
      f1_vect[i] = rpg.elem(i);
    }

    rpg.generate(rg);
    vector<ymuint> f2_vect(p);
    for (ymuint i = 0; i < p; ++ i) {
      f2_vect[i] = rpg.elem(i);
    }

    const vector<RegVect*>& vlist = rvmgr.vect_list();
    ymuint nv = vlist.size();
    hash_map<ymuint32, PhfNode*> v1_hash;
    hash_map<ymuint32, PhfNode*> v2_hash;
    vector<PhfNode*> v1_array(nv);
    vector<PhfNode*> v2_array(nv);
    vector<PhfNode*> v_array;
    ymuint e_id = 0;
    for (ymuint i = 0; i < nv; ++ i) {
      RegVect* rv = vlist[i];
      ymuint32 v1 = 0U;
      for (ymuint b = 0; b < p; ++ b) {
	ymuint pos = f1_vect[b];
	if ( rv->val(pos) ) {
	  v1 |= (1U << b);
	}
      }
      hash_map<ymuint32, PhfNode*>::iterator p1 = v1_hash.find(v1);
      PhfNode* node1 = NULL;
      if ( p1 == v1_hash.end() ) {
	ymuint id1 = v1_hash.size();
	node1 = new PhfNode(id1, v1);
	v_array.push_back(node1);
	v1_hash.insert(make_pair(v1, node1));
      }
      else {
	node1 = p1->second;
      }
      v1_array[i] = node1;
    }

    ymuint offset = v1_hash.size();
    for (ymuint i = 0; i < nv; ++ i) {
      RegVect* rv = vlist[i];
      ymuint32 v2 = 0U;
      for (ymuint b = 0; b < p; ++ b) {
	ymuint pos = f2_vect[b];
	if ( rv->val(pos) ) {
	  v2 |= (1U << b);
	}
      }
      hash_map<ymuint32, PhfNode*>::iterator p2 = v2_hash.find(v2);
      PhfNode* node2 = NULL;
      if ( p2 == v2_hash.end() ) {
	ymuint id2 = v2_hash.size() + offset;
	node2 = new PhfNode(id2, v2);
	v_array.push_back(node2);
	v2_hash.insert(make_pair(v2, node2));
      }
      else {
	node2 = p2->second;
      }
      v2_array[i] = node2;

      PhfNode* node1 = v1_array[i];
      PhfEdge* edge = new PhfEdge(e_id, node1, node2, i);
      node1->add_edge(edge);
      node2->add_edge(edge);
      ++ e_id;
    }

    ymuint nn = v_array.size();

    { // simple check
      bool not_simple = false;
      for (ymuint i = 0; i < nv; ++ i) {
	PhfNode* node = v1_array[i];
	ymuint ne = node->edge_num();
	vector<ymuint> mark(nn, false);
	for (ymuint j = 0; j < ne; ++ j) {
	  PhfEdge* edge = node->edge(j);
	  PhfNode* node2 = edge->node2();
	  if ( mark[node2->id()] ) {
	    not_simple = true;
	    break;
	  }
	  mark[node2->id()] = true;
	}
      }
      if ( not_simple ) {
	continue;
      }
    }

    vector<PhfEdge*> edge_list;
    {
      vector<bool> v_mark(nn, false);

      bool error = false;
      for ( ; ; ) {
	PhfNode* node0 = NULL;
	for (ymuint i = 0; i < nn; ++ i) {
	  PhfNode* node = v_array[i];
	  if ( !v_mark[node->id()] ) {
	    node0 = node;
	    break;
	  }
	}
	if ( node0 == NULL ) {
	  break;
	}
	v_mark[node0->id()] = true;
	bool stat = dfs1(node0, NULL, v_mark, edge_list);
	if ( !stat ) {
	  error = true;
	  break;
	}
      }
      if ( error ) {
	continue;
      }
    }

    ymuint ne = edge_list.size();
    for (ymuint i = 0; i < ne; ++ i) {
      PhfEdge* edge = edge_list[i];
      PhfNode* node1 = edge->node1();
      PhfNode* node2 = edge->node2();
      if ( edge->dir() ) {
	ymuint32 val = node2->val() ^ edge->val();
	node1->set_val(val);
      }
      else {
	ymuint32 val = node1->val() ^ edge->val();
	node2->set_val(val);
      }
      cout << "Edge#" << setw(6) << edge->id() << ": " << edge->val() << endl
	   << "Node#" << setw(6) << node1->id() << ": " << node1->val() << endl
	   << "Node#" << setw(6) << node2->id() << ": " << node2->val() << endl
	   << endl;
    }

    for (ymuint i = 0; i < nv; ++ i) {
      PhfNode* node1 = v1_array[i];
      PhfNode* node2 = v2_array[i];
      cout << "#" << i << ": "
	   << setw(6) << node1->id() << " = " << node1->val()
	   << ", "
	   << setw(6) << node2->id() << " = " << node2->val()
	   << ": "
	   << (node1->val() ^ node2->val()) << endl;
    }
    break;
  }

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::phf;

  return phf(argc, argv);
}
