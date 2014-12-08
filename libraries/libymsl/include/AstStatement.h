#ifndef ASTSTATEMENT_H
#define ASTSTATEMENT_H

/// @file AstStatement.h
/// @brief AstStatement のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

#if 0
//////////////////////////////////////////////////////////////////////
/// @brief スタートメントの種類
//////////////////////////////////////////////////////////////////////
enum AstStmtType {
  kAstAssignment,
  kAstIf,
  kAstElif,
  kAstElse,
  kAstFor,
  kAstWhile,
  kAstDoWhile,
  kAstSwitch,
  kAstCaseItem,
  kAstGoto,
  kAstLabel,
  kAstBreak,
  kAstContinue,
  kAstReturn,
  kAstBlock
};
#endif


//////////////////////////////////////////////////////////////////////
/// @class AstStatement AstStatement.h "AstStatement.h"
/// @brief ステートメントを表すクラス
//////////////////////////////////////////////////////////////////////
class AstStatement :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstStatement(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstStatement();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ブロックを返す．
  ///
  /// ブロックを持たない要素の場合 NULL を返す．
  virtual
  AstBlock*
  block() const;

  /// @brief ラベルステートメントの場合に名前を返す．
  ///
  /// それ以外では ShString() を返す．
  virtual
  ShString
  label() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const = 0;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTATEMENT_H
