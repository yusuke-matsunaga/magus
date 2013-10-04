#ifndef YM_SMTLIBV2_SMTVARBINDING_H
#define YM_SMTLIBV2_SMTVARBINDING_H

/// @file ym_smtlibv2/SmtVarBinding.h
/// @brief
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtVarBinding SmtVarBinding.h "ym_smtlibv2/SmtVarBinding.h"
/// @brief 変数割り当てを表すクラス
//////////////////////////////////////////////////////////////////////
struct SmtVarBinding
{

  /// @brief 変数
  const SmtId* mVar;

  /// @brief 割り当て式
  const SmtTerm* mExpr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTVARBINDING_H
