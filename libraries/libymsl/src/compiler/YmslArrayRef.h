#ifndef YMSLARRAYREF_H
#define YMSLARRAYREF_H

/// @file YmslArrayRef.h
/// @brief YmslArrayRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslArrayRef YmslArrayRef.h "YmslArrayRef.h"
/// @brief 配列参照を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslArrayRef :
  public YmslLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] body 本体
  /// @param[in] index インデックス
  YmslArrayRef(YmslLeaf* body,
	       YmslExpr* index);

  /// @brief デストラクタ
  virtual
  ~YmslArrayRef();


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

  /// @brief 配列のインデックスを得る．
  ///
  /// kArrayRef のみ有効
  virtual
  YmslExpr*
  array_index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体の式
  YmslLeaf* mBody;

  // 配列のインデックス
  YmslExpr* mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLARRAYREF_H
