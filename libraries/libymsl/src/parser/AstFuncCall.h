#ifndef ASTFUNCCALL_H
#define ASTFUNCCALL_H

/// @file AstFuncCall.h
/// @brief AstFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncCall AstFuncCall.h "AstFuncCall.h"
/// @brief 関数呼び出しを表す Ast
//////////////////////////////////////////////////////////////////////
class AstFuncCall :
  public AstLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  AstFuncCall(AstLeaf* func,
	      AstExprList* expr_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  LeafType
  leaf_type() const;

  /// @brief 関数本体を返す．
  virtual
  const AstLeaf*
  body() const;

  /// @brief 引数リストの要素数を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 引数リストの要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  const AstExpr*
  arglist_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  AstLeaf* mFunc;

  // 引数のリスト
  vector<AstExpr*> mExprList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFUNCCALL_H
