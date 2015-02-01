#ifndef NAMEDTYPE_H
#define NAMEDTYPE_H

/// @file Namedtype.h
/// @brief NamedType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class NamedType NamedType.h "NamedType.h"
/// @brief 名前付き型を表すクラス
//////////////////////////////////////////////////////////////////////
class NamedType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] type_id タイプID
  /// @param[in] name 名前
  NamedType(TypeId type_id,
	    ShString name);

  /// @brief デストラクタ
  virtual
  ~NamedType();


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

#endif // NAMEDTYPE_H
