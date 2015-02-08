#ifndef IRINPLACEUNIOP_H
#define IRINPLACEUNIOP_H

/// @file IrInplaceUniOp.h
/// @brief IrInplaceUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrInplaceUniOp IrInplaceUniOp.h "IrInplaceUniOp.h"
/// @brief 自己代入型の単項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class IrInplaceUniOp :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺値
  IrInplaceUniOp(OpCode opcode,
		 IrNode* lhs_addr);

  /// @brief デストラクタ
  virtual
  ~IrInplaceUniOp();


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

  /// @brief ロード/ストア対象のアドレスを得る．
  ///
  /// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
  virtual
  IrNode*
  address() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // アドレス
  IrNode* mAddress;

};

END_NAMESPACE_YM_YMSL


#endif // IRINPLACEUNIOP_H
