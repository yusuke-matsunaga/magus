#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap.h"
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

  /// @brief カットを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] cut 対応するカット
  void
  set_cut(const SbjNode* node,
	  const Cut* cut);

  /// @brief カットを取り出す．
  /// @param[in] node 該当のノード
  const Cut*
  get_cut(const SbjNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごとに選択されたカットを格納した配列
  vector<const Cut*> mCutArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @blif カットを記録する．
// @param[in] node 該当のノード
// @param[in] cut 対応するカット
inline
void
MapRecord::set_cut(const SbjNode* node,
		   const Cut* cut)
{
  mCutArray[node->id()] = cut;
}

// @brief カットを取り出す．
// @param[in] node 該当のノード
inline
const Cut*
MapRecord::get_cut(const SbjNode* node) const
{
  return mCutArray[node->id()];
}

END_NAMESPACE_LUTMAP

#endif // MAPRECORD_H
