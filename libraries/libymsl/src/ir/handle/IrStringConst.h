#ifndef IRSTRINGCONST_H
#define IRSTRINGCONST_H

/// @file IrStringConst.h
/// @brief IrStringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrStringConst IrStringConst.h "IrStringConst.h"
/// @brief 文字列定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrStringConst :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrStringConst(ShString name,
		Ymsl_STRING const_val);

  /// @brief デストラクタ
  virtual
  ~IrStringConst();


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

  /// @brief 文字列を返す．
  ///
  /// kStringConst のみ有効
  virtual
  Ymsl_STRING
  string_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数値
  Ymsl_STRING mConstVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRSTRINGCONST_H
