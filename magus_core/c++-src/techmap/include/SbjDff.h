#ifndef SBJDFF_H
#define SBJDFF_H

/// @file SbjDff.h
/// @brief SbjDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjDff SbjGraph.h "SbjGraph.h"
/// @brief DFF を表すクラス
///
/// 以下の入力(ノードからすると出力)を持つ．
/// - data:   データ入力
/// - clock:  クロック
/// - clear:  (非同期)リセット
/// - preset: (非同期)セット
/// ただし，data 以外の入力は接続していない場合もある．
/// データ出力(ノードからすると入力)を持つ．
/// @sa SbjNode
//////////////////////////////////////////////////////////////////////
class SbjDff
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] input データ入力ノード
  /// @param[in] output データ出力ノード
  /// @param[in] clock クロックノード
  /// @param[in] clear クリア端子ノード
  /// @param[in] preset プリセット端子ノード
  SbjDff(int id,
	 SbjNode* input,
	 SbjNode* output,
	 SbjNode* clock,
	 SbjNode* clear,
	 SbjNode* preset);

  /// @brief デストラクタ
  ~SbjDff();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  int
  id() const;

  /// @brief データ入力を得る．
  const SbjNode*
  data_input() const;

  /// @brief クロック入力を得る．
  const SbjNode*
  clock() const;

  /// @brief DFFノードの場合のリセット入力を得る．
  const SbjNode*
  clear() const;

  /// @brief セット入力を得る．
  const SbjNode*
  preset() const;

  /// @brief データ出力を得る．
  const SbjNode*
  data_output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内容の設定に用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  int mId;

  // データ入力
  SbjNode* mDataInput;

  // クロック
  SbjNode* mClock;

  // リセット端子
  SbjNode* mClear;

  // セット端子
  SbjNode* mPreset;

  // データ出力
  SbjNode* mDataOutput;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
SbjDff::SbjDff(int id,
	       SbjNode* input,
	       SbjNode* output,
	       SbjNode* clock,
	       SbjNode* clear,
	       SbjNode* preset) :
  mId(id),
  mDataInput(input),
  mClock(clock),
  mClear(clear),
  mPreset(preset),
  mDataOutput(output)
{
}

// @brief デストラクタ
inline
SbjDff::~SbjDff()
{
}

// @brief ID番号を得る．
inline
int
SbjDff::id() const
{
  return mId;
}

// @brief データ入力を得る．
inline
const SbjNode*
SbjDff::data_input() const
{
  return mDataInput;
}

// @brief クロック入力を得る．
inline
const SbjNode*
SbjDff::clock() const
{
  return mClock;
}

// @brief リセット入力を得る．
inline
const SbjNode*
SbjDff::clear() const
{
  return mClear;
}

// @brief セット入力を得る．
inline
const SbjNode*
SbjDff::preset() const
{
  return mPreset;
}

// @brief データ出力を得る．
inline
const SbjNode*
SbjDff::data_output() const
{
  return mDataOutput;
}

END_NAMESPACE_SBJ

#endif // SBJGRAPH_H
