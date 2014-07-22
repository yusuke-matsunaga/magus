#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "cell/cell_nsdef.h"
#include "cell/CellFFInfo.h"
#include "cell/CellLatchInfo.h"
#include "networks/cmn.h"
#include "Match.h"


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
  /// @param[in] inv 極性
  /// @param[in] ff_info ピンの割り当て情報
  /// @param[in] cell セル
  void
  set_dff_match(const BdnDff* dff,
		bool inv,
		const Cell* cell,
		const CellFFInfo& ff_info);

  /// @brief ラッチのマッチを記録する．
  /// @param[in] latch ラッチ
  /// @param[in] inv 極性
  /// @param[in] latch_info ピンの割り当て情報
  /// @param[in] cell セル
  void
  set_latch_match(const BdnLatch* latch,
		  bool inv,
		  const Cell* cell,
		  const CellLatchInfo& latch_info);

  /// @brief 論理ゲートのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] match 対応するマッチ
  /// @param[in] cell セル
  void
  set_logic_match(const BdnNode* node,
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
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // D-FF の割り当て情報
  struct DffInfo
  {
    DffInfo() :
      mCell(NULL)
    {
    }

    // セル
    const Cell* mCell;

    // ピンの割り当て情報
    CellFFInfo mPinInfo;

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
    CellLatchInfo mPinInfo;

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

  // マッピング要求情報
  struct MapReq
  {
    // コンストラクタ
    MapReq(const BdnNode* node = NULL,
	   bool inv = false) :
      mNode(node),
      mInv(inv)
    {
    }

    // ノード
    const BdnNode* mNode;

    // 極性
    bool mInv;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成を行う．
  void
  gen_port(const BdnPort* sbj_port);

  /// @brief D-FF の生成を行う．
  void
  gen_dff(const BdnDff* sbj_dff);

  /// @brief ラッチの生成を行う．
  void
  gen_latch(const BdnLatch* sbj_latch);

  /// @brief マッピング要求を追加する．
  void
  add_mapreq(const BdnNode* node,
	     bool inv);

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性
  CmnNode*
  back_trace(const BdnNode* node,
	     bool inv);

  /// @brief D-FF の割り当て情報を取り出す．
  DffInfo&
  get_dff_info(const BdnDff* dff,
	       bool inv);

  /// @brief ラッチの割り当て情報を取り出す．
  LatchInfo&
  get_latch_info(const BdnLatch* latch,
		 bool inv);

  /// @brief NodeInfo を取り出す．
  NodeInfo&
  get_node_info(const BdnNode* node,
		bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の CMN
  CmnMgr* mMapGraph;

  // D-FF の割り当て情報を格納した配列
  // キーは BdnDff の ID 番号
  vector<DffInfo> mDffInfo;

  // ラッチの割り当て情報を格納した配列
  // キーは BdnLatch の ID 番号
  vector<LatchInfo> mLatchInfo;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは BdnNode の ID 番号
  vector<NodeInfo> mNodeInfo;

  // マッピング要求リスト
  vector<MapReq> mMapReqList;

  // back_trace 中に用いる作業領域
  vector<CmnNode*> mTmpFanins;

};

END_NAMESPACE_YM_CELLMAP

#endif // MAPRECORD_H
