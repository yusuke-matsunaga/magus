#ifndef IRBITNEG_H
#define IRBITNEG_H

/// @file IrBitNeg.h
/// @brief IrBitNeg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBitNeg IrBitNeg.h "IrBitNeg.h"
/// @brief ビット反転命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrBitNeg :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrBitNeg(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrBitNeg();


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

#endif // IRBITNEG_H
