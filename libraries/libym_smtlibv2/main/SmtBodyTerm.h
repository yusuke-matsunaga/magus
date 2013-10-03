#ifndef SMTBODYTERM_H
#define SMTBODYTERM_H

/// @file SmtBodyTerm.h
/// @brief SmtBodyTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtTerm.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtBodyTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief body() を持つクラスの共通の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtBodyTerm :
  public SmtTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  SmtBodyTerm(const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtBodyTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kLet/kForall/kExists/kAttr 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const SmtTerm* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLetTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief let を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtLetTerm :
  public SmtBodyTerm
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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バインドリスト
  vector<pair<ShString, const SmtTerm*> > mBindList;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtQualTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief forall/exists に共通の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtQualTerm :
  public SmtBodyTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  SmtQualTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
	      const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtQualTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < forall_var_num() )
  virtual
  pair<ShString, const SmtSort*>
  sorted_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<pair<ShString, const SmtSort*> > mVarList;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtForallTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief forall を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtForallTerm :
  public SmtQualTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  SmtForallTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
		const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtForallTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtExistsTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief exists を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtExistsTerm :
  public SmtQualTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数リスト
  /// @param[in] body 本体
  SmtExistsTerm(const vector<pair<ShString, const SmtSort*> >& var_list,
		const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtExistsTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtAttrTerm SmtBodyTerm.h "SmtBodyTerm.h"
/// @brief attribute を表す SmtTerm の派生クラス
//////////////////////////////////////////////////////////////////////
class SmtAttrTerm :
  public SmtBodyTerm
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] attr_list 属性のリスト
  SmtAttrTerm(const SmtTerm* body,
	      const vector<const SmtAttr*>& attr_list);

  /// @brief デストラクタ
  virtual
  ~SmtAttrTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief attr 型の場合に属性リストの要素数を返す．
  virtual
  ymuint
  attr_num() const;

  /// @brief attr 型の場合に属性を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  const SmtAttr*
  attr(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性リスト
  vector<const SmtAttr*> mAttrList;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTBODYTERM_H
