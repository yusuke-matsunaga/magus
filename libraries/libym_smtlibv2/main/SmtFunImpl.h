#ifndef SMTFUNIMPL_H
#define SMTFUNIMPL_H

/// @file SmtFunImpl.h
/// @brief SmtFunImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtFun.h"
#include "ym_smtlibv2/SmtSortedVar.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtFunImpl SmtFunImpl.h "SmtFunImpl.h"
/// @brief SmtFun の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtFunImpl :
  public SmtFun
{
  friend class SmtFunMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  SmtFunImpl(const SmtId* name,
	     const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtFunImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief 出力の型を返す．
  virtual
  const SmtSort*
  output_sort() const;

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
  const SmtId*
  input_var(ymuint pos) const;

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

  // 名前
  const SmtId* mName;

  // 出力の型
  const SmtSort* mOutputSort;

  // ハッシュ用のリンクポインタ
  SmtFunImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFun1 SmtFunImpl.h "SmtFunImpl.h"
/// @brief 宣言のみで引数がない関数
//////////////////////////////////////////////////////////////////////
class SmtDeclFun1 :
  public SmtFunImpl
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
/// @class SmtFun1 SmtFunImpl.h "SmtFunImpl.h"
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
  /// @param[in] attr 属性
  /// @param[in] param_num パラメータの数
  SmtDeclFun2(const SmtId* name,
	      const vector<const SmtSort*>& input_list,
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
  const SmtId*
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
  friend class SmtFunMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] output_sort 出力の型
  /// @param[in] input_var_list 型つき入力変数のリスト
  /// @param[in] body 本体
  SmtDefFun2(const SmtId* name,
	     const vector<SmtSortedVar>& input_var_list,
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
  const SmtId*
  input_var(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 入力の変数と型の配列
  // 実際には必要な領域を確保する．
  SmtSortedVar mInputList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTFUNIMPL_H
