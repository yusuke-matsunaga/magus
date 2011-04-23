
/// @file libym_cell/dotlib/DotlibMgr.cc
/// @brief DotlibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibMgr.h"
#include "../ci/CiLibrary.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibMgr::DotlibMgr()
{
}

// @brief デストラクタ
DotlibMgr::~DotlibMgr()
{
}

// @brief パース木から CellLibrary を生成する．
// @param[in] root パース木の根のノード
const CellLibrary*
DotlibMgr::gen_library(const PtNode* root)
{
  Token root_attr = root->attr_token();
  assert_cond( root_attr.type() == SYMBOL, __FILE__, __LINE__);
  assert_cond( root_attr.value() == "library", __FILE__, __LINE__);

  if ( root->value_num() != 1 ) {
    // エラー
    put_msg(__FILE__, __LINE__, root_attr.loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "'library' attribute require only one value.");
    return NULL;
  }
  Token root_value = root->value(0);
  if ( root_value.type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, root_value.loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "library name should be a string.");
    return NULL;
  }

  CiLibrary* library = new CiLibrary(root_value.value());

  ymuint n = root->child_num();
  for (ymuint i = 0; i < n; ++ i) {
    const PtNode* node = root->child(i);
    Token attr = node->attr_token();
  }
}

END_NAMESPACE_YM_CELL_DOTLIB

