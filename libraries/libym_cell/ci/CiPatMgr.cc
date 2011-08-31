
/// @file CiPatMgr.cc
/// @brief CiPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiPatMgr.h"
#include "ym_cell/CellPatGraph.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CellPatGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellPatGraph::CellPatGraph() :
  mInputNum(0U),
  mEdgeNum(0U),
  mEdgeList(NULL)
{
}

// @brief デストラクタ
CellPatGraph::~CellPatGraph()
{
  // mEdgeList はメモリアロケータが管理している．
}


//////////////////////////////////////////////////////////////////////
// クラス CiPatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiPatMgr::CiPatMgr() :
  mNodeNum(0U),
  mNodeTypeArray(NULL),
  mEdgeArray(NULL),
  mPatNum(0),
  mPatArray(NULL)
{
}

// @brief デストラクタ
CiPatMgr::~CiPatMgr()
{
  init();
}

// @brief 内容を初期化する．
// @note 以前確保されたメモリは開放される．
void
CiPatMgr::init()
{
  // mAlloc の初期化は CellMgr で行われる．
  mNodeNum = 0U;
  mPatNum = 0U;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @param[in] alloc メモリアロケータ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
CiPatMgr::load(istream& s,
		 AllocBase& alloc)
{
  // 以前の内容を捨てる．
  init();

  // ノードと枝の情報を読み込む．
  mNodeNum = BinIO::read_32(s);
  void* p = alloc.get_memory(sizeof(ymuint32) * mNodeNum);
  mNodeTypeArray = new (p) ymuint32[mNodeNum];
  void* q = alloc.get_memory(sizeof(ymuint32) * mNodeNum * 2);
  mEdgeArray = new (q) ymuint32[mNodeNum * 2];
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    mNodeTypeArray[i] = BinIO::read_32(s);
    mEdgeArray[i * 2] = BinIO::read_32(s);
    mEdgeArray[i * 2 + 1] = BinIO::read_32(s);
    if ( node_type(i) == kCellPatInput ) {
      assert_cond( input_id(i) == i, __FILE__, __LINE__);
    }
  }

  // パタングラフの情報を読み込む．
  mPatNum = BinIO::read_32(s);
  void* r = alloc.get_memory(sizeof(CellPatGraph) * mPatNum);
  mPatArray = new (r) CellPatGraph[mPatNum];
  for (ymuint id = 0; id < mPatNum; ++ id) {
    CellPatGraph& pg = mPatArray[id];
    pg.mInputNum = BinIO::read_32(s);
    ymuint n = BinIO::read_32(s);
    pg.mEdgeNum = n;
    if ( n > 0 ) {
      void* p = alloc.get_memory(sizeof(ymuint32) * n);
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

  return true;
}

// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
ymuint
CiPatMgr::max_input() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < mPatNum; ++ i) {
    const CellPatGraph& pat = this->pat(i);
    ymuint ni = pat.input_num();
    if ( ans < ni ) {
      ans = ni;
    }
  }
  return ans;
}

// @brief パタンを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
const CellPatGraph&
CiPatMgr::pat(ymuint id) const
{
  return mPatArray[id];
}

// @relates CiPatMgr
// @brief CiPatMgr の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] pat_mgr パタングラフ
void
dump(ostream& s,
     const CiPatMgr& pat_mgr)
{
  s << "==== CiPatMgr dump start ====" << endl;

  // ノードの種類の出力
  ymuint nn = pat_mgr.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    s << "Node#" << i << ": ";
    switch ( pat_mgr.node_type(i) ) {
    case kCellPatInput: s << "INPUT#" << pat_mgr.input_id(i) ; break;
    case kCellPatAnd:   s << "AND"; break;
    case kCellPatXor:   s << "XOR"; break;
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
    const CellPatGraph& pat = pat_mgr.pat(i);
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

  s << "==== CiPatMgr dump end ====" << endl;
}

END_NAMESPACE_YM_CELL
