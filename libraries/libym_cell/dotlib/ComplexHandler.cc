
/// @file libym_cell/dotlib/ComplexHandler.cc
/// @brief ComplexHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ComplexHandler.h"
#include "DotlibParser.h"
#include "GroupHandler.h"
#include "PtMgr.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス ComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
ComplexHandler::ComplexHandler(GroupHandler* parent) :
  DotlibHandler(parent->parser(), parent->ptmgr(), parent)
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
ComplexHandler::read_attr(ShString attr_name,
			  const FileRegion& attr_loc)
{
  vector<const PtValue*> value_list;

  if ( !parse_complex(value_list) ) {
    return false;
  }

  if ( debug() ) {
    cout << attr_name << " (";
    const char* comma = "";
    for (vector<const PtValue*>::const_iterator p = value_list.begin();
	 p != value_list.end(); ++ p) {
      cout << comma << *p;
      comma = ", ";
    }
    cout << ")" << endl;
  }

#if 1
  PtNode* node = ptmgr().new_ptcomplex(attr_name, attr_loc, value_list);
  parent()->pt_node()->add_child(node);
#endif

  return expect_nl();
}

END_NAMESPACE_YM_CELL_DOTLIB
