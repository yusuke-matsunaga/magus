#ifndef IRFLOATCONST_H
#define IRFLOATCONST_H

/// @file IrFloatConst.h
/// @brief IrFloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFloatConst IrFloatConst.h "IrFloatConst.h"
/// @brief 実数定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrFloatConst :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrFloatConst(ShString name,
	       Ymsl_FLOAT const_val);

  /// @brief デストラクタ
  virtual
  ~IrFloatConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief 実数値を返す．
  ///
  /// kFloatConst のみ有効
  virtual
  Ymsl_FLOAT
  float_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数値
  Ymsl_FLOAT mConstVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRFLOATCONST_H
