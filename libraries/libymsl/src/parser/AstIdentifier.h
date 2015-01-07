#ifndef ASTIDENTIFIER_H
#define ASTIDENTIFIER_H

/// @file AstIdentifier.h
/// @brief AstIdentifier のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIdentifier AstIdentifier.h "AstIdentifier.h"
/// @brief 識別子を表すクラス
//////////////////////////////////////////////////////////////////////
class AstIdentifier :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] symbol_list シンボルリスト
  /// @param[in] loc ファイル位置
  AstIdentifier(AstSymbolList* symbol_list,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIdentifier();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルリストを返す．
  const vector<AstSymbol*>&
  symbol_list() const;

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボルリスト
  vector<AstSymbol*> mSymbolList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTIDENTIFIER_H
