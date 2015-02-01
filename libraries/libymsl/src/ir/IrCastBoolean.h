#ifndef IRCASTBOOLEAN_H
#define IRCASTBOOLEAN_H

/// @file IrCastBoolean.h
/// @brief IrCastBoolean のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrCastBoolean IrCastBoolean.h "IrCastBoolean.h"
/// @brief cast_to_boolean 命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrCastBoolean :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrCastBoolean(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrCastBoolean();


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

#endif // IRCASTBOOLEAN_H
