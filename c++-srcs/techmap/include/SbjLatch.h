#ifndef SBJLATCH_H
#define SBJLATCH_H

/// @file SbjLatch.h
/// @brief SbjLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

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
  SbjLatch(
    SizeType id,     ///< [in] ID番号
    SbjNode* input,  ///< [in] データ入力ノード
    SbjNode* output, ///< [in] データ出力ノード
    SbjNode* enable, ///< [in] イネーブル端子ノード
    SbjNode* clear,  ///< [in] クリア端子ノード
    SbjNode* preset  ///< [in] プリセット端子ノード
  ) : mId(id),
      mDataInput(input),
      mEnable(enable),
      mClear(clear),
      mPreset(preset),
      mDataOutput(output)
  {
  }

  /// @brief デストラクタ
  ~SbjLatch() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  SizeType
  id() const
  {
    return mId;
  }

  /// @brief データ入力を得る．
  const SbjNode*
  data_input() const
  {
    return mDataInput;
  }

  /// @brief イネーブル入力を得る．
  const SbjNode*
  enable() const
  {
    return mEnable;
  }

  /// @brief リセット入力を得る．
  const SbjNode*
  clear() const
  {
    return mClear;
  }

  /// @brief セット入力を得る．
  const SbjNode*
  preset() const
  {
    return mPreset;
  }

  /// @brief データ出力を得る．
  const SbjNode*
  data_output() const
  {
    return mDataOutput;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  SizeType mId;

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

END_NAMESPACE_SBJ

#endif // SBJGRAPH_H
