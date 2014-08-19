#ifndef PTBASEIMPL_H
#define PTBASEIMPL_H

/// @file libym_YmCell/tests/dotlib_ptview/PtBaseNode.h
/// @brief PtBaseNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtBaseNode PtBaseNode.h
/// @brief PtNode の実装クラス
//////////////////////////////////////////////////////////////////////
class PtBaseNode :
  public PtNode
{
  friend class ParseTreeModel;

public:

  /// @brief コンストラクタ
  /// @param[in] node 対応するパース木のノード
  PtBaseNode(const DotlibNode* node);

  /// @brief デストラクタ
  virtual
  ~PtBaseNode();


public:

  /// @brief 子供の数を返す．
  virtual
  int
  child_num() const;

  /// @brief 子供を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
  virtual
  PtNode*
  child(int pos) const;

  /// @brief データを返す．
  /// @param[in] column コラム番号
  /// @param[in] role
  virtual
  QVariant
  data(int column,
       int role) const;

  /// @brief 対象のファイル上での位置を返す．
  virtual
  FileRegion
  loc() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供の配列を作る．
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するノード
  const DotlibNode* mNode;

  // 子供の配列を作っているかを示すフラグ
  mutable
  bool mExpanded;

  // 子供の配列
  mutable
  std::vector<PtNode*> mChildren;

};

END_NAMESPACE_YM_DOTLIB

#endif // PTBASEIMPL_H
