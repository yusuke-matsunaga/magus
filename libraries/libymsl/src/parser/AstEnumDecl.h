#ifndef ASTENUMDECL_H
#define ASTENUMDECL_H

/// @file AstEnumDecl.h
/// @brief AstEnumDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstEnumDecl AstEnumDecl.h "AstEnumDecl.h"
/// @brief enum 定義を表すクラス
//////////////////////////////////////////////////////////////////////
class AstEnumDecl :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] const_list 定数リスト
  /// @param[in] loc ファイル位置
  AstEnumDecl(AstSymbol* name,
	      AstEnumConstList* const_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstEnumDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief enum 定数の数を返す．
  ///
  /// kEnumDecl のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief enum 定数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < enum_num() )
  ///
  /// kEnumDecl のみ有効
  virtual
  const AstEnumConst*
  enum_const(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  AstSymbol* mName;

  // 定数リスト
  vector<AstEnumConst*> mConstList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTENUMDECL_H
