#ifndef VSM_H
#define VSM_H

/// @file Vsm.h
/// @brief Vsm のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "VsmValue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @brief Vsm の命令コード
///
/// スタックマシンを模倣している．
//////////////////////////////////////////////////////////////////////
enum VsmOpcode {
  VSM_NOP,

  VSM_PUSH_INT_IMM,
  VSM_PUSH_FLOAT_IMM,
  VSM_PUSH_FLOAT_ZERO,
  VSM_PUSH_FLOAT_ONE,
  VSM_PUSH_OBJ_NULL,

  VSM_LOAD_GLOBAL_INT,
  VSM_LOAD_GLOBAL_FLOAT,
  VSM_LOAD_GLOBAL_OBJ,

  VSM_LOAD_LOCAL_INT,
  VSM_LOAD_LOCAL_FLOAT,
  VSM_LOAD_LOCAL_OBJ,

  VSM_STORE_GLOBAL_INT,
  VSM_STORE_GLOBAL_FLOAT,
  VSM_STORE_GLOBAL_OBJ,

  VSM_STORE_LOCAL_INT,
  VSM_STORE_LOCAL_FLOAT,
  VSM_STORE_LOCAL_OBJ,

  VSM_INT_MINUS,
  VSM_INT_INC,
  VSM_INT_DEC,
  VSM_INT_NOT,
  VSM_INT_TO_BOOL,
  VSM_INT_TO_FLOAT,

  VSM_INT_ADD,
  VSM_INT_SUB,
  VSM_INT_MUL,
  VSM_INT_DIV,
  VSM_INT_MOD,
  VSM_INT_LSHIFT,
  VSM_INT_RSHIFT,
  VSM_INT_EQ,
  VSM_INT_NE,
  VSM_INT_LT,
  VSM_INT_LE,
  VSM_INT_AND,
  VSM_INT_OR,
  VSM_INT_XOR,

  VSM_INT_ITE,

  VSM_FLOAT_MINUS,
  VSM_FLOAT_TO_BOOL,
  VSM_FLOAT_TO_INT,

  VSM_FLOAT_ADD,
  VSM_FLOAT_SUB,
  VSM_FLOAT_MUL,
  VSM_FLOAT_DIV,
  VSM_FLOAT_EQ,
  VSM_FLOAT_NE,
  VSM_FLOAT_LT,
  VSM_FLOAT_LE,

  VSM_FLOAT_ITE,

  VSM_OBJ_MINUS,
  VSM_OBJ_INC,
  VSM_OBJ_DEC,
  VSM_OBJ_NOT,
  VSM_OBJ_TO_INT,
  VSM_OBJ_TO_FLOAT,

  VSM_OBJ_ADD,
  VSM_OBJ_SUB,
  VSM_OBJ_MUL,
  VSM_OBJ_DIV,
  VSM_OBJ_MOD,
  VSM_OBJ_LSHIFT,
  VSM_OBJ_RSHIFT,
  VSM_OBJ_EQ,
  VSM_OBJ_NE,
  VSM_OBJ_LT,
  VSM_OBJ_LE,
  VSM_OBJ_AND,
  VSM_OBJ_OR,
  VSM_OBJ_XOR,

  VSM_OBJ_ITE,

  VSM_JUMP,
  VSM_JUMP_R,
  VSM_BRANCH_TRUE,
  VSM_BRANCH_FALSE,

  VSM_CALL,
  VSM_CALL_R,
  VSM_RETURN,

  VSM_HALT
};


//////////////////////////////////////////////////////////////////////
/// @class Vsm Vsm.h "Vsm.h"
/// @brief YMSL の VSM(Virtual Stack Machine)
//////////////////////////////////////////////////////////////////////
class Vsm
{
public:

  /// @brief コンストラクタ
  Vsm();

  /// @brief デストラクタ
  ~Vsm();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief バイトコードを実行する．
  /// @param[in] code_list コードの配列
  /// @param[in] base ベースレジスタ
  void
  execute(const VsmCodeList& code_list,
	  Ymsl_INT base);

  /// @brief スタックの内容を読む
  /// @param[in] index インデックス
  VsmValue
  read_stack(Ymsl_INT index);

