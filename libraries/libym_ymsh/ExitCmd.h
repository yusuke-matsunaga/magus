#ifndef LIBYM_YMSH_EXITCMD_H
#define LIBYM_YMSH_EXITCMD_H

/// @file libym_ymsh/ExitCmd.h
/// @brief ExitCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/YmshCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class ExitCmd ExitCmd.h "ExitCmd.h"
/// @brief set コマンドのクラス
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

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_EXITCMD_H
