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
/// @brief YmslVM の命令コード
//////////////////////////////////////////////////////////////////////
enum YmslOpcode {
  YMVM_NOP,

  YMVM_PUSH_INT_IMM,
  YMVM_PUSH_FLOAT_IMM,
  YMVM_PUSH_FLOAT_ZERO,
  YMVM_PUSH_FLOAT_ONE,
  YMVM_PUSH_OBJ_NULL,

  YMVM_LOAD_GLOBAL_INT,
  YMVM_LOAD_GLOBAL_FLOAT,
  YMVM_LOAD_GLOBAL_OBJ,

  YMVM_LOAD_LOCAL_INT,
  YMVM_LOAD_LOCAL_FLOAT,
  YMVM_LOAD_LOCAL_OBJ,

  YMVM_STORE_GLOBAL_INT,
  YMVM_STORE_GLOBAL_FLOAT,
  YMVM_STORE_GLOBAL_OBJ,

  YMVM_STORE_LOCAL_INT,
  YMVM_STORE_LOCAL_FLOAT,
  YMVM_STORE_LOCAL_OBJ,

  YMVM_JUMP,
  YMVM_BRANCH_TRUE,
  YMVM_BRANCH_FALSE,

  YMVM_CALL,
  YMVM_RETURN,

  YMVM_INT_MINUS,
  YMVM_INT_INC,
  YMVM_INT_DEC,
  YMVM_INT_NOT,
  YMVM_INT_TO_BOOL,
  YMVM_INT_TO_FLOAT,

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
  YMVM_INT_AND,
  YMVM_INT_OR,
  YMVM_INT_XOR,

  YMVM_INT_ITE,

  YMVM_FLOAT_MINUS,
  YMVM_FLOAT_TO_BOOL,
  YMVM_FLOAT_TO_INT,

  YMVM_FLOAT_ADD,
  YMVM_FLOAT_SUB,
  YMVM_FLOAT_MUL,
  YMVM_FLOAT_DIV,
  YMVM_FLOAT_EQ,
  YMVM_FLOAT_NE,
  YMVM_FLOAT_LT,
  YMVM_FLOAT_LE,
  YMVM_FLOAT_GT,
  YMVM_FLOAT_GE,

  YMVM_FLOAT_ITE,

  YMVM_OBJ_MINUS,
  YMVM_OBJ_INC,
  YMVM_OBJ_DEC,
  YMVM_OBJ_NOT,
  YMVM_OBJ_TO_INT,
  YMVM_OBJ_TO_FLOAT,

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
  YMVM_OBJ_AND,
  YMVM_OBJ_OR,
  YMVM_OBJ_XOR,

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
  /// @param[in] code_list コードの配列
  void
  execute(const YmslCodeList& code_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief INT をプッシュする．
  void
  push_INT(Ymsl_INT val);

  /// @brief FLOAT をプッシュする．
  void
  push_FLOAT(Ymsl_FLOAT val);

  /// @brief OBJ をプッシュする．
  void
  push_OBJPTR(Ymsl_OBJPTR val);

  /// @brief INT をポップする．
  Ymsl_INT
  pop_INT();

  /// @brief FLOAT をポップする．
  Ymsl_FLOAT
  pop_FLOAT();

  /// @brief OBJ をポップする．
  Ymsl_OBJPTR
  pop_OBJPTR();

  /// @brief グローバル変数の INT の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_INT
  load_global_INT(Ymsl_INT index);

  /// @brief グローバル変数の FLOAT の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_FLOAT
  load_global_FLOAT(Ymsl_INT index);

  /// @brief グローバル変数の OBJ の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_OBJPTR
  load_global_OBJPTR(Ymsl_INT index);

  /// @brief グローバル変数に INT の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_global_INT(Ymsl_INT index,
		   Ymsl_INT val);

