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

  /// @brief 名前付き型の時 true を返す．
  ///
  /// 名前付き型の場合には type_id() は意味を持たない．
  virtual
  bool
  named_type() const;

  /// @brief 名前付き方の時に名前を返す．
  virtual
  const AstSymbol*
  name() const;

  /// @brief 型番号を返す．
  virtual
  TypeId
  type_id() const = 0;

  /// @brief map 型の時にキーの型を返す．
  virtual
  const AstType*
  key_type() const;

  /// @brief array/set/map 型の時に要素の型を返す．
  virtual
  const AstType*
  elem_type() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const = 0;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTYPE_H
