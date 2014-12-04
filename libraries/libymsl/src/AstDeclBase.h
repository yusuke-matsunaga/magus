#ifndef ASTDECLBASE_H
#define ASTDECLBASE_H

/// @file AstDeclBase.h
/// @brief AstDeclBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


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

  /// @brief ファイル位置を得る．
  virtual
  const FileRegion&
  file_region() const;

  /// @brief 名前を得る．
  ///
  /// 名前を持つ要素のみ意味を持つ．
  virtual
  ShString
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル位置
  FileRegion mLoc;

  // 名前
  ShString mName;

};

END_NAMESPACE_YM_YMSL

#endif // ASTDECLBASE_H
