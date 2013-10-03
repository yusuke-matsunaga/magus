#ifndef YM_SMTLIBV2_SMTEXPR_H
#define YM_SMTLIBV2_SMTEXPR_H

/// @file ym_smtlibv2/SmtExpr.h
/// @brief SmtExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtExpr SmtExpr.h "ym_smtlibv2/SmtExpr.h"
/// @brief s-expr を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtExpr
{
public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を表す列挙型
  enum tType {
    /// @brief constant
    kConst,
    /// @brief <symbol>
    kSymbol,
    /// @brief <keyword>
    kKeyword,
    /// @brief list
    kList
  };


public:

  /// @brief デストラクタ
  virtual
  ~SmtExpr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const = 0;

  /// @brief 定数型の場合に SmtConst を返す．
  virtual
  const SmtConst*
  const_val() const;

  /// @brief kSymbol 型の場合にシンボル名を表す文字列を返す．
  virtual
  ShString
  symbol_val() const;

  /// @brief kKeyword 型の場合にキーワードを表す文字列を返す．
  virtual
  ShString
  keyword_val() const;

  /// @brief kList 型の場合に要素数を返す．
  virtual
  ymuint
  list_num() const;

  /// @brief kList 型の場合に要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < lit_num() )
  virtual
  const SmtExpr*
  list_elem(ymuint pos) const;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTEXPR_H
