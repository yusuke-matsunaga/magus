
/// @file BdnLatch.sip
/// @brief BdnLatch の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class BdnLatch
{
%TypeHeaderCode
#include "networks/BdnLatch.h"
using nsYm::BdnLatch;
%End

public:

  /// @brief ID番号の取得
  /// @return ID番号を返す．
  /// @note ID番号はラッチの生成時に BdnMgr により自動的に割り振られる．
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

  /// @brief ラッチイネーブルノードを返す．
  const BdnNode*
  enable() const;

  /// @brief ラッチイネーブルノードを返す．
  BdnNode*
  _enable();

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

  BdnLatch();

  ~BdnLatch();

};
