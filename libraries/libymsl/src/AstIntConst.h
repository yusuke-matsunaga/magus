#ifndef ASTINTCONST_H
#define ASTINTCONST_H

/// @file AstInt.h
/// @brief AstInt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIntConst AstIntConst.h "AstIntConst.h"
/// @brief 整数型定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstIntConst :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstIntConst(int val,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIntConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL


#endif // ASTINTCONST_H
