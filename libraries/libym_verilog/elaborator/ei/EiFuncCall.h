#ifndef LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H
#define LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H

/// @file libym_verilog/elaborator/ei/EiFuncCall.h
/// @brief EiFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFuncCall.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EiFcBase EiFuncCall.h "EiFuncCall.h"
/// @brief function call/system-function call に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class EiFcBase :
  public EiExprBase
{
  friend class EiFactory;

protected:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiFcBase(const PtExpr* pt_expr,
	   ymuint arg_size,
	   ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiFcBase();


public:
  //////////////////////////////////////////////////////////////////////
  // ElbStmt の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の数を返す．
  virtual
  ymuint
  argument_num() const;

  /// @brief 引数の取得
  /// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
  virtual
  ElbExpr*
  argument(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ElbExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要求される式の型を計算してセットする．
  /// @param[in] type 要求される式の型
  /// @note 必要であればオペランドに対して再帰的に処理を行なう．
  virtual
  void
  _set_reqsize(const VlValueType& type);

  /// @brief オペランドを返す．
  /// @param[in] pos 位置番号
  /// @note 演算子の時，意味を持つ．
  /// @note このクラスでは NULL を返す．
  virtual
  ElbExpr*
  _operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数の数
  ymuint32 mArgNum;

  // 引数の配列
  ElbExpr** mArgList;

};


//////////////////////////////////////////////////////////////////////
/// @class EiFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiFuncCall :
  public EiFcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] func 関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiFuncCall(const PtExpr* pt_expr,
	     const ElbTaskFunc* func,
	     ymuint arg_size,
	     ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  virtual
  bool
  is_const() const;

  /// @brief 関数呼び出しの時に true を返す．
  virtual
  bool
  is_funccall() const;

  /// @brief 対象の関数を返す．
  /// @note kVpiFuncCall の時，意味を持つ．
  virtual
  const VlTaskFunc*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const ElbTaskFunc* mFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class EiSysFuncCall EiFuncCall.h "EiFuncCall.h"
/// @brief システム関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class EiSysFuncCall :
  public EiFcBase
{
  friend class EiFactory;

private:

  /// @brief コンストラクタ
  /// @param[in] pt_expr パース木の定義要素
  /// @param[in] user_systf システム関数
  /// @param[in] arg_size 引数の数
  /// @param[in] arg_list 引数のリスト
  EiSysFuncCall(const PtExpr* pt_expr,
		const ElbUserSystf* user_systf,
		ymuint arg_size,
		ElbExpr** arg_list);

  /// @brief デストラクタ
  virtual
  ~EiSysFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // VlObj の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型の取得
  virtual
  tVpiObjType
  type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // VlExpr の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 式のタイプを返す．
  virtual
  VlValueType
  value_type() const;

  /// @brief 定数の時 true を返す．
  /// @note このクラスは false を返す．
  virtual
  bool
  is_const() const;

  /// @brief システム関数よびあどい時に true を返す．
  virtual
  bool
  is_sysfunccall() const;

  /// @brief 対象のシステム関数を返す．
  /// @note kVpiSysFuncCall の時，意味を持つ．
  virtual
  const VlUserSystf*
  user_systf() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // システム関数
  const ElbUserSystf* mUserSystf;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EI_EIFUNCCALL_H
