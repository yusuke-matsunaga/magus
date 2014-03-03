
/// @file BdnDff.sip
/// @brief BdnDff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnDff
{
%TypeHeaderCode
#include "networks/BdnDff.h"
using nsYm::BdnDff;
%End

public:

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号は D-FF の生成時に BdnMgr により自動的に割り振られる．
  /// @sa BdnMgr
  unsigned int
  id() const;

  /// @brief 名前を返す．
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief データ出力ノードを返す．
  const BdnNode*
  output() const;

  /// @brief データ出力ノードを返す．
  BdnNode*
  _output();

  /// @brief データ入力ノードを返す．
  const BdnNode*
  input() const;

  /// @brief データ入力ノードを返す．
  BdnNode*
  _input();

  /// @brief クロックのノードを返す．
  const BdnNode*
  clock() const;

  /// @brief クロックのノードを返す．
  BdnNode*
  _clock();

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  clear() const;

  /// @brief クリア信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  _clear();

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  const BdnNode*
  preset() const;

  /// @brief プリセット信号のノードを返す．
  /// @note NULL の場合もある．
  BdnNode*
  _preset();


private:

  BdnDff();

  ~BdnDff();

};