
/// @file libym_techmap/cellmap/PatMgr.cc
/// @brief PatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PatMgr.h"
#include "PatGraph.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELLMAP

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
  // mEdgeList はメモリアロケータが管理している．
}


//////////////////////////////////////////////////////////////////////
// クラス PatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] alloc メモリアロケータ
PatMgr::PatMgr(AllocBase& alloc) :
  mAlloc(alloc),
  mNodeNum(0U),
  mNodeTypeArray(NULL),
  mEdgeArray(NULL),
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
  // mAlloc の初期化は CellMgr で行われる．
  mNodeNum = 0U;
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
  mNodeNum = BinIO::read_32(s);
  void* p = mAlloc.get_memory(sizeof(ymuint32) * mNodeNum);
  mNodeTypeArray = new (p) ymuint32[mNodeNum];
  void* q = mAlloc.get_memory(sizeof(ymuint32) * mNodeNum * 2);
  mEdgeArray = new (q) ymuint32[mNodeNum * 2];
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeTypeArray[i] = BinIO::read_32(s);
    mEdgeArray[i * 2] = BinIO::read_32(s);
    mEdgeArray[i * 2 + 1] = BinIO::read_32(s);
    if ( node_type(i) == kInput ) {
      assert_cond( input_id(i) == i, __FILE__, __LINE__);
    }
  }

  // パタングラフの情報を読み込む．
  mPatNum = BinIO::read_32(s);
  void* r = mAlloc.get_memory(sizeof(PatGraph) * mPatNum);
  mPatArray = new (r) PatGraph[mPatNum];
  for (ymuint i = 0; i < mPatNum; ++ i) {
    load_patgraph(s, i);
  }

  return true;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @param[in] id 番号
void
PatMgr::load_patgraph(istream& s,
		      ymuint id)
{
  PatGraph& pg = mPatArray[id];
  pg.mInputNum = BinIO::read_32(s);
  ymuint n = BinIO::read_32(s);
  pg.mEdgeNum = n;
  if ( n > 0 ) {
    void* p = mAlloc.get_memory(sizeof(ymuint32) * n);
    pg.mEdgeList = new (p) ymuint32[n];
    for (ymuint i = 0; i < n; ++ i) {
      pg.mEdgeList[i] = BinIO::read_32(s);
    }
  }
  else {
    pg.mEdgeList = NULL;
  }
  pg.mRepId = BinIO::read_32(s);
}

// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
ymuint
PatMgr::max_input() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < mPatNum; ++ i) {
    const PatGraph& pat = this->pat(i);
    ymuint ni = pat.input_num();
    if ( ans < ni ) {
      ans = ni;
    }
  }
  return ans;
}

// @brief パタンを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
const PatGraph&
PatMgr::pat(ymuint id) const
{
  return mPatArray[id];
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
    s << "Pat#" << i << ": "
      << "Rep#" << pat.rep_id() << ": ";
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

END_NAMESPACE_YM_CELLMAP
