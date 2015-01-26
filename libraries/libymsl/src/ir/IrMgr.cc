
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"

#include "IrScope.h"
#include "IrVar.h"
#include "IrFunction.h"

#include "IrTrue.h"
#include "IrFalse.h"
#include "IrIntConst.h"
#include "IrFloatConst.h"
#include "IrStringConst.h"
#include "IrVarExpr.h"
#include "IrArrayRef.h"
#include "IrMemberRef.h"
#include "IrOpExpr.h"
#include "IrFuncCall.h"
#include "IrScopeExpr.h"
#include "IrFuncExpr.h"
#include "IrEnumExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrMgr::IrMgr()
{
}

// @brief デストラクタ
IrMgr::~IrMgr()
{
}

// @brief スコープを生成する．
// @param[in] parent 親のスコープ
// @param[in] name 名前
IrScope*
IrMgr::new_Scope(IrScope* parent,
		 ShString name)
{
  void* p = mAlloc.get_memory(sizeof(IrScope));
  return new (p) IrScope(parent, name);
}

// @brief 変数を生成する．
// @param[in] name 名前
// @param[in] type 型
// @param[in] index インデックス
IrVar*
IrMgr::new_Var(ShString name,
	       const IrType* type,
	       ymuint index)
{
  void* p = mAlloc.get_memory(sizeof(IrVar));
  return new (p) IrVar(name, type, index);
}

// @brief 関数を生成する．
// @param[in] name 名前
// @param[in] type 出力の型
// @param[in] input_type_list 入力の型のリスト
// @param[in] index インデックス
IrFunction*
IrMgr::new_Function(ShString name,
		    const IrType* type,
		    const vector<const IrType*>& input_type_list,
		    ymuint index)
{
  void* p = mAlloc.get_memory(sizeof(IrFunction));
  return new (p) IrFunction(name, type, input_type_list, index);
}

// @brief true 定数を生成する．
IrExpr*
IrMgr::new_True()
{
  void* p = mAlloc.get_memory(sizeof(IrTrue));
  return new (p) IrTrue();
}

// @brief False 定数を生成する．
IrExpr*
IrMgr::new_False()
{
  void* p = mAlloc.get_memory(sizeof(IrFalse));
  return new (p) IrFalse();
}

// @brief 整数値定数を生成する．
// @param[in] val 値
IrExpr*
IrMgr::new_IntConst(int val)
{
  void* p = mAlloc.get_memory(sizeof(IrIntConst));
  return new (p) IrIntConst(val);
}

// @brief 実数値定数を生成する．
// @param[in] val 値
IrExpr*
IrMgr::new_FloatConst(double val)
{
  void* p = mAlloc.get_memory(sizeof(IrFloatConst));
  return new (p) IrFloatConst(val);
}

// @brief 文字列定数を生成する．
// @param[in] val 値
IrExpr*
IrMgr::new_StringConst(const char* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStringConst));
  return new (p) IrStringConst(val);
}

// @brief 変数式を生成する．
// @param[in] var 変数
IrExpr*
IrMgr::new_VarExpr(IrVar* var)
{
  void* p = mAlloc.get_memory(sizeof(IrVarExpr));
  return new (p) IrVarExpr(var);
}

// @brief 配列参照式を生成する．
// @param[in] body 配列を表す式
// @param[in] index インデックス
IrExpr*
IrMgr::new_ArrayRef(IrExpr* body,
		    IrExpr* index)
{
  void* p = mAlloc.get_memory(sizeof(IrArrayRef));
  return new (p) IrArrayRef(body, index);
}

// @brief メンバ参照式を生成する．
// @param[in] obj 対象のオブジェクト
// @param[in] offset メンバのオフセット
IrExpr*
IrMgr::new_MemberRef(IrExpr* obj,
		     ymuint offset)
{
  void* p = mAlloc.get_memory(sizeof(IrMemberRef));
  return new (p) IrMemberRef(obj, offset);
}

// @brief 単項演算式を生成する．
// @param[in] type 型
// @param[in] opr1 オペランド
IrExpr*
IrMgr::new_OpExpr(ExprType type,
		  IrExpr* opr1)
{
  void* p = mAlloc.get_memory(sizeof(IrOpExpr));
  return new (p) IrOpExpr(type, opr1);
}

// @brief 二項演算式を生成する．
// @param[in] type 型
// @param[in] opr1, opr2 オペランド
IrExpr*
IrMgr::new_OpExpr(ExprType type,
		  IrExpr* opr1,
		  IrExpr* opr2)
{
  void* p = mAlloc.get_memory(sizeof(IrOpExpr));
  return new (p) IrOpExpr(type, opr1, opr2);
}

// @brief 三項演算式を生成する．
// @param[in] type 型
// @param[in] opr1, opr2, opr3 オペランド
IrExpr*
IrMgr::new_OpExpr(ExprType type,
		  IrExpr* opr1,
		  IrExpr* opr2,
		  IrExpr* opr3)
{
  void* p = mAlloc.get_memory(sizeof(IrOpExpr));
  return new (p) IrOpExpr(type, opr1, opr2, opr3);
}

// @brief 関数呼び出し式を生成する．
// @param[in] func 関数式
// @param[in] arglist 引数のリスト
IrExpr*
IrMgr::new_FuncCall(IrExpr* func,
		    const vector<IrExpr*>& arglist)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncCall));
  return new (p) IrFuncCall(func, arglist);
}

// @brief スコープ式を生成する．
// @param[in] scope スコープ
IrExpr*
IrMgr::new_ScopeExpr(IrScope* scope)
{
  void* p = mAlloc.get_memory(sizeof(IrScopeExpr));
  return new (p) IrScopeExpr(scope);
}

// @brief 関数式を生成する．
// @param[in] func 関数
IrExpr*
IrMgr::new_FuncExpr(IrFunction* func)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncExpr));
  return new (p) IrFuncExpr(func);
}

// @brief enum 型式を生成する．
// @param[in] type enum 型
IrExpr*
IrMgr::new_EnumExpr(const IrType* type)
{
  void* p = mAlloc.get_memory(sizeof(IrEnumExpr));
  return new (p) IrEnumExpr(type);
}

END_NAMESPACE_YM_YMSL
