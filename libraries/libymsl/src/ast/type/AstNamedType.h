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
///
/// 実は scope_list の中身も name もともに AstSymbol であるが，
/// 最後だけ型名なので区別している．
//////////////////////////////////////////////////////////////////////
class AstNamedType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 型名
  /// @param[in] loc ファイル位置
  AstNamedType(AstExpr* name,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstNamedType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 名前付き方の時に名前を返す．
  virtual
  const AstExpr*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型名
  AstExpr* mName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTNAMEDTYPE_H
