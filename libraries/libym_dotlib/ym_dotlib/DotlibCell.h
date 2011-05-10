#ifndef YM_DOTLIB_DOTLIBCELL_H
#define YM_DOTLIB_DOTLIBCELL_H

/// @file ym_dotlib/DotlibCell.h
/// @brief DotlibCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibCell DotlibCell.h "ym_dotlib/DotlibCell.h"
/// @brief DotlibNode の木から取り出したセルの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibCell :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibCell();

  /// @brief デストラクタ
  ~DotlibCell();


public:

  /// @brief 内容を初期化する．
  void
  clear();

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
  const DotlibNode* mFf;

  // latch グループ
  const DotlibNode* mLatch;

  // statetable グループ
  const DotlibNode* mStatetable;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBCELL_H
