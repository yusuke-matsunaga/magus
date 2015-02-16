#ifndef IRINTCONST_H
#define IRINTCONST_H

/// @file IrIntConst.h
/// @brief IrIntConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrIntConst IrIntConst.h "IrIntConst.h"
/// @brief 整数定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrIntConst :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrIntConst(ShString name,
	     Ymsl_INT const_val);

  /// @brief デストラクタ
  virtual
  ~IrIntConst();


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

  /// @brief 整数値を返す．
  ///
  /// kIntConst のみ有効
  virtual
  Ymsl_INT
  int_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数値
  Ymsl_INT mConstVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRINTCONST_H
