#ifndef YMSLNAMEDTYPE_H
#define YMSLNAMEDTYPE_H

/// @file YmslNamedtype.h
/// @brief YmslNamedType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslNamedType YmslNamedType.h "YmslNamedType.h"
/// @brief 名前付き型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslNamedType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  YmslNamedType(ShString name);

  /// @brief デストラクタ
  virtual
  ~YmslNamedType();


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
