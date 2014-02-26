#ifndef YMSH_YMSH_NSDEF_H
#define YMSH_YMSH_NSDEF_H

/// @file ymsh/ymsh_nsdef.h
/// @brief Ymsh の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief ymsh 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_YMSH \
BEGIN_NAMESPACE_YM		\
BEGIN_NAMESPACE(nsYmsh)

/// @brief aig 用の名前空間の終了
#define END_NAMESPACE_YM_YMSH \
END_NAMESPACE(nsYmsh) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_YMSH

class Ymsh;
class YmshCmd;

END_NAMESPACE_YM_YMSH


BEGIN_NAMESPACE_YM

using nsYmsh::Ymsh;
using nsYmsh::YmshCmd;

END_NAMESPACE_YM

#endif // YMSH_YMSH_NSDEF_H
