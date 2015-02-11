#ifndef ASTBREAK_H
#define ASTBREAK_H

/// @file AstBreak.h
/// @brief AstBreak のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBreak AstBreak.h "AstBreak.h"
/// @brief break 文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstBreak :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstBreak(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBreak();


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

#endif // ASTBREAK_H
