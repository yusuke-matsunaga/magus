#ifndef SMTVARTERM_H
#define SMTVARTERM_H

/// @file SmtVarTerm.h
/// @brief SmtVarTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtVarTerm SmtVarTerm.h "SmtVarTerm.h"
/// @brief 識別子を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtVarTerm :
  public SmtTermImpl
{
  friend class SmtTermMgr;
protected:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  SmtVarTerm(const SmtVarFun* var);

  /// @brief デストラクタ
  virtual
  ~SmtVarTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kVar 型の場合に変数を返す．
  virtual
  const SmtVarFun*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  const SmtVarFun* mVar;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTVARTERM_H
