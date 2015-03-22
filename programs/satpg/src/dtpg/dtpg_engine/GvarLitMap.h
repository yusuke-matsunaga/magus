#ifndef GVARLITMAP_H
#define GVARLITMAP_H

/// @file GvarLitMap.h
/// @brief GvarLitMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GvarLitMap LitMap.h "LitMap.h"
/// @brief TpgNode->gvar() を用いた LitMap
//////////////////////////////////////////////////////////////////////
class GvarLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  GvarLitMap(const TpgNode* node);

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_size() const;

  /// @brief 入力のリテラルを返す．
  virtual
  Literal
  input(ymuint pos) const;

  /// @brief 出力のリテラルを返す．
  virtual
  Literal
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード
  const TpgNode* mNode;

};

END_NAMESPACE_YM_SATPG

#endif // GVARLITMAP_H
