
/// @file YmslCodeList.cc
/// @brief YmslCodeList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslCodeList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] size コードサイズ
YmslCodeList::YmslCodeList(ymuint size)
{
  mSize = size;
  mBody = new Ymsl_CODE[size];
}

// @brief デストラクタ
YmslCodeList::~YmslCodeList()
{
  delete [] mBody;
}

// @brief 命令を書き込む．
// @param[inout] addr アドレス
// @param[in] op 命令
//
// addr は次の位置を指すように変更される．
void
YmslCodeList::write_opcode(Ymsl_INT& addr,
			   Ymsl_CODE op)
{
  mBody[addr] = op;
  ++ addr;
}

// @brief INT を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
void
YmslCodeList::write_int(Ymsl_INT& addr,
			Ymsl_INT val)
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  mBody[addr] = val;
  ++ addr;
}

// @brief FLOAT を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
void
YmslCodeList::write_float(Ymsl_INT& addr,
			  Ymsl_FLOAT val)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_FLOAT* f_ptr = reinterpret_cast<Ymsl_FLOAT*>(c_ptr);
  *f_ptr = val;
  addr += sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
}

// @brief OBJPTR を書き込む．
// @param[inout] addr アドレス
// @param[in] val 値
//
// addr は次の位置を指すように変更される．
void
YmslCodeList::write_objptr(Ymsl_INT& addr,
			   Ymsl_OBJPTR val)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_OBJPTR* o_ptr = reinterpret_cast<Ymsl_OBJPTR*>(c_ptr);
  *o_ptr = val;
  addr += sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
}

// @brief 命令を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
Ymsl_CODE
YmslCodeList::read_opcode(Ymsl_INT& addr)
{
  Ymsl_CODE op = mBody[addr];
  ++ addr;
  return op;
}

// @brief INT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
Ymsl_INT
YmslCodeList::read_int(Ymsl_INT& addr)
{
  ASSERT_COND( sizeof(Ymsl_INT) == sizeof(Ymsl_CODE) );
  Ymsl_INT val = mBody[addr];
  ++ addr;
  return val;
}

// @brief FLOAT を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
Ymsl_FLOAT
YmslCodeList::read_float(Ymsl_INT& addr)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_FLOAT* f_ptr = reinterpret_cast<Ymsl_FLOAT*>(c_ptr);
  Ymsl_FLOAT val = *f_ptr;
  addr += sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE);
  return val;
}

// @brief OBJPTR を読みだす．
// @param[inout] addr アドレス
// @return 読みだした値を返す．
Ymsl_OBJPTR
YmslCodeList::read_objptr(Ymsl_INT& addr)
{
  Ymsl_CODE* c_ptr = mBody + addr;
  Ymsl_OBJPTR* o_ptr = reinterpret_cast<Ymsl_OBJPTR*>(c_ptr);
  Ymsl_OBJPTR val = *o_ptr;
  addr += sizeof(Ymsl_OBJPTR) / sizeof(Ymsl_CODE);
  return val;
}

END_NAMESPACE_YM_YMSL
