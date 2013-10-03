#ifndef SMTKEYWORDEXPR_H
#define SMTKEYWORDEXPR_H

/// @file SmtKeywordExpr.h
/// @brief SmtKeywordExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtKeywordExpr SmtKeywordExpr.h "SmtKeywordExpr.h"
/// @brief <keyword> タイプの SmtExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtKeywordExpr :
  public SmtExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val キーワード
  SmtKeywordExpr(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtKeywordExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kKeyword 型の場合にキーワードを表す文字列を返す．
  virtual
  ShString
  keyword_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // キーワード
  ShString mVal;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTKEYWORDEXPR_H
