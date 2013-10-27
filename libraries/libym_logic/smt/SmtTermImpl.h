#ifndef SMTTERMIMPL_H
#define SMTTERMIMPL_H

/// @file SmtTermImpl.h
/// @brief SmtTermImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SmtTerm.h"
#include "ym_logic/SmtAttr.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtTermImpl SmtTermImpl.h "SmtTermImpl.h"
/// @brief SmtTerm の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtTermImpl :
  public SmtTerm
{
  friend class SmtSolverImpl;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtTermImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymuint32
  int_value() const;

  /// @brief kDecConst/kHexConst/kBinConst/kStrConst 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const;

  /// @brief kVarTerm 型の場合に関数を返す．
  virtual
  const SmtVar*
  var() const;

  /// @brief kFunTerm 型の場合に関数の型を返す．
  virtual
  tSmtFun
  function_type() const;

  /// @brief kFunTerm 型の場合に関数を返す．
  virtual
  const SmtFun*
  function() const;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;

  /// @brief kForall/kExists 型の場合に変数リストの要素数を返す．
  virtual
  ymuint
  var_num() const;

  /// @brief kForall/kExists 型の場合に変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
  virtual
  const SmtVar*
  bound_var(ymuint pos) const;

  /// @brief annotated 型の場合に属性リストの要素数を返す．
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
  ShString
  attr_value(ymuint pos) const;

  /// @brief kForall/kExists/kAnnotated 型の場合に本体の項を返す．
  virtual
  const SmtTerm*
  body() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtNumTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief <numeric> 型の term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtNumTerm :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtNumTerm(ymuint val);

  /// @brief デストラクタ
  virtual
  ~SmtNumTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kNumConst 型の場合に整数値を返す．
  virtual
  ymuint32
  int_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtStrTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief 文字列を値とする term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtStrTerm :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtStrTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtStrTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kDecConst/kHexConst/kBinConst/kStrConst 型の場合に文字列を返す．
  virtual
  ShString
  str_value() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  ShString mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDecTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief <decimal> を値とする term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtDecTerm :
  public SmtStrTerm
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtDecTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtDecTerm();


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
/// @class SmtHexTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief <hexadecimal> を値とする term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtHexTerm :
  public SmtStrTerm
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtHexTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtHexTerm();


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
/// @class SmtBinTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief <binary> を値とする term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtBinTerm :
  public SmtStrTerm
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] val 値
  SmtBinTerm(const ShString& val);

  /// @brief デストラクタ
  virtual
  ~SmtBinTerm();


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
/// @class SmtVarTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief 変数を持つ term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtVarTerm :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  SmtVarTerm(const SmtVar* var);

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

  /// @brief kVarTerm 型の場合に関数を返す．
  virtual
  const SmtVar*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  const SmtVar* mVar;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFunTerm1 SmtTermImpl.h "SmtTermImpl.h"
/// @brief 関数呼び出しを表す term のクラス(引数なし)
//////////////////////////////////////////////////////////////////////
class SmtFunTerm1 :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] fun 関数
  SmtFunTerm1(const SmtFun* fun);

  /// @brief デストラクタ
  virtual
  ~SmtFunTerm1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kFunTerm 型の場合に関数の型を返す．
  virtual
  tSmtFun
  function_type() const;

  /// @brief kFunTerm 型の場合に関数を返す．
  virtual
  const SmtFun*
  function() const;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const SmtFun* mFun;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFunTerm2 SmtTermImpl.h "SmtTermImpl.h"
/// @brief 関数呼び出しを表す term のクラス(引数あり)
//////////////////////////////////////////////////////////////////////
class SmtFunTerm2 :
  public SmtFunTerm1
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] fun 関数
  /// @param[in] arg_list 引数のリスト
  SmtFunTerm2(const SmtFun* fun,
	      const vector<const SmtTerm*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~SmtFunTerm2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 引数のリスト
  const SmtTerm* mInputList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFunTerm3 SmtTermImpl.h "SmtTermImpl.h"
/// @brief 組み込み関数呼び出しを表す term のクラス(引数なし)
//////////////////////////////////////////////////////////////////////
class SmtFunTerm3 :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] fun_type 関数の型
  SmtFunTerm3(tSmtFun fun_type);

  /// @brief デストラクタ
  virtual
  ~SmtFunTerm3();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief kFunTerm 型の場合に関数の型を返す．
  virtual
  tSmtFun
  function_type() const;

  /// @brief kFunTerm 型の場合に関数を返す．
  /// @note 組み込み関数の場合には NULL を返す．
  virtual
  const SmtFun*
  function() const;

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数の型
  tSmtFun mFunType;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFunTerm4 SmtTermImpl.h "SmtTermImpl.h"
/// @brief 組み込み関数呼び出しを表す term のクラス(引数あり)
//////////////////////////////////////////////////////////////////////
class SmtFunTerm4 :
  public SmtFunTerm3
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] fun_type 関数の型
  /// @param[in] arg_list 引数のリスト
  SmtFunTerm4(tSmtFun fun_type,
	      const vector<const SmtTerm*>& arg_list);

  /// @brief デストラクタ
  virtual
  ~SmtFunTerm4();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief kFunTerm 型の場合に入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief kFunTerm 型の場合に入力を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtTerm*
  input(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 引数のリスト
  const SmtTerm* mInputList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtQualTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief forall/exists 文に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class SmtQualTerm :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] var_list 束縛変数のリスト
  /// @param[in] body 本体の式
  SmtQualTerm(const vector<const SmtVar*>& var_list,
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
  /// @param[in] pos 位置番号 ( 0 <= pos < var_num() )
  virtual
  const SmtVar*
  bound_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体の式
  const SmtTerm* mBody;

  // 束縛変数の数
  ymuint32 mVarNum;

  // 束縛変数のリスト
  const SmtVar* mVarList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtForallTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief forall 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtForallTerm :
  public SmtQualTerm
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] var_list 束縛変数のリスト
  /// @param[in] body 本体の式
  SmtForallTerm(const vector<const SmtVar*>& var_list,
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
/// @class SmtExistsTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief exists 文を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtExistsTerm :
  public SmtQualTerm
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] var_list 束縛変数のリスト
  /// @param[in] body 本体の式
  SmtExistsTerm(const vector<const SmtVar*>& var_list,
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
/// @class SmtAnnotatedTerm SmtTermImpl.h "SmtTermImpl.h"
/// @brief annotated 型の term を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtAnnotatedTerm :
  public SmtTermImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] body 本体の式
  /// @param[in] attr_list 属性リスト
  SmtAnnotatedTerm(const SmtTerm* body,
		   const vector<SmtAttr>& attr_list);

  /// @brief デストラクタ
  virtual
  ~SmtAnnotatedTerm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief annotated 型の場合に属性リストの要素数を返す．
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
  ShString
  attr_value(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体の式
  const SmtTerm* mBody;

  // 属性リストの要素数
  ymuint32 mAttrNum;

  // 属性リスト
  SmtAttr mAttrList[1];

};

END_NAMESPACE_YM_SMT

#endif // SMTTERMIMPL_H
