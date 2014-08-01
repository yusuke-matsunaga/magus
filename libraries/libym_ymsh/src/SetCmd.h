#ifndef SETCMD_H
#define SETCMD_H

/// @file SetCmd.h
/// @brief SetCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/YmshCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class SetCmd SetCmd.h "SetCmd.h"
/// @brief set コマンドのクラス
//////////////////////////////////////////////////////////////////////
class SetCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  SetCmd();

  /// @brief デストラクタ
  virtual
  ~SetCmd();


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
/// @class UnsetCmd SetCmd.h "SetCmd.h"
/// @brief unset コマンドのクラス
//////////////////////////////////////////////////////////////////////
class UnsetCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  UnsetCmd();

  /// @brief デストラクタ
  virtual
  ~UnsetCmd();


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

#endif // SETCMD_H