  /// @brief スタックの内容を書き込む．
  /// @param[in] index インデックス
  /// @param[in] val 値
  void
  write_stack(Ymsl_INT index,
	      VsmValue val);


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

  // 関数テーブルのサイズ
  Ymsl_INT mFuncTableSize;

  // 関数テーブル
  VsmFunction** mFuncTable;

  // グローバル変数領域のサイズ
  Ymsl_INT mGlobalHeapSize;

  // グローバル変数領域
  VsmValue* mGlobalHeap;

  // ローカルスタックのサイズ
  Ymsl_INT mLocalStackSize;

  // ローカルスタック
  VsmValue* mLocalStack;

  // スタックポインタ
  Ymsl_INT mSP;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief スタックの内容を読む
// @param[in] index インデックス
inline
VsmValue
Vsm::read_stack(Ymsl_INT index)
{
  return mLocalStack[index];
}

// @brief スタックの内容を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::write_stack(Ymsl_INT index,
		 VsmValue val)
{
  mLocalStack[index] = val;
}

// @brief INT をプッシュする．
inline
void
Vsm::push_INT(Ymsl_INT val)
{
  mLocalStack[mSP].int_value = val;
  ++ mSP;
}

// @brief FLOAT をプッシュする．
inline
void
Vsm::push_FLOAT(Ymsl_FLOAT val)
{
  mLocalStack[mSP].float_value = val;
  ++ mSP;
}

// @brief OBJPTR をプッシュする．
inline
void
Vsm::push_OBJPTR(Ymsl_OBJPTR val)
{
  mLocalStack[mSP].obj_value = val;
  ++ mSP;
}

// @brief INT をポップする．
inline
Ymsl_INT
Vsm::pop_INT()
{
  -- mSP;
  return mLocalStack[mSP].int_value;
}

// @brief FLOAT をポップする．
inline
Ymsl_FLOAT
Vsm::pop_FLOAT()
{
  -- mSP;
  return mLocalStack[mSP].float_value;
}

// @brief OBJPTR をポップする．
inline
Ymsl_OBJPTR
Vsm::pop_OBJPTR()
{
  -- mSP;
  return mLocalStack[mSP].obj_value;
}

// @brief グローバル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
Vsm::load_global_INT(Ymsl_INT index)
{
  return mGlobalHeap[index].int_value;
}

// @brief グローバル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
Vsm::load_global_FLOAT(Ymsl_INT index)
{
  return mGlobalHeap[index].float_value;
}

// @brief グローバル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
Vsm::load_global_OBJPTR(Ymsl_INT index)
{
  return mGlobalHeap[index].obj_value;
}

// @brief グローバル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_global_INT(Ymsl_INT index,
		      Ymsl_INT val)
{
  mGlobalHeap[index].int_value = val;
}

// @brief グローバル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_global_FLOAT(Ymsl_INT index,
			Ymsl_FLOAT val)
{
  mGlobalHeap[index].float_value = val;
}

// @brief グローバル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_global_OBJPTR(Ymsl_INT index,
			 Ymsl_OBJPTR val)
{
  mGlobalHeap[index].obj_value = val;
}

// @brief ローカル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
Vsm::load_local_INT(Ymsl_INT index)
{
  return mLocalStack[index].int_value;
}

// @brief ローカル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
Vsm::load_local_FLOAT(Ymsl_INT index)
{
  return mLocalStack[index].float_value;
}

// @brief ローカル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
Vsm::load_local_OBJPTR(Ymsl_INT index)
{
  return mLocalStack[index].obj_value;
}

// @brief ローカル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_local_INT(Ymsl_INT index,
		     Ymsl_INT val)
{
  mLocalStack[index].int_value = val;
}

// @brief ローカル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_local_FLOAT(Ymsl_INT index,
		       Ymsl_FLOAT val)
{
  mLocalStack[index].float_value = val;
}

// @brief ローカル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
Vsm::store_local_OBJPTR(Ymsl_INT index,
			Ymsl_OBJPTR val)
{
  mLocalStack[index].obj_value = val;
}

END_NAMESPACE_YM_YMSL

#endif // VSM_H
