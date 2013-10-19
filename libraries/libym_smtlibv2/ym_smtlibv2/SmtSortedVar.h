#ifndef YM_SMTLIBV2_SMTSORTEDVAR_H
#define YM_SMTLIBV2_SMTSORTEDVAR_H

/// @file ym_smtlibv2/SmtSortedVar.h
/// @brief SmtSortedVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtSortedVar SmtSortedVar.h "ym_smtlibv2/SmtSortedVar.h"
/// @brief 型付きの変数を表すクラス
//////////////////////////////////////////////////////////////////////
struct SmtSortedVar
{

  /// @brief 空のコンストラクタ
  SmtSortedVar():
    mSort(NULL),
    mVar(NULL)
  {
  }

  /// @brief コンストラクタ
  /// @param[in] sort 型
  /// @param[in] var 変数
  SmtSortedVar(const SmtSort* sort,
	       const SmtId* var) :
    mSort(sort),
    mVar(var)
  {
  }

  /// @brief 型
  const SmtSort* mSort;

  /// @brief 変数
  const SmtId* mVar;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSORTEDVAR_H
