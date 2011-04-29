#ifndef YM_DOTLIB_DOTLIB_NSDEF_H
#define YM_DOTLIB_DOTLIB_NSDEF_H

/// @file ym_dotlib/dotlib_nsdef.h
/// @brief dotlib サブモジュールの定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief dotlib の名前空間の開始
#define BEGIN_NAMESPACE_YM_DOTLIB \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsDotlib)

/// @brief dotlib の名前空間の終了
#define END_NAMESPACE_YM_DOTLIB \
END_NAMESPACE(nsDotlib) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_DOTLIB

// クラス名の前方宣言
class DotlibParser;
class DotlibNode;

END_NAMESPACE_YM_DOTLIB


BEGIN_NAMESPACE_YM

using nsDotlib::DotlibParser;
using nsDotlib::DotlibNode;

END_NAMESPACE_YM

#endif // YM_DOTLIB_DOTLIB_NSDEF_H
