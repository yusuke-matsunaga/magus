#ifndef IRBITOR_H
#define IRBITOR_H

/// @file IrBitOr.h
/// @brief IrBitOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBitOr IrBitOr.h "IrBitOr.h"
/// @brief ビットOR命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrBitOr :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrBitOr(IrNode* src1,
	 IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrBitOr();


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

#endif // IRBITOR_H
