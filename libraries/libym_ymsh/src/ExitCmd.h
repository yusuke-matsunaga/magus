#ifndef EXITCMD_H
#define EXITCMD_H

/// @file ExitCmd.h
/// @brief ExitCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/YmshCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class ExitCmd ExitCmd.h "ExitCmd.h"
/// @brief exit コマンドのクラス
//////////////////////////////////////////////////////////////////////
class ExitCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  ExitCmd();

  /// @brief デストラクタ
  virtual
  ~ExitCmd();


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
/// @class UmaskCmd ExitCmd.h "ExitCmd.h"
/// @brief umask コマンドのクラス
//////////////////////////////////////////////////////////////////////
class UmaskCmd :
  public YmshCmd
{
public:

  /// @brief コンストラクタ
  UmaskCmd();

  /// @brief デストラクタ
  virtual
  ~UmaskCmd();


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

#endif // EXITCMD_H
