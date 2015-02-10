
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"
#include "BooleanConst.h"
#include "IntConst.h"
#include "FloatConst.h"
#include "StringConst.h"
#include "IrUniOp.h"
#include "IrBinOp.h"
#include "IrTriOp.h"
#include "IrLoad.h"
#include "IrStore.h"
#include "IrInplaceUniOp.h"
#include "IrInplaceBinOp.h"
#include "IrFuncCall.h"
#include "IrReturn.h"
#include "IrJump.h"
#include "IrLabel.h"

#include "IrConstHandle.h"
#include "IrFuncHandle.h"
#include "IrLabelHandle.h"
#include "IrScopeHandle.h"
#include "IrTypeHandle.h"
#include "IrVarHandle.h"
#include "IrArrayRef.h"
#include "IrMemberRef.h"

#include "Var.h"
#include "Function.h"
#include "ConstVal.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief true 定数を生成する．
const ConstVal*
IrMgr::new_True()
{
  void* p = mAlloc.get_memory(sizeof(BooleanConst));
  return new (p) BooleanConst(mTypeMgr.boolean_type(), true);
}

// @brief False 定数を生成する．
const ConstVal*
IrMgr::new_False()
{
  void* p = mAlloc.get_memory(sizeof(BooleanConst));
  return new (p) BooleanConst(mTypeMgr.boolean_type(), false);
}

// @brief 整数値定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_IntConst(int val)
{
  void* p = mAlloc.get_memory(sizeof(IntConst));
  return new (p) IntConst(mTypeMgr.int_type(), val);
}

// @brief 実数値定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_FloatConst(double val)
{
  void* p = mAlloc.get_memory(sizeof(FloatConst));
  return new (p) FloatConst(mTypeMgr.float_type(), val);
}

// @brief 文字列定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_StringConst(const char* val)
{
  void* p = mAlloc.get_memory(sizeof(StringConst));
  return new (p) StringConst(mTypeMgr.string_type(), val);
}

// @brief 定数参照式を生成する．
// @param[in] const_val 定数値
IrNode*
IrMgr::new_ConstNode(const ConstVal* const_val)
{
  IrHandle* h = new_ConstHandle(ShString(), const_val);
  return new_Load(h);
}

// @brief 単項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1 オペランド
IrNode*
IrMgr::new_UniOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1)
{
  void* p = mAlloc.get_memory(sizeof(IrUniOp));
  return new (p) IrUniOp(opcode, type, opr1);
}

// @brief 二項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1, opr2 オペランド
IrNode*
IrMgr::new_BinOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1,
		 IrNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(IrBinOp));
  return new (p) IrBinOp(opcode, type, opr1, opr2);
}

// @brief 三項演算式を生成する．
// @param[in] opcode オペコード
// @param[in] type 出力の型
// @param[in] opr1, opr2, opr3 オペランド
IrNode*
IrMgr::new_TriOp(OpCode opcode,
		 const Type* type,
		 IrNode* opr1,
		 IrNode* opr2,
		 IrNode* opr3)
{
  void* p = mAlloc.get_memory(sizeof(IrTriOp));
  return new (p) IrTriOp(opcode, type, opr1, opr2, opr3);
}

// @brief load 文を生成する．
// @param[in] addr アドレス
IrNode*
IrMgr::new_Load(IrHandle* addr)
{
  const Type* type = NULL;
  switch ( addr->handle_type() ) {
  case IrHandle::kVar:
  case IrHandle::kMemberRef:
    type = addr->var()->value_type();
    break;

  case IrHandle::kFunction:
  case IrHandle::kMethodRef:
    type = addr->function()->type();
    break;

  case IrHandle::kConstant:
    type = addr->constant()->value_type();
    break;

  case IrHandle::kArrayRef:
    type = addr->array_expr()->value_type()->elem_type();
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }
  void* p = mAlloc.get_memory(sizeof(IrLoad));
  return new (p) IrLoad(type, addr);
}

// @brief store 文を生成する．
// @param[in] addr アドレス
// @param[in] val 書き込む値
IrNode*
IrMgr::new_Store(IrHandle* addr,
		 IrNode* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStore));
  return new (p) IrStore(addr, val);
}

// @brief 自己代入型の単項演算を生成する．
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺式
IrNode*
IrMgr::new_InplaceUniOp(OpCode opcode,
			IrHandle* lhs_addr)
{
  void* p = mAlloc.get_memory(sizeof(IrInplaceUniOp));
  return new (p) IrInplaceUniOp(opcode, lhs_addr);
}

