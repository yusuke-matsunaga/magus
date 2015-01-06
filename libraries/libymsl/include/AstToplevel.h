#ifndef ASTTOPLEVEL_H
#define ASTTOPLEVEL_H

/// @file AstToplevel.h
/// @brief AstToplevel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstToplevel AstToplevel.h "AstToplevel.h"
/// @brief AST のトップレベルを表すクラス
//////////////////////////////////////////////////////////////////////
class AstToplevel :
  public Ast
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

  /// @brief 出力する
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トップレベルのステートメントリスト
  vector<AstStatement*> mStmtList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTTOPLEVEL_H
