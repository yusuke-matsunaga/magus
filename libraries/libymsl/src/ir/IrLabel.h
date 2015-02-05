#ifndef IRLABEL_H
#define IRLABEL_H

/// @file IrLabel.h
/// @brief IrLabel のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLabel IrLabel.h "IrLabel.h"
/// @brief ラベルを表すクラス
//////////////////////////////////////////////////////////////////////
class IrLabel :
  public IrNode
{
public:

  /// @brief コンストラクタ
  IrLabel();

  /// @brief デストラクタ
  virtual
  ~IrLabel();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定義済みの時に true を返す．
  ///
  /// kOpLabel のみ意味を持つ．
  virtual
  bool
  is_defined() const;

  /// @brief 定義済みにする．
  ///
  /// kOpLabel のみ意味を持つ．
  virtual
  void
  set_defined();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義済みフラグ
  bool mDefined;

};

END_NAMESPACE_YM_YMSL


#endif // IRLABEL_H
