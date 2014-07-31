#ifndef NETWORKS_CMNDFF_H
#define NETWORKS_CMNDFF_H

/// @file YmNetworks/CmnDff.h
/// @brief CmnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/cmn.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

//////////////////////////////////////////////////////////////////////
/// @class CmnDff CmnDff.h "YmNetworks/CmnDff.h"
/// @brief D-FFを表すクラス
//////////////////////////////////////////////////////////////////////
class CmnDff :
  public DlElem
{
  friend class CmnMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] cell セル
  CmnDff(const string& name,
	 const CmnDffCell* cell);

  /// @brief デストラクタ
  ~CmnDff();


public:

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号は D-FF の生成時に CmnMgr により自動的に割り振られる．
  /// @sa CmnMgr
  ymuint
  id() const;

  /// @brief 名前を返す．
  string
  name() const;

  /// @brief 対応するセルを返す．
  const CmnDffCell*
  cell() const;

  /// @brief 肯定のデータ出力ノードを返す．
  const CmnNode*
  output1() const;

  /// @brief 否定のデータ出力ノードを返す．
  const CmnNode*
  output2() const;

  /// @brief データ入力ノードを返す．
  const CmnNode*
  input() const;

  /// @brief クロックのノードを返す．
  const CmnNode*
  clock() const;

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  const CmnNode*
  clear() const;

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  const CmnNode*
  preset() const;

  /// @brief 肯定のデータ出力ノードを返す．
  CmnNode*
  _output1();

  /// @brief 否定のデータ出力ノードを返す．
  CmnNode*
  _output2();

  /// @brief データ入力ノードを返す．
  CmnNode*
  _input();

  /// @brief クロックのノードを返す．
  CmnNode*
  _clock();

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  CmnNode*
  _clear();

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  CmnNode*
  _preset();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 名前
  string mName;

  // セル
  const CmnDffCell* mCell;

  // 肯定のデータ出力ノード
  CmnNode* mOutput1;

  // 否定のデータ出力ノード
  CmnNode* mOutput2;

  // データ入力ノード
  CmnNode* mInput;

  // クロックノード
  CmnNode* mClock;

  // クリア信号ノード
  CmnNode* mClear;

  // プリセット信号ノード
  CmnNode* mPreset;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID 番号の取得
// @return ID 番号を返す．
// @note ID 番号は D-FF の生成時に CmnMgr により自動的に割り振られる．
// @sa CmnMgr
inline
ymuint
CmnDff::id() const
{
  return mId;
}

// @brief 名前を返す．
inline
string
CmnDff::name() const
{
  return mName;
}

// @brief 対応するセルを返す．
inline
const CmnDffCell*
CmnDff::cell() const
{
  return mCell;
}

// @brief 肯定のデータ出力ノードを返す．
inline
const CmnNode*
CmnDff::output1() const
{
  return mOutput1;
}

// @brief 否定のデータ出力ノードを返す．
inline
const CmnNode*
CmnDff::output2() const
{
  return mOutput2;
}

// @brief データ入力ノードを返す．
inline
const CmnNode*
CmnDff::input() const
{
  return mInput;
}

// @brief クロックのノードを返す．
inline
const CmnNode*
CmnDff::clock() const
{
  return mClock;
}

// @brief クリア信号のノードを返す．
// @note NULL の場合もある．
inline
const CmnNode*
CmnDff::clear() const
{
  return mClear;
}

// @brief プリセット信号のノードを返す．
// @note NULL の場合もある．
inline
const CmnNode*
CmnDff::preset() const
{
  return mPreset;
}

// @brief 肯定のデータ出力ノードを返す．
inline
CmnNode*
CmnDff::_output1()
{
  return mOutput1;
}

// @brief 否定のデータ出力ノードを返す．
inline
CmnNode*
CmnDff::_output2()
{
  return mOutput2;
}

// @brief データ入力ノードを返す．
inline
CmnNode*
CmnDff::_input()
{
  return mInput;
}

// @brief クロックのノードを返す．
inline
CmnNode*
CmnDff::_clock()
{
  return mClock;
}

// @brief クリア信号のノードを返す．
// @note NULL の場合もある．
inline
CmnNode*
CmnDff::_clear()
{
  return mClear;
}

// @brief プリセット信号のノードを返す．
// @note NULL の場合もある．
inline
CmnNode*
CmnDff::_preset()
{
  return mPreset;
}

END_NAMESPACE_YM_NETWORKS_CMN

#endif // NETWORKS_CMNDFF_H
