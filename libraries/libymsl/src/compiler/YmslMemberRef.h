#ifndef YMSLMEMBERREF_H
#define YMSLMEMBERREF_H

/// @file YmslMemberRef.h
/// @brief YmslMemberRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslMemberRef YmslMemberRef.h "YmslMemberRef.h"
/// @brief メンバ参照を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslMemberRef :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] offset メンバのオフセット
  YmslMemberRef(YmslExpr* body,
		ymuint offset);

  /// @brief デストラクタ
  virtual
  ~YmslMemberRef();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 本体を返す．
  ///
  /// kMemberRef, kArrayRef, kFuncCall のみ有効
  virtual
  YmslExpr*
  body() const;

  /// @brief メンバのオフセットを得る．
  ///
  /// kMemberRef のみ有効
  virtual
  ymuint
  member_offset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体の式
  YmslExpr* mBody;

  // オフセット
  ymuint mOffset;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLMEMBERREF_H
