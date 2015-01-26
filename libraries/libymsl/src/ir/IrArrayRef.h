#ifndef YMSLARRAYREF_H
#define YMSLARRAYREF_H

/// @file IrArrayRef.h
/// @brief IrArrayRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrArrayRef IrArrayRef.h "IrArrayRef.h"
/// @brief 配列参照を表すクラス
//////////////////////////////////////////////////////////////////////
class IrArrayRef :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] index インデックス
  IrArrayRef(IrExpr* body,
	       IrExpr* index);

  /// @brief デストラクタ
  virtual
  ~IrArrayRef();


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
  /// kMemberRef, kArrayRef のみ有効
  virtual
  IrExpr*
  body() const;

  /// @brief 配列のインデックスを得る．
  ///
  /// kArrayRef のみ有効
  virtual
  IrExpr*
  array_index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体の式
  IrExpr* mBody;

  // 配列のインデックス
  IrExpr* mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLARRAYREF_H
