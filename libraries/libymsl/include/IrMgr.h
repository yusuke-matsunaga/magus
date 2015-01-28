#ifndef IRMGR_H
#define IRMGR_H

/// @file IrMgr.h
/// @brief IrMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMgr IrMgr.h "IrMgr.h"
/// @brief IR オブジェクトを管理するクラス
//////////////////////////////////////////////////////////////////////
class IrMgr
{
public:

  /// @brief コンストラクタ
  IrMgr();

  /// @brief デストラクタ
  ~IrMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープを生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] name 名前
  Scope*
  new_Scope(Scope* parent,
	    ShString name = ShString());

  /// @brief 変数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 型
  /// @param[in] index インデックス
  Var*
  new_Var(ShString name,
	  const Type* type,
	  ymuint index);

  /// @brief 関数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  /// @param[in] index インデックス
  Function*
  new_Function(ShString name,
	       const Type* type,
	       const vector<const Type*>& input_type_list,
	       ymuint index);

  /// @brief true 定数を生成する．
  IrExpr*
  new_True();

  /// @brief False 定数を生成する．
  IrExpr*
  new_False();

  /// @brief 整数値定数を生成する．
  /// @param[in] val 値
  IrExpr*
  new_IntConst(int val);

  /// @brief 実数値定数を生成する．
  /// @param[in] val 値
  IrExpr*
  new_FloatConst(double val);

  /// @brief 文字列定数を生成する．
  /// @param[in] val 値
  IrExpr*
  new_StringConst(const char* val);

  /// @brief 変数式を生成する．
  /// @param[in] var 変数
  IrExpr*
  new_VarExpr(Var* var);

  /// @brief 配列参照式を生成する．
  /// @param[in] body 配列を表す式
  /// @param[in] index インデックス
  IrExpr*
  new_ArrayRef(IrExpr* body,
	       IrExpr* index);

  /// @brief メンバ参照式を生成する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] offset メンバのオフセット
  IrExpr*
  new_MemberRef(IrExpr* obj,
		ymuint offset);

  /// @brief 単項演算式を生成する．
  /// @param[in] type 型
  /// @param[in] opr1 オペランド
  IrExpr*
  new_OpExpr(ExprType type,
	     IrExpr* opr1);

  /// @brief 二項演算式を生成する．
  /// @param[in] type 型
  /// @param[in] opr1, opr2 オペランド
  IrExpr*
  new_OpExpr(ExprType type,
	     IrExpr* opr1,
	     IrExpr* opr2);

  /// @brief 三項演算式を生成する．
  /// @param[in] type 型
  /// @param[in] opr1, opr2, opr3 オペランド
  IrExpr*
  new_OpExpr(ExprType type,
	     IrExpr* opr1,
	     IrExpr* opr2,
	     IrExpr* opr3);

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] func 関数式
  /// @param[in] arglist 引数のリスト
  IrExpr*
  new_FuncCall(IrExpr* func,
	       const vector<IrExpr*>& arglist);

  /// @brief スコープ式を生成する．
  /// @param[in] scope スコープ
  IrExpr*
  new_ScopeExpr(Scope* scope);

  /// @brief 関数式を生成する．
  /// @param[in] func 関数
  IrExpr*
  new_FuncExpr(Function* func);

  /// @brief enum 型式を生成する．
  /// @param[in] type enum 型
  IrExpr*
  new_EnumExpr(const Type* type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

};

END_NAMESPACE_YM_YMSL

#endif // IRMGR_H
