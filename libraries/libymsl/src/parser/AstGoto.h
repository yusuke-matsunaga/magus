#ifndef ASTGOTO_H
#define ASTGOTO_H

/// @file AstGoto.h
/// @brief AstGoto のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstGoto AstGoto.h "AstGoto.h"
/// @brief while 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstGoto :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  AstGoto(AstSymbol* label,
	  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstGoto();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  AstSymbol* mLabel;

};

END_NAMESPACE_YM_YMSL

#endif // ASTGOTO_H
