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

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

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
  tSmtFunAttr
  attr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の型
  const SmtSort* mOutputSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtTrueFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'true' function
//////////////////////////////////////////////////////////////////////
class SmtTrueFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtTrueFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtTrueFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtFalseFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'false' function
//////////////////////////////////////////////////////////////////////
class SmtFalseFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtFalseFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtFalseFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtNotFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'not' function
//////////////////////////////////////////////////////////////////////
class SmtNotFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtNotFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtNotFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLogBinFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 論理二項演算を表す基底クラス
//////////////////////////////////////////////////////////////////////
class SmtLogBinFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtLogBinFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtLogBinFun();


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

  /// @brief 属性を返す．
  virtual
  tSmtFunAttr
  attr() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtAndFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'and' function
//////////////////////////////////////////////////////////////////////
class SmtAndFun :
  public SmtLogBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtAndFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtAndFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtOrFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'and' function
//////////////////////////////////////////////////////////////////////
class SmtOrFun :
  public SmtLogBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtOrFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtOrFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtXorFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'xor' function
//////////////////////////////////////////////////////////////////////
class SmtXorFun :
  public SmtLogBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtXorFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtXorFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtImpFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'implication(=>)' function
//////////////////////////////////////////////////////////////////////
class SmtImpFun :
  public SmtLogBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] bool_sort 出力の型
  /// @note bool_sort はブール型でなければならない．
  SmtImpFun(const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtImpFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtCompFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 等価/非等価比較関数の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtCompFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] bool_sort ブール型
  SmtCompFun(const SmtSort* input_sort,
	     const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtCompFun();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の型
  const SmtSort* mInputSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtEqFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief equality (=) 関数
//////////////////////////////////////////////////////////////////////
class SmtEqFun :
  public SmtCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] bool_sort ブール型
  SmtEqFun(const SmtSort* input_sort,
	   const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtEqFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

  /// @brief 属性を返す．
  virtual
  tSmtFunAttr
  attr() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDiseqFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'distinct' 関数
//////////////////////////////////////////////////////////////////////
class SmtDiseqFun :
  public SmtCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] bool_sort ブール型
  SmtDiseqFun(const SmtSort* input_sort,
	      const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtDiseqFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

  /// @brief 属性を返す．
  virtual
  tSmtFunAttr
  attr() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtIteFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 'ite' (if-then-else) function
//////////////////////////////////////////////////////////////////////
class SmtIteFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] data_sort データの型
  /// @param[in] bool_sort ブール型
  SmtIteFun(const SmtSort* data_sort,
	    const SmtSort* bool_sort);

  /// @brief デストラクタ
  virtual
  ~SmtIteFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ブール型
  const SmtSort* mBoolSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtUminusFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 単項のマイナス演算子
//////////////////////////////////////////////////////////////////////
class SmtUminusFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtUminusFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtUminusFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 入力の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  virtual
  const SmtSort*
  input_sort(ymuint pos) const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtArithBinFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 算術二項演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtArithBinFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtArithBinFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtArithBinFun();


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

  /// @brief 属性を返す．
  virtual
  tSmtFunAttr
  attr() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtAddFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '+' function
//////////////////////////////////////////////////////////////////////
class SmtAddFun :
  public SmtArithBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtAddFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtAddFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtSubFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '-' function
//////////////////////////////////////////////////////////////////////
class SmtSubFun :
  public SmtArithBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtSubFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtSubFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtMulFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '*' function
//////////////////////////////////////////////////////////////////////
class SmtMulFun :
  public SmtArithBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtMulFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtMulFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtDivFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '*' function
//////////////////////////////////////////////////////////////////////
class SmtDivFun :
  public SmtArithBinFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] output_sort 出力の型
  /// @note 入力の型も出力と同じ(IntかReal)
  SmtDivFun(const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtDivFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtArithCompFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief 算術比較演算子の基底クラス
//////////////////////////////////////////////////////////////////////
class SmtArithCompFun :
  public SmtFunImpl
{
  friend class SmtSolverImpl;

protected:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtArithCompFun(const SmtSort* input_sort,
		  const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtArithCompFun();


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

  /// @brief 属性を返す．
  virtual
  tSmtFunAttr
  attr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の型
  const SmtSort* mInputSort;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLeFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '<=' function
//////////////////////////////////////////////////////////////////////
class SmtLeFun :
  public SmtArithCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtLeFun(const SmtSort* input_sort,
	   const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtLeFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtLtFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '<' function
//////////////////////////////////////////////////////////////////////
class SmtLtFun :
  public SmtArithCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtLtFun(const SmtSort* input_sort,
	   const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtLtFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtGeFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '>=' function
//////////////////////////////////////////////////////////////////////
class SmtGeFun :
  public SmtArithCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtGeFun(const SmtSort* input_sort,
	   const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtGeFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtGtFun SmtFunImpl.h "SmtFunImpl.h"
/// @brief '>' function
//////////////////////////////////////////////////////////////////////
class SmtGtFun :
  public SmtArithCompFun
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] input_sort 入力の型
  /// @param[in] output_sort 出力の型
  /// @note output_sort はブール型でなければならない．
  SmtGtFun(const SmtSort* input_sort,
	   const SmtSort* output_sort);

  /// @brief デストラクタ
  virtual
  ~SmtGtFun();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の場合に型を返す．
  /// @note 普通の関数は kSmtFun_UserDef を返す．
  virtual
  tSmtFun
  type() const;

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
  SmtDeclFun2(const vector<const SmtSort*>& input_sort_list,
	      const SmtSort* output_sort);

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

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
