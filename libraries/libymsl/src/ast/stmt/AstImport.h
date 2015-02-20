#ifndef ASTIMPORT_H
#define ASTIMPORT_H

/// @file AstImport.h
/// @brief AstImport のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstImport AstImport.h "AstImport.h"
/// @brief import 文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstImport :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] module モジュール名
  /// @param[in] alias エイリアス
  /// @param[in] loc ファイル位置
  AstImport(AstSymbol* module,
	    AstSymbol* alias,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstImport();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

  /// @brief インポートするモジュール名を返す．
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_module() const;

  /// @brief インポートするモジュールのエイリアスを返す．
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_alias() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール名
  AstSymbol* mModule;

  // エイリアス
  AstSymbol* mAlias;

};

END_NAMESPACE_YM_YMSL

#endif // ASTIMPORT_H
