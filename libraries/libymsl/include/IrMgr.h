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
  /// @param[in] type_mgr 型を管理するオブジェクト
  IrMgr(TypeMgr& type_mgr);

  /// @brief デストラクタ
  ~IrMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief true 定数を生成する．
  IrNode*
  new_True();

  /// @brief False 定数を生成する．
  IrNode*
  new_False();

  /// @brief 整数値定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_IntConst(int val);

  /// @brief 実数値定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_FloatConst(double val);

  /// @brief 文字列定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_StringConst(const char* val);

  /// @brief 変数参照を生成する．
  IrNode*
  new_VarRef(const Var* var);

  /// @brief 単項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1 オペランド
  IrNode*
  new_UniOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1);

  /// @brief 二項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1, opr2 オペランド
  IrNode*
  new_BinOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1,
	    IrNode* opr2);

  /// @brief 三項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1, opr2, opr3 オペランド
  IrNode*
  new_TriOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1,
	    IrNode* opr2,
	    IrNode* opr3);

  /// @brief load 文を生成する．
  /// @param[in] var 変数
  IrNode*
  new_Load(const Var* var);

  /// @brief store 文を生成する．
  /// @param[in] var 変数
  /// @param[in] val 書き込む値
  IrNode*
  new_Store(const Var* var,
	    IrNode* val);

  /// @brief 配列用の load 文を生成する．
  /// @param[in] array 配列
  /// @param[in] index インデックス
  IrNode*
  new_ArrayLoad(IrNode* array,
		IrNode* index);

  /// @brief 配列用の store 文を生成する．
  /// @param[in] array 配列
  /// @param[in] index インデックス
  /// @param[in] val 書き込む値
  IrNode*
  new_ArrayStore(IrNode* array,
		 IrNode* index,
		 IrNode* val);

  /// @brief クラスメンバ用の load 文を生成する．
  /// @param[in] obj オブジェクト
  /// @param[in] var メンバ変数
  IrNode*
  new_MemberLoad(IrNode* obj,
		 const Var* var);

  /// @brief クラスメンバ用の store 文を生成する．
  /// @param[in] obj オブジェクト
  /// @param[in] var メンバ変数
  /// @param[in] val 書き込む値
  IrNode*
  new_MemberStore(IrNode* obj,
		  const Var* var,
		  IrNode* val);

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] func 関数
  /// @param[in] arglist 引数のリスト
  IrNode*
  new_FuncCall(const Function* func,
	       const vector<IrNode*>& arglist);

  /// @brief リターン命令を生成する．
  /// @param[in] ret_val 返り値
  IrNode*
  new_Return(IrNode* ret_val = NULL);

  /// @brief ジャンプ系のノードを生成する．
  /// @param[in] opcode オペコード
  /// @param[in] label ジャンプ先のラベル
  /// @param[in] cond 条件
  IrNode*
  new_Jump(OpCode opcode,
	   IrNode* label,
	   IrNode* cond = NULL);

  /// @brief ラベルノードを生成する．
  IrNode*
  new_Label();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型を管理するオブジェクト
  TypeMgr& mTypeMgr;

  // メモリアロケータ
  SimpleAlloc mAlloc;

};

END_NAMESPACE_YM_YMSL

#endif // IRMGR_H
