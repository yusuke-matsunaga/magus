#ifndef IRINPLACEBINOP_H
#define IRINPLACEBINOP_H

/// @file IrInplaceBinOp.h
/// @brief IrInplaceBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrInplaceOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrInplaceBinOp IrInplaceBinOp.h "IrInplaceBinOp.h"
/// @brief 自己代入型のニ項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class IrInplaceBinOp :
  public IrInplaceOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺値
  /// @param[in] opr1 オペランド
  IrInplaceBinOp(OpCode opcode,
		 IrHandle* lhs_addr,
		 IrNode* opr1);

  /// @brief デストラクタ
  virtual
  ~IrInplaceBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子のみ有効
  virtual
  IrNode*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  IrNode* mOperand;

};

END_NAMESPACE_YM_YMSL

#endif // IRINPLACEBINOP_H
