#ifndef YM_BDN_BDNDFF_H
#define YM_BDN_BDNDFF_H

/// @file ym_bdn/BdnDff.h
/// @brief BdnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/bdn_nsdef.h"
#include "ym_bdn/BdnNode.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BdnDff BdnDff.h "ym_bdn/BdnDff.h"
/// @brief DFFを表すクラス
//////////////////////////////////////////////////////////////////////
class BdnDff :
  public DlElem
{
  friend class BdNetwork;

private:

  /// @brief コンストラクタ
  BdnDff();

  /// @brief デストラクタ
  ~BdnDff();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号は D-FF の生成時に BdNetwork により自動的に割り振られる．
  /// @sa BdNetwork
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

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

  /// @brief クロックのノードを返す．
  const BdnNode*
  clock() const;

  /// @brief クロックのノードを返す．
  BdnNode*
  clock();

  /// @brief セット信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  set() const;

  /// @brief セット信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  set();

  /// @brief リセット信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  reset() const;

  /// @brief リセット信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  reset();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // データ出力ノード
  BdnNode mOutput;

  // データ入力ノード
  BdnNode mInput;

  // クロックノード
  BdnNode mClock;

  // セット信号ノード
  BdnNode mSet;

  // リセット信号ノード
  BdnNode mReset;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID 番号の取得
// @return ID 番号を返す．
// @note ID 番号は D-FF の生成時に BdNetwork により自動的に割り振られる．
// @sa BdNetwork
inline
ymuint
BdnDff::id() const
{
  return mId;
}

// @brief データ出力ノードを返す．
inline
const BdnNode*
BdnDff::output() const
{
  return &mOutput;
}

// @brief データ出力ノードを返す．
inline
BdnNode*
BdnDff::output()
{
  return &mOutput;
}

// @brief データ入力ノードを返す．
inline
const BdnNode*
BdnDff::input() const
{
  return &mInput;
}

// @brief データ入力ノードを返す．
inline
BdnNode*
BdnDff::input()
{
  return &mInput;
}

// @brief クロックのノードを返す．
inline
const BdnNode*
BdnDff::clock() const
{
  return &mClock;
}

// @brief クロックのノードを返す．
inline
BdnNode*
BdnDff::clock()
{
  return &mClock;
}

// @brief セット信号のノードを返す．
// @note NULL の場合もある．
inline
const BdnNode*
BdnDff::set() const
{
  return &mSet;
}

// @brief セット信号のノードを返す．
// @note NULL の場合もある．
inline
BdnNode*
BdnDff::set()
{
  return &mSet;
}

// @brief リセット信号のノードを返す．
// @note NULL の場合もある．
inline
const BdnNode*
BdnDff::reset() const
{
  return &mReset;
}

// @brief リセット信号のノードを返す．
// @note NULL の場合もある．
inline
BdnNode*
BdnDff::reset()
{
  return &mReset;
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNDFF_H
