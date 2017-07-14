#ifndef MAPGEN_H
#define MAPGEN_H

/// @file MapGen.h
/// @brief MapGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "ym/ym_cell.h"
#include "ym/ym_bnet.h"
#include "ym/CellFFInfo.h"
#include "ym/CellLatchInfo.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class MapGen MapGen.h "MapGen.h"
/// @brief マッピング結果を BnNetwork にセットするクラス
//////////////////////////////////////////////////////////////////////
class MapGen
{
public:

  /// @brief コンストラクタ
  MapGen();

  /// @brief デストラクタ
  ~MapGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピング結果を BnNetwork にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] record マッピング結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  void
  generate(const SbjGraph& sbjgraph,
	   const MapRecord& record,
	   BnNetwork& mapgraph);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードの割り当て情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mCell = nullptr;
      mMapNode = nullptr;
    }

    // マッチ
    Cut mMatch;

    // セル
    const Cell* mCell;

    // マップ結果
    BnNode* mMapNode;

  };

  // マッピング要求情報
  struct MapReq
  {
    // コンストラクタ
    MapReq(const SbjNode* node = nullptr,
	   bool inv = false) :
      mNode(node),
      mInv(inv)
    {
    }

    // ノード
    const SbjNode* mNode;

    // 極性
    bool mInv;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートの生成を行う．
  void
  gen_port(const SbjPort* sbj_port);

  /// @brief D-FF の生成を行う．
  void
  gen_dff(const SbjDff* sbj_dff);

  /// @brief ラッチの生成を行う．
  void
  gen_latch(const SbjLatch* sbj_latch);

  /// @brief マッピング要求を追加する．
  void
  add_mapreq(const SbjNode* node,
	     bool inv);

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性
  BnNode*
  back_trace(const SbjNode* node,
	     bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 答えのマップグラフ
  BnNetwork* mMapGraph;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは BdnNode の ID 番号
  vector<NodeInfo> mNodeInfo;

  // マッピング要求リスト
  vector<MapReq> mMapReqList;

  // back_trace 中に用いる作業領域
  vector<BnNode*> mTmpFanins;

};

END_NAMESPACE_YM_CELLMAP

#endif // MAGUS_CELLMAP_MAPRECORD_H
