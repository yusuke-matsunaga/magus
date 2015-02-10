#ifndef ASTFALSE_H
#define ASTFALSE_H

/// @file AstFalse.h
/// @brief AstFalse のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFalse AstFalse.h "AstFalse.h"
/// @brief false を表すクラス
//////////////////////////////////////////////////////////////////////
class AstFalse :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstFalse(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFalse();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  expr_type() const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFALSE_H
