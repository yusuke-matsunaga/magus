#ifndef IRBITAND_H
#define IRBITAND_H

/// @file IrBitAnd.h
/// @brief IrBitAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBitAnd IrBitAnd.h "IrBitAnd.h"
/// @brief ビットAND命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrBitAnd :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrBitAnd(IrNode* src1,
	   IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrBitAnd();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const;

};

END_NAMESPACE_YM_YMSL

#endif // IRBITAND_H
