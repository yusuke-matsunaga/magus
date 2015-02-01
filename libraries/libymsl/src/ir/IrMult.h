#ifndef IRLOAD_H
#define IRLOAD_H

/// @file IrLoad.h
/// @brief IrLoad のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLoad IrLoad.h "IrLoad.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLoad :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrLoad(IrNode* src1,
	 IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrLoad();


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

#endif // IRLOAD_H
