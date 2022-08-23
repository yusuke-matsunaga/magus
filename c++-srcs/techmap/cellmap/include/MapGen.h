#ifndef MAPGEN_H
#define MAPGEN_H

/// @file MapGen.h
/// @brief MapGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.

#include "cellmap_nsdef.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "ym/clib.h"
#include "ym/BnModifier.h"
#include "ym/ClibFFInfo.h"
#include "ym/ClibLatchInfo.h"


BEGIN_NAMESPACE_CELLMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class MapGen MapGen.h "MapGen.h"
/// @brief マッピング結果を BnNetwork にセットするクラス
//////////////////////////////////////////////////////////////////////
class MapGen :
  public BnModifier
{
public:

  /// @brief コンストラクタ
  MapGen() = default;

  /// @brief デストラクタ
  ~MapGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピング結果を BnNetwork に変換する．
  void
  generate(
    const SbjGraph& sbjgraph, ///< [in] サブジェクトグラフ
    const MapRecord& record   ///< [in] マッピング結果
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードの割り当て情報
  struct NodeInfo
  {
    // マッチ
    Cut mMatch;

    // セル
    const ClibCell* mCell{nullptr};

    // マップ結果
    SizeType mMapNode{BNET_NULLID};

  };

  // マッピング要求情報
  struct MapReq
  {
    // コンストラクタ
    MapReq(
      const SbjNode* node = nullptr,
      bool inv = false
    ) : mNode(node),
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
  gen_port(
    const SbjPort* sbj_port
  );

  /// @brief D-FF の生成を行う．
  void
  gen_dff(
    const SbjDff* sbj_dff,
    const MapRecord& record
  );

  /// @brief ラッチの生成を行う．
  void
  gen_latch(
    const SbjLatch* sbj_latch
  );

  /// @brief マッピング要求を追加する．
  void
  add_mapreq(
    const SbjNode* node,
    bool inv
  );

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性
  /// @param[in] record セルの割当結果
  int
  back_trace(
    const SbjNode* node,
    bool inv,
    const MapRecord& record
  );

  /// @brief node に関係する情報を得る．
  NodeInfo&
  node_info(
    const SbjNode* node,
    bool inv
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは SbjNode の ID 番号
  vector<NodeInfo> mNodeInfo;

  // マッピング要求リスト
  vector<MapReq> mMapReqList;

};

END_NAMESPACE_CELLMAP

#endif // MAGUS_CELLMAP_MAPRECORD_H
