#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "SbjGraph.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
///
/// 具体的には各ノードごとに選択されたカットを保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MapRecord() = default;

  /// @brief コピーコンストラクタ
  MapRecord(
    const MapRecord& src
  ) : mCutArray{src.mCutArray}
  {
  }

  /// @brief デストラクタ
  ~MapRecord() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(
    const SbjGraph& sbjgraph
  )
  {
    mCutArray.clear();
    mCutArray.resize(sbjgraph.node_num(), nullptr);
  }

  /// @brief カットを記録する．
  void
  set_cut(
    const SbjNode* node, ///< [in] 該当のノード
    const Cut* cut       ///< [in] 対応するカット
  )
  {
    mCutArray[node->id()] = cut;
  }

  /// @brief カットを取り出す．
  const Cut*
  get_cut(
    const SbjNode* node ///< [in] 該当のノード
  ) const
  {
    return mCutArray[node->id()];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごとに選択されたカットを格納した配列
  vector<const Cut*> mCutArray;

};

END_NAMESPACE_LUTMAP

#endif // MAPRECORD_H
