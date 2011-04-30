#ifndef PTNODE_H
#define PTNODE_H

/// @file libym_dotlib/tests/vlview/PtNode.h
/// @brief PtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include <QAbstractItemModel>
#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtNode PtNode.h
/// @brief dotlib のパース木を表示するためのモデル
//////////////////////////////////////////////////////////////////////
class PtNode
{
  friend class ParseTreeModel;

public:

  /// @brief コンストラクタ
  /// @param[in] node 対応するパース木のノード
  PtNode(const DotlibNode* node);

  /// @brief デストラクタ
  virtual
  ~PtNode();


public:

  /// @brief 親のインデックスを返す．
  QModelIndex
  parent_index() const;

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

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応するノード
  const DotlibNode* mNode;

  // 親のノード
  QModelIndex mParentIndex;

  // 子供の配列を作っているかを示すフラグ
  mutable
  bool mExpanded;

  // 子供の配列
  mutable
  std::vector<PtNode*> mChildren;

};


//////////////////////////////////////////////////////////////////////
/// @class PtRootNode PtNode.h
/// @brief 根のノード
//////////////////////////////////////////////////////////////////////
class PtRootNode :
  public PtNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] node 対応するパース木のノード
  PtRootNode(const DotlibNode* node);

  /// @brief デストラクタ
  virtual
  ~PtRootNode();


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

  /// @brief 子供の配列を作る．
  virtual
  void
  expand() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根元のノード
  PtNode* mRoot;

};

END_NAMESPACE_YM_DOTLIB

#endif // PTNODE_H
