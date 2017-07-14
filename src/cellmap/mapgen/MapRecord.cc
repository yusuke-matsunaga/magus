
/// @file MapRecord.cc
/// @brief MapRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MapRecord.h"

#include "YmCell/CellFFInfo.h"
#include "YmCell/CellLatchInfo.h"

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnDff.h"
#include "YmNetworks/BdnLatch.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/CmnMgr.h"
#include "YmNetworks/CmnPort.h"
#include "YmNetworks/CmnDff.h"
#include "YmNetworks/CmnDffCell.h"
#include "YmNetworks/CmnLatch.h"

#include "Match.h"


BEGIN_NAMESPACE_YM_CELLMAP

// コンストラクタ
MapRecord::MapRecord()
{
}

// デストラクタ
MapRecord::~MapRecord()
{
}

// @brief @brief 作業領域を初期化する．
// @param[in] sbjgraph サブジェクトグラフ
void
MapRecord::init(const BdnMgr& sbjgraph)
{
  mDffInfo.clear();
  mDffInfo.resize(sbjgraph.max_dff_id() * 2);
  mLatchInfo.clear();
  mLatchInfo.resize(sbjgraph.max_latch_id() * 2);
  mNodeInfo.clear();
  mNodeInfo.resize(sbjgraph.max_node_id() * 2);
}

// @brief マッチの情報だけコピーする．
void
MapRecord::copy(const MapRecord& src)
{
  ymuint n = src.mNodeInfo.size();
  mNodeInfo.clear();
  mNodeInfo.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mNodeInfo[i].mMatch = src.mNodeInfo[i].mMatch;
  }
}

// @brief D-FF のマッチを記録する．
// @param[in] dff D-FF
// @param[in] inv 極性
// @param[in] cell セル
// @param[in] ff_info ピンの割り当て情報
void
MapRecord::set_dff_match(const BdnDff* dff,
			 bool inv,
			 const Cell* cell,
			 const CellFFInfo& ff_info)
{
  ymuint offset = inv ? 1 : 0;
  DffInfo& dffinfo = mDffInfo[dff->id() * 2 + offset];
  dffinfo.mCell = cell;
  dffinfo.mPinInfo = ff_info;
}

// @brief ラッチのマッチを記録する．
// @param[in] latch ラッチ
// @param[in] latch_info ピンの割り当て情報
// @param[in] cell セル
void
MapRecord::set_latch_match(const BdnLatch* latch,
			   bool inv,
			   const Cell* cell,
			   const CellLatchInfo& latch_info)
{
  ymuint offset = inv ? 1 : 0;
  LatchInfo& latchinfo = mLatchInfo[latch->id() * 2 + offset];
  latchinfo.mCell = cell;
  latchinfo.mPinInfo = latch_info;
}

// @brief 論理ゲートのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] match 対応するマッチ
// @param[in] cell セル
void
MapRecord::set_logic_match(const BdnNode* node,
			   bool inv,
			   const Match& match,
			   const Cell* cell)
{
  NodeInfo& node_info = get_node_info(node, inv);
  node_info.mMatch = match;
  node_info.mCell = cell;
}

// @brief インバータのマッチを記録する．
// @param[in] node 該当のノード
// @param[in] inv 極性
// @param[in] cell セル
void
MapRecord::set_inv_match(const BdnNode* node,
			 bool inv,
			 const Cell* cell)
{
  NodeInfo& node_info = get_node_info(node, inv);
  node_info.mMatch.resize(1);
  node_info.mMatch.set_leaf(0, node, !inv);
  node_info.mCell = cell;
}

END_NAMESPACE_YM_CELLMAP
