#ifndef IRLOGOR_H
#define IRLOGOR_H

/// @file IrLogOr.h
/// @brief IrLogOr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrBinOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLogOr IrLogOr.h "IrLogOr.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLogOr :
  public IrBinOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] src1, src2 オペランド
  IrLogOr(IrNode* src1,
	 IrNode* src2);

  /// @brief デストラクタ
  virtual
  ~IrLogOr();


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

#endif // IRLOGOR_H