// @brief 自己代入型の二項演算を生成する．
// @param[in] opcode オペコード
// @param[in] lhs_addr 左辺式
// @param[in] opr オペランド
IrNode*
IrMgr::new_InplaceBinOp(OpCode opcode,
			IrHandle* lhs_addr,
			IrNode* opr)
{
  void* p = mAlloc.get_memory(sizeof(IrInplaceBinOp));
  return new (p) IrInplaceBinOp(opcode, lhs_addr, opr);
}

// @brief 関数呼び出し式を生成する．
// @param[in] func_addr 関数アドレス
// @param[in] arglist 引数のリスト
IrNode*
IrMgr::new_FuncCall(IrHandle* func_addr,
		    const vector<IrNode*>& arglist)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncCall));
  return new (p) IrFuncCall(func_addr, arglist);
}

// @brief リターン命令を生成する．
// @param[in] ret_val 返り値
IrNode*
IrMgr::new_Return(IrNode* ret_val)
{
  void* p = mAlloc.get_memory(sizeof(IrReturn));
  return new (p) IrReturn(ret_val);
}

// @brief ジャンプノードを生成する．
// @param[in] label ジャンプ先のラベル
IrNode*
IrMgr::new_Jump(IrNode* label)
{
  void* p = mAlloc.get_memory(sizeof(IrJump));
  return new (p) IrJump(IrNode::kJump, label, NULL);
}

// @brief 分岐ノードを生成する．
// @param[in] label ジャンプ先のラベル
// @param[in] cond 条件
IrNode*
IrMgr::new_BranchTrue(IrNode* label,
		      IrNode* cond)
{
  void* p = mAlloc.get_memory(sizeof(IrJump));
  return new (p) IrJump(IrNode::kBranchTrue, label, cond);
}

// @brief 分岐ノードを生成する．
// @param[in] label ジャンプ先のラベル
// @param[in] cond 条件
IrNode*
IrMgr::new_BranchFalse(IrNode* label,
		       IrNode* cond)
{
  void* p = mAlloc.get_memory(sizeof(IrJump));
  return new (p) IrJump(IrNode::kBranchFalse, label, cond);
}

// @brief ラベルノードを生成する．
IrNode*
IrMgr::new_Label()
{
  void* p = mAlloc.get_memory(sizeof(IrLabel));
  return new (p) IrLabel();
}

// @brief スコープ参照を生成する．
// @param[in] scope スコープ
IrHandle*
IrMgr::new_ScopeHandle(Scope* scope)
{
  void* p = mAlloc.get_memory(sizeof(IrScopeHandle));
  return new (p) IrScopeHandle(scope);
}

// @brief 変数参照を生成する．
// @param[in] var 変数
IrHandle*
IrMgr::new_VarHandle(const Var* var)
{
  void* p = mAlloc.get_memory(sizeof(IrVarHandle));
  return new (p) IrVarHandle(var);
}

// @brief 関数参照を生成する．
// @param[in] func 関数
IrHandle*
IrMgr::new_FuncHandle(const Function* func)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncHandle));
  return new (p) IrFuncHandle(func);
}

// @brief 定数参照を生成する．
// @param[in] name 名前
// @param[in] const_val 定数値
IrHandle*
IrMgr::new_ConstHandle(ShString name,
		       const ConstVal* const_val)
{
  void* p = mAlloc.get_memory(sizeof(IrConstHandle));
  return new (p) IrConstHandle(name, const_val);
}

// @brief ラベル参照を生成する．
// @param[in] name 名前
// @param[in] label ラベル
IrHandle*
IrMgr::new_LabelHandle(ShString name,
		       IrNode* label)
{
  void* p = mAlloc.get_memory(sizeof(IrLabelHandle));
  return new (p) IrLabelHandle(name, label);
}

// @brief 名前付き型参照を生成する．
// @param[in] type 型
IrHandle*
IrMgr::new_TypeHandle(const Type* type,
		      Scope* scope)
{
  void* p = mAlloc.get_memory(sizeof(IrTypeHandle));
  return new (p) IrTypeHandle(type, scope);
}

// @brief 配列参照を生成する．
// @param[in] array 配列
// @param[in] index インデックス
IrHandle*
IrMgr::new_ArrayRef(IrNode* array,
		    IrNode* index)
{
  void* p = mAlloc.get_memory(sizeof(IrArrayRef));
  return new (p) IrArrayRef(array, index);
}

// @brief クラスメンバ参照を生成する．
// @param[in] obj オブジェクト
// @param[in] var メンバ変数
IrHandle*
IrMgr::new_MemberRef(IrNode* obj,
		     const Var* var)
{
  void* p = mAlloc.get_memory(sizeof(IrMemberRef));
  return new (p) IrMemberRef(obj, var);
}

END_NAMESPACE_YM_YMSL
