#ifndef ASTMEMBERREF_H
#define ASTMEMBERREF_H

/// @file AstMemberRef.h
/// @brief AstMemberRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstMemberRef AstMemberRef.h "AstMemberRef.h"
/// @brief メンバ参照を表すクラス
//////////////////////////////////////////////////////////////////////
class AstMemberRef :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体の式
  /// @param[in] member メンバ名
  /// @param[in] loc ファイル位置
  AstMemberRef(AstExpr* body,
	       AstSymbol* member,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstMemberRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 本体の式を返す．
  ///
  /// kMemberRef, kArrayRef のみ有効
  virtual
  const AstExpr*
  body() const;

  /// @brief メンバ名を返す．
  ///
  /// kMemberRef のみ有効
  virtual
  const AstSymbol*
  member() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オブジェクト名
  AstExpr* mBody;

  // メンバ名
  AstSymbol* mMember;

};

END_NAMESPACE_YM_YMSL

#endif // ASTMEMBERREF_H
