#ifndef SMTSYMBOLEXPR_H
#define SMTSYMBOLEXPR_H

/// @file SmtSymbolExpr.h
/// @brief SmtSymbolExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtSymbolExpr SmtSymbolExpr.h "SmtSymbolExpr.h"
/// @brief <symbol> タイプの SmtExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtSymbolExpr :
  public SmtExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val シンボル名
  explicit
  SmtSymbolExpr(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtSymbolExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kSymbol 型の場合にシンボル名を表す文字列を返す．
  virtual
  ShString
  symbol_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボル名
  ShString mVal;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTSYMBOLEXPR_H
