#ifndef YMSLFUNCCALL_H
#define YMSLFUNCCALL_H

/// @file YmslFuncCall.h
/// @brief YmslFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFuncCall YmslFuncCall.h "YmslFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class YmslFuncCall :
  public YmslLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  /// @param[in] arglist 引数リスト
  YmslFuncCall(YmslLeaf* func,
	       const vector<YmslExpr*>& arglist);

  /// @brief デストラクタ
  virtual
  ~YmslFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  LeafType
  leaf_type() const;

  /// @brief 本体を返す．
  ///
  /// kMemberRef, kArrayRef, kFuncCall のみ有効
  virtual
  YmslLeaf*
  body() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  YmslExpr*
  arglist_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  YmslLeaf* mFunc;

  // 引数の数
  ymuint mArgNum;

  // 引数のリスト
  YmslExpr** mArgList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNCCALL_H
