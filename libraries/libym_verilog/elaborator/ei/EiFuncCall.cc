
/// @file libym_verilog/elb_impl/EiFuncCall.cc
/// @brief EiFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiFuncCall.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiFuncCall.h"
#include "ElbTaskFunc.h"
#include "ElbUserSystf.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_FuncCall(const PtExpr* pt_expr,
			const ElbTaskFunc* func,
			ymuint arg_size,
			ElbExpr** arg_list)
{
  void* p = mAlloc.get_memory(sizeof(EiFuncCall));
  EiFuncCall* expr = new (p) EiFuncCall(pt_expr, func,
					arg_size, arg_list);

  return expr;
}

// @brief システム関数呼び出し式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
ElbExpr*
EiFactory::new_SysFuncCall(const PtExpr* pt_expr,
			   const ElbUserSystf* user_systf,
			   ymuint arg_size,
			   ElbExpr** arg_list)
{
  void* p = mAlloc.get_memory(sizeof(EiSysFuncCall));
  EiSysFuncCall* expr = new (p) EiSysFuncCall(pt_expr, user_systf,
					      arg_size, arg_list);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFcBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFcBase::EiFcBase(const PtExpr* pt_expr,
		   ymuint arg_size,
		   ElbExpr** arg_list) :
  EiExprBase(pt_expr),
  mArgNum(arg_size),
  mArgList(arg_list)
{
}

// @brief デストラクタ
EiFcBase::~EiFcBase()
{
}

// @brief 引数の数を返す．
ymuint
EiFcBase::argument_num() const
{
  return mArgNum;
}

// @brief 引数の取得
// @param[in] pos 位置番号 ( 0 <= pos < argument_num() )
ElbExpr*
EiFcBase::argument(ymuint pos) const
{
  return mArgList[pos];
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiFcBase::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
// @note このクラスでは NULL を返す．
ElbExpr*
EiFcBase::_operand(ymuint pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] func 関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiFuncCall::EiFuncCall(const PtExpr* pt_expr,
		       const ElbTaskFunc* func,
		       ymuint arg_size,
		       ElbExpr** arg_list) :
  EiFcBase(pt_expr, arg_size, arg_list),
  mFunc(func)
{
}

// @brief デストラクタ
EiFuncCall::~EiFuncCall()
{
}

// @brief 型の取得
tVpiObjType
EiFuncCall::type() const
{
  return kVpiFuncCall;
}

// @brief 式のタイプを返す．
tVpiValueType
EiFuncCall::value_type() const
{
  switch ( mFunc->func_type() ) {
  case kVpiIntFunc:
    return kVpiValueInteger;

  case kVpiRealFunc:
    return kVpiValueReal;

  case kVpiTimeFunc:
    return kVpiValueTime;

  case kVpiSizedFunc:
    return pack(kVpiValueUS, mFunc->bit_size());

  case kVpiSizedSignedFunc:
    return pack(kVpiValueSS, mFunc->bit_size());

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
bool
EiFuncCall::is_const() const
{
  if ( mFunc->is_constant_function() ) {
    ymuint n = argument_num();
    for (ymuint i = 0; i < n; ++ i) {
      if ( !argument(i)->is_const() ) {
	return false;
      }
    }
    return true;
  }
  return false;
}

// @brief 関数呼び出しの時に true を返す．
bool
EiFuncCall::is_funccall() const
{
  return true;
}

// @brief 対象の関数を返す．
// @note kVpiFuncCall の時，意味を持つ．
const VlTaskFunc*
EiFuncCall::function() const
{
  return mFunc;
}


//////////////////////////////////////////////////////////////////////
// クラス EiSysFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] user_systf システム関数
// @param[in] arg_size 引数の数
// @param[in] arg_list 引数のリスト
EiSysFuncCall::EiSysFuncCall(const PtExpr* pt_expr,
			     const ElbUserSystf* user_systf,
			     ymuint arg_size,
			     ElbExpr** arg_list) :
  EiFcBase(pt_expr, arg_size, arg_list),
  mUserSystf(user_systf)
{
}

// @brief デストラクタ
EiSysFuncCall::~EiSysFuncCall()
{
}

// @brief 型の取得
tVpiObjType
EiSysFuncCall::type() const
{
  return kVpiSysFuncCall;
}

// @brief 式のタイプを返す．
tVpiValueType
EiSysFuncCall::value_type() const
{
  assert_cond(mUserSystf->system_function(), __FILE__, __LINE__);

  switch ( mUserSystf->function_type() ) {
  case vpiIntFunc:
    return kVpiValueInteger;
    break;

  case vpiRealFunc:
    return kVpiValueReal;
    break;

  case vpiTimeFunc:
    return kVpiValueTime;
    break;

  case vpiSizedFunc:
    return pack(kVpiValueUS, mUserSystf->size());
    break;

  case vpiSizedSignedFunc:
    return pack(kVpiValueSS, mUserSystf->size());
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return kVpiValueNone;
}

// @brief 定数の時 true を返す．
// @note このクラスは false を返す．
bool
EiSysFuncCall::is_const() const
{
  return false;
}

// @brief システム関数よびあどい時に true を返す．
bool
EiSysFuncCall::is_sysfunccall() const
{
  return true;
}

// @brief 対象のシステム関数を返す．
// @note kVpiSysFuncCall の時，意味を持つ．
const VlUserSystf*
EiSysFuncCall::user_systf() const
{
  return mUserSystf;
}

END_NAMESPACE_YM_VERILOG
