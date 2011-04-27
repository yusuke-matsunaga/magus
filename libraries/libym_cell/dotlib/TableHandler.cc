
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

#include "PtMgr.h"
#include "PtNode.h"
#include "PtPin.h"
#include "PtTable.h"
#include "PtValue.h"


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
  return new GenGroupHandler(parent);
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス TableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のハンドラ
TableHandler::TableHandler(GroupHandler* parent) :
  Str1GroupHandler(parent),
  mTable(NULL)
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

// @brief attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
TableHandler::add_attr(const ShString& attr_name,
		       PtValue* value)
{
  assert_cond( mTable != NULL, __FILE__, __LINE__);

  if ( attr_name == "index_1" ) {
    mTable->mIndex1 = value;
  }
  else if ( attr_name == "index_2" ) {
    mTable->mIndex2 = value;
  }
  else if ( attr_name == "index_3" ) {
    mTable->mIndex3 = value;
  }
  else if ( attr_name == "values" ) {
    mTable->mValues = value;
  }
  else {
#warning "TODO: エラーメッセージの出力"
    return false;
  }
  return true;
}

// @brief domain グループをセットする．
// @param[in] domain ドメイン
bool
TableHandler::add_domain(PtDomain* domain)
{
  if ( mTable->mDomain != NULL ) {
#warning "TODO: エラーメッセージの出力"
    return false;
  }
  mTable->mDomain = domain;
  return true;
}

// @brief group statement の最初に呼ばれる関数
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] valuet 値
bool
TableHandler::begin_group(const ShString& attr_name,
			  const FileRegion& attr_loc,
			  const ShString& value)
{
  mTable = ptmgr().new_pttable(value);
  return parent()->add_table(attr_name, mTable);
}

// @brief group statement の最後に呼ばれる関数
bool
TableHandler::end_group()
{
  mTable = NULL;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
