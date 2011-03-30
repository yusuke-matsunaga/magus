#ifndef YM_BDN_BDNLATCH_H
#define YM_BDN_BDNLATCH_H

/// @file ym_bdn/BdnLatch.h
/// @brief BdnLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_BDN

class BdnAuxData;

//////////////////////////////////////////////////////////////////////
/// @class BdnLatch BdnLatch.h "ym_bdn/BdnLatch.h"
/// @brief ラッチを表すクラス
//////////////////////////////////////////////////////////////////////
class BdnLatch :
  public DlElem
{
  friend class BdNetwork;

private:

  /// @brief コンストラクタ
  BdnLatch();

  /// @brief デストラクタ
  ~BdnLatch();


public:

  /// @brief ID番号の取得
  /// @return ID番号を返す．
  /// @note ID番号はラッチの生成時に BdNetwork により自動的に割り振られる．
  /// @sa BdNetwork
  ymuint
  id() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief データ出力ノードを返す．
  const BdnNode*
  output() const;

  /// @brief データ出力ノードを返す．
  BdnNode*
  output();

  /// @brief データ入力ノードを返す．
  const BdnNode*
  input() const;

  /// @brief データ入力ノードを返す．
  BdnNode*
  input();

  /// @brief ラッチイネーブルノードを返す．
  const BdnNode*
  enable() const;

  /// @brief ラッチイネーブルノードを返す．
  BdnNode*
  enable();


private:
  //////////////////////////////////////////////////////////////////////
  // 内容を変更する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  /// @param[in] name 名前
  /// @param[in] output 出力ノード
  /// @param[in] input 入力ノード
  /// @param[in] enable ラッチイネーブルノード
  void
  set(const string& name,
      BdnNode* output,
      BdnNode* input,
      BdnNode* enable);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 名前
  string mName;

  // データ出力ノード
  BdnNode* mOutput;

  // データ入力ノード
  BdnNode* mInput;

  // ラッチイネーブルノード
  BdnNode* mEnable;

  // BdnNode の付加するデータ
  BdnAuxData* mAuxData;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号の取得
// @return ID番号を返す．
// @note ID番号はラッチの生成時に BdNetwork により自動的に割り振られる．
// @sa BdNetwork
inline
ymuint
BdnLatch::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
string
BdnLatch::name() const
{
  return mName;
}

// @brief データ出力ノードを返す．
inline
const BdnNode*
BdnLatch::output() const
{
  return mOutput;
}

// @brief データ出力ノードを返す．
inline
BdnNode*
BdnLatch::output()
{
  return mOutput;
}

// @brief データ入力ノードを返す．
inline
const BdnNode*
BdnLatch::input() const
{
  return mInput;
}

// @brief データ入力ノードを返す．
inline
BdnNode*
BdnLatch::input()
{
  return mInput;
}

// @brief ラッチイネーブルノードを返す．
inline
const BdnNode*
BdnLatch::enable() const
{
  return mEnable;
}

// @brief ラッチイネーブルノードを返す．
inline
BdnNode*
BdnLatch::enable()
{
  return mEnable;
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNLATCH_H
