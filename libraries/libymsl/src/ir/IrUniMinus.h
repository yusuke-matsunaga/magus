#ifndef IRUNIMINUS_H
#define IRUNIMINUS_H

/// @file IrUniMinus.h
/// @brief IrUniMinus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrUniMinus IrUniMinus.h "IrUniMinus.h"
/// @brief 単項マイナス命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrUniMinus :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrUniMinus(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrUniMinus();


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

#endif // IRUNIMINUS_H
