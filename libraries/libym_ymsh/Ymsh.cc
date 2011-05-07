
/// @file libym_ymsh/Ymsh.cc
/// @brief Ymsh の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/Ymsh.h"
#include "YmshImpl.h"

#include "SetCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス YmshBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmshBase::YmshBase() :
  mImpl(new YmshImpl)
{
  YmshCmd* set_cmd = new SetCmd;
  mImpl->reg_command(set_cmd);
}

// @brief デストラクタ
YmshBase::~YmshBase()
{
  delete mImpl;
}

// @brief コマンドループを実行する．
void
YmshBase::run()
{
  mImpl->run();
}


//////////////////////////////////////////////////////////////////////
// クラス Ymsh
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Ymsh::Ymsh()
{
}

// @brief デストラクタ
Ymsh::~Ymsh()
{
}

END_NAMESPACE_YM_YMSH
