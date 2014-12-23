#ifndef ASTASSIGNMENT_H
#define ASTASSIGNMENT_H

/// @file AstAssignment.h
/// @brief AstAssignment のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstAssignment AstAssignment.h "AstAssignment.h"
/// @brief 代入文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstAssignment :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] token トークン
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  AstAssignment(TokenType token,
		AstPrimary* left,
		AstExpr* right);

  /// @brief デストラクタ
  virtual
  ~AstAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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

  // トークン
  TokenType mToken;

  // 左辺式
  AstPrimary* mLeft;

  // 右辺式
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTASSIGNMENT_H
