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

  /// @brief プリミティブ型の時 true を返す．
  virtual
  bool
  primitive_type() const;

  /// @brief 名前付き型の時 true を返す．
  virtual
  bool
  named_type() const;

  /// @brief array 型の時 true を返す．
  virtual
  bool
  array_type() const;

  /// @brief set 型の時 true を返す．
  virtual
  bool
  set_type() const;

  /// @brief map 型の時 true を返す．
  virtual
  bool
  map_type() const;

  /// @brief プリミティブ型の時に型を返す．
  virtual
  TypeId
  type_id() const;

  /// @brief 名前付き方の時に名前を返す．
  virtual
  AstSymbol*
  name() const;

  /// @brief map 型の時にキーの型を返す．
  virtual
  AstType*
  key_type() const;

  /// @brief array/set/map 型の時に要素の型を返す．
  virtual
  AstType*
  elem_type() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const = 0;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTYPE_H
