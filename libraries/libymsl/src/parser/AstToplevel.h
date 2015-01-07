#ifndef ASTTOPLEVEL_H
#define ASTTOPLEVEL_H

/// @file AstToplevel.h
/// @brief AstToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstToplevel AstToplevel.h "AstToplevel.h"
/// @brief AST のトップレベルを表すクラス
//////////////////////////////////////////////////////////////////////
class AstToplevel :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] root 根のノード
  /// @param[in] loc ファイル位置
  AstToplevel(AstStmtList* root,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_YMSL

#endif // ASTTOPLEVEL_H
