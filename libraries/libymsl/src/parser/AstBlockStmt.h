#ifndef ASTBLOCKSTMT_H
#define ASTBLOCKSTMT_H

/// @file AstBlockStmt.h
/// @brief AstBlockStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBlockStmt AstBlockStmt.h "AstBlockStmt.h"
/// @brief block文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstBlockStmt :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstBlockStmt(AstBlock* block,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBlockStmt();


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

  // 本体
  AstBlock* mBlock;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBLOCKSTMT_H
