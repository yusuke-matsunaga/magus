
/// @file Ymsh.cc
/// @brief Ymsh の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ymsh/Ymsh.h"
#include "YmshImpl.h"

#include "SetCmd.h"
#include "AliasCmd.h"
#include "ExitCmd.h"
#include "DirCmd.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス YmshBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmshBase::YmshBase() :
  mImpl(new YmshImpl)
{
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

// @brief 実装クラスを返す．
YmshImpl*
YmshBase::impl()
{
  return mImpl;
}


//////////////////////////////////////////////////////////////////////
// クラス Ymsh
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Ymsh::Ymsh()
{
  YmshCmd* set_cmd = new SetCmd;
  impl()->reg_command(set_cmd);

  YmshCmd* unset_cmd = new UnsetCmd;
  impl()->reg_command(unset_cmd);

  YmshCmd* alias_cmd = new AliasCmd;
  impl()->reg_command(alias_cmd);

  YmshCmd* unalias_cmd = new UnaliasCmd;
  impl()->reg_command(unalias_cmd);

  YmshCmd* exit_cmd = new ExitCmd;
  impl()->reg_command(exit_cmd);

  YmshCmd* umask_cmd = new UmaskCmd;
  impl()->reg_command(umask_cmd);

  YmshCmd* pwd_cmd = new PwdCmd;
  impl()->reg_command(pwd_cmd);

  YmshCmd* cd_cmd = new CdCmd;
  impl()->reg_command(cd_cmd);

  YmshCmd* mkdir_cmd = new MkdirCmd;
  impl()->reg_command(mkdir_cmd);

  YmshCmd* rmdir_cmd = new RmdirCmd;
  impl()->reg_command(rmdir_cmd);
}

// @brief デストラクタ
Ymsh::~Ymsh()
{
}

END_NAMESPACE_YM_YMSH
