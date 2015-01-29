#ifndef ASTTYPE_H
#define ASTTYPE_H

/// @file AstType.h
/// @brief AstType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstType AstType.h "AstType.h"
/// @brief 値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class AstType :
  public Ast
{
public:

  /// @brief コンストラクタ
  AstType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型番号を返す．
  virtual
  TypeId
  type_id() const = 0;

  /// @brief キーの型を返す．
  ///
  /// kMapType のみ有効
  virtual
  const AstType*
  key_type() const;

  /// @brief 要素の型を返す．
  ///
  /// kArrayType, kSetType, kMapType のみ有効
  virtual
  const AstType*
  elem_type() const;

  /// @brief スコープ名の数を返す．
  ///
  /// kNamedType のみ有効
  virtual
  ymuint
  scope_num() const;

  /// @brief スコープ名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < scope_num() )
  ///
  /// kNamedType のみ有効
  virtual
  const AstSymbol*
  scope(ymuint pos) const;

  /// @brief 名前を返す．
  ///
  /// kNamedType のみ有効
  virtual
  const AstSymbol*
  name() const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTYPE_H
