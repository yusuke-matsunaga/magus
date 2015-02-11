#ifndef IRINPLACEUNIOP_H
#define IRINPLACEUNIOP_H

/// @file IrInplaceUniOp.h
/// @brief IrInplaceUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInplaceOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrInplaceUniOp IrInplaceUniOp.h "IrInplaceUniOp.h"
/// @brief 自己代入型の単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class IrInplaceUniOp :
  public IrInplaceOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺値
  IrInplaceUniOp(OpCode opcode,
		 IrHandle* lhs_addr);

  /// @brief デストラクタ
  virtual
  ~IrInplaceUniOp();

};

END_NAMESPACE_YM_YMSL


#endif // IRINPLACEUNIOP_H
