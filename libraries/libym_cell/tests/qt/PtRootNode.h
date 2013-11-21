#ifndef PTROOTNODE_H
#define PTROOTNODE_H

/// @file libym_cell/tests/dotlib_ptview/PtRootNode.h
/// @brief PtRootNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtRootNode PtRootNode.h
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
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根元のノード
  PtNode* mRoot;

};

END_NAMESPACE_YM_DOTLIB

#endif // PTROOTNODE_H
