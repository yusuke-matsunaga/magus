#ifndef IRCONSTHANDLE_H
#define IRCONSTHANDLE_H

/// @file IrConstHandle.h
/// @brief IrConstHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrConstHandle IrConstHandle.h "IrConstHandle.h"
/// @brief 定数値を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrConstHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrConstHandle(ShString name,
		const ConstVal* const_val);

  /// @brief デストラクタ
  virtual
  ~IrConstHandle();


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

  /// @brief 定数値を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const ConstVal*
  constant() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 定数値
  const ConstVal* mConstVal;

};

END_NAMESPACE_YM_YMSL

#endif // IRCONSTHANDLE_H
