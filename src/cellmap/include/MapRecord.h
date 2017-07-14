#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_CELLMAP

class Match;

//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:

  /// @brief コンストラクタ
  MapRecord();

  /// @brief コピーコンストラクタ
  MapRecord(const MapRecord& src);

  /// @brief デストラクタ
  ~MapRecord();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(const SbjGraph& sbjgraph);

  /// @brief D-FF のマッチを記録する．
  /// @param[in] dff D-FF
  /// @param[in] inv 極性
  /// @param[in] ff_info ピンの割り当て情報
  /// @param[in] cell セル
  void
  set_dff_match(const SbjDff* dff,
		bool inv,
		const Cell* cell,
		const CellFFInfo& ff_info);

  /// @brief ラッチのマッチを記録する．
  /// @param[in] latch ラッチ
  /// @param[in] inv 極性
  /// @param[in] latch_info ピンの割り当て情報
  /// @param[in] cell セル
  void
  set_latch_match(const SbjLatch* latch,
		  bool inv,
		  const Cell* cell,
		  const CellLatchInfo& latch_info);

  /// @brief 論理ゲートのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] match 対応するマッチ
  /// @param[in] cell セル
  void
  set_logic_match(const SbjNode* node,
		  bool inv,
		  const Match& match,
		  const Cell* cell);

  /// @brief インバータのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_inv_match(const SbjNode* node,
		bool inv,
		const Cell* cell);

#if 0
  /// @brief マッチを取り出す．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  const Match&
  get_match(const SbjNode* node,
	    bool inv);
#endif

  /// @brief D-FF の割り当て情報を取り出す．
  DffInfo&
  get_dff_info(const SbjDff* dff,
	       bool inv);

  /// @brief ラッチの割り当て情報を取り出す．
  LatchInfo&
  get_latch_info(const SbjLatch* latch,
		 bool inv);

  /// @brief NodeInfo を取り出す．
  NodeInfo&
  get_node_info(const SbjNode* node,
		bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // D-FF の割り当て情報
  struct DffInfo
  {
    DffInfo() :
      mCell(nullptr)
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
      mCell = nullptr;
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
      mCell = nullptr;
      mMapNode = nullptr;
    }

    // マッチ
    Match mMatch;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // D-FF の割り当て情報を格納した配列
  // キーは SbjDff の ID 番号
  vector<DffInfo> mDffInfo;

  // ラッチの割り当て情報を格納した配列
  // キーは SbjLatch の ID 番号
  vector<LatchInfo> mLatchInfo;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは SbjNode の ID 番号
  vector<NodeInfo> mNodeInfo;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

#if 0
// @brief マッチを取り出す．
// @param[in] node 該当のノード
// @param[in] inv 極性
inline
const Match&
MapRecord::get_match(const BdnNode* node,
		     bool inv)
{
  return get_node_info(node, inv).mMatch;
}
#endif

// @brief D-FF の割り当て情報を取り出す．
inline
MapRecord::DffInfo&
MapRecord::get_dff_info(const BdnDff* dff,
			bool inv)
{
  bool offset = inv ? 1 : 0;
  return mDffInfo[dff->id() * 2 + offset];
}

// @brief ラッチの割り当て情報を取り出す．
inline
MapRecord::LatchInfo&
MapRecord::get_latch_info(const BdnLatch* latch,
			  bool inv)
{
  bool offset = inv ? 1 : 0;
  return mLatchInfo[latch->id() * 2 + offset];
}

// @brief NodeInfo を取り出す．
inline
MapRecord::NodeInfo&
MapRecord::get_node_info(const BdnNode* node,
			 bool inv)
{
  return mNodeInfo[node->id() * 2 + static_cast<ymuint>(inv)];
}

END_NAMESPACE_YM_CELLMAP

#endif // MAPRECORD_H
