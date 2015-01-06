#ifndef ASTSTATEMENT_H
#define ASTSTATEMENT_H

/// @file AstStatement.h
/// @brief AstStatement のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "YmUtils/ShString.h"


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

  /// @brief ラベルステートメントの場合に名前を返す．
  ///
  /// それ以外では ShString() を返す．
  virtual
  ShString
  label() const;

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size() = 0;

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr) = 0;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデントする．
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデント量
  static
  void
  print_indent(ostream& s,
	       ymuint indent);

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTATEMENT_H
