
/// @file phf_sim.cc
/// @brief phf_sim のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_IGF

BEGIN_NONAMESPACE

class PhfEdge;

struct PhfNode
{
  ymuint mId;

  vector<PhfEdge*> mEdgeList;

  bool mVisited;

};

struct PhfEdge
{
  // もとのデータ番号
  ymuint mIndex;

  // ノードのリスト
  vector<PhfNode*> mNodeList;

};

bool
dfs(PhfNode* node,
    PhfEdge* edge_from,
    vector<pair<ymuint, ymuint> >& index_list)
{
  for (vector<PhfEdge*>::iterator p = node->mEdgeList.begin();
       p != node->mEdgeList.end(); ++ p) {
    PhfEdge* edge = *p;
    if ( edge == edge_from ) {
      continue;
    }
    for (vector<PhfNode*>::iterator q = edge->mNodeList.begin();
	 q != edge->mNodeList.end(); ++ q) {
      PhfNode* node1 = *q;
      if ( node1 != node ) {
	index_list.push_back(make_pair(edge->mIndex, node1->mId));
	if ( node1->mVisited ) {
	  return false;
	}
	node1->mVisited = true;
	bool stat = dfs(node1, edge, index_list);
	if ( !stat ) {
	  return false;
	}
      }
    }
  }
  return true;
}

END_NONAMESPACE

// @param[in] n データ数
// @param[in] k スロット数
// @param[in] m 多重度
int
phf_sim1(ymuint n,
	 ymuint k,
	 ymuint m,
	 RandGen& rg)
{
  vector<vector<ymuint> > func_list(m, vector<ymuint>(n));

  // ランダムに関数を作る．
  for (ymuint f_pos = 0; f_pos < m; ++ f_pos) {
    vector<ymuint>& func = func_list[f_pos];
    for (ymuint i = 0; i < n; ++ i) {
      ymuint pos = rg.int32() % k;
      func[i] = f_pos * k + pos;
    }
  }

  // ノードを作る．
  ymuint mk = m * k;
  vector<PhfNode> node_array(mk);
  for (ymuint i = 0; i < mk; ++ i) {
    node_array[i].mId = i;
  }

  // 枝を作る．
  vector<PhfEdge> edge_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    PhfEdge* edge = &edge_array[i];
    edge->mIndex = i;
    for (ymuint f_pos = 0; f_pos < m; ++ f_pos) {
      ymuint v = func_list[f_pos][i];
      PhfNode* node = &node_array[v];
      edge->mNodeList.push_back(node);
      node->mEdgeList.push_back(edge);
    }
  }

  // simple チェック
  for (ymuint i = 0; i < n; ++ i) {
    PhfEdge* edge = &edge_array[i];
    PhfNode* node0 = edge->mNodeList[0];
    for (vector<PhfEdge*>::iterator p = node0->mEdgeList.begin();
	 p != node0->mEdgeList.end(); ++ p) {
      PhfEdge* edge1 = *p;
      if ( edge == edge1 ) {
	continue;
      }
      bool diff = false;
      for (ymuint j = 0; j < m; ++ j) {
	if ( edge->mNodeList[j] != edge1->mNodeList[j] ) {
	  diff = true;
	  break;
	}
      }
      if ( !diff ) {
#if 0
	cout << "Not simple" << endl;
	cout << "Index#" << i << ":";
	for (ymuint j = 0; j < m; ++ j) {
	  cout << " " << func_list[j][i];
	}
	cout << endl;
	cout << "Index#" << edge1->mIndex << ":";
	for (ymuint j = 0; j < m; ++ j) {
	  cout << " " << func_list[j][edge1->mIndex];
	}
	cout << endl;
#endif
	return 1;
      }
    }
  }

  // acyclic チェック
  for (ymuint i = 0; i < mk; ++ i) {
    node_array[i].mVisited = false;
  }
  for ( ; ; ) {
    // mVisited マークのついていないノードを探す．
    PhfNode* node0 = NULL;
    for (ymuint i = 0; i < mk; ++ i) {
      PhfNode* node = &node_array[i];
      if ( !node->mVisited ) {
	node0 = node;
	break;
      }
    }
    if ( node0 == NULL ) {
      // すべてチェックした．
      break;
    }

    vector<pair<ymuint, ymuint> > index_list;
    node0->mVisited = true;
    bool stat = dfs(node0, NULL, index_list);
    if ( !stat ) {
#if 0
      cout << "Cyclic" << endl;
      for (vector<pair<ymuint, ymuint> >::iterator p = index_list.begin();
	   p != index_list.end(); ++ p) {
	ymuint index = p->first;
	ymuint node_id = p->second;
	cout << "Node#" << node_id  << ",  ";
	cout << "Index#" << index << ":";
	for (ymuint j = 0; j < m; ++ j) {
	  cout << " " << func_list[j][index];
	}
	cout << endl;
      }
#endif
      return 2;
    }
  }
  return 0;
}

int
phf_sim(int argc,
	const char** argv)
{
  if ( argc != 4 ) {
    cerr << "Usage: phf_sim <n> <k> <m>" << endl;
    return -1;
  }

  int tmp_n = atoi(argv[1]);
  if ( tmp_n <= 0 ) {
    cerr << "'n' must be positive integer" << endl;
    return -1;
  }

  ymuint n = static_cast<ymuint>(tmp_n);

  int tmp_k = atoi(argv[2]);
  if ( tmp_k <= 0 ) {
    cerr << "'k' must be positive integer" << endl;
    return -1;
  }

  ymuint k = static_cast<ymuint>(tmp_k);

  int tmp_m = atoi(argv[3]);
  if ( tmp_m <= 0 ) {
    cerr << "'m' must be positive integer" << endl;
    return -1;
  }
  ymuint m = static_cast<ymuint>(tmp_m);

  RandGen rg;

  ymuint nf = 40;
  vector<vector<ymuint> > func_list(nf, vector<ymuint>(n));

  // ランダムに関数を作る．
  for (ymuint j = 0; j < nf; ++ j) {
    vector<ymuint>& func = func_list[j];
    for (ymuint i = 0; i < n; ++ i) {
      ymuint pos = rg.int32() % k;
      func[i] = pos;
    }
  }

  ymuint c_ok = 0;
  ymuint c_simple = 0;
  ymuint c_cyclic = 0;
  ymuint nt = 2000;
  for (ymuint i1 = 0; i1 < nf; ++ i1) {
    const vector<ymuint>& func1 = func_list[i1];
    for (ymuint i2 = i1 + 1; i2 < nf; ++ i2) {
      const vector<ymuint>& func2 = func_list[i2];
      switch ( phf_sim1(n, k, m, func1, func2) ) {
      case 0: ++ c_ok; break;
      case 1: ++ c_simple; break;
      case 2: ++ c_cyclic; break;
      }
    }
  }

  cout << "Total " << nt << " trials" << endl
       << " # of Success:    " << c_ok << endl
       << " # of Not Simple: " << c_simple << endl
       << " # of Cyclic:     " << c_cyclic << endl;

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::phf_sim;

  return phf_sim(argc, argv);
}
