﻿#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "Cut.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjNode.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_CELLMAP

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
  /// @param[in] src コピー元のオブジェクト
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

  /// @brief 定数０セルをセットする．
  void
  set_const0(const ClibCell* cell);

  /// @brief 定数1セルをセットする．
  void
  set_const1(const ClibCell* cell);

  /// @brief D-FF のマッチを記録する．
  /// @param[in] dff D-FF
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_dff_match(const SbjDff* dff,
		bool inv,
		const ClibCell* cell);

  /// @brief ラッチのマッチを記録する．
  /// @param[in] latch ラッチ
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_latch_match(const SbjLatch* latch,
		  bool inv,
		  const ClibCell* cell);

  /// @brief 論理ゲートのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] match 対応するマッチ
  /// @param[in] cell セル
  void
  set_logic_match(const SbjNode* node,
		  bool inv,
		  const Cut& match,
		  const ClibCell* cell);

  /// @brief インバータのマッチを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] inv 極性
  /// @param[in] cell セル
  void
  set_inv_match(const SbjNode* node,
		bool inv,
		const ClibCell* cell);

  /// @brief 定数０セルを返す．
  const ClibCell*
  const0_cell() const;

  /// @brief 定数1セルを返す．
  const ClibCell*
  const1_cell() const;

  /// @brief D-FF の割り当て情報を取り出す．
  const ClibCell*
  get_dff_cell(const SbjDff* dff,
	       bool inv) const;

  /// @brief ラッチの割り当て情報を取り出す．
  const ClibCell*
  get_latch_cell(const SbjLatch* latch,
		 bool inv) const;

  /// @brief node に対応するセルを返す．
  const ClibCell*
  get_node_cell(const SbjNode* node,
		bool inv) const;

  /// @brief node に対応するマッチ(Cut)を返す．
  const Cut&
  get_node_match(const SbjNode* node,
		 bool inv) const;


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
    }

    // マッチ
    Cut mMatch;

    // セル
    const ClibCell* mCell;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NodeInfo のアクセッサー
  NodeInfo&
  _node_info(const SbjNode* node,
	     bool inv);

  /// @brief NodeInfo のアクセッサー
  const NodeInfo&
  _node_info(const SbjNode* node,
	     bool inv) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定数０セル
  const ClibCell* mConst0;

  // 定数1セル
  const ClibCell* mConst1;

  // D-FF の割り当て情報を格納した配列
  // キーは SbjDff の ID 番号 * 2 + (0/1)
  vector<const ClibCell*> mDffInfo;

  // ラッチの割り当て情報を格納した配列
  // キーは SbjLatch の ID 番号 * 2 + (0/1)
  vector<const ClibCell*> mLatchInfo;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは SbjNode の ID 番号 * 2 + (0/1)
  vector<NodeInfo> mNodeInfo;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 定数０セルを返す．
inline
const ClibCell*
MapRecord::const0_cell() const
{
  return mConst0;
}

// @brief 定数1セルを返す．
inline
const ClibCell*
MapRecord::const1_cell() const
{
  return mConst1;
}

// @brief D-FF の割り当て情報を取り出す．
inline
const ClibCell*
MapRecord::get_dff_cell(const SbjDff* dff,
			bool inv) const
{
  ymuint offset = inv ? 1 : 0;
  return mDffInfo[dff->id() * 2 + offset];
}

// @brief ラッチの割り当て情報を取り出す．
inline
const ClibCell*
MapRecord::get_latch_cell(const SbjLatch* latch,
			  bool inv) const
{
  ymuint offset = inv ? 1 : 0;
  return mLatchInfo[latch->id() * 2 + offset];
}

// @brief node に対応するセルを返す．
inline
const ClibCell*
MapRecord::get_node_cell(const SbjNode* node,
			 bool inv) const
{
  return _node_info(node, inv).mCell;
}

// @brief node に対応するマッチ(Cut)を返す．
inline
const Cut&
MapRecord::get_node_match(const SbjNode* node,
			  bool inv) const
{
  return _node_info(node, inv).mMatch;
}

// @brief NodeInfo のアクセッサー
inline
MapRecord::NodeInfo&
MapRecord::_node_info(const SbjNode* node,
		      bool inv)
{
  ymuint offset = inv ? 1 : 0;
  return mNodeInfo[node->id() * 2 + offset];
}

// @brief NodeInfo のアクセッサー
inline
const MapRecord::NodeInfo&
MapRecord::_node_info(const SbjNode* node,
		      bool inv) const
{
  ymuint offset = inv ? 1 : 0;
  return mNodeInfo[node->id() * 2 + offset];
}

END_NAMESPACE_CELLMAP

#endif // MAPRECORD_H
