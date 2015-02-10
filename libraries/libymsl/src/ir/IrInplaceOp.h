#ifndef IRINPLACEOP_H
#define IRINPLACEOP_H

/// @file IrInplaceOp.h
/// @brief IrInplaceOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrInplaceOp IrInplaceOp.h "IrInplaceOp.h"
/// @brief 自己代入型の演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class IrInplaceOp :
  public IrOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] irtype IR型
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺値
  IrInplaceOp(IrType irtype,
	      OpCode opcode,
	      IrHandle* lhs_addr);

  /// @brief デストラクタ
  virtual
  ~IrInplaceOp();


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
  /// kIrLoad, kIrStore, kIrInplaceUniOp, kIrInplaceBinOp のみ有効
  virtual
  IrHandle*
  address() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // アドレス
  IrHandle* mAddress;

};

END_NAMESPACE_YM_YMSL


#endif // IRINPLACEOP_H
