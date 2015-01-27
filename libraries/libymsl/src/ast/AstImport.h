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
  /// @param[in] num モジュール数
  /// @param[in] module_list モジュールのリスト
  /// @param[in] loc ファイル位置
  AstImport(ymuint num,
	    AstSymbol** module_list,
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
  StmtType
  stmt_type() const;

  /// @brief インポートするモジュール数を返す．
  ///
  /// kImport のみ有効
  virtual
  ymuint
  import_num() const;

  /// @brief インポートするモジュール名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < inport_num() )
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_module(ymuint pos) const;

  /// @brief インポートするモジュールのエイリアスを返す．
  /// @param[in] pos 位置 ( 0 <= pos < inport_num() )
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_alias(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール数
  ymuint mNum;

  // モジュール名とエイリアスのリスト
  // 実体はポインタ配列でサイズは mNum * 2
  AstSymbol** mModuleList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTIMPORT_H
