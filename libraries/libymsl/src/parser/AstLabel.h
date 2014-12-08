#ifndef ASTLABEL_H
#define ASTLABEL_H

/// @file AstLabel.h
/// @brief AstLabel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstLabel AstLabel.h "AstLabel.h"
/// @brief ラベルを表す Ast
//////////////////////////////////////////////////////////////////////
class AstLabel :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] loc ファイル位置
  AstLabel(AstSymbol* label,
	   const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstLabel();


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

#endif // ASTLABEL_H
