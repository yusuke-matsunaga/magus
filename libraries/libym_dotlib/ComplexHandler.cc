
/// @file libym_dotlib/ComplexHandler.cc
/// @brief ComplexHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ComplexHandler.h"
#include "DotlibParserImpl.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "DotlibNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
ComplexHandler::ComplexHandler(GroupHandler* parent) :
  DotlibHandler(parent)
{
}

// @brief デストラクタ
ComplexHandler::~ComplexHandler()
{
}

// @brief 構文要素を処理する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @return エラーが起きたら false を返す．
bool
ComplexHandler::read_attr(const ShString& attr_name,
			  const FileRegion& attr_loc)
{
  DotlibNodeImpl* value = parse_complex();
  if ( value == NULL ) {
    return false;
  }

  if ( !expect_nl() ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << value << endl;
  }

  if ( !set_value(attr_name, attr_loc, value) ) {
    return false;
  }

  return true;
}

// @brief 値を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値のリスト
bool
ComplexHandler::set_value(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  DotlibNodeImpl* value)
{
  FileRegion loc(attr_loc, value->loc());
  return parent()->add_attr(attr_name, value, loc);
}

END_NAMESPACE_YM_DOTLIB
