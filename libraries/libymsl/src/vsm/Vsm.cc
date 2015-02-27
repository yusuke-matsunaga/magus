
/// @file Vsm.cc
/// @brief Vsm の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Vsm.h"
#include "VsmCodeList.h"
#include "VsmFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス Vsm
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Vsm::Vsm()
{
  mFuncTableSize = 0;
  mFuncTable = NULL;

  mGlobalHeapSize = 0;
  mGlobalHeap = NULL;

  mLocalStackSize = 0;
  mLocalStack = NULL;

  mSP = 0;
}

// @brief デストラクタ
Vsm::~Vsm()
{
  delete [] mFuncTable;
  delete [] mGlobalHeap;
  delete [] mLocalStack;
}

// @brief バイトコードを実行する．
// @param[in] code_list コードの配列
// @param[in] base ベースレジスタ
void
Vsm::execute(const VsmCodeList& code_list,
	     Ymsl_INT base)
{
  for (Ymsl_INT pc = 0; pc < code_list.size(); ) {
    Ymsl_CODE code = code_list.read_opcode(pc);

    switch ( code ) {
    case VSM_PUSH_INT_IMM:
      {
	Ymsl_INT val = code_list.read_int(pc);
	push_INT(val);
      }
      break;

    case VSM_PUSH_FLOAT_IMM:
      {
	Ymsl_FLOAT val = code_list.read_float(pc);
	push_FLOAT(val);
      }
      break;

    case VSM_PUSH_FLOAT_ZERO:
      push_FLOAT(0.0);
      break;

    case VSM_PUSH_FLOAT_ONE:
      push_FLOAT(1.0);
      break;

    case VSM_PUSH_OBJ_NULL:
      push_OBJPTR(NULL);
      break;

    case VSM_LOAD_GLOBAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = load_global_INT(index);
	push_INT(val);
      }
      break;

    case VSM_LOAD_GLOBAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = load_global_FLOAT(index);
	push_FLOAT(val);
      }
      break;

    case VSM_LOAD_GLOBAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = load_global_OBJPTR(index);
	push_OBJPTR(val);
      }
      break;

    case VSM_LOAD_LOCAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = load_local_INT(base + index);
	push_INT(val);
      }
      break;

    case VSM_LOAD_LOCAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = load_local_FLOAT(base + index);
	push_FLOAT(val);
      }
      break;

    case VSM_LOAD_LOCAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = load_local_OBJPTR(base + index);
	push_OBJPTR(val);
      }
      break;

    case VSM_STORE_GLOBAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = pop_INT();
	store_global_INT(index, val);
      }
      break;

    case VSM_STORE_GLOBAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = pop_FLOAT();
	store_global_FLOAT(index, val);
      }
      break;

    case VSM_STORE_GLOBAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = pop_OBJPTR();
	store_global_OBJPTR(index, val);
      }
      break;

    case VSM_STORE_LOCAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = pop_INT();
	store_local_INT(base + index, val);
      }
      break;

    case VSM_STORE_LOCAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = pop_FLOAT();
	store_local_FLOAT(base + index, val);
      }
      break;

    case VSM_STORE_LOCAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = pop_OBJPTR();
	store_local_OBJPTR(base + index, val);
      }
      break;

    case VSM_INT_MINUS:
      {
	Ymsl_INT val = pop_INT();
	val = - val;
	push_INT(val);
      }
      break;

    case VSM_INT_INC:
      {
	Ymsl_INT val = pop_INT();
	++ val;
	push_INT(val);
      }
      break;

    case VSM_INT_DEC:
      {
	Ymsl_INT val = pop_INT();
	-- val;
	push_INT(val);
      }
      break;

    case VSM_INT_NOT:
      {
	Ymsl_INT val = pop_INT();
	val = ~val;
	push_INT(val);
      }
      break;

    case VSM_INT_TO_BOOL:
      {
	Ymsl_INT val = pop_INT();
	Ymsl_INT val1 = (val != 0);
	push_FLOAT(val1);
      }
      break;

    case VSM_INT_TO_FLOAT:
      {
	Ymsl_INT val = pop_INT();
	Ymsl_FLOAT fval = val;
	push_FLOAT(fval);
      }
      break;

    case VSM_INT_ADD:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 + val2;
	push_INT(val);
      }
      break;

    case VSM_INT_SUB:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 - val2;
	push_INT(val);
      }
      break;

    case VSM_INT_MUL:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 * val2;
	push_INT(val);
      }
      break;

    case VSM_INT_DIV:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 / val2;
	push_INT(val);
      }
      break;

    case VSM_INT_MOD:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 % val2;
	push_INT(val);
      }
      break;

    case VSM_INT_LSHIFT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 << val2;
	push_INT(val);
      }
      break;

    case VSM_INT_RSHIFT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 >> val2;
	push_INT(val);
      }
      break;

    case VSM_INT_EQ:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 == val2);
	push_INT(val);
      }
      break;

    case VSM_INT_NE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 != val2);
	push_INT(val);
      }
      break;

    case VSM_INT_LT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 < val2);
	push_INT(val);
      }
      break;

    case VSM_INT_LE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 <= val2);
	push_INT(val);
      }
      break;

    case VSM_INT_AND:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 & val2;
	push_INT(val);
      }
      break;

    case VSM_INT_OR:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 | val2;
	push_INT(val);
      }
      break;

    case VSM_INT_XOR:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 ^ val2;
	push_INT(val);
      }
      break;

    case VSM_INT_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val3 = pop_INT();
	Ymsl_INT val = val1 ? val2 : val3;
	push_INT(val);
      }
      break;

    case VSM_FLOAT_MINUS:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val = -val1;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_TO_BOOL:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_INT val = (val1 != 0.0);
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_TO_INT:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_INT val = val1;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_ADD:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 + val2;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_SUB:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 - val2;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_MUL:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 * val2;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_DIV:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 / val2;
	push_FLOAT(val);
      }
      break;

    case VSM_FLOAT_EQ:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 == val2);
	push_INT(val);
      }
      break;

    case VSM_FLOAT_NE:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 != val2);
	push_INT(val);
      }
      break;

    case VSM_FLOAT_LT:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 < val2);
	push_INT(val);
      }
      break;

    case VSM_FLOAT_LE:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 <= val2);
	push_INT(val);
      }
      break;

    case VSM_FLOAT_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val3 = pop_FLOAT();
	Ymsl_FLOAT val = val1 ? val2 : val3;
	push_FLOAT(val);
      }
      break;

    case VSM_OBJ_MINUS:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_INC:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_DEC:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_NOT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_TO_INT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case VSM_OBJ_TO_FLOAT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_FLOAT val;
	push_FLOAT(val);
      }
      break;

    case VSM_OBJ_ADD:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_SUB:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_MUL:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_DIV:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_MOD:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_LSHIFT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_INT val2 = pop_INT();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_RSHIFT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_INT val2 = pop_INT();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_EQ:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case VSM_OBJ_NE:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case VSM_OBJ_LT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case VSM_OBJ_LE:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case VSM_OBJ_AND:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_OR:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_XOR:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_OBJ_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	Ymsl_OBJPTR val3 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case VSM_JUMP:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	pc = addr;
      }
      break;

    case VSM_JUMP_R:
      {
	Ymsl_INT addr = pop_INT();
	pc = addr;
      }
      break;

    case VSM_BRANCH_TRUE:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	Ymsl_INT cond = pop_INT();
	if ( code ) {
	  pc = addr;
	}
      }
      break;

    case VSM_BRANCH_FALSE:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	Ymsl_INT cond = pop_INT();
	if ( !code ) {
	  pc = addr;
	}
      }
      break;

    case VSM_CALL:
      {
	Ymsl_INT index = code_list.read_int(pc);
	ASSERT_COND( index >= 0 && index < mFuncTableSize );
	const VsmFunction* func = mFuncTable[index];
	Ymsl_INT base = mSP;
	func->execute(*this, base);
      }
      break;

    case VSM_CALL_R:
      {
	Ymsl_INT index = pop_INT();
	ASSERT_COND( index >= 0 && index < mFuncTableSize );
	const VsmFunction* func = mFuncTable[index];
	Ymsl_INT base = mSP;
	func->execute(*this, base);
      }
      break;

    case VSM_RETURN:
      {
      }
      break;

    case VSM_HALT:
      {
      }
      break;
    }
  }
}

END_NAMESPACE_YM_YMSL
