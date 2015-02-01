#ifndef IRCASTINT_H
#define IRCASTINT_H

/// @file IrCastInt.h
/// @brief IrCastInt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrCastInt IrCastInt.h "IrCastInt.h"
/// @brief cast_to_int 命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrCastInt :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrCastInt(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrCastInt();


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

#endif // IRCASTINT_H
