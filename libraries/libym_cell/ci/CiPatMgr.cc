
/// @file CiPatMgr.cc
/// @brief CiPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiPatMgr.h"
#include "CiPatGraph.h"
#include "ym_utils/BinIO.h"
#include "libcomp/LcPatMgr.h"
#include "libcomp/LcPatNode.h"
#include "libcomp/LcPatHandle.h"


BEGIN_NAMESPACE_YM_CELL

using namespace nsLibcomp;

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
  assert_cond( id < pat_num(), __FILE__, __LINE__);
  return mPatArray[id];
}

// @brief LcPatMgr の情報をコピーする．
// @param[in] src コピー元
// @param[in] alloc メモリアロケータ
void
CiPatMgr::copy(const LcPatMgr& src,
	       Alloc& alloc)
{
  // ノードの情報をコピーする．
  ymuint nn = src.node_num();
  set_node_num(nn, alloc);
  for (ymuint i = 0; i < nn; ++ i) {
    LcPatNode* src_node = src.node(i);
    ymuint32 v = 0U;
    if ( src_node->is_input() ) {
      v = kCellPatInput | (src_node->input_id() << 2);
    }
    else if ( src_node->is_and() ) {
      v = kCellPatAnd;
    }
    else if ( src_node->is_xor() ) {
      v = kCellPatXor;
    }
    mNodeTypeArray[i] = v;
    for (ymuint j = 0; j < 2; ++ j) {
      ymuint v = 0U;
      if ( !src_node->is_input() ) {
	v = src_node->fanin(j)->id() * 2;
	if ( src_node->fanin_inv(j) ) {
	  v |= 1U;
	}
      }
      mEdgeArray[i * 2 + j] = v;
    }
  }

  // パタンの情報をコピーする．
  ymuint np = src.pat_num();
  set_pat_num(np, alloc);
  for (ymuint i = 0; i < np; ++ i) {
    vector<ymuint> node_list;
    ymuint v = src.pat_node_list(i, node_list);
    ymuint ne = node_list.size();
    CiPatGraph& pg = mPatArray[i];
    pg.init(src.rep_id(i), v, ne, alloc);
    for (ymuint j = 0; j < ne; ++ j) {
      pg.set_edge(j, node_list[j]);
    }
  }
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
void
CiPatMgr::dump(BinO& bos) const
{
  // パタングラフのノード情報のダンプ
  bos << mNodeNum;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    bos << mNodeTypeArray[i]
	<< mEdgeArray[i * 2 + 0]
	<< mEdgeArray[i * 2 + 1];
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
CiPatMgr::restore(BinI& bis,
		  Alloc& alloc)
{
  // ノードと枝の情報を読み込む．
  ymuint32 nn;
  bis >> nn;
  set_node_num(nn, alloc);
  for (ymuint i = 0; i < nn; ++ i) {
    bis >> mNodeTypeArray[i]
	>> mEdgeArray[i * 2]
	>> mEdgeArray[i * 2 + 1];
    if ( node_type(i) == kCellPatInput ) {
      assert_cond( input_id(i) == i, __FILE__, __LINE__);
    }
  }

  // パタングラフの情報を読み込む．
  ymuint32 np;
  bis >> np;
  set_pat_num(np, alloc);
  for (ymuint id = 0; id < mPatNum; ++ id) {
    mPatArray[id].restore(bis, alloc);
  }

  return true;
}

// @brief ノード数を設定する．
// @param[in] nn ノード数
// @param[in] alloc メモリアロケータ
void
CiPatMgr::set_node_num(ymuint nn,
		       Alloc& alloc)
{
  mNodeNum = nn;
  void* p = alloc.get_memory(sizeof(ymuint32) * mNodeNum);
  mNodeTypeArray = new (p) ymuint32[mNodeNum];
  void* q = alloc.get_memory(sizeof(ymuint32) * mNodeNum * 2);
  mEdgeArray = new (q) ymuint32[mNodeNum * 2];
}

// @brief パタン数を設定する．
void
CiPatMgr::set_pat_num(ymuint np,
		      Alloc& alloc)
{
  mPatNum = np;
  void* p = alloc.get_memory(sizeof(CiPatGraph) * mPatNum);
  mPatArray = new (p) CiPatGraph[mPatNum];
}

END_NAMESPACE_YM_CELL
