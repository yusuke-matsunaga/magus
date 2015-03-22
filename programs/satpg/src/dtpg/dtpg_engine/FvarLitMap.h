#ifndef FVARLITMAP_H
#define FVARLITMAP_H

/// @file FvarLitMap.h
/// @brief FvarLitMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "LitMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FvarLitMap LitMap.h "LitMap.h"
/// @brief TpgNode->fvar() を用いた LitMap
//////////////////////////////////////////////////////////////////////
class FvarLitMap :
  public LitMap
{
public:

  /// @brief コンストラクタ
  FvarLitMap(const TpgNode* node);

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

#endif // FVARLITMAP_H
