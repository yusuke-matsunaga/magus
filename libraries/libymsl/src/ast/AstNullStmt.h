#ifndef ASTNULLSTMT_H
#define ASTNULLSTMT_H

/// @file AstNullStmt.h
/// @brief AstNullStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstNullStmt AstNullStmt.h "AstNullStmt.h"
/// @brief 空文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstNullStmt :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstNullStmt(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstNullStmt();


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

#endif // ASTNULLSTMT_H
