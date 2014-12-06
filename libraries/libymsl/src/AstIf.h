#ifndef ASTIF_H
#define ASTIF_H

/// @file AstIf.h
/// @brief AstIf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIf AstIf.h "AstIf.h"
/// @brief if 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstIf :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] top 先頭の if ブロック
  /// @param[in] elif_list elif ブロックリスト
  /// @param[in] loc ファイル位置
  AstIf(AstIfBlock* top,
	AstIfBlock* elif_list,
	const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIf();


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

  // if ブロックのリスト
  vector<AstIfBlock*> mIfBlockList;

};

END_NAMESPACE_YM_YMSL


#endif // ASTIF_H
