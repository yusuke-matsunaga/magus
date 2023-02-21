#ifndef MAPRECORD_H
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
  explicit
  MapRecord(
    const ClibCellLibrary& library ///< [in] セルライブラリ
  );

  /// @brief コピーコンストラクタ
  MapRecord(
    const MapRecord& src ///< [in] コピー元のオブジェクト
  );

  /// @brief デストラクタ
  ~MapRecord();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief @brief 作業領域を初期化する．
  void
  init(
    const SbjGraph& sbjgraph ///< [in] サブジェクトグラフ
  );

  /// @brief 定数０セルのセル番号をセットする．
  void
  set_const0(
    SizeType cell_id ///< [in] セル番号
  );

  /// @brief 定数1セルをセットする．
  void
  set_const1(
    SizeType cell_id ///< [in] セル番号
  );

  /// @brief D-FF のマッチを記録する．
  void
  set_dff_match(
    const SbjDff* dff, ///< [in] D-FF
    bool inv,          ///< [in] 極性
    SizeType cell_id   ///< [in] セル番号
  );

  /// @brief ラッチのマッチを記録する．
  void
  set_latch_match(
    const SbjLatch* latch, ///< [in] ラッチ
    bool inv,              ///< [in] 極性
    SizeType cell_id       ///< [in] セル番号
  );

  /// @brief 論理ゲートのマッチを記録する．
  void
  set_logic_match(
    const SbjNode* node, ///< [in] 該当のノード
    bool inv,            ///< [in] 極性
    const Cut& match,    ///< [in] 対応するマッチ
    SizeType cell_id     ///< [in] セル番号
  );

  /// @brief インバータのマッチを記録する．
  void
  set_inv_match(
    const SbjNode* node, ///< [in] 該当のノード
    bool inv,            ///< [in] 極性
    SizeType cell_id     ///< [in] セル番号
  );

  /// @brief セルライブラリを得る．
  const ClibCellLibrary&
  cell_library() const
  {
    return mCellLibrary;
  }

  /// @brief 定数０セルを返す．
  SizeType
  const0_cell() const
  {
    return mConst0Id;
  }

  /// @brief 定数1セルを返す．
  SizeType
  const1_cell() const
  {
    return mConst1Id;
  }

  /// @brief D-FF の割り当て情報を取り出す．
  /// @return D-FFのセル番号を返す．
  SizeType
  get_dff_cell(
    const SbjDff* dff, ///< [in] 対象の D-FF
    bool inv           ///< [in] 出力の極性
  ) const
  {
    int offset = inv ? 1 : 0;
    return mDffInfo[dff->id() * 2 + offset];
  }

  /// @brief ラッチの割り当て情報を取り出す．
  /// @return ラッチのセル番号を返す．
  SizeType
  get_latch_cell(
    const SbjLatch* latch, ///< [in] 対象のラッチ
    bool inv               ///< [in] 出力の極性
  ) const
  {
    int offset = inv ? 1 : 0;
    return mDffInfo[latch->id() * 2 + offset];
  }

  /// @brief node に対応するセルを返す．
  /// @return セル番号を返す．
  SizeType
  get_node_cell(
    const SbjNode* node, ///< [in] 対象のノード
    bool inv             ///< [in] 極性
  ) const
  {
    return _node_info(node, inv).mCellId;
  }

  /// @brief node に対応するマッチ(Cut)を返す．
  const Cut&
  get_node_match(
    const SbjNode* node, ///< [in] 対象のノード
    bool inv		 ///< [in] 極性
  ) const
  {
    return _node_info(node, inv).mMatch;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードの割り当て情報
  struct NodeInfo
  {

    // マッチ
    Cut mMatch;

    // セル番号
    SizeType mCellId{CLIB_NULLID};

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NodeInfo のアクセッサー
  NodeInfo&
  _node_info(
    const SbjNode* node,
    bool inv
  )
  {
    ymuint offset = inv ? 1 : 0;
    return mNodeInfo[node->id() * 2 + offset];
  }

  /// @brief NodeInfo のアクセッサー
  const NodeInfo&
  _node_info(
    const SbjNode* node,
    bool inv
  ) const
  {
    ymuint offset = inv ? 1 : 0;
    return mNodeInfo[node->id() * 2 + offset];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  const ClibCellLibrary& mCellLibrary;

  // 定数０セルのセル番号
  SizeType mConst0Id;

  // 定数1セルのセル番号
  SizeType mConst1Id;

  // D-FF の割り当て情報を格納した配列
  // キーは SbjDff の ID 番号 * 2 + (0/1)
  vector<int> mDffInfo;

  // ラッチの割り当て情報を格納した配列
  // キーは SbjLatch の ID 番号 * 2 + (0/1)
  vector<int> mLatchInfo;

  // 各ノードの極性ごと作業領域を格納した配列
  // キーは SbjNode の ID 番号 * 2 + (0/1)
  vector<NodeInfo> mNodeInfo;

};

END_NAMESPACE_CELLMAP

#endif // MAPRECORD_H
