#ifndef IRBITXOR_H
#define IRBITXOR_H

/// @file IrBitXor.h
/// @brief IrBitXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBitXor IrBitXor.h "IrBitXor.h"
/// @brief ビットXOR命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrBitXor :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrBitXor(IrNode* src1,
	   IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrBitXor();


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

#endif // IRBITXOR_H
