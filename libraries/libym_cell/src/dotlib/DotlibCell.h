#ifndef DOTLIBCELL_H
#define DOTLIBCELL_H

/// @file DotlibCell.h
/// @brief DotlibCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibCell DotlibCell.h "DotlibCell.h"
/// @brief DotlibNode の木から取り出したセルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibCell :
  public DotlibAttrMap
{
public:

  /// @brief コンストラクタ
  DotlibCell();

  /// @brief デストラクタ
  ~DotlibCell();


public:

  /// @brief 内容を設定する．
  bool
  set_data(const DotlibNode* cell_node);

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 面積を返す．
  double
  area() const;

  /// @brief ピングループのリストを返す．
  const list<const DotlibNode*>&
  pin_list() const;

  /// @brief バスグループのリストを返す．
  const list<const DotlibNode*>&
  bus_list() const;

  /// @brief バンドルグループのリストを返す．
  const list<const DotlibNode*>&
  bundle_list() const;

  /// @brief ff グループを返す．
  /// @note なければ NULL を返す．
  const DotlibNode*
  ff() const;

  /// @brief latch グループを返す．
  /// @note なければ NULL を返す．
  const DotlibNode*
  latch() const;

  /// @brief statetable グループを返す．
  /// @note なければ NULL を返す．
  const DotlibNode*
  statetable() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル名
  ShString mName;

  // 面積
  double mArea;

  // "bus_naming_style"
  const DotlibNode* mBusNamingStyle;

  // ピングループのリスト
  list<const DotlibNode*> mPinList;

  // バスグループのリスト
  list<const DotlibNode*> mBusList;

  // バンドルグループのリスト
  list<const DotlibNode*> mBundleList;

  // ff グループ
  const DotlibNode* mFF;

  // latch グループ
  const DotlibNode* mLatch;

  // statetable グループ
  const DotlibNode* mStateTable;

};

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIBCELL_H
