#ifndef ASTNAMEDTYPE_H
#define ASTNAMEDTYPE_H

/// @file AstNamedType.h
/// @brief AstNamedType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstNamedType AstNamedType.h "AstNamedType.h"
/// @brief 名前付き型を表す AstType
//////////////////////////////////////////////////////////////////////
class AstNamedType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 型名
  AstNamedType(AstSymbol* name);

  /// @brief デストラクタ
  virtual
  ~AstNamedType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前付き型の時 true を返す．
  virtual
  bool
  named_type() const;

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 名前付き方の時に名前を返す．
  virtual
  const AstSymbol*
  name() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型名
  AstSymbol* mName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTNAMEDTYPE_H
