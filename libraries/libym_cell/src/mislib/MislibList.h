#ifndef MISLIBLIST_H
#define MISLIBLIST_H

/// @file MislibList.h
/// @brief MislibList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibList MislibList.h "MislibList.h"
/// @brief MislibNode のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class MislibList :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  MislibList();

  /// @brief デストラクタ
  virtual
  ~MislibList();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 末尾に要素を追加する．
  virtual
  void
  push_back(MislibNodeImpl* pin);

  /// @brief 先頭の要素を取り出す．
  virtual
  const MislibNode*
  top() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  MislibNodeImpl* mTop;

  // 末尾の要素
  MislibNodeImpl* mEnd;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBLIST_H
