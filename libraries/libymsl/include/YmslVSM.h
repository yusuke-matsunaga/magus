#ifndef YMSLVSM_H
#define YMSLVSM_H

/// @file YmslVSM.h
/// @brief YmslVSM のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslValue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @brief YmslVSM の命令コード
///
/// スタックマシンを模倣している．
//////////////////////////////////////////////////////////////////////
enum YmslOpcode {
  YMVSM_NOP,

  YMVSM_PUSH_INT_IMM,
  YMVSM_PUSH_FLOAT_IMM,
  YMVSM_PUSH_FLOAT_ZERO,
  YMVSM_PUSH_FLOAT_ONE,
  YMVSM_PUSH_OBJ_NULL,

  YMVSM_LOAD_GLOBAL_INT,
  YMVSM_LOAD_GLOBAL_FLOAT,
  YMVSM_LOAD_GLOBAL_OBJ,

  YMVSM_LOAD_LOCAL_INT,
  YMVSM_LOAD_LOCAL_FLOAT,
  YMVSM_LOAD_LOCAL_OBJ,

  YMVSM_STORE_GLOBAL_INT,
  YMVSM_STORE_GLOBAL_FLOAT,
  YMVSM_STORE_GLOBAL_OBJ,

  YMVSM_STORE_LOCAL_INT,
  YMVSM_STORE_LOCAL_FLOAT,
  YMVSM_STORE_LOCAL_OBJ,

  YMVSM_INT_MINUS,
  YMVSM_INT_INC,
  YMVSM_INT_DEC,
  YMVSM_INT_NOT,
  YMVSM_INT_TO_BOOL,
  YMVSM_INT_TO_FLOAT,

  YMVSM_INT_ADD,
  YMVSM_INT_SUB,
  YMVSM_INT_MUL,
  YMVSM_INT_DIV,
  YMVSM_INT_MOD,
  YMVSM_INT_LSHIFT,
  YMVSM_INT_RSHIFT,
  YMVSM_INT_EQ,
  YMVSM_INT_NE,
  YMVSM_INT_LT,
  YMVSM_INT_LE,
  YMVSM_INT_AND,
  YMVSM_INT_OR,
  YMVSM_INT_XOR,

  YMVSM_INT_ITE,

  YMVSM_FLOAT_MINUS,
  YMVSM_FLOAT_TO_BOOL,
  YMVSM_FLOAT_TO_INT,

  YMVSM_FLOAT_ADD,
  YMVSM_FLOAT_SUB,
  YMVSM_FLOAT_MUL,
  YMVSM_FLOAT_DIV,
  YMVSM_FLOAT_EQ,
  YMVSM_FLOAT_NE,
  YMVSM_FLOAT_LT,
  YMVSM_FLOAT_LE,

  YMVSM_FLOAT_ITE,

  YMVSM_OBJ_MINUS,
  YMVSM_OBJ_INC,
  YMVSM_OBJ_DEC,
  YMVSM_OBJ_NOT,
  YMVSM_OBJ_TO_INT,
  YMVSM_OBJ_TO_FLOAT,

  YMVSM_OBJ_ADD,
  YMVSM_OBJ_SUB,
  YMVSM_OBJ_MUL,
  YMVSM_OBJ_DIV,
  YMVSM_OBJ_MOD,
  YMVSM_OBJ_LSHIFT,
  YMVSM_OBJ_RSHIFT,
  YMVSM_OBJ_EQ,
  YMVSM_OBJ_NE,
  YMVSM_OBJ_LT,
  YMVSM_OBJ_LE,
  YMVSM_OBJ_AND,
  YMVSM_OBJ_OR,
  YMVSM_OBJ_XOR,

  YMVSM_OBJ_ITE,

  YMVSM_JUMP,
  YMVSM_BRANCH_TRUE,
  YMVSM_BRANCH_FALSE,

  YMVSM_CALL,
  YMVSM_RETURN,

  YMVSM_HALT
};


//////////////////////////////////////////////////////////////////////
/// @class YmslVSM YmslVSM.h "YmslVSM.h"
/// @brief YMSL の VSM(Virtual Stack Machine)
//////////////////////////////////////////////////////////////////////
class YmslVSM
{
public:

  /// @brief コンストラクタ
  YmslVSM();

  /// @brief デストラクタ
  ~YmslVSM();


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
YmslVSM::push_INT(Ymsl_INT val)
{
  mLocalStack[mSP].int_value = val;
  ++ mSP;
}

// @brief FLOAT をプッシュする．
inline
void
YmslVSM::push_FLOAT(Ymsl_FLOAT val)
{
  mLocalStack[mSP].float_value = val;
  ++ mSP;
}

// @brief OBJPTR をプッシュする．
inline
void
YmslVSM::push_OBJPTR(Ymsl_OBJPTR val)
{
  mLocalStack[mSP].obj_value = val;
  ++ mSP;
}

// @brief INT をポップする．
inline
Ymsl_INT
YmslVSM::pop_INT()
{
  -- mSP;
  return mLocalStack[mSP].int_value;
}

// @brief FLOAT をポップする．
inline
Ymsl_FLOAT
YmslVSM::pop_FLOAT()
{
  -- mSP;
  return mLocalStack[mSP].float_value;
}

// @brief OBJPTR をポップする．
inline
Ymsl_OBJPTR
YmslVSM::pop_OBJPTR()
{
  -- mSP;
  return mLocalStack[mSP].obj_value;
}

// @brief グローバル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
YmslVSM::load_global_INT(Ymsl_INT index)
{
  return mGlobalHeap[index].int_value;
}

// @brief グローバル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
YmslVSM::load_global_FLOAT(Ymsl_INT index)
{
  return mGlobalHeap[index].float_value;
}

// @brief グローバル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
YmslVSM::load_global_OBJPTR(Ymsl_INT index)
{
  return mGlobalHeap[index].obj_value;
}

// @brief グローバル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_global_INT(Ymsl_INT index,
			 Ymsl_INT val)
{
  mGlobalHeap[index].int_value = val;
}

// @brief グローバル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_global_FLOAT(Ymsl_INT index,
			   Ymsl_FLOAT val)
{
  mGlobalHeap[index].float_value = val;
}

// @brief グローバル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_global_OBJPTR(Ymsl_INT index,
			    Ymsl_OBJPTR val)
{
  mGlobalHeap[index].obj_value = val;
}

// @brief ローカル変数の INT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_INT
YmslVSM::load_local_INT(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].int_value;
}

// @brief ローカル変数の FLOAT の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_FLOAT
YmslVSM::load_local_FLOAT(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].float_value;
}

// @brief ローカル変数の OBJ の値を取り出す．
// @param[in] index インデックス
inline
Ymsl_OBJPTR
YmslVSM::load_local_OBJPTR(Ymsl_INT index)
{
  return mLocalStack[mBASE - index].obj_value;
}

// @brief ローカル変数に INT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_local_INT(Ymsl_INT index,
			Ymsl_INT val)
{
  mLocalStack[mBASE - index].int_value = val;
}

// @brief ローカル変数に FLOAT の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_local_FLOAT(Ymsl_INT index,
			  Ymsl_FLOAT val)
{
  mLocalStack[mBASE - index].float_value = val;
}

// @brief ローカル変数に OBJ の値を書き込む．
// @param[in] index インデックス
// @param[in] val 値
inline
void
YmslVSM::store_local_OBJPTR(Ymsl_INT index,
			   Ymsl_OBJPTR val)
{
  mLocalStack[mBASE - index].obj_value = val;
}

END_NAMESPACE_YM_YMSL

#endif // YMSLVSM_H
