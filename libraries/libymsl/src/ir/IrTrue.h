#ifndef IRTRUE_H
#define IRTRUE_H

/// @file IrTrue.h
/// @brief IrTrue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTrue IrTrue.h "IrTrue.h"
/// @brief true を表すノード
//////////////////////////////////////////////////////////////////////
class IrTrue :
  public IrNode
{
public:

  /// @brief コンストラクタ
  IrTrue();

  /// @brief デストラクタ
  virtual
  ~IrTrue();


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

#endif // IRTRUE_H
