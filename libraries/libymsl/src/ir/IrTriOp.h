#ifndef IRTRIOP_H
#define IRTRIOP_H

/// @file IrTriOp.h
/// @brief IrTriOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTriOp IrTriOp.h "IrTriOp.h"
/// @brief 三項演算を表すノード
//////////////////////////////////////////////////////////////////////
class IrTriOp :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] type 型
  /// @param[in] src1, src2, src3 オペランド
  IrTriOp(OpCode opcode,
	  const Type* type,
	  IrNode* src1,
	  IrNode* src2,
	  IrNode* src3);

  /// @brief デストラクタ
  virtual
  ~IrTriOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

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

  // opcode
  OpCode mOpCode;

  // オペランド
  IrNode* mOperand[3];

};

END_NAMESPACE_YM_YMSL

#endif // IRTRIOP_H
