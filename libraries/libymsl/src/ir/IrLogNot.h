#ifndef IRLOGNOT_H
#define IRLOGNOT_H

/// @file IrLogNot.h
/// @brief IrLogNot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrUniOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLogNot IrLogNot.h "IrLogNot.h"
/// @brief 論理否定命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLogNot :
  public IrUniOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1 オペランド
  IrLogNot(IrNode* src1);

  /// @brief デストラクタ
  virtual
  ~IrLogNot();


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

#endif // IRLOGNOT_H
