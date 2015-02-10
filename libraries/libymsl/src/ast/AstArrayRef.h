#ifndef ASTARRAYREF_H
#define ASTARRAYREF_H

/// @file AstArrayRef.h
/// @brief AstArrayRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstArrayRef AstArrayRef.h "AstArrayRef.h"
/// @brief 配列参照を表すクラス
//////////////////////////////////////////////////////////////////////
class AstArrayRef :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体の式
  /// @param[in] index インデックスの式
  /// @param[in] loc ファイル位置
  AstArrayRef(AstExpr* body,
	      AstExpr* index,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstArrayRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  expr_type() const;

  /// @brief 配列本体を返す．
  ///
  /// kMemberRef, kArrayRef, kFuncCall のみ有効
  virtual
  const AstExpr*
  body() const;

  /// @brief インデックスを返す．
  ///
  /// kArrayRef のみ有効
  virtual
  const AstExpr*
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列の本体
  AstExpr* mBody;

  // インデックス
  AstExpr* mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // ASTARRAYREF_H
