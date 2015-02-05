
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"
#include "IrTrue.h"
#include "IrFalse.h"
#include "IrIntConst.h"
#include "IrFloatConst.h"
#include "IrStringConst.h"
#include "IrVarRef.h"
#include "IrLoad.h"
#include "IrStore.h"
#include "IrUniOp.h"
#include "IrBinOp.h"
#include "IrTriOp.h"
#include "IrFuncCall.h"
#include "IrReturn.h"
#include "IrJump.h"
#include "IrLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type_mgr 型を管理するオブジェクト
IrMgr::IrMgr(TypeMgr& type_mgr) :
  mTypeMgr(type_mgr)
{
}

// @brief デストラクタ
IrMgr::~IrMgr()
{
}

// @brief クリアする．
void
IrMgr::clear()
{
  mAlloc.destroy();
}

// @brief true 定数を生成する．
IrNode*
IrMgr::new_True()
{
  void* p = mAlloc.get_memory(sizeof(IrTrue));
  return new (p) IrTrue(mTypeMgr.boolean_type());
}

// @brief False 定数を生成する．
IrNode*
IrMgr::new_False()
{
  void* p = mAlloc.get_memory(sizeof(IrFalse));
  return new (p) IrFalse(mTypeMgr.boolean_type());
}

// @brief 整数値定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_IntConst(int val)
{
  void* p = mAlloc.get_memory(sizeof(IrIntConst));
  return new (p) IrIntConst(mTypeMgr.int_type(), val);
}

// @brief 実数値定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_FloatConst(double val)
{
  void* p = mAlloc.get_memory(sizeof(IrFloatConst));
  return new (p) IrFloatConst(mTypeMgr.float_type(), val);
}

// @brief 文字列定数を生成する．
// @param[in] val 値
IrNode*
IrMgr::new_StringConst(const char* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStringConst));
  return new (p) IrStringConst(mTypeMgr.string_type(), val);
}

// @brief 変数参照を生成する．
IrNode*
IrMgr::new_VarRef(const Var* var)
{
  void* p = mAlloc.get_memory(sizeof(IrVarRef));
  return new (p) IrVarRef(var);
}

// @brief ロード命令を生成する．
// @param[in] type 型
// @param[in] base ベースアドレス
// @param[in] offset オフセット
//
// offset は int 型でなければならない．
IrNode*
IrMgr::new_Load(const Type* type,
		IrNode* base,
		IrNode* offset)
{
  void* p = mAlloc.get_memory(sizeof(IrLoad));
  return new (p) IrLoad(type, base, offset);
}

// @brief ストア命令を生成する．
// @param[in] base ベースアドレス
// @param[in] offset オフセット
// @param[in] val 値
//
// base は array 型でなければならない．
// offset は int 型でなければならない．
// val の型と base の要素型が等しくなければならない．
IrNode*
IrMgr::new_Store(IrNode* base,
		 IrNode* offset,
		 IrNode* val)
{
  void* p = mAlloc.get_memory(sizeof(IrStore));
  return new (p) IrStore(base, offset, val);
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

// @brief 関数呼び出し式を生成する．
// @param[in] func 関数式
// @param[in] arglist 引数のリスト
IrNode*
IrMgr::new_FuncCall(const Function* func,
		    const vector<IrNode*>& arglist)
{
  void* p = mAlloc.get_memory(sizeof(IrFuncCall));
  return new (p) IrFuncCall(func, arglist);
}

// @brief リターン命令を生成する．
// @param[in] ret_val 返り値
IrNode*
IrMgr::new_Return(IrNode* ret_val)
{
  void* p = mAlloc.get_memory(sizeof(IrReturn));
  return new (p) IrReturn(ret_val);
}

// @brief ジャンプ系のノードを生成する．
// @param[in] opcode オペコード
// @param[in] label ジャンプ先のラベル
// @param[in] cond 条件
IrNode*
IrMgr::new_Jump(OpCode opcode,
		IrNode* label,
		IrNode* cond)
{
  void* p = mAlloc.get_memory(sizeof(IrJump));
  return new (p) IrJump(opcode, label, cond);
}

// @brief ラベルノードを生成する．
IrNode*
IrMgr::new_Label()
{
  void* p = mAlloc.get_memory(sizeof(IrLabel));
  return new (p) IrLabel();
}

END_NAMESPACE_YM_YMSL
