#ifndef SMTLISTEXPR_H
#define SMTLISTEXPR_H

/// @file SmtListExpr.h
/// @brief SmtListExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtExpr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtListExpr SmtListExpr.h "SmtListExpr.h"
/// @brief リストタイプの SmtExpr の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtListExpr :
  public SmtExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_list 要素のリスト
  SmtListExpr(const vector<const SmtExpr*>& elem_list);

  /// @brief デストラクタ
  virtual
  ~SmtListExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kList 型の場合に要素数を返す．
  virtual
  ymuint
  list_num() const;

  /// @brief kList 型の場合に要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < lit_num() )
  virtual
  const SmtExpr*
  list_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  vector<const SmtExpr*> mElemList;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLISTEXPR_H
