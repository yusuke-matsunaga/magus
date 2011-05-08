#ifndef YM_YMSH_YMSH_NSDEF_H
#define YM_YMSH_YMSH_NSDEF_H

/// @file ym_ymsh/ymsh_nsdef.h
/// @brief Ymsh の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief ym_ymsh 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_YMSH \
BEGIN_NAMESPACE_YM		\
BEGIN_NAMESPACE(nsYmsh)

/// @brief ym_aig 用の名前空間の終了
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

#endif // YM_YMSH_YMSH_NSDEF_H
