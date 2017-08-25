
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"

#include "ym/CellFFInfo.h"
#include "ym/CellLatchInfo.h"
#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnDff.h"
#include "ym/BnLatch.h"

#include "SbjGraph.h"
#include "SbjPort.h"


BEGIN_NAMESPACE_YM_CELLMAP

// コンストラクタ
MapRecord::MapRecord()
{
}

// @brief コピーコンストラクタ
MapRecord::MapRecord(const MapRecord& src) :
  mConst0(src.mConst0),
  mDffInfo(src.mDffInfo),
  mLatchInfo(src.mLatchInfo),
  mNodeInfo(src.mNodeInfo)
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const SbjGraph& sbjgraph)
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
MapRecord::set_const0(const Cell* cell)
{
  mConst0 = cell;
}

// @brief 定数1セルをセットする．
void
MapRecord::set_const1(const Cell* cell)
{
  mConst1 = cell;
}

// @brief D-FF のマッチを記録する．
// @param[in] dff D-FF
// @param[in] inv 極性
// @param[in] cell セル
void
MapRecord::set_dff_match(const SbjDff* dff,
			 bool inv,
			 const Cell* cell)
{
  ymuint offset = inv ? 1 : 0;
  mDffInfo[dff->id() * 2 + offset] = cell;
}

// @brief ラッチのマッチを記録する．
// @param[in] latch ラッチ
// @param[in] latch_info ピンの割り当て情報
// @param[in] cell セル
void
MapRecord::set_latch_match(const SbjLatch* latch,
			   bool inv,
			   const Cell* cell)
{
  ymuint offset = inv ? 1 : 0;
  mLatchInfo[latch->id() * 2 + offset] = cell;
}

// @brief 論理ゲートのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] match 対応するマッチ
// @param[in] cell セル
void
MapRecord::set_logic_match(const SbjNode* node,
			   bool inv,
			   const Cut& match,
			   const Cell* cell)
{
  NodeInfo& node_info = _node_info(node, inv);
  node_info.mMatch = match;
  node_info.mCell = cell;
}

// @brief インバータのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] cell セル
void
MapRecord::set_inv_match(const SbjNode* node,
			 bool inv,
			 const Cell* cell)
{
  NodeInfo& node_info = _node_info(node, inv);
  node_info.mMatch.resize(1);
  node_info.mMatch.set_leaf(0, node, !inv);
  node_info.mCell = cell;
}

END_NAMESPACE_YM_CELLMAP
