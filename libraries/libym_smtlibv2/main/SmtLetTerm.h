#ifndef SMTLETTERM_H
#define SMTLETTERM_H

/// @file SmtLetTerm.h
/// @brief SmtLetTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLetTerm SmtLetTerm.h "SmtLetTerm.h"
//////////////////////////////////////////////////////////////////////
class SmtLetTerm :
  public SmtTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] bind_list バインドリスト
  /// @param[in] body 本体
  SmtLetTerm(const vector<pair<ShString, const SmtTerm*> >& bind_list,
	     const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtLetTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素数を返す．
  virtual
  ymuint
  let_binding_num() const;

  /// @brief kLet 型の場合に変数バインディングリストの要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < let_binding_num() )
  virtual
  pair<ShString, const SmtTerm*>
  let_binding(ymuint pos) const;

  /// @brief kLet 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  let_body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数バインディングのリスト
  vector<pair<ShString, const SmtTerm*> > mBindList;

  // 本体
  const SmtTerm* mBody;

};


END_NAMESPACE_YM_SMTLIBV2

#endif // SMTLETTERM_H
