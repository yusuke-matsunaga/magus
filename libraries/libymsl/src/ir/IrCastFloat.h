#ifndef IRCASTFLOAT_H
#define IRCASTFLOAT_H

/// @file IrCastFloat.h
/// @brief IrCastFloat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrCastFloat IrCastFloat.h "IrCastFloat.h"
/// @brief cast_to_float 命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrCastFloat :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrCastFloat(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrCastFloat();


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

#endif // IRCASTFLOAT_H
