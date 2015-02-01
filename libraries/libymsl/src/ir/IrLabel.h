#ifndef IRLABEL_H
#define IRLABEL_H

/// @file IrLabel.h
/// @brief IrLabel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLabel IrLabel.h "IrLabel.h"
/// @brief ラベルを表すクラス
//////////////////////////////////////////////////////////////////////
class IrLabel :
  public IrNode
{
public:

  /// @brief コンストラクタ
  IrLabel();

  /// @brief デストラクタ
  virtual
  ~IrLabel();


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


#endif // IRLABEL_H
