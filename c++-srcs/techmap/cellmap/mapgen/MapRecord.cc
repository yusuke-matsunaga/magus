
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.

#include "MapRecord.h"

#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnDff.h"

#include "SbjGraph.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_CELLMAP

// コンストラクタ
MapRecord::MapRecord(
  const ClibCellLibrary& library
) : mCellLibrary{library}
{
}

// @brief コピーコンストラクタ
MapRecord::MapRecord(
  const MapRecord& src
) : mCellLibrary{src.mCellLibrary},
    mConst0Id{src.mConst0Id},
    mConst1Id{src.mConst1Id},
    mDffInfo{src.mDffInfo},
    mLatchInfo{src.mLatchInfo},
    mNodeInfo{src.mNodeInfo}
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
void
MapRecord::init(
  const SbjGraph& sbjgraph
)
{
  mDffInfo.clear();
  mDffInfo.resize(sbjgraph.dff_num() * 2);
  mLatchInfo.clear();
  mLatchInfo.resize(sbjgraph.latch_num() * 2);
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.node_num() * 2);
}

// @brief 定数０セルをセットする．
void
MapRecord::set_const0(
  SizeType cell_id
)
{
  mConst0Id = cell_id;
}

// @brief 定数1セルをセットする．
void
MapRecord::set_const1(
  SizeType cell_id
)
{
  mConst1Id = cell_id;
}

// @brief D-FF のマッチを記録する．
void
MapRecord::set_dff_match(
  const SbjDff* dff,
  bool inv,
  SizeType cell_id
)
{
  int offset = inv ? 1 : 0;
  mDffInfo[dff->id() * 2 + offset] = cell_id;
}

// @brief ラッチのマッチを記録する．
void
MapRecord::set_latch_match(
  const SbjLatch* latch,
  bool inv,
  SizeType cell_id
)
{
  int offset = inv ? 1 : 0;
  mLatchInfo[latch->id() * 2 + offset] = cell_id;
}

// @brief 論理ゲートのマッチを記録する．
void
MapRecord::set_logic_match(
  const SbjNode* node,
  bool inv,
  const Cut& match,
  SizeType cell_id
)
{
  NodeInfo& node_info = _node_info(node, inv);
  node_info.mMatch = match;
  node_info.mCellId = cell_id;
}

// @brief インバータのマッチを記録する．
void
MapRecord::set_inv_match(
  const SbjNode* node,
  bool inv,
  SizeType cell_id
)
{
  NodeInfo& node_info = _node_info(node, inv);
  node_info.mMatch.resize(1);
  node_info.mMatch.set_leaf(0, node, !inv);
  node_info.mCellId = cell_id;
}

END_NAMESPACE_CELLMAP
