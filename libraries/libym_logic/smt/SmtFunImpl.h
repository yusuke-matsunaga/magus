#ifndef SMTFUNIMPL_H
#define SMTFUNIMPL_H

/// @file SmtFunImpl.h
/// @brief SmtFunImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 20113Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SmtFun.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtFunImpl SmtFunImpl.h "SmtFunImpl.h"
/// @brief SmtFun の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtFunImpl :
  public SmtFun
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  SmtFunImpl(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtFunImpl();


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
  const SmtVar*
  input_var(ymuint pos) const;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  output_sort() const;

  /// @brief 本体の式を返す．
  /// @note uninterpreted function の場合は NULL を返す．
  virtual
  const SmtTerm*
  body() const;

  /// @brief 属性を返す．
  virtual
  tAttr
  attr() const;

  /// @brief パラメータの数を返す．
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
/// @class SmtDeclFun1 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 宣言のみで引数がない関数
//////////////////////////////////////////////////////////////////////
class SmtDeclFun1 :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  SmtDeclFun1(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtDeclFun1();

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDeclFun2 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 宣言のみの関数
//////////////////////////////////////////////////////////////////////
class SmtDeclFun2 :
  public SmtDeclFun1
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] input_sort_list 入力の型のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] attr 属性
  /// @param[in] param_num パラメータの数
  SmtDeclFun2(const vector<const SmtSort*>& input_sort_list,
	      const SmtSort* output_sort,
	      tAttr attr,
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
  const SmtVar*
  input_var(ymuint pos) const;

  /// @brief 属性を返す．
  virtual
  tAttr
  attr() const;

  /// @brief パラメータの数を返す．
  virtual
  ymuint
  param_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性
  tAttr mAttr;

  // パラメータの数
  ymuint32 mParamNum;

  // 入力数
  ymuint32 mInputNum;

  // 入力の型の配列
  // 実際には必要な領域を確保する．
  const SmtSort* mInputList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDefFun1 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 定義を持つ引数のない関数
//////////////////////////////////////////////////////////////////////
class SmtDefFun1 :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体
  SmtDefFun1(const SmtSort* output_sort,
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
  body() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  const SmtTerm* mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDefFun2 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 定義を持つ関数
//////////////////////////////////////////////////////////////////////
class SmtDefFun2 :
  public SmtDefFun1
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_var_list 入力変数のリスト
  /// @param[in] output_sort 出力の型
  /// @param[in] body 本体
  SmtDefFun2(const vector<const SmtVar*>& input_var_list,
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
  const SmtVar*
  input_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 入力の変数と型の配列
  // 実際には必要な領域を確保する．
  const SmtVar* mInputList[1];

};


END_NAMESPACE_YM_SMT

#endif // SMTFUNIMPL_H
