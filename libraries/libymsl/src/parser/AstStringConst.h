#ifndef ASTSTRINGCONST_H
#define ASTSTRINGCONST_H

/// @file AstStringConst.h
/// @brief AstStringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstStringConst AstStringConst.h "AstStringConst.h"
/// @brief 整数型定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstStringConst :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstStringConst(const char* val,
		 const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstStringConst();


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
  const char* mVal;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTRINGCONST_H
