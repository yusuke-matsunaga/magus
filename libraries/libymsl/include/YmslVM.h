#ifndef YMSLVM_H
#define YMSLVM_H

/// @file YmslVM.h
/// @brief YmslVM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL


//////////////////////////////////////////////////////////////////////
/// @brief YmslVM の命令
//////////////////////////////////////////////////////////////////////
enum YmslInst {
  YMVM_PUSH_INT8_IMM,
  YMVM_PUSH_INT16_IMM,
  YMVM_PUSH_INT32_IMM,
  YMVM_PUSH_DOUBLE_IMM,
  YMVM_PUSH_INT_ZERO,
  YMVM_PUSH_INT_ONE,
  YMVM_PUSH_DOUBLE_ZERO,
  YMVM_PUSH_DOUBLE_ONE,
  YMVM_PUSH_OBJ_NULL,

  YMVM_LOAD_INT_NEAR,
  YMVM_LOAD_INT_FAR,
  YMVM_LOAD_DOUBLE_NEAR,
  YMVM_LOAD_DOUBLE_FAR,
  YMVM_LOAD_OBJ_NEAR,
  YMVM_LOAD_OBJ_FAR,

  YMVM_STORE_INT_NEAR,
  YMVM_STORE_INT_FAR,
  YMVM_STORE_DOUBLE_NEAR,
  YMVM_STORE_DOUBLE_FAR,
  YMVM_STORE_OBJ_NEAR,
  YMVM_STORE_OBJ_FAR,

  YMVM_JUMP_NEAR,
  YMVM_JUMP_FAR,
  YMVM_BRANCH_TRUE_NEAR,
  YMVM_BRANCH_TRUE_FAR,
  YMVM_BRANCH_FALSE_NEAR,
  YMVM_BRANCH_FALSE_FAR,

  YMVM_CALL_NEAR,
  YMVM_CALL_FAR,
  YMVM_RETURN,

  YMVM_INT_MINUS,
  YMVM_INT_INC,
  YMVM_INT_DEC,
  YMVM_INT_BITNEG,
  YMVM_INT_LOGNOT,
  YMVM_INT_TO_DOUBLE,

  YMVM_INT_ADD,
  YMVM_INT_SUB,
  YMVM_INT_MUL,
  YMVM_INT_DIV,
  YMVM_INT_MOD,
  YMVM_INT_LSHIFT,
  YMVM_INT_RSHIFT,
  YMVM_INT_EQ,
  YMVM_INT_NE,
  YMVM_INT_LT,
  YMVM_INT_LE,
  YMVM_INT_GT,
  YMVM_INT_GE,
  YMVM_INT_BITAND,
  YMVM_INT_BITOR,
  YMVM_INT_BITXOR,
  YMVM_INT_LOGAND,
  YMVM_INT_LOGOR,
  YMVM_INT_LOGXOR,

  YMVM_INT_ITE,

  YMVM_DOUBLE_MINUS,
  YMVM_DOUBLE_TO_INT,

  YMVM_DOUBLE_ADD,
  YMVM_DOUBLE_SUB,
  YMVM_DOUBLE_MUL,
  YMVM_DOUBLE_DIV,
  YMVM_DOUBLE_EQ,
  YMVM_DOUBLE_NE,
  YMVM_DOUBLE_LT,
  YMVM_DOUBLE_LE,
  YMVM_DOUBLE_GT,
  YMVM_DOUBLE_GE,

  YMVM_DOUBLE_ITE,

  YMVM_OBJ_MINUS,
  YMVM_OBJ_INC,
  YMVM_OBJ_DEC,
  YMVM_OBJ_BITNEG,
  YMVM_OBJ_TO_INT,
  YMVM_OBJ_TO_DOUBLE,

  YMVM_OBJ_ADD,
  YMVM_OBJ_SUB,
  YMVM_OBJ_MUL,
  YMVM_OBJ_DIV,
  YMVM_OBJ_MOD,
  YMVM_OBJ_LSHIFT,
  YMVM_OBJ_RSHIFT,
  YMVM_OBJ_EQ,
  YMVM_OBJ_NE,
  YMVM_OBJ_LT,
  YMVM_OBJ_LE,
  YMVM_OBJ_GT,
  YMVM_OBJ_GE,
  YMVM_OBJ_BITAND,
  YMVM_OBJ_BITOR,
  YMVM_OBJ_BITXOR,

  YMVM_OBJ_ITE,
};


//////////////////////////////////////////////////////////////////////
/// @class YmslVM YmslVM.h "YmslVM.h"
/// @brief YMSL の VM(virtual machine)
//////////////////////////////////////////////////////////////////////
class YmslVM
{
public:

  /// @brief コンストラクタ
  YmslVM();

  /// @brief デストラクタ
  ~YmslVM();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief バイトコードを実行する．
  void
  execute(ymuint code_size,
	  const ymuint8* byte_codes);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief INT をプッシュする．
  void
  push_int(ymint32 val);

  /// @brief DOUBLE をプッシュする．
  void
  push_double(double val);

  /// @brief OBJ をプッシュする．
  void
  push_obj(YmslObj* val);

  /// @brief INT をポップする．
  ymint32
  pop_int();

  /// @brief DOUBLE をポップする．
  double
  pop_double();

  /// @brief OBJ をポップする．
  YmslObj*
  pop_obj();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバル変数領域
  vector<YmslValue> mGlobalHeap;

  // 値のスタック
  vector<YmslValue> mLocalStack;

  // base レジスタ
  ymuint mBase;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief INT をプッシュする．
inline
void
YmslVM::push_int(ymint32 val)
{
  mLocalStack.push_back(val);
}

// @brief DOUBLE をプッシュする．
inline
void
YmslVM::push_double(double val)
{
  mLocalStack.push_back(val);
}

// @brief OBJ をプッシュする．
inline
void
YmslVM::push_obj(YmslObj* val)
{
  mLocalStack.push_back(val);
}

// @brief INT をポップする．
inline
ymint32
YmslVM::pop_int()
{
  ymint32 val = mLocalStack.back().int_value;
  mLocalStack.pop_back();
  return val;
}

// @brief DOUBLE をポップする．
inline
double
YmslVM::pop_double()
{
  double val = mLocalStack.back().double_value;
  mLocalStack.pop_back();
  return val;
}

// @brief OBJ をポップする．
inline
YmslObj*
YmslVM::pop_obj()
{
  YmslObj* val = mLocalStack.back().obj_value;
  mLocalStack.pop_back();
  reutrn val;
}

END_NAMESPACE_YM_YMSL


#endif // YMSLVM_H
