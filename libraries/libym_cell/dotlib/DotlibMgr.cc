
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
  string root_attr = root->attr_name();
  assert_cond( root_attr == "library", __FILE__, __LINE__);

  if ( root->value_num() != 1 ) {
    // エラー
    put_msg(__FILE__, __LINE__, root_attr.loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "'library' attribute require only one value.");
    return NULL;
  }

  const PtNode* root_value = root->value(0);
  if ( root_value->type() != SYMBOL ) {
    put_msg(__FILE__, __LINE__, root_value.loc(),
	    kMsgError,
	    "DOTLIB_PARSER",
	    "library name should be a string.");
    return NULL;
  }

  CiLibrary* library = new CiLibrary(root_value->string_value());

  ymuint n = root->child_num();
  for (ymuint i = 0; i < n; ++ i) {
    const PtNode* node = root->child(i);
    string attr = node->attr_name();
  }
}

END_NAMESPACE_YM_CELL_DOTLIB

