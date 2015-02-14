#ifndef BOOLEANCONST_H
#define BOOLEANCONST_H

/// @file BooleanConst.h
/// @brief BooleanConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConstVal.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class BooleanConst BooleanConst.h "BooleanConst.h"
/// @brief boolean型定数を表すノード
//////////////////////////////////////////////////////////////////////
class BooleanConst :
  public ConstVal
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] val 値
  BooleanConst(const Type* type,
	       bool val);

  /// @brief デストラクタ
  virtual
  ~BooleanConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ブール値を返す．
  ///
  /// value_type() が boolean_type の時有効
  virtual
  Ymsl_BOOLEAN
  boolean_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  bool mVal;

};

END_NAMESPACE_YM_YMSL

#endif // BOOLEANCONST_H
