
/// @file libym_techmap/PatGraph.cc
/// @brief PatGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatMgr.h"
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

BEGIN_NONAMESPACE

ymuint
read_word(istream& s)
{
  static char buf[4];
  s.read(buf, 4);
  ymuint ans =
    static_cast<ymuint>(buf[0]) |
    (static_cast<ymuint>(buf[1]) << 8) |
    (static_cast<ymuint>(buf[2]) << 16) |
    (static_cast<ymuint>(buf[3]) << 24);
  return ans;
}

END_NONAMESPACE

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
  mInputNum = read_word(s);
  mEdgeNum = read_word(s);
  mEdgeList = new ymuint32[mEdgeNum];
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    mEdgeList[i] = read_word(s);
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
  init();
}

// @brief 内容を初期化する．
// @note 以前確保されたメモリは開放される．
void
PatMgr::init()
{
  delete [] mNodeTypeArray;
  delete [] mEdgeArray;
  delete [] mInputArray;
  delete [] mPatArray;
  mNodeNum = 0U;
  mInputNum = 0U;
  mPatNum = 0U;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
PatMgr::load(istream& s)
{
  // 以前の内容を捨てる．
  init();

  // ノードと枝の情報を読み込む．
  mNodeNum = read_word(s);
  mNodeTypeArray = new ymuint32[mNodeNum];
  mEdgeArray = new ymuint32[mNodeNum * 2];
  ymuint max_input = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeTypeArray[i] = read_word(s);
    mEdgeArray[i * 2] = read_word(s);
    mEdgeArray[i * 2 + 1] = read_word(s);
    if ( node_type(i) == kInput ) {
      if ( max_input < input_id(i) ) {
	max_input = input_id(i);
      }
    }
  }

  // 入力の情報を作り出す．
  mInputNum = max_input + 1;
  mInputArray = new ymuint32[mInputNum];
  for (ymuint i = 0; i < node_num(); ++ i) {
    if ( node_type(i) == kInput ) {
      mInputArray[input_id(i)] = i;
    }
  }

  // パタングラフの情報を読み込む．
  mPatNum = read_word(s);
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
    case PatMgr::kInput: s << "INPUT#" << pat_mgr.input_id(i) ; break;
    case PatMgr::kAnd:   s << "AND"; break;
    case PatMgr::kXor:   s << "XOR"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    s << endl;
  }
  s << endl;

  // 入力の情報の出力
  ymuint ni = pat_mgr.input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    s << "Input#" << i << ": Node#" << pat_mgr.input_node(i) << endl;
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
  s << endl;

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
