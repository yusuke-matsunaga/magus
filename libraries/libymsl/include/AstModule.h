#ifndef ASTMODULE_H
#define ASTMODULE_H

/// @file AstModule.h
/// @brief AstModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstModule AstModule.h "AstModule.h"
/// @brief import 用のモジュールを表すクラス
//////////////////////////////////////////////////////////////////////
class AstModule :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] module モジュール名
  /// @param[in] alias エイリアス名
  /// @param[in] loc ファイル位置
  AstModule(AstSymbol* module,
	    AstSymbol* alias,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstModule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュール名を返す．
  AstSymbol*
  module_name() const;

  /// @brief エイリアス名を返す．
  ///
  /// 場合によっては NULL を返す．
  AstSymbol*
  alias_name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール名
  AstSymbol* mModuleName;

  // エイリアス名
  AstSymbol* mAliasName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTMODULE_H
