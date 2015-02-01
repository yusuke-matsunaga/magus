#ifndef IRPLUS_H
#define IRPLUS_H

/// @file IrPlus.h
/// @brief IrPlus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrPlus IrPlus.h "IrPlus.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrPlus :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrPlus(IrNode* src1,
	 IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrPlus();


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

#endif // IRPLUS_H
