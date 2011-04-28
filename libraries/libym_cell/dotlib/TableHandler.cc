
/// @file libym_cell/dotlib/TableHandler.cc
/// @brief TableHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TableHandler.h"

#include "SimpleHandler.h"
#include "ComplexHandler.h"
#include "GroupHandler.h"
#include "TimingHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

BEGIN_NONAMESPACE

inline
SimpleHandler*
new_simple(GroupHandler* parent)
{
  return new SimpleHandler(parent);
}

inline
ComplexHandler*
new_complex(GroupHandler* parent)
{
  return new ComplexHandler(parent);
}

inline
GroupHandler*
new_group(GroupHandler* parent)
{
  return new GroupHandler(parent);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
TableHandler::TableHandler(GroupHandler* parent) :
  Str1GroupHandler(parent)
{
  // simple attributes

  // complex attribute
  DotlibHandler* complex = new_complex(this);
  reg_handler("index_1", complex);
  reg_handler("index_2", complex);
  reg_handler("index_3", complex);
  reg_handler("values", complex);

  // group statements
  reg_handler("domain", new_group(this));
}

// @brief デストラクタ
TableHandler::~TableHandler()
{
}

END_NAMESPACE_YM_CELL_DOTLIB
