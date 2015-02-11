
/// @file IrMgr_handle.cc
/// @brief IrMgr の実装ファイル(IrHandle生成関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"

#include "handle/IrConstHandle.h"
#include "handle/IrFuncHandle.h"
#include "handle/IrLabelHandle.h"
#include "handle/IrScopeHandle.h"
#include "handle/IrTypeHandle.h"
#include "handle/IrVarHandle.h"
#include "handle/IrArrayRef.h"
#include "handle/IrMemberRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

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
