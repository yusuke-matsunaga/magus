#ifndef SMTCONSTTERM_H
#define SMTCONSTTERM_H

/// @file SmtConstTerm.h
/// @brief SmtConstTerm の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtConstTerm SmtConstTerm.h "SmtConstTerm.h"
/// @brief 定数を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtConstTerm :
  public SmtTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 定数を表すインスタンス
  SmtConstTerm(const SmtConst* val);

  /// @brief デストラクタ
  virtual
  ~SmtConstTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kConstant 型の場合に SmtConst を返す．
  virtual
  const SmtConst*
  const_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値を表すインスタンス
  const SmtConst* mVal;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTCONSTTERM_H
