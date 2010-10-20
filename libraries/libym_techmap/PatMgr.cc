
/// @file libym_techmap/PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatMgr.h"
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス PatGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatGraph::PatGraph() :
  mInputNum(0U),
  mEdgeNum(0U),
  mEdgeList(NULL)
{
}

// @brief デストラクタ
PatGraph::~PatGraph()
{
  delete [] mEdgeList;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
PatGraph::load(istream& s)
{
  s >> mInputNum
    >> mEdgeNum;
  mEdgeList = new ymuint32[mEdgeNum];
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    s >> mEdgeList[i];
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatMgr::PatMgr() :
  mNodeNum(0U),
  mNodeTypeArray(NULL),
  mEdgeArray(NULL),
  mInputNum(0U),
  mInputArray(NULL),
  mPatNum(0),
  mPatArray(NULL)
{
}

// @brief デストラクタ
PatMgr::~PatMgr()
{
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
  delete [] mInputArray;
  delete [] mPatArray;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
PatMgr::load(istream& s)
{
  s >> mNodeNum;
  cout << "mNodeNum = " << mNodeNum << endl;
  mNodeTypeArray = new ymuint32[mNodeNum];
  mEdgeArray = new ymuint32[mNodeNum * 2];
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    s >> mNodeTypeArray[i]
      >> mEdgeArray[i * 2]
      >> mEdgeArray[i * 2 + 1];
  }

  s >> mInputNum;
  mInputArray = new ymuint32[mInputNum];
  for (ymuint i = 0; i < mInputNum; ++ i) {
    s >> mInputArray[i];
  }

  s >> mPatNum;
  mPatArray = new PatGraph[mPatNum];
  for (ymuint i = 0; i < mPatNum; ++ i) {
    if ( !mPatArray[i].load(s) ) {
      return false;
    }
  }

  return true;
}

// @relates PatMgr
// @brief PatMgr の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] pat_mgr パタングラフ
void
dump(ostream& s,
     const PatMgr& pat_mgr)
{
  s << "==== PatMgr dump start ====" << endl;

  // ノードの種類の出力
  ymuint nn = pat_mgr.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( pat_mgr.node_type(i) ) {
    case PatMgr::kInput: s << "INPUT"; break;
    case PatMgr::kAnd:   s << "AND"; break;
    case PatMgr::kXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 枝の情報の出力
  ymuint ne = pat_mgr.edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    s << "Edge#" << i << ": " << pat_mgr.edge_from(i)
      << " -> " << pat_mgr.edge_to(i) << "(" << pat_mgr.edge_pos(i) << ")";
    if ( pat_mgr.edge_inv(i) ) {
      s << " ***";
    }
    s << endl;
  }

  // パタングラフの情報の出力
  ymuint np = pat_mgr.pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    const PatGraph& pat = pat_mgr.pat(i);
    s << "Pat#" << i << ": ";
    if ( pat.root_inv() ) {
      s << "[inv]";
    }
    s << "(" << pat.input_num() << "), ";
    ymuint n = pat.edge_num();
    for (ymuint i = 0; i < n; ++ i) {
      s << " " << pat.edge(i);
    }
    s << endl;
  }

  s << "==== PatMgr dump end ====" << endl;
}

END_NAMESPACE_YM_TECHMAP