  /// @brief グローバル変数に FLOAT の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_global_FLOAT(Ymsl_INT index,
		     Ymsl_FLOAT val);

  /// @brief グローバル変数に OBJ の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_global_OBJPTR(Ymsl_INT index,
		      Ymsl_OBJPTR val);

  /// @brief ローカル変数の INT の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_INT
  load_local_INT(Ymsl_INT index);

  /// @brief ローカル変数の FLOAT の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_FLOAT
  load_local_FLOAT(Ymsl_INT index);

  /// @brief ローカル変数の OBJ の値を取り出す．
  /// @param[in] index インデックス
  Ymsl_OBJPTR
  load_local_OBJPTR(Ymsl_INT index);

  /// @brief ローカル変数に INT の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_local_INT(Ymsl_INT index,
		  Ymsl_INT val);

  /// @brief ローカル変数に FLOAT の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_local_FLOAT(Ymsl_INT index,
		    Ymsl_FLOAT val);

  /// @brief ローカル変数に OBJ の値を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  store_local_OBJPTR(Ymsl_INT index,
		     Ymsl_OBJPTR val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバル変数領域のサイズ
  Ymsl_INT mGlobalHeapSize;

  // グローバル変数領域
  YmslValue* mGlobalHeap;

  // ローカルスタックのサイズ
  Ymsl_INT mLocalStackSize;

  // ローカルスタック
  YmslValue* mLocalStack;

  // スタックポインタ
  Ymsl_INT mSP;

  // base レジスタ
  Ymsl_INT mBASE;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief INT をプッシュする．
inline
void
YmslVM::push_INT(Ymsl_INT val)
{
  mLocalStack[mSP].int_value = val;
  ++ mSP;
}

// @brief FLOAT をプッシュする．
inline
void
YmslVM::push_FLOAT(Ymsl_FLOAT val)
{
  mLocalStack[mSP].float_value = val;
  ++ mSP;
}

// @brief OBJPTR をプッシュする．
inline
void
YmslVM::push_OBJPTR(Ymsl_OBJPTR val)
{
  mLocalStack[mSP].obj_value = val;
  ++ mSP;
}

// @brief INT をポップする．
inline
Ymsl_INT
YmslVM::pop_INT()
{
  -- mSP;
  return mLocalStack[mSP].int_value;
}

// @brief FLOAT をポップする．
inline
Ymsl_FLOAT
YmslVM::pop_FLOAT()
{
  -- mSP;
  return mLocalStack[mSP].float_value;
}

// @brief OBJPTR をポップする．
inline
Ymsl_OBJPTR
YmslVM::pop_OBJPTR()
{
  -- mSP;
  return mLocalStack[mSP].obj_value;
}

// @brief グローバル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
YmslVM::load_global_INT(Ymsl_INT index)
{
  return mGlobalHeap[index].int_value;
}

// @brief グローバル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
YmslVM::load_global_FLOAT(Ymsl_INT index)
{
  return mGlobalHeap[index].float_value;
}

// @brief グローバル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
YmslVM::load_global_OBJPTR(Ymsl_INT index)
{
  return mGlobalHeap[index].obj_value;
}

// @brief グローバル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_global_INT(Ymsl_INT index,
			 Ymsl_INT val)
{
  mGlobalHeap[index].int_value = val;
}

// @brief グローバル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_global_FLOAT(Ymsl_INT index,
			   Ymsl_FLOAT val)
{
  mGlobalHeap[index].float_value = val;
}

// @brief グローバル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_global_OBJPTR(Ymsl_INT index,
			    Ymsl_OBJPTR val)
{
  mGlobalHeap[index].obj_value = val;
}

// @brief ローカル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
YmslVM::load_local_INT(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].int_value;
}

// @brief ローカル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
YmslVM::load_local_FLOAT(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].float_value;
}

// @brief ローカル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
YmslVM::load_local_OBJPTR(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].obj_value;
}

// @brief ローカル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_local_INT(Ymsl_INT index,
			Ymsl_INT val)
{
  mLocalStack[mBASE - index].int_value = val;
}

// @brief ローカル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_local_FLOAT(Ymsl_INT index,
			  Ymsl_FLOAT val)
{
  mLocalStack[mBASE - index].float_value = val;
}

// @brief ローカル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVM::store_local_OBJPTR(Ymsl_INT index,
			   Ymsl_OBJPTR val)
{
  mLocalStack[mBASE - index].obj_value = val;
}

END_NAMESPACE_YM_YMSL

#endif // YMSLVM_H
