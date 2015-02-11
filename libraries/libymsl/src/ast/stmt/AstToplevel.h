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
  /// @param[in] stmt_num 文のリストの要素数
  /// @param[in] stmt_list 文のリスト
  /// @param[in] loc ファイル位置
  AstToplevel(ymuint num,
	      AstStatement** stmt_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstToplevel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTOPLEVEL_H
