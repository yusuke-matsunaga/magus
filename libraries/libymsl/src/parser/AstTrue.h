#ifndef ASTTRUE_H
#define ASTTRUE_H

/// @file AstTrue.h
/// @brief AstTrue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstTrue AstTrue.h "AstTrue.h"
/// @brief true を表すクラス
//////////////////////////////////////////////////////////////////////
class AstTrue :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstTrue(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstTrue();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTRUE_H
