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

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] func 関数
  /// @param[in] arglist 引数のリスト
  IrNode*
  new_FuncCall(const Function* func,
	       const vector<IrNode*>& arglist);

  /// @brief ジャンプ系のノードを生成する．
  /// @param[in] opcode オペコード
  IrNode*
  new_Jump(OpCode opcode);


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
