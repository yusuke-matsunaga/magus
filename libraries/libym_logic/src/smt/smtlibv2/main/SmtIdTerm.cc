
/// @file SmtIdTerm.cc
/// @brief SmtIdTerm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtIdTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtIdTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 識別子
SmtIdTerm::SmtIdTerm(const SmtId* id) :
  mId(id)
{
}

// @brief デストラクタ
SmtIdTerm::~SmtIdTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtIdTerm::type() const
{
  return kIdentifier;
}

// @brief kQualIdentifier 型の場合に識別子を返す．
const SmtId*
SmtIdTerm::identifier() const
{
  return mId;
}


//////////////////////////////////////////////////////////////////////
// クラス SmtQualIdTerm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id 識別子
// @param[in] sort 型
SmtQualIdTerm::SmtQualIdTerm(const SmtId* id,
			     const SmtSort* sort) :
  SmtIdTerm(id),
  mSort(sort)
{
}

// @brief デストラクタ
SmtQualIdTerm::~SmtQualIdTerm()
{
}

// @brief 型を返す．
SmtTerm::tType
SmtQualIdTerm::type() const
{
  return kQualIdentifier;
}

// @brief kQualIdentifier 型の場合に型を返す．
// @note 型がついていない場合には NULL を返す．
const SmtSort*
SmtQualIdTerm::identifier_sort() const
{
  return mSort;
}

END_NAMESPACE_YM_SMTLIBV2
