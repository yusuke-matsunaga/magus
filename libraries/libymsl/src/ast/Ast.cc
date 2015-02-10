
/// @file Ast.cc
/// @brief Ast の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Ast
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
Ast::Ast(const FileRegion& loc) :
  mLoc(loc)
{
}

// @brief デストラクタ
Ast::~Ast()
{
}

// @brief ファイル位置を得る．
const FileRegion&
Ast::file_region() const
{
  return mLoc;
}

END_NAMESPACE_YM_YMSL
