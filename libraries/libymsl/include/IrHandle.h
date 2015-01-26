#ifndef IRHANDLE_H
#define IRHANDLE_H

/// @file IrHandle.h
/// @brief IrHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrHandle IrHandle.h "IrHandle.h"
/// @brief Symbol Table のハンドル
//////////////////////////////////////////////////////////////////////
class IrHandle
{
  friend class IrScope;

public:

  /// @brief コンストラクタ
  IrHandle();

  /// @brief デストラクタ
  virtual
  ~IrHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const = 0;

  /// @brief スコープを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrScope*
  scope() const;

  /// @brief 変数宣言を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrVar*
  var() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrFunction*
  function() const;

  /// @brief ラベルを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrLabel*
  label() const;

  /// @brief 名前付き型を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrType*
  named_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素を指すリンク
  IrHandle* mLink;

};

END_NAMESPACE_YM_YMSL

#endif // IRHANDLE_H
