
/// @file YmslVSM.cc
/// @brief YmslVSM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslVSM.h"
#include "YmslCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslVSM
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslVSM::YmslVSM()
{
  mGlobalHeapSize = 0;
  mGlobalHeap = NULL;

  mLocalStackSize = 0;
  mLocalStack = NULL;

  mSP = 0;
  mBASE = 0;

  mFuncStackSize = 0;
  mFuncStack = NULL;

  mFP = 0;
}

// @brief デストラクタ
YmslVSM::~YmslVSM()
{
  delete [] mGlobalHeap;
  delete [] mLocalStack;
  delete [] mFuncStack;
}

// @brief バイトコードを実行する．
// @param[in] code_list コードの配列
// @param[in] func_table 関数テーブル
void
YmslVSM::execute(const YmslCodeList& code_list,
		 const vector<const Function*>& func_table)
{
  for (Ymsl_INT pc = 0; pc < code_list.size(); ) {
    Ymsl_CODE code = code_list.read_opcode(pc);

    switch ( code ) {
    case YMVSM_PUSH_INT_IMM:
      {
	Ymsl_INT val = code_list.read_int(pc);
	push_INT(val);
      }
      break;

    case YMVSM_PUSH_FLOAT_IMM:
      {
	Ymsl_FLOAT val = code_list.read_float(pc);
	push_FLOAT(val);
      }
      break;

    case YMVSM_PUSH_FLOAT_ZERO:
      push_FLOAT(0.0);
      break;

    case YMVSM_PUSH_FLOAT_ONE:
      push_FLOAT(1.0);
      break;

    case YMVSM_PUSH_OBJ_NULL:
      push_OBJPTR(NULL);
      break;

    case YMVSM_LOAD_GLOBAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = load_global_INT(index);
	push_INT(val);
      }
      break;

    case YMVSM_LOAD_GLOBAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = load_global_FLOAT(index);
	push_FLOAT(val);
      }
      break;

    case YMVSM_LOAD_GLOBAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = load_global_OBJPTR(index);
	push_OBJPTR(val);
      }
      break;

    case YMVSM_LOAD_LOCAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = load_local_INT(index);
	push_INT(val);
      }
      break;

    case YMVSM_LOAD_LOCAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = load_local_FLOAT(index);
	push_FLOAT(val);
      }
      break;

    case YMVSM_LOAD_LOCAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = load_local_OBJPTR(index);
	push_OBJPTR(val);
      }
      break;

    case YMVSM_STORE_GLOBAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = pop_INT();
	store_global_INT(index, val);
      }
      break;

    case YMVSM_STORE_GLOBAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = pop_FLOAT();
	store_global_FLOAT(index, val);
      }
      break;

    case YMVSM_STORE_GLOBAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = pop_OBJPTR();
	store_global_OBJPTR(index, val);
      }
      break;

    case YMVSM_STORE_LOCAL_INT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_INT val = pop_INT();
	store_local_INT(index, val);
      }
      break;

    case YMVSM_STORE_LOCAL_FLOAT:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_FLOAT val = pop_FLOAT();
	store_local_FLOAT(index, val);
      }
      break;

    case YMVSM_STORE_LOCAL_OBJ:
      {
	Ymsl_INT index = code_list.read_int(pc);
	Ymsl_OBJPTR val = pop_OBJPTR();
	store_local_OBJPTR(index, val);
      }
      break;

    case YMVSM_INT_MINUS:
      {
	Ymsl_INT val = pop_INT();
	val = -val;
	push_INT(val);
      }
      break;

    case YMVSM_INT_INC:
      {
	Ymsl_INT val = pop_INT();
	++ val;
	push_INT(val);
      }
      break;

    case YMVSM_INT_DEC:
      {
	Ymsl_INT val = pop_INT();
	-- val;
	push_INT(val);
      }
      break;

    case YMVSM_INT_NOT:
      {
	Ymsl_INT val = pop_INT();
	val = ~val;
	push_INT(val);
      }
      break;

    case YMVSM_INT_TO_BOOL:
      {
	Ymsl_INT val = pop_INT();
	Ymsl_INT val1 = (val != 0);
	push_FLOAT(val1);
      }
      break;

    case YMVSM_INT_TO_FLOAT:
      {
	Ymsl_INT val = pop_INT();
	Ymsl_FLOAT fval = val;
	push_FLOAT(fval);
      }
      break;

    case YMVSM_INT_ADD:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 + val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_SUB:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 - val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_MUL:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 * val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_DIV:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 / val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_MOD:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 % val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_LSHIFT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 << val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_RSHIFT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 >> val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_EQ:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 == val2);
	push_INT(val);
      }
      break;

    case YMVSM_INT_NE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 != val2);
	push_INT(val);
      }
      break;

    case YMVSM_INT_LT:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 < val2);
	push_INT(val);
      }
      break;

    case YMVSM_INT_LE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = (val1 <= val2);
	push_INT(val);
      }
      break;

    case YMVSM_INT_AND:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 & val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_OR:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 | val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_XOR:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val = val1 ^ val2;
	push_INT(val);
      }
      break;

    case YMVSM_INT_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_INT val2 = pop_INT();
	Ymsl_INT val3 = pop_INT();
	Ymsl_INT val = val1 ? val2 : val3;
	push_INT(val);
      }
      break;

    case YMVSM_FLOAT_MINUS:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val = -val1;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_TO_BOOL:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_INT val = (val1 != 0.0);
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_TO_INT:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_INT val = val1;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_ADD:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 + val2;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_SUB:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 - val2;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_MUL:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 * val2;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_DIV:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val = val1 / val2;
	push_FLOAT(val);
      }
      break;

    case YMVSM_FLOAT_EQ:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 == val2);
	push_INT(val);
      }
      break;

    case YMVSM_FLOAT_NE:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 != val2);
	push_INT(val);
      }
      break;

    case YMVSM_FLOAT_LT:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 < val2);
	push_INT(val);
      }
      break;

    case YMVSM_FLOAT_LE:
      {
	Ymsl_FLOAT val1 = pop_FLOAT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_INT val = (val1 <= val2);
	push_INT(val);
      }
      break;

    case YMVSM_FLOAT_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_FLOAT val2 = pop_FLOAT();
	Ymsl_FLOAT val3 = pop_FLOAT();
	Ymsl_FLOAT val = val1 ? val2 : val3;
	push_FLOAT(val);
      }
      break;

    case YMVSM_OBJ_MINUS:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_INC:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_DEC:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_NOT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_TO_INT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case YMVSM_OBJ_TO_FLOAT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	//
	Ymsl_FLOAT val;
	push_FLOAT(val);
      }
      break;

    case YMVSM_OBJ_ADD:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_SUB:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_MUL:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_DIV:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_MOD:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_LSHIFT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_INT val2 = pop_INT();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_RSHIFT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_INT val2 = pop_INT();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_EQ:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case YMVSM_OBJ_NE:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case YMVSM_OBJ_LT:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case YMVSM_OBJ_LE:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_INT val;
	push_INT(val);
      }
      break;

    case YMVSM_OBJ_AND:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_OR:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_XOR:
      {
	Ymsl_OBJPTR val1 = pop_OBJPTR();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_OBJ_ITE:
      {
	Ymsl_INT val1 = pop_INT();
	Ymsl_OBJPTR val2 = pop_OBJPTR();
	Ymsl_OBJPTR val3 = pop_OBJPTR();
	//
	Ymsl_OBJPTR val;
	push_OBJPTR(val);
      }
      break;

    case YMVSM_JUMP:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	pc = addr;
      }
      break;

    case YMVSM_JUMP_R:
      {
	Ymsl_INT addr = pop_INT();
	pc = addr;
      }
      break;

    case YMVSM_BRANCH_TRUE:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	Ymsl_INT cond = pop_INT();
	if ( code ) {
	  pc = addr;
	}
      }
      break;

    case YMVSM_BRANCH_FALSE:
      {
	Ymsl_INT addr = code_list.read_int(pc);
	Ymsl_INT cond = pop_INT();
	if ( !code ) {
	  pc = addr;
	}
      }
      break;

    case YMVSM_CALL:
      {
	Ymsl_INT index = code_list.read_int(pc);
	ASSERT_COND( index >= 0 && index < func_table.size() );
	const Function* func = func_table[index];
	ymuint n = func->arg_num();
	Ymsl_INT base = mSP - n;
	push_func(pc, mBASE);
      }
      break;

    case YMVSM_CALL_R:
      {
	Ymsl_INT index = pop_INT();
	ASSERT_COND( index >= 0 && index < func_table.size() );
	const Function* func = func_table[index];
	push_func(pc, mBASE);
      }
      break;

    case YMVSM_RETURN:
      {
      }
      break;

    case YMVSM_HALT:
      {
      }
      break;
    }
  }
}

END_NAMESPACE_YM_YMSL
