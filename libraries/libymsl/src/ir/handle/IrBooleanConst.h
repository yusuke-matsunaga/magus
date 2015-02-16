#ifndef IRBOOLEANCONST_H
#define IRBOOLEANCONST_H

/// @file IrBooleanConst.h
/// @brief IrBooleanConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrBooleanConst IrBooleanConst.h "IrBooleanConst.h"
/// @brief ブール定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrBooleanConst :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrBooleanConst(ShString name,
		 Ymsl_BOOLEAN const_val);

  /// @brief デストラクタ
  virtual
  ~IrBooleanConst();


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

  /// @brief ブール値を返す．
  ///
  /// kBooleanConst のみ有効
  virtual
  Ymsl_BOOLEAN
  boolean_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数値
  Ymsl_BOOLEAN mConstVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRBOOLEANCONST_H
