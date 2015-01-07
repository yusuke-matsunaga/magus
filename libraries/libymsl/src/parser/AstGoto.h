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

  /// @brief スコープの生成と変数名の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase1(YmslScope* parent_scope);

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

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
	  Ymsl_INT& addr);

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
