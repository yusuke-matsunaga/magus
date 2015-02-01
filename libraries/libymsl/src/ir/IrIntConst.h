#ifndef IRINTCONST_H
#define IRINTCONST_H

/// @file IrIntConst.h
/// @brief IrIntConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrIntConst IrIntConst.h "IrIntConst.h"
/// @brief 整数定数を表すノード
//////////////////////////////////////////////////////////////////////
class IrIntConst :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  IrIntConst(int val);

  /// @brief デストラクタ
  virtual
  ~IrIntConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const;

  /// @brief 整数値を返す．
  virtual
  int
  int_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRINTCONST_H
