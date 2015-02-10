#ifndef OPCODE_H
#define OPCODE_H

/// @file OpCode.h
/// @brief OpCode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class オペコードを表す列挙型
//////////////////////////////////////////////////////////////////////
enum OpCode {
  // 単項演算
  kOpCastBoolean,
  kOpCastInt,
  kOpCastFloat,
  kOpBitNeg,
  kOpLogNot,
  kOpUniMinus,
  // 特殊な単項演算
  kOpInc,
  kOpDec,
  // 二項演算
  kOpBitAnd,
  kOpBitOr,
  kOpBitXor,
  kOpLogAnd,
  kOpLogOr,
  kOpAdd,
  kOpSub,
  kOpMul,
  kOpDiv,
  kOpMod,
  kOpLshift,
  kOpRshift,
  kOpEqual,
  kOpNotEq,
  kOpLt,
  kOpLe,
  // 三項演算
  kOpIte,
};

/// @brief OpCode を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] opcode オペコード
ostream&
operator<<(ostream& s,
	   OpCode opcode);

END_NAMESPACE_YM_YMSL

#endif // OPCODE_H
