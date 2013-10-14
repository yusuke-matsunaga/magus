#ifndef SMTQUALTERM_H
#define SMTQUALTERM_H

/// @file SmtCompTerm.h
/// @brief SmtCompTerm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtTermImpl.h"
#include "ym_smtlibv2/SmtVarBinding.h"
#include "ym_smtlibv2/SmtSortedVar.h"
#include "ym_smtlibv2/SmtAttr.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtCompTerm SmtCompTerm.h "SmtCompTerm.h"
/// @brief 複合文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtCompTerm :
  public SmtTermImpl
{
  friend class SmtMgr;
protected:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  SmtCompTerm(const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtCompTerm();


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
/// @class SmtLet SmtCompTerm.h "SmtCompTerm.h"
/// @brief let 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtLet :
  public SmtCompTerm
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] var_binding 変数割り当てのリスト
  /// @param[in] body 本体
  SmtLet(const vector<SmtVarBinding>& var_binding,
	 const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtLet();


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
  SmtVarBinding
  let_binding(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数割り当ての数
  ymuint32 mBindingNum;

  // 変数割り当ての配列
  // 実際には必要な領域が確保される．
  SmtVarBinding mBindingList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtQualTerm SmtCompTerm.h "SmtCompTerm.h"
/// @brief forall/exists の共通の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtQualTerm :
  public SmtCompTerm
{
  friend class SmtMgr;
protected:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数のリスト
  /// @param[in] body 本体
  SmtQualTerm(const vector<SmtSortedVar>& var_list,
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
  SmtSortedVar
  sorted_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の数
  ymuint32 mVarNum;

  // 変数の配列
  // 実際には必要なサイズを確保する．
  SmtSortedVar mVarList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtForall SmtCompTerm.h "SmtCompTerm.h"
/// @brief forall 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtForall :
  public SmtQualTerm
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数のリスト
  /// @param[in] body 本体
  SmtForall(const vector<SmtSortedVar>& var_list,
	    const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtForall();


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
/// @class SmtExists SmtCompTerm.h "SmpCompTerm.h"
/// @brief forall 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtExists :
  public SmtQualTerm
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] var_list 変数のリスト
  /// @param[in] body 本体
  SmtExists(const vector<SmtSortedVar>& var_list,
	    const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtExists();


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
/// @class SmtAttrTerm SmtCompTerm.h "SmpCompTerm.h"
/// @brief attr 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtAttrTerm :
  public SmtCompTerm
{
  friend class SmtMgr;
private:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] attr_list 属性のリスト
  SmtAttrTerm(const SmtTerm* body,
	      const vector<SmtAttr>& attr_list);

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

  /// @brief attr 型の場合に属性キーを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  ShString
  attr_keyword(ymuint pos) const;

  /// @brief attr 型の場合に属性値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < attr_attr_num() )
  virtual
  const SmtTerm*
  attr_value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性の数
  ymuint32 mAttrNum;

  // 属性の配列
  // 実際には必要なサイズが確保される．
  SmtAttr mAttrList[1];

};


END_NAMESPACE_YM_SMTLIBV2

#endif // SMTQUALTERM_H
