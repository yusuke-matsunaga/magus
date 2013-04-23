
/// @file CiPat2Mgr.cc
/// @brief CiPat2Mgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiPat2Mgr.h"
#include "CiPat2Graph.h"
#include "libcomp/LcPat2Mgr.h"
#include "libcomp/LcPat2Node.h"
#include "libcomp/LcPat2Handle.h"


BEGIN_NAMESPACE_YM_CELL

using namespace nsLibcomp;

//////////////////////////////////////////////////////////////////////
// クラス CiPat2Mgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiPat2Mgr::CiPat2Mgr() :
  mNodeNum(0),
  mNodeTypeArray(NULL),
  mEdgeNum(0),
  mEdgeArray(NULL),
  mPatNum(0),
  mPatArray(NULL)
{
}

// @brief デストラクタ
CiPat2Mgr::~CiPat2Mgr()
{
}

// @brief このセルライブラリに含まれるセルの最大の入力数を得る．
ymuint
CiPat2Mgr::max_input() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < mPatNum; ++ i) {
    const CellPat2Graph& pat = this->pat(i);
    ymuint ni = pat.input_num();
    if ( ans < ni ) {
      ans = ni;
    }
  }
  return ans;
}

// @brief パタンを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
const CellPat2Graph&
CiPat2Mgr::pat(ymuint id) const
{
  assert_cond( id < pat_num(), __FILE__, __LINE__);
  return mPatArray[id];
}

// @brief LcPatMgr の情報をコピーする．
// @param[in] src コピー元
// @param[in] alloc メモリアロケータ
void
CiPat2Mgr::copy(const LcPat2Mgr& src,
		Alloc& alloc)
{
  // ノードの情報をコピーする．
  ymuint nn = src.node_num();
  set_node_num(nn, alloc);
  ymuint ne = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    LcPat2Node* src_node = src.node(i);
    ne += src_node->fanin_num();
  }
  set_edge_num(ne, alloc);
  ymuint edge_id = 0;
  for (ymuint i = 0; i < nn; ++ i) {
    LcPat2Node* src_node = src.node(i);
    ymuint32 v = 0U;
    ymuint ni = src_node->fanin_num();
    if ( src_node->is_input() ) {
      v = kCellPatInput | (src_node->input_id() << 2);
    }
    else if ( src_node->is_and() ) {
      v = kCellPatAnd | (ni << 2);
    }
    else if ( src_node->is_xor() ) {
      v = kCellPatXor | (ni << 2);
    }
    mNodeTypeArray[i] = v;
    for (ymuint j = 0; j < ni; ++ j) {
      ymuint v = (j << 1);
      if ( src_node->fanin_inv(j) ) {
	v |= 1U;
      }
      mEdgeArray[edge_id * 3 + 0] = src_node->fanin(j)->id();
      mEdgeArray[edge_id * 3 + 1] = src_node->id();
      mEdgeArray[edge_id * 3 + 2] = v;
      ++ edge_id;
    }
  }

  // パタンの情報をコピーする．
  ymuint np = src.pat_num();
  set_pat_num(np, alloc);
  for (ymuint i = 0; i < np; ++ i) {
    vector<ymuint32> node_list;
    ymuint v = src.pat_node_list(i, node_list);
    ymuint ne = node_list.size();
    CiPat2Graph& pg = mPatArray[i];
    pg.init(src.rep_id(i), v, ne, alloc);
    for (ymuint j = 0; j < ne; ++ j) {
      pg.set_edge(j, node_list[j]);
    }
  }
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
void
CiPat2Mgr::dump(BinO& bos) const
{
  // パタングラフのノード情報のダンプ
  bos << mNodeNum;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    bos << mNodeTypeArray[i];
  }

  // パタングラフの枝情報のダンプ
  bos << mEdgeNum;
  for (ymuint i = 0; i < mEdgeNum; ++ i) {
    bos << mEdgeArray[i];
  }

  // パタングラフの情報のダンプ
  bos << mPatNum;
  for (ymuint i = 0; i < mPatNum; ++ i) {
    mPatArray[i].dump(bos);
  }
}

// @brief データを読み込んでセットする．
// @param[in] bis 入力元のストリーム
// @param[in] alloc メモリアロケータ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
CiPat2Mgr::restore(BinI& bis,
		   Alloc& alloc)
{
  // ノードの情報を読み込む．
  ymuint32 nn;
  bis >> nn;
  set_node_num(nn, alloc);
  for (ymuint i = 0; i < nn; ++ i) {
    bis >> mNodeTypeArray[i];
    if ( node_type(i) == kCellPatInput ) {
      assert_cond( input_id(i) == i, __FILE__, __LINE__);
    }
  }

  // 枝の情報を読み込む．
  ymuint32 ne;
  bis >> ne;
  set_edge_num(ne, alloc);
  for (ymuint i = 0; i < ne; ++ i) {
    bis >> mEdgeArray[i];
  }

  // パタングラフの情報を読み込む．
  ymuint32 np;
  bis >> np;
  set_pat_num(np, alloc);
  for (ymuint id = 0; id < np; ++ id) {
    mPatArray[id].restore(bis, alloc);
  }

  return true;
}

// @brief ノード数を設定する．
// @param[in] nn ノード数
// @param[in] alloc メモリアロケータ
void
CiPat2Mgr::set_node_num(ymuint nn,
			Alloc& alloc)
{
  mNodeNum = nn;
  void* p = alloc.get_memory(sizeof(ymuint32) * mNodeNum);
  mNodeTypeArray = new (p) ymuint32[mNodeNum];
}

// @brief 枝数を設定する．
// @param[in] ne 枝数
// @param[in] alloc メモリアロケータ
void
CiPat2Mgr::set_edge_num(ymuint ne,
			Alloc& alloc)
{
  mEdgeNum = ne;
  void* p = alloc.get_memory(sizeof(ymuint32) * mEdgeNum * 3);
  mEdgeArray = new (p) ymuint32[mEdgeNum * 3];
}

// @brief パタン数を設定する．
void
CiPat2Mgr::set_pat_num(ymuint np,
		       Alloc& alloc)
{
  mPatNum = np;
  void* p = alloc.get_memory(sizeof(CiPat2Graph) * mPatNum);
  mPatArray = new (p) CiPat2Graph[mPatNum];
}

END_NAMESPACE_YM_CELL
