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
  /// @param[in] scope_num 階層の数
  /// @param[in] scope_list スコープ名のリスト
  /// @param[in] name 型名
  /// @param[in] loc ファイル位置
  AstNamedType(ymuint scope_num,
	       AstSymbol** scope_list,
	       AstSymbol* name,
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

  /// @brief スコープ名の数を返す．
  virtual
  ymuint
  scope_num() const;

  /// @brief スコープ名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < scope_num() )
  virtual
  const AstSymbol*
  scope(ymuint pos) const;

  /// @brief 名前付き方の時に名前を返す．
  virtual
  const AstSymbol*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スコープ数
  ymuint mScopeNum;

  // スコープ名のリスト
  AstSymbol** mScopeList;

  // 型名
  AstSymbol* mName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTNAMEDTYPE_H
