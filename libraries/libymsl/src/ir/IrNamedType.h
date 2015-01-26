#ifndef YMSLNAMEDTYPE_H
#define YMSLNAMEDTYPE_H

/// @file IrNamedtype.h
/// @brief IrNamedType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrNamedType IrNamedType.h "IrNamedType.h"
/// @brief 名前付き型を表すクラス
//////////////////////////////////////////////////////////////////////
class IrNamedType :
  public IrType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  IrNamedType(ShString name);

  /// @brief デストラクタ
  virtual
  ~IrNamedType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型名を得る．
  ///
  /// enum のみ有効
  virtual
  ShString
  type_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLNAMEDTYPE_H
