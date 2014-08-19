#ifndef ALIASCMD_H
#define ALIASCMD_H

/// @file AliasCmd.h
/// @brief AliasCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/YmshCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class AliasCmd AliasCmd.h "AliasCmd.h"
/// @brief alias コマンドのクラス
//////////////////////////////////////////////////////////////////////
class AliasCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  AliasCmd();

  /// @brief デストラクタ
  virtual
  ~AliasCmd();


public:
  //////////////////////////////////////////////////////////////////////
  // 処理を行うメイン関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドの実行を行う．
  /// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
  /// @return 実行後の状態を表すコードを返す．
  virtual
  int
  exec(const vector<string>& argv);

};


//////////////////////////////////////////////////////////////////////
/// @class UnaliasCmd AliasCmd.h "AliasCmd.h"
/// @brief unalias コマンドのクラス
//////////////////////////////////////////////////////////////////////
class UnaliasCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  UnaliasCmd();

  /// @brief デストラクタ
  virtual
  ~UnaliasCmd();


public:
  //////////////////////////////////////////////////////////////////////
  // 処理を行うメイン関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドの実行を行う．
  /// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
  /// @return 実行後の状態を表すコードを返す．
  virtual
  int
  exec(const vector<string>& argv);

};

END_NAMESPACE_YM_YMSH

#endif // ALIASCMD_H
