#ifndef IRSTROE_H
#define IRSTROE_H

/// @file IrStore.h
/// @brief IrStore のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrStore IrStore.h "IrStore.h"
/// @brief ストア命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrStore :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrStore(IrNode* src1,
	  IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrStore();


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

#endif // IRSTORE_H
