#ifndef ASTCONTINUE_H
#define ASTCONTINUE_H

/// @file AstContinue.h
/// @brief AstContinue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstContinue AstContinue.h "AstContinue.h"
/// @brief continue 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstContinue :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstContinue(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstContinue();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTCONTINUE_H
