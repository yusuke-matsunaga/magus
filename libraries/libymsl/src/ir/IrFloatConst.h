#ifndef IRFLOATCONST_H
#define IRFLOATCONST_H

/// @file IrFloatConst.h
/// @brief IrFloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFloatConst IrFloatConst.h "IrFloatConst.h"
/// @brief 実数定数を表すノード
//////////////////////////////////////////////////////////////////////
class IrFloatConst :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] val 値
  IrFloatConst(const Type* type,
	       double val);

  /// @brief デストラクタ
  virtual
  ~IrFloatConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 実数値を返す．
  virtual
  double
  float_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRFLOATCONST_H
