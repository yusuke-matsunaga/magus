#ifndef ASTDECLBASE_H
#define ASTDECLBASE_H

/// @file AstDeclBase.h
/// @brief AstDeclBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstDeclBase AstDeclBase.h "AstDeclBase.h"
/// @brief 宣言要素の基底クラス
//////////////////////////////////////////////////////////////////////
class AstDeclBase :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] loc ファイル位置
  AstDeclBase(ShString name,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstDeclBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  virtual
  ShString
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTDECLBASE_H
