
/// @file libym_ymsh/ExitCmd.cc
/// @brief ExitCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ExitCmd.h"
#include "YmshImpl.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス ExitCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ExitCmd::ExitCmd() :
  YmshCmd("exit")
{
}

// @brief デストラクタ
ExitCmd::~ExitCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
ExitCmd::exec(const vector<string>& argv)
{
  // 引数は無視する．
  ymsh()->exit();
  return 0;
}

END_NAMESPACE_YM_YMSH
