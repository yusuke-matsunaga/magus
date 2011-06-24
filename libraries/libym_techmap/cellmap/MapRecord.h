#ifndef LIBYM_TECHMAP_CELLMAP_MAPRECORD_H
#define LIBYM_TECHMAP_CELLMAP_MAPRECORD_H

/// @file libym_techmap/cellmap/MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.h 1293 2008-02-21 02:25:52Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_networks/cmn.h"
#include "Match.h"
#include "FFPosArray.h"
#include "LatchPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:

  /// @brief コンストラクタ
  MapRecord();

  /// @brief デストラクタ
  virtual
  ~MapRecord();


public:

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(const BdnMgr& sbjgraph);

  /// @brief マッチの情報だけコピーする．
  void
  copy(const MapRecord& src);

  /// @brief D-FF のマッチを記録する．
  /// @param[in] dff D-FF
  /// @param[in] cell セル
  /// @param[in] pos_array ピン情報の配列
  void
  set_dff_match(const BdnDff* dff,
		const Cell* cell,
		FFPosArray pos_array);

  /// @brief ラッチのマッチを記録する．
  /// @param[in] latch ラッチ
  /// @param[in] cell セル
  /// @param[in] pos_array ピン情報の配列
  void
  set_latch_match(const BdnLatch* latch,
		  const Cell* cell,
		  LatchPosArray pos_array);

  /// @brief マッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] match 対応するマッチ
  /// @param[in] cell セル
  void
  set_match(const BdnNode* node,
	    bool inv,
	    const Match& match,
	    const Cell* cell);

  /// @brief インバータのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_inv_match(const BdnNode* node,
		bool inv,
		const Cell* cell);

  /// @brief マッチを取り出す．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  const Match&
  get_match(const BdnNode* node,
	    bool inv);

  /// @brief マッピング結果を CmnMgr にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] const0_cell 定数0のセル
  /// @param[in] const1_cell 定数1のセル
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  void
  gen_mapgraph(const BdnMgr& sbjgraph,
	       const Cell* const0_cell,
	       const Cell* const1_cell,
	       CmnMgr& mapgraph);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 最終結果を作るためのバックトレースを行う．
  CmnNode*
  back_trace(const BdnNode* node,
	     bool inv,
	     CmnMgr& mapnetwork);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // D-FF の割り当て情報
  struct DffInfo
  {
    DffInfo()
    {
      mCell = NULL;
    }

    // セル
    const Cell* mCell;

    // ピンの割り当て情報
    FFPosArray mPosArray;
  };

  // ラッチの割り当て情報
  struct LatchInfo
  {
    LatchInfo()
    {
      mCell = NULL;
    }

    // セル
    const Cell* mCell;

    // ピンの割り当て情報
    LatchPosArray mPosArray;
  };

  // ノードの割り当て情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mCell = NULL;
      mMapNode = NULL;
    }

    // マッチ
    Match mMatch;

    // セル
    const Cell* mCell;

    // マップ結果
    CmnNode* mMapNode;

  };


private:

  /// @brief D-FF の割り当て情報を取り出す．
  DffInfo&
  get_dff_info(const BdnDff* dff);

  /// @brief ラッチの割り当て情報を取り出す．
  LatchInfo&
  get_latch_info(const BdnLatch* latch);

  /// @brief NodeInfo を取り出す．
  NodeInfo&
  get_node_info(const BdnNode* node,
		bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // D-FF の割り当て情報を格納した配列
  // キーは BdnDff の ID 番号
  vector<DffInfo> mDffInfo;

  // ラッチの割り当て情報を格納した配列
  // キーは BdnLatch の ID 番号
  vector<LatchInfo> mLatchInfo;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは BdnNode の ID 番号
  vector<NodeInfo> mNodeInfo;

  // back_trace 中に用いる作業領域
  vector<CmnNode*> mTmpFanins;

};

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_MAPRECORD_H
