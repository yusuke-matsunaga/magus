#ifndef IRLOGAND_H
#define IRLOGAND_H

/// @file IrLogAnd.h
/// @brief IrLogAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLogAnd IrLogAnd.h "IrLogAnd.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLogAnd :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrLogAnd(IrNode* src1,
	   IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrLogAnd();


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

#endif // IRLOGAND_H
