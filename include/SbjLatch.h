#ifndef SBJLATCH_H
#define SBJLATCH_H

/// @file SbjLatch.h
/// @brief SbjLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjLatch SbjGraph.h "SbjGraph.h"
/// @brief ラッチを表すクラス
///
/// 以下の入力(ノードからすると出力)を持つ．
/// - data:   データ入力
/// - enable: イネーブル
/// - clear:  (非同期)リセット
/// - preset: (非同期)セット
/// ただし，data 以外の入力は接続していない場合もある．
/// データ出力(ノードからすると入力)を持つ．
/// @sa SbjNode
//////////////////////////////////////////////////////////////////////
class SbjLatch
{
  friend class SbjGraph;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SbjLatch(SbjNode* input,
	   SbjNode* output,
	   SbjNode* enable,
	   SbjNode* clear,
	   SbjNode* preset);

  /// @brief デストラクタ
  ~SbjLatch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief データ入力を得る．
  const SbjNode*
  data_input() const;

  /// @brief イネーブル入力を得る．
  const SbjNode*
  enable() const;

  /// @brief リセット入力を得る．
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
  ymuint mId;

  // データ入力
  SbjNode* mDataInput;

  // イネーブル
  SbjNode* mEnable;

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
SbjLatch::SbjLatch(SbjNode* input,
		   SbjNode* output,
		   SbjNode* enable,
		   SbjNode* clear,
		   SbjNode* preset) :
  mDataInput(input),
  mEnable(enable),
  mClear(clear),
  mPreset(preset),
  mDataOutput(output)
{
}

// @brief デストラクタ
inline
SbjLatch::~SbjLatch()
{
}

// @brief ID番号を得る．
inline
ymuint
SbjLatch::id() const
{
  return mId;
}

// @brief データ入力を得る．
inline
const SbjNode*
SbjLatch::data_input() const
{
  return mDataInput;
}

// @brief イネーブル入力を得る．
inline
const SbjNode*
SbjLatch::enable() const
{
  return mEnable;
}

// @brief リセット入力を得る．
inline
const SbjNode*
SbjLatch::clear() const
{
  return mClear;
}

// @brief セット入力を得る．
inline
const SbjNode*
SbjLatch::preset() const
{
  return mPreset;
}

// @brief データ出力を得る．
inline
const SbjNode*
SbjLatch::data_output() const
{
  return mDataOutput;
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H
