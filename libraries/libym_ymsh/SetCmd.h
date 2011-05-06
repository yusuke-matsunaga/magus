#ifndef LIBYM_YMSH_SETCMD_H
#define LIBYM_YMSH_SETCMD_H

/// @file libym_ymsh/SetCmd.h
/// @brief SetCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/YmshCmd.h"


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

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_SETCMD_H
