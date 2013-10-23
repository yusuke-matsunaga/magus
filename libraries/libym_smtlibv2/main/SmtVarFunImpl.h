#ifndef SMTVARFUNIMPL_H
#define SMTFVARUNIMPL_H

/// @file SmtVarFunImpl.h
/// @brief SmtVarFunImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtVarFun.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtVarFunImpl SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief SmtVarFun の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtVarFunImpl :
  public SmtVarFun
{
  friend class SmtFunMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  SmtVarFunImpl(const SmtId* name);

  /// @brief デストラクタ
  virtual
  ~SmtVarFunImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief 変数の場合に true を返す．
  virtual
  bool
  is_var() const;

  /// @brief 変数の型を返す．
  /// @note 型がついていない場合は NULL を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  var_sort() const;

  /// @brief global 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_global() const;

  /// @brief forall 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_forall() const;

  /// @brief exists 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_exists() const;

  /// @brief 関数の場合に true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief 入力数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtVarFun*
  input_var(ymuint pos) const;

  /// @brief 出力の型を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  output_sort() const;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtTerm*
  fun_body() const;

  /// @brief 関数の属性を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  tFunAttr
  fun_attr() const;

  /// @brief パラメータの数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const SmtId* mName;

  // ハッシュ用のリンクポインタ
  SmtVarFunImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtVarBase SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 変数を表す基底クラス
//////////////////////////////////////////////////////////////////////
class SmtVarBase :
  public SmtVarFunImpl
{
  friend class SmtNameMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前を表す識別子
  /// @param[in] sort 型
  SmtVarBase(const SmtId* name,
	     const SmtSort* sort = NULL);

  /// @brief デストラクタ
  virtual
  ~SmtVarBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数の場合に true を返す．
  virtual
  bool
  is_var() const;

  /// @brief 変数の型を返す．
  /// @note 型がついていない場合は NULL を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  var_sort() const;

  /// @brief global 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_global() const;

  /// @brief forall 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_forall() const;

  /// @brief exists 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_exists() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  const SmtSort* mSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtGlobalVar SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief global 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtGlobalVar :
  public SmtVarBase
{
  friend class SmtNameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前を表す識別子
  /// @param[in] sort 型
  SmtGlobalVar(const SmtId* name,
	       const SmtSort* sort = NULL);

  /// @brief デストラクタ
  virtual
  ~SmtGlobalVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief global 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_global() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtForallVar SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief global 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtForallVar :
  public SmtVarBase
{
  friend class SmtNameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前を表す識別子
  /// @param[in] sort 型
  SmtForallVar(const SmtId* name,
	       const SmtSort* sort);

  /// @brief デストラクタ
  virtual
  ~SmtForallVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief forall 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_forall() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtExistsVar SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief global 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtExistsVar :
  public SmtVarBase
{
  friend class SmtNameMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前を表す識別子
  /// @param[in] sort 型
  SmtExistsVar(const SmtId* name,
	       const SmtSort* sort);

  /// @brief デストラクタ
  virtual
  ~SmtExistsVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief exists 変数の場合に true を返す．
  /// @note is_var() == false の場合にはエラーとなる．
  virtual
  bool
  is_exists() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFunBase SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 関数を表す基底クラス
//////////////////////////////////////////////////////////////////////
class SmtFunBase :
  public SmtVarFunImpl
{
  friend class SmtNameMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  SmtFunBase(const SmtId* name,
	     const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtFunBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の場合に true を返す．
  virtual
  bool
  is_fun() const;

  /// @brief 入力数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtVarFun*
  input_var(ymuint pos) const;

  /// @brief 出力の型を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtSort*
  output_sort() const;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  const SmtTerm*
  fun_body() const;

  /// @brief 関数の属性を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  tFunAttr
  fun_attr() const;

  /// @brief パラメータの数を返す．
  /// @note is_fun() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の型
  const SmtSort* mOutputSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDeclFun1 SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 宣言のみで引数がない関数
//////////////////////////////////////////////////////////////////////
class SmtDeclFun1 :
  public SmtFunBase
{
  friend class SmtFunMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  SmtDeclFun1(const SmtId* name,
	      const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtDeclFun1();

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDeclFun2 SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 宣言のみの関数
//////////////////////////////////////////////////////////////////////
class SmtDeclFun2 :
  public SmtDeclFun1
{
  friend class SmtFunMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] input_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] fun_attr 属性
  /// @param[in] param_num パラメータの数
  SmtDeclFun2(const SmtId* name,
	      const vector<const SmtSort*>& input_list,
	      const SmtSort* output_sort,
	      tFunAttr fun_attr,
	      ymuint param_num);

  /// @brief デストラクタ
  virtual
  ~SmtDeclFun2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtVarFun*
  input_var(ymuint pos) const;

  /// @brief 属性を返す．
  virtual
  tFunAttr
  fun_attr() const;

  /// @brief パラメータの数を返す．
  virtual
  ymuint
  param_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性
  tFunAttr mFunAttr;

  // パラメータの数
  ymuint32 mParamNum;

  // 入力数
  ymuint32 mInputNum;

  // 入力の型の配列
  // 実際には必要な領域を確保する．
  const SmtSort* mInputList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDefFun1 SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 定義を持つ引数のない関数
//////////////////////////////////////////////////////////////////////
class SmtDefFun1 :
  public SmtFunBase
{
  friend class SmtFunMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体
  SmtDefFun1(const SmtId* name,
	     const SmtSort* output_sort,
	     const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtDefFun1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  fun_body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const SmtTerm* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDefFun2 SmtVarFunImpl.h "SmtVarFunImpl.h"
/// @brief 定義を持つ関数
//////////////////////////////////////////////////////////////////////
class SmtDefFun2 :
  public SmtDefFun1
{
  friend class SmtFunMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  /// @param[in] input_var_list 入力変数のリスト
  /// @param[in] body 本体
  SmtDefFun2(const SmtId* name,
	     const vector<const SmtVarFun*>& input_var_list,
	     const SmtSort* output_sort,
	     const SmtTerm* body);

  /// @brief デストラクタ
  virtual
  ~SmtDefFun2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

  /// @brief 入力変数を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtVarFun*
  input_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 入力の変数の配列
  // 実際には必要な領域を確保する．
  const SmtVarFun* mInputList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTVARFUNIMPL_H
